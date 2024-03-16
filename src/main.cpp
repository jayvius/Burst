#include <string>
#include "ofMain.h"
#include "ofApp.h"
#include "scanner.h"

int main(int argc, char *argv[])
{
	bool drawAxis = false;
	if (argc > 1) {
		if (std::string(argv[1]) == "--axis")
			drawAxis = true;
	}

	ofSetupOpenGL(1024,768, OF_WINDOW);
	ofRunApp(new ofApp(drawAxis));
}
