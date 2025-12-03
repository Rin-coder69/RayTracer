#include "Camera.h"

void Camera::SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
	this->eye = eye;

	// create camera axis
	//this->forward = normalized direction vector (target <-- eye)
	this->forward = glm::normalize(target - eye);
	//this->right = normalized vector from the cross product of the forward and up vector
	this->right = glm::normalize(glm::cross(this->forward, up));
	//this->up = normalized vector from the cross product of the right and forward vector
	this->up = glm::normalize(glm::cross(this->right, this->forward));

	CalculateViewPlane();
}

	Ray Camera::GetRay(const glm::vec2& uv) const {
	Ray ray;

	//ray.origin = camera eye
	glm::vec3 dir = lowerLeft + horizontal * uv.x + vertical * uv.y - eye;
	//ray.direction = lower left position + horizontal vector * uv.x + vertical vector * uv.y - camera eye;
	dir = glm::normalize(dir);
	return Ray{ eye,dir };
}

void Camera::CalculateViewPlane() {
// convert fov (degrees) to radians
	float theta = glm::radians(fov);

	// halfHeight at distance 1 from the eye
	float halfHeight = std::tan(theta * 0.5f);
	float halfWidth = aspectRatio * halfHeight;

	// horizontal = full width vector on the view plane
	horizontal = right * (2.0f * halfWidth);

	// vertical = full height vector on the view plane
	vertical = up * (2.0f * halfHeight);

	// lowerLeft = center of view plane (eye + forward) minus half horizontal and half vertical
	lowerLeft = eye + forward - horizontal * 0.5f - vertical * 0.5f;
}

