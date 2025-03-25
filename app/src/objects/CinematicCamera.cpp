#include "objects/CinematicCamera.h"
#include "objects/SmoothCamera.hpp"

CinematicCamera::CinematicCamera(obj_data begining, obj_data ending, float animation_time, glm::vec3 offset, bool ret_to_pos, std::function<void()> beg_func, std::function<void()> end_func)
: begining(begining), ending(ending), animation_time(animation_time), offset(offset), ret_to_pos(ret_to_pos), beg_func(beg_func), end_func(end_func)
{
    time = 0.0f;
    ended = false;
}

void CinematicCamera::Init() {
    CameraLockScript::disabled = true;
    SmoothCamera::disabled = true;
    old_data.position = Camera::main->cameraTransform->position;
    old_data.rotation = Camera::main->cameraTransform->rotation;
    old_view = Camera::main->view;
    Camera::main->view = Perspective;
    beg_func();
}
void CinematicCamera::Update() {}

void CinematicCamera::LateUpdate() {
    if (ended)
        return;
    if (time > animation_time) {
        CameraLockScript::disabled = false;
        SmoothCamera::disabled = false;
        ended = true;
        end_func();
        if (ret_to_pos) {
            Camera::main->cameraTransform->MoveTo(old_data.position);
            Camera::main->cameraTransform->RotateTo(old_data.rotation);
            Camera::main->view = old_view;
        }
        return;
    }

    //std::cout << Camera::main->cameraTransform->position.x << ' '<< Camera::main->cameraTransform->position.y << ' '<< Camera::main->cameraTransform->position.z << ' ' << '\n';
	//std::cout << Camera::main->cameraTransform->rotation.x << ' '<< Camera::main->cameraTransform->rotation.y << '\n';

    obj_data now = position_in_time(time/animation_time);
    Camera::main->cameraTransform->MoveTo(now.position);
    Camera::main->cameraTransform->RotateTo(now.rotation);

    time += Time::deltaTime;
    //std::cout << time << '\n';
}

void CinematicCamera::OnDestroy() {}

CinematicCamera::obj_data CinematicCamera::position_in_time(float t) {
    obj_data result;
    glm::vec3 control = {
        ((begining.position.x + ending.position.x)/2)+offset.x,
        ((begining.position.y + ending.position.y)/2)+offset.y,
        ((begining.position.z + ending.position.z)/2)+offset.z,
    };
    float rt = 1.0f-t;

    result.position.x = rt*rt*begining.position.x + 2*rt*t*control.x + t*t*ending.position.x;
    result.position.y = rt*rt*begining.position.y + 2*rt*t*control.y + t*t*ending.position.y;
    result.position.z = rt*rt*begining.position.z + 2*rt*t*control.z + t*t*ending.position.z;

    {
        float delta = begining.rotation.x - ending.rotation.x;
        int k = static_cast<int>(std::round(delta / 360.0));
        const_cast<float&>(ending.rotation.x) += 360.0 * k;
    }

    {
        float delta = begining.rotation.y - ending.rotation.y;
        int k = static_cast<int>(std::round(delta / 360.0));
        const_cast<float&>(ending.rotation.y) += 360.0 * k;
    }

    result.rotation.x = begining.rotation.x + t * (ending.rotation.x - begining.rotation.x);
    result.rotation.y = begining.rotation.y + t * (ending.rotation.y - begining.rotation.y);
    return result;
}

bool CinematicCamera::hasEnded() {
    return ended;
}

void CinematicCamera::skip() {
    time = animation_time;
}