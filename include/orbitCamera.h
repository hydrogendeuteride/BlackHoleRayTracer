#ifndef BLACKHOLERAYTRACER_ORBITCAMERA_H
#define BLACKHOLERAYTRACER_ORBITCAMERA_H

#include "cameraBase.h"

class OrbitCamera : public CameraBase
{
public:
    glm::vec3 target;
    float distance;

    OrbitCamera(glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f), float distance = 10.0f,
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
            : CameraBase(target, up, yaw, pitch, 0.0f), target(target), distance(distance)
    {}

    glm::mat4 getViewMatrix() override
    {
        glm::vec3 position = calculatePosition();
        return glm::lookAt(position, target, up);
    }

    void processMouseMovement(float xOffset, float yOffset) override
    {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        updateCameraVectors();
    }

protected:
    void updateCameraVectors() override
    {
        glm::quat qYaw = glm::angleAxis(glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat qPitch = glm::angleAxis(glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

        orientation = qPitch * qYaw;
        orientation = glm::normalize(orientation);

        glm::vec3 cameraPos = calculatePosition();
        front = glm::normalize(target - cameraPos);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

private:
    glm::vec3 calculatePosition()
    {
        glm::vec3 position;
        position.x = target.x + distance * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        position.y = target.y + distance * sin(glm::radians(pitch));
        position.z = target.z + distance * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        return position;
    }
};

#endif //BLACKHOLERAYTRACER_ORBITCAMERA_H