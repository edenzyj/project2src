#include "ofApp.h"
#include "Camera.h"
#include "CameraOrbit.h"

Camera g_camera;
CameraOrbit g_camera_orbit;

extern int g_window_w;
extern int g_window_h;

//mouse stuff
glm::vec2 g_mouse_prev;

glm::vec3 getLightDirection(DirectionalLight& l) {
	return glm::normalize(l.direction * -1);
}

glm::vec3 getLightColor(DirectionalLight& l) {
	return l.color * l.intensity;
}
glm::vec3 g_main_mesh_center;
//--------------------------------------------------------------
void ofApp::setup(){

	ofDisableArbTex();
	ofEnableDepthTest();

	triangle.load("cube.ply");//bunny.ply
	cubeMesh.load("cube.ply");

	//calculate main mesh center (for camera centering)
	g_main_mesh_center = glm::vec3(0);
	vector<glm::vec3> vertices = triangle.getVertices();

	for (int i = 0; i < vertices.size(); i++)
	{
		g_main_mesh_center += vertices[i];
	}
	g_main_mesh_center /= (float)vertices.size();

	shader.load("vertex_color.vert", "vertex_color.frag");
	skyboxShader.load("skybox.vert", "skybox.frag");

	//set camera:
	g_camera.Reset();
	g_camera.m_default_distance = 5;
	g_camera.m_center = glm::vec3(0);

	// set camera orbit:
	g_camera_orbit.Reset();
	g_camera_orbit.m_radius = 4;

	cubemap.load("cube_front.jpg", "cube_back.jpg",
		"cube_right.jpg", "cube_left.jpg",
		"cube_top.jpg", "cube_bottom.jpg");

}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;

	DirectionalLight dirLight;

	
	g_camera.m_aspect = (float)g_window_w / (float)g_window_h;

	g_camera_orbit.m_aspect = (float)g_window_w / (float)g_window_h;

	/*
	glm::mat4 model_matrix = g_camera.ModelMatrix();
	glm::mat4 view_matrix = g_camera.ViewMatrix();
	glm::mat4 proj_matrix = g_camera.ProjectionMatrix();*/
	glm::mat4 model_matrix = g_camera_orbit.ModelMatrix();
	glm::mat4 view_matrix = g_camera_orbit.ViewMatrix();
	glm::mat4 proj_matrix = g_camera_orbit.ProjectionMatrix();
	mat4 mvp = proj_matrix * view_matrix * model_matrix;


	/*mat3 normalMatrix = (transpose(inverse(mat3(model_matrix))));


	dirLight.direction = normalize(vec3(1, 0, -1));
	dirLight.color = vec3(1, 1, 1);
	dirLight.intensity = 1.0f;

	shader.begin();
	shader.setUniformMatrix4f("mvp", mvp);
	shader.setUniformMatrix4f("model", model_matrix);
	shader.setUniformMatrix3f("normal", normalMatrix);

	shader.setUniform3f("cameraPos", glm::vec3(0, 0, 5));
	shader.setUniform3f("meshSpecCol", glm::vec3(1, 1, 1)); //ks
	shader.setUniform3f("meshCol", glm::vec3(1.0, 0.0, 0.0)); //kd
	shader.setUniform3f("lightDir", getLightDirection(dirLight));
	shader.setUniform3f("lightCol", getLightColor(dirLight)); //ld ls
	shader.setUniform3f("ambientCol", glm::vec3(0.0, 0.0, 0.0)); // sa
	//mainMesh.drawWireframe();

	triangle.draw();

	shader.end();*/
	drawBunny(dirLight, proj_matrix, view_matrix, model_matrix);
	drawSkybox(dirLight, proj_matrix, view_matrix, model_matrix);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	g_mouse_prev = glm::vec2(x, y);
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (button == 0)  //left-button
	{
		//dragged enough distance?
		if (glm::length((glm::vec2(x, y) - g_mouse_prev)) > 5)
		{
			g_camera_orbit.RotateHorizotal((x - g_mouse_prev.x) * 0.001);
			g_camera_orbit.RotateVertical((y - g_mouse_prev.y) * 0.001);
		}
	}
	else if (button == 1)
	{
		g_camera.Move((float)(x - g_mouse_prev.x) * 0.1, (float)(g_mouse_prev.y - y) * 0.1);
	}
	else if (button == 2)  //right-button
	{
		g_camera_orbit.Zoom((y - g_mouse_prev.y) / 200.0f);
	}

	g_mouse_prev = glm::vec2(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	if (button == 0)  //left-button
	{
		g_mouse_prev = glm::vec2(x, y);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
	g_window_w = w;
	g_window_h = h;
}


//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
void ofApp::drawBunny(DirectionalLight& dirLight, glm::mat4& proj, glm::mat4& view, glm::mat4 model)
{
	using namespace glm;

	//centerize and rotate
	model = translate(-g_main_mesh_center) * model;
	model = rotate(-glm::half_pi<float>(), vec3(1, 0, 0)) * model;

	mat4 mvp = proj * view * model;
	mat3 normalMatrix = (transpose(inverse(mat3(model))));

	dirLight.direction = normalize(vec3(1, 0, -1));
	dirLight.color = vec3(1, 1, 1); 
	dirLight.intensity = 1.0f;
	shader.begin();
	shader.setUniformMatrix4f("mvp", mvp);
	shader.setUniformMatrix4f("model", model);
	shader.setUniform3f("cameraPos", g_camera_orbit.Pos());
	shader.setUniform3f("meshSpecCol", glm::vec3(1, 1, 1));
	//shader.setUniform3f("meshCol", glm::vec3(1.0, 0.0, 0.0));
	shader.setUniform3f("meshCol", glm::vec3(1.0, 1.0, 0.0));
	shader.setUniform3f("lightDir", getLightDirection(dirLight));
	shader.setUniform3f("lightCol", getLightColor(dirLight));
	shader.setUniformMatrix3f("normal", normalMatrix);
	//shader.setUniform3f("ambientCol", glm::vec3(0.2, 0.2, 0.2));//0.2 0.2 0.2
	shader.setUniform3f("ambientCol", glm::vec3(1.0, 1.0, 1.0));//0.2 0.2 0.2
	shader.setUniformTexture("envMap", cubemap.getTexture(), 0);
	triangle.draw();
	shader.end();
}
void ofApp::drawSkybox(DirectionalLight& dirLight, glm::mat4& proj, glm::mat4& view, glm::mat4 model)
{
	using namespace glm;

	//make bigger
	//model = model * glm::scale(vec3(6, 6, 6));
	model = model * glm::scale(vec3(12, 12, 12));


	mat4 mvp = proj * view * model;

	glDepthFunc(GL_LEQUAL);
	skyboxShader.begin();
	skyboxShader.setUniformMatrix4f("mvp", mvp);
	skyboxShader.setUniformTexture("envMap", cubemap.getTexture(), 0);
	cubeMesh.draw();
	skyboxShader.end();
	glDepthFunc(GL_LESS);
}