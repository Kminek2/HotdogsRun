#pragma once

#include "Collisions.h"
#include <map>

namespace default_obb_data {
using default_obbs = std::map<std::string, std::vector<OBB>>;
default_obbs data = {
    {"prostySkosAsfalt",
     {OBB({0, 0, 0}, {9.0f, 2.6f, 1000.0f}, {glm::vec3(0.7071f, -0.7071f, 0.0f), glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)})}},
    {"prostySkosLod",
     {OBB({0, 0, 0}, {9.0f, 2.6f, 1000.0f}, {glm::vec3(0.7071f, -0.7071f, 0.0f), glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)})}},
    {"prostySkosZwir",
     {OBB({0, 0, 0}, {9.0f, 2.6f, 1000.0f}, {glm::vec3(0.7071f, -0.7071f, 0.0f), glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)})}},
    {"zakrestSkosAsfalt",
     {OBB({0, -2.5f, 0}, {3.4f, 2.4f, 1000.0f}, {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}),
      OBB({-4.3f, -4.3f, 0}, {2.4f, 2.7f, 1000.0f},
	  {glm::vec3(0.7071f, -0.7071f, 0.0f), glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}),
      OBB({4.3f, -4.3f, 0}, {2.7f, 2.4f, 1000.0f},
	  {glm::vec3(0.7071f, -0.7071f, 0.0f), glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)})}},
    {"zakrestSkosLod",
     {OBB({0, -2.5f, 0}, {3.4f, 2.4f, 1000.0f}, {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}),
      OBB({-4.3f, -4.3f, 0}, {2.4f, 2.7f, 1000.0f},
	  {glm::vec3(0.7071f, -0.7071f, 0.0f), glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}),
      OBB({4.3f, -4.3f, 0}, {2.7f, 2.4f, 1000.0f},
	  {glm::vec3(0.7071f, -0.7071f, 0.0f), glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)})}},
    {"zakrestSkosZwir",
     {OBB({0, -2.5f, 0}, {3.4f, 2.4f, 1000.0f}, {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}),
      OBB({-4.3f, -4.3f, 0}, {2.4f, 2.7f, 1000.0f},
	  {glm::vec3(0.7071f, -0.7071f, 0.0f), glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}),
      OBB({4.3f, -4.3f, 0}, {2.7f, 2.4f, 1000.0f},
	  {glm::vec3(0.7071f, -0.7071f, 0.0f), glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)})}},
    {"zakretAsfalt",
     {OBB({1.8f, -1.8f, 0}, {6.6f, 3.2f, 1000.0f},
	  {glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(-0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)})}},
    {"zakretLod",
     {OBB({1.8f, -1.8f, 0}, {6.6f, 3.2f, 1000.0f},
	  {glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(-0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)})}},
    {"zakretZwir",
     {OBB({1.8f, -1.8f, 0}, {6.6f, 3.2f, 1000.0f},
	  {glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(-0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)})}},
    {"zakretPolSkosAsfalt",
     {OBB({-2.4f, 2.4f, 0}, {2.8f, 5.5f, 1000.0f},
	  {glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(-0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}),
      OBB({0.0f, -1.0f, 0}, {2.8f, 5.5f, 1000.0f}, {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)})}},
    {"zakretPolSkosLod",
     {OBB({-2.4f, 2.4f, 0}, {2.8f, 5.5f, 1000.0f},
	  {glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(-0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}),
      OBB({0.0f, -1.0f, 0}, {2.8f, 5.5f, 1000.0f}, {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)})}},
    {"zakretPolSkosZwir",
     {OBB({-2.4f, 2.4f, 0}, {2.8f, 5.5f, 1000.0f},
	  {glm::vec3(0.7071f, 0.7071f, 0.0f), glm::vec3(-0.7071f, 0.7071f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)}),
      OBB({0.0f, -1.0f, 0}, {2.8f, 5.5f, 1000.0f}, {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)})}}};
}