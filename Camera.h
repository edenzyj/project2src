#pragma once

#include "ofMain.h"

class Camera
{
public:

	////MODEL transform:

	//virtual trackball:
	glm::vec2 m_mouse_prev;  //previous mouse click position (-1~1 NDC)
	glm::mat4 m_trackball_matrix;  //current trackball (rotations) matrix

	////VIEW transform:

	//camera position at (0,0,dist), assume look at (0,0,0) and up at (0,0,1)
	float m_default_distance;
	glm::vec3 m_center;

	float m_scale;
	glm::vec3 m_translation;

	////PROJECTION transform:

	//perspective projection parameters
	float m_aspect;
	float m_fovy;
	float m_znear, m_zfar;

	Camera()
	{
		Reset();
	}
	void Reset()
	{
		m_mouse_prev = glm::vec2(0,0);  //i.e., screen center
		m_trackball_matrix = glm::mat4();
		m_scale = 1;
		m_translation = glm::vec3(0);
		m_aspect = 1;
		m_fovy = glm::radians(45.0f);
		m_znear = 0.01;
		m_zfar = 1000;
	}

	glm::mat4 ModelMatrix();
	glm::mat4 ViewMatrix();
	glm::mat4 ProjectionMatrix();

	//update trackball matrix w/ a mouse drag p0->p1
	void UpdateTrackball(glm::vec2 p0, glm::vec2 p1);

	void Zoom(float scale);  //zoom-in zoom-out

	void Move(float horizontal_amount, float vertical_amount);  //translate on the screen
};