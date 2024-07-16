#ifndef BLACKHOLERAYTRACER_ORBITCAMERA_H
#define BLACKHOLERAYTRACER_ORBITCAMERA_H

#include <iostream>
#include "cameraBase.h"

class OrbitCamera : public CameraBase
{
public:
    glm::vec3 target;
    float distance;

    OrbitCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH,
                glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f))
            : CameraBase(position, up, yaw, pitch, 0.0f), target(target)
    {
        distance = glm::sqrt(glm::dot(position - target, position - target));
        calculatePosition();
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() override
    {
        return glm::transpose(glm::lookAt(-position, target, worldUp));
    }

    void processMouseMovement(float xOffset, float yOffset) override
    {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (pitch > 89.9f)
            pitch = 89.9f;
        if (pitch < -89.9f)
            pitch = -89.9f;

        calculatePosition();
        updateCameraVectors();
    }

    void processKeyboard(CameraMovement direction, float deltaTime) override{}
    void processMouseScroll(float yOffset) override{}

protected:
    void updateCameraVectors() override
    {
        front = glm::normalize(position - target);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

private:
    void calculatePosition()
    {
        position.x = target.x + distance * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        position.y = target.y + distance * sin(glm::radians(pitch));
        position.z = target.z + distance * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    }
};

#endif //BLACKHOLERAYTRACER_ORBITCAMERA_H