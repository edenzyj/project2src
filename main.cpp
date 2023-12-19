#include "ofMain.h"
#include "ofApp.h"

int g_window_w = 1200;
int g_window_h = 1200;
//========================================================================
int main() {
	ofGLWindowSettings glSettings;
	glSettings.setSize(1200, 1200);
	glSettings.windowMode = OF_WINDOW;
	glSettings.setGLVersion(4, 1);

	ofCreateWindow(glSettings);
	ofRunApp(new ofApp());

}
