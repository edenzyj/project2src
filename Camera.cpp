#include "Camera.h"

extern int g_window_w;
extern int g_window_h;

glm::mat4 Camera::ModelMatrix()
{	
	//build model matrix
	glm::mat4 model = glm::mat4();
	//step0: re-center everything (usually in the object center)
	model = glm::translate(-m_center) * model;
	//step1: scale
	model = glm::scale(glm::vec3(m_scale)) * model;
	//step2: rotation
	model = m_trackball_matrix * model;
	//step3: translation
	model = glm::translate(m_translation) * model;

	return model;
}

glm::mat4 Camera::ViewMatrix()
{
	return glm::lookAt(glm::vec3(0, 0, m_default_distance), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::ProjectionMatrix()
{
	return glm::perspective(m_fovy, m_aspect, m_znear, m_zfar);
}

void Camera::UpdateTrackball(glm::vec2 p0, glm::vec2 p1)
{
	glm::vec2 screen_center((float)g_window_w / 2, (float)g_window_h / 2);

	//offset vector po->p1 to be from center?
	{
		glm::vec2 offset = p1 - p0;
		p0 = screen_center;
		p1 = screen_center + offset;
	}

	//project p0 and p1 to unit sphere in NDC centered at screen center
	glm::vec3 P0, P1;
	for (int Case = 0; Case < 2; Case++)
	{
		glm::vec2 *p = NULL;
		glm::vec3 *P = NULL;
		if (Case == 0)
		{
			p = &p0;
			P = &P0;
		}
		else
		{
			p = &p1;
			P = &P1;
		}

		//turn p to NDC
		(*p).x = ((*p).x - (float)screen_center.x) / ((float)g_window_w / 2);
		(*p).y = ((*p).y - (float)screen_center.y) / ((float)g_window_h / 2);

		float length = (*p).x*(*p).x + (*p).y*(*p).y;
		if (length <= 1)  //inside sphere on screen
		{
			(*P).x = (*p).x;
			(*P).y = (*p).y;
			(*P).z = -sqrt(1 - (*p).x*(*p).x - (*p).y*(*p).y);
		}
		else  //outside
		{
			(*P).x = (*p).x / length;
			(*P).y = (*p).y / length;
			(*P).z = 0;
		}
	}
	
	//rotation axis is P0 cross P1
	glm::vec3 axis = glm::cross(P0, P1);
	axis = glm::normalize(axis);

	//rotation angle is P0 dot P1
	float angle = - acos(glm::dot(P0, P1));

	//cout << "P0:" << P0 << " P1:" << P1 << " axis:" << axis << " angle:" << angle << endl;

	glm::mat4 rot = glm::rotate(angle, axis);

	//multiply to the current trackball rotation matrix
	m_trackball_matrix = rot * m_trackball_matrix;
}

void Camera::Zoom(float scale)
{
	m_scale += scale;

	if (m_scale < 0.01)
		m_scale = (float)0.01;

	if (m_scale > 100)
		m_scale = 100;
}

void Camera::Move(float horizontal_amount, float vertical_amount)
{
	m_translation.x += (horizontal_amount * (m_default_distance * 0.01));
	m_translation.y += (vertical_amount * (m_default_distance * 0.01));
}
