#include "Transform.h"

Buffer<glm::mat4>* Transform::transformBuffer = new Buffer<glm::mat4>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

void Transform::Translate(glm::vec3 by) {
	glm::vec3 abs = glm::vec3(
		by.x * front.x + by.y * right.x + by.z * up.x,
		by.x * front.y + by.y * right.y + by.z * up.y,
		by.x * front.z + by.y * right.z + by.z * up.z
	);
	Move(abs);
	UpdateMatrix();
}