#include <string>
#include <cstdlib>

#include <fmt/core.h>
#include "ofMain.h"

#include "ofApp.h"

void fatal(std::string errorMsg)
{
	fmt::print("ERROR: {}\n", errorMsg);
	exit(1);
}

int main(int argc, char *argv[])
{
	std::string inputFileName = "";
	bool drawAxis = false;

	if (argc < 2)
		fatal("missing input filename");

	int argIndex = 1;
	inputFileName = argv[argIndex++];

	while (argIndex < argc) {
		if (std::string(argv[argIndex]) == "--axis")
			drawAxis = true;
		else
			fatal(fmt::format("invalid argument {}", argv[argIndex]));
		argIndex++;
	}

	ofSetupOpenGL(1024,768, OF_FULLSCREEN);
	ofRunApp(new ofApp(inputFileName, drawAxis));
}
