#include "Transform.h"

Buffer<glm::mat4>* Transform::transformBuffer = new Buffer<glm::mat4>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);