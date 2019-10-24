#include "Light.h"


Light::Light() {
	position = glm::vec3(0, 0, 0);
}

void Light::SetPosition(glm::vec3 _position) {
	position = _position;
}

void Light::SetPosition(float x, float y, float z) {
	position = glm::vec3(x, y, z);
}

glm::vec3 Light::GetPosition() {
	return position;
}


