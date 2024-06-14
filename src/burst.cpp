#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>

#include "GL/gl3w.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fmt/core.h>

#include "types.hpp"
#include "cube.hpp"
#include "buffer.hpp"
#include "vm.hpp"

std::string load_shader_src(const std::string &shader_filename)
{
    std::ifstream input(shader_filename);
    std::stringstream buffer;
    buffer << input.rdbuf();
    auto src = buffer.str();
    input.close();
    return src;
}

GLuint create_shader(GLenum shader_type, const std::string &shader_src)
{
    if (shader_type != GL_VERTEX_SHADER
            && shader_type != GL_FRAGMENT_SHADER
            && shader_type != GL_GEOMETRY_SHADER) {
        fprintf(stderr, "ERROR: unknown shader type %d\n", shader_type);
        exit(1);
    }

    const char *shader_code = shader_src.c_str();
    const GLint size = shader_src.size();

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, const_cast<const char**>(&shader_code), &size);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint info_log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

        std::string info_log;
        info_log.reserve(info_log_length);
        glGetShaderInfoLog(shader, info_log_length, NULL, info_log.data());

        switch (shader_type) {
            case GL_VERTEX_SHADER:
                fprintf(stderr, "vertex shader %s\n", info_log.c_str());
                break;
            case GL_FRAGMENT_SHADER:
                fprintf(stderr, "fragment shader %s\n", info_log.c_str());
                break;
            case GL_GEOMETRY_SHADER:
                fprintf(stderr, "geometry shader %s\n", info_log.c_str());
                break;
            default:
                fprintf(stderr, "unknown shader %s\n", info_log.c_str());
                break;
        }
        exit(1);
    }

    return shader;
}

GLuint create_program(std::string vertex_shader_src,
                      std::string fragment_shader_src,
                      std::string geometry_shader_src,
                      std::vector<std::string> feedback_outputs)
{
    std::vector<GLuint> shaders;
    GLuint program = glCreateProgram();

    GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_src);
    glAttachShader(program, vertex_shader);

    GLuint fragment_shader;
    if (!fragment_shader_src.empty()) {
        fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_src);
        glAttachShader(program, fragment_shader);
    }

    GLuint geometry_shader;
    if (!geometry_shader_src.empty()) {
        geometry_shader = create_shader(GL_GEOMETRY_SHADER, geometry_shader_src);
        glAttachShader(program, geometry_shader);
    }

    if (!feedback_outputs.empty()) {
        std::vector<const char*> outputs;
        for (auto &s: feedback_outputs) {
            outputs.push_back(s.c_str());
        }
        glTransformFeedbackVaryings(program,
                                    feedback_outputs.size(),
                                    outputs.data(),
                                    GL_INTERLEAVED_ATTRIBS);
    }

    glLinkProgram(program);

    glDetachShader(program, vertex_shader);
    if (!fragment_shader_src.empty()) {
        glDetachShader(program, fragment_shader);
    }
    if (!geometry_shader_src.empty()) {
        glDetachShader(program, geometry_shader);
    }

    glUseProgram(program);
    return program;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

GLFWwindow* create_window(int width, int height, const char *title)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    return window;
}

void init_gl()
{
    gl3wInit();

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE); 
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
}

struct Trackball
{
    float yOffset;
    glm::vec2 lastMousePosition;
    bool mouseDrag;
    double xSpeed;
    double ySpeed;
    float dampen;
    glm::quat modelRotation;
};

void initTrackball(Trackball &trackball)
{
    trackball.yOffset = 20.0f;
    trackball.mouseDrag = false;
    trackball.xSpeed = 0.0;
    trackball.ySpeed = 0.0;
    trackball.dampen = 0.4;
    trackball.modelRotation = glm::quat(1.0, 0.0, 0.0, 0.0);
}

void trackballDollyCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    Trackball *trackball = static_cast<Trackball*>(glfwGetWindowUserPointer(window));
    trackball->yOffset += 0.1 * yOffset;
    if (trackball->yOffset < 0.1)
        trackball->yOffset = 0.1;
}

void updateTrackball(Trackball &trackball)
{
    trackball.xSpeed /= 1.01f;
    trackball.ySpeed /= 1.01f;
    glm::quat yRot = glm::angleAxis(static_cast<float>(glm::radians(trackball.xSpeed)), glm::vec3(0,1,0));
    glm::quat xRot = glm::angleAxis(static_cast<float>(glm::radians(trackball.ySpeed)), glm::vec3(1,0,0));
    trackball.modelRotation = yRot * xRot * trackball.modelRotation;
}

void updateModelFromTrackball(Trackball &trackball, glm::mat4 &model)
{
    model = glm::mat4(trackball.modelRotation);
}

void updateViewFromTrackball(Trackball &trackball, glm::mat4 &view)
{
    view = glm::lookAt(
        glm::vec3(0.0, 0.0, trackball.yOffset),
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0));
}

void trackballToggleCallback(GLFWwindow* window, int button, int action, int mods)
{
    Trackball *trackball = static_cast<Trackball*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        trackball->mouseDrag = true;
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        trackball->lastMousePosition = glm::vec2(xPos, yPos);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        trackball->mouseDrag = false;
    }
}

static void trackballDragCallback(GLFWwindow* window, double xpos, double ypos)
{
    Trackball *trackball = static_cast<Trackball*>(glfwGetWindowUserPointer(window));
    if (trackball->mouseDrag) {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        trackball->xSpeed = std::lerp(trackball->xSpeed, (xPos - trackball->lastMousePosition.x) * trackball->dampen, 0.1);
        trackball->ySpeed = std::lerp(trackball->ySpeed, (yPos - trackball->lastMousePosition.y) * trackball->dampen, 0.1);
        trackball->lastMousePosition = glm::vec2(xPos, yPos);
    }
}

int main(int argc, char *argv[])
{
    GLFWwindow* window = create_window(800, 800, "Burst");
    init_gl();

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, trackballDollyCallback);
    glfwSetMouseButtonCallback(window, trackballToggleCallback);
    glfwSetCursorPosCallback(window, trackballDragCallback);

    fmt::print("renderer: {}\n", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    fmt::print("opengl version: {}\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    GLuint program = create_program(load_shader_src("src/triangle.vs"), load_shader_src("src/triangle.fs"), "", {});
    GLuint programNormals = create_program(load_shader_src("src/normals.vs"),
        load_shader_src("src/normals.fs"), load_shader_src("src/normals.gs"), {});

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    Buffer buffer;
    init(buffer);
    // VertexIndex v1 = addVertex(buffer, {{0.5, 0.0, 0.0}, {255, 255, 255, 255}});
    // VertexIndex v2 = addVertex(buffer, {{0.0, 0.5, 0.0}, {255, 255, 255, 255}});
    // VertexIndex v3 = addVertex(buffer, {{-0.5, 0.0, 0.0}, {255, 255, 255, 255}});
    // addTriangle(buffer, v1, v2, v3);
    //glm::mat4 rotation = glm::mat4(1.0f);
    //rotation = glm::rotate(rotation, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
    //rotation = glm::scale(rotation, glm::vec3(2.0, 2.0, 0.1));
    //addCube(buffer, rotation);
    VM vm;
    run(vm, "R0: ry 5.0 box (R0 R1)\nR1: ty 1.0 R0", buffer);
    //run(vm, "R0: ry 5.0 box R0", buffer);

    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view = glm::mat4(1.0);

    Trackball trackball;
    initTrackball(trackball);
    glfwSetWindowUserPointer(window, &trackball);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updateTrackball(trackball);
        updateModelFromTrackball(trackball, model);
        updateViewFromTrackball(trackball, view);
        int screenWidth, screenHeight;
        glfwGetWindowSize(window, &screenWidth, &screenHeight);
        double ratio = static_cast<double>(screenWidth) / static_cast<double>(screenHeight);
        glm::mat4 projection = glm::perspective(glm::radians(45.0), ratio, 0.1, 1000.0);

        glUseProgram(program);
        unsigned int model_id = glGetUniformLocation(program, "model");
        glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));
        unsigned int view_id = glGetUniformLocation(program, "view");
        glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
        unsigned int projection_id = glGetUniformLocation(program, "projection");
        glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection));
        draw(buffer);

        // glUseProgram(programNormals);
        // model_id = glGetUniformLocation(programNormals, "model");
        // glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));
        // view_id = glGetUniformLocation(programNormals, "view");
        // glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
        // projection_id = glGetUniformLocation(programNormals, "projection");
        // glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection));
        // draw(buffer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
