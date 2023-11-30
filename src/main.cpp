#include "ofMain.h"
#include "ofApp.h"
#include "scanner.h"

int main()
{
	ofSetupOpenGL(1024,768, OF_WINDOW);
	ofRunApp(new ofApp());
    /*
    std::string src = "a : tx 20 tx 10.0 box # test";
    Scanner scanner(src);
    while (std::optional<Token> t = scanner.next()) {
        printf("%u %s\n", t->type, t->lexeme.c_str());
    }
    */
}
