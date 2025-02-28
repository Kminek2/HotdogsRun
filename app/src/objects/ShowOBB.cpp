#include "objects/ShowOBB.h"

void ShowOBB::Init() {}
void ShowOBB::Update() {
    DebugPoints::ClearLines();
}

void ShowOBB::LateUpdate() {
    std::vector<Vertex> corners;
    std::vector<uint32_t> indices;
	for (const auto& obb_rel : gameObject->obbs) {
        OBB obb = Collisions::getAbsOBB(obb_rel, *gameObject);
        for (int i = 0; i < 8; ++i) {
            const float sx = (i & 1) ? 1.0f : -1.0f;
            const float sy = (i & 2) ? 1.0f : -1.0f;
            const float sz = (i & 4) ? 1.0f : -1.0f;

            const glm::vec3 offset = 
                obb.axes[0] * (sx * obb.sizes.x) +
                obb.axes[1] * (sy * obb.sizes.y) +
                obb.axes[2] * (sz * obb.sizes.z);

            corners.push_back({obb.center + offset, {0.0, 0.0}, {1,1,1}});
        }
        indices.push_back(corners.size()-1);
        indices.push_back(corners.size()-2);
        indices.push_back(corners.size()-3);
        indices.push_back(corners.size()-1);
        indices.push_back(corners.size()-3);
        indices.push_back(corners.size()-7);
        indices.push_back(corners.size()-2);
        indices.push_back(corners.size()-4);
        indices.push_back(corners.size()-3);
        indices.push_back(corners.size()-4);
        indices.push_back(corners.size()-4);
        indices.push_back(corners.size()-8);
        indices.push_back(corners.size()-7);
        indices.push_back(corners.size()-8);
        indices.push_back(corners.size()-7);
        indices.push_back(corners.size()-5);
        indices.push_back(corners.size()-1);
        indices.push_back(corners.size()-5);
        indices.push_back(corners.size()-6);
        indices.push_back(corners.size()-5);
        indices.push_back(corners.size()-6);
        indices.push_back(corners.size()-2);
        indices.push_back(corners.size()-6);
        indices.push_back(corners.size()-8);
    }
    DebugPoints::AddLines(corners, indices);
}
void ShowOBB::OnDestroy() {}