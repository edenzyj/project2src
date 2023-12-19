#pragma once
#include "ofxEasyCubemap.h"
#include "ofMain.h"

struct DirectionalLight {
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void drawBunny(DirectionalLight& dirLight, glm::mat4& proj, glm::mat4& view, glm::mat4 model);
		void drawSkybox(DirectionalLight& dirLight, glm::mat4& proj, glm::mat4& view, glm::mat4 model);
		ofMesh triangle;
		ofMesh cubeMesh;  //skybox

		ofShader shader;
		ofShader skyboxShader;

		ofxEasyCubemap cubemap;
};
