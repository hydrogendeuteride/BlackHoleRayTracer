#ifndef BLACKHOLERAYTRACER_FREECAMERA_H
#define BLACKHOLERAYTRACER_FREECAMERA_H

#include "cameraBase.h"

class FreeCamera : public CameraBase
{
public:
    FreeCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float roll = ROLL)
            : CameraBase(position, up, yaw, pitch, roll) {}

    glm::mat4 getViewMatrix() override
    {
        updateCameraVectors();
        return glm::transpose(glm::lookAt(position, position + front, up));
    }

    void processKeyboard(CameraMovement direction, float deltaTime) override
    {
        float velocity = movementSpeed * deltaTime;
        if(direction == FORWARD)
            position -= front * velocity;
        if(direction == BACKWARD)
            position += front * velocity;
        if(direction == LEFT)
            position -= right * velocity;
        if(direction == RIGHT)
            position += right * velocity;
        if(direction == UP)
            position += up * velocity;
        if(direction == DOWN)
            position -= up * velocity;

        if(direction == ROLL_LEFT)
            roll += 57.3f * deltaTime;
        if(direction == ROLL_RIGHT)
            roll -= 57.3f * deltaTime;

        updateCameraVectors();
    }

    void processMouseMovement(float xOffset, float yOffset) override
    {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw -= xOffset;
        pitch -= yOffset;

        updateCameraVectors();
    }

    void processMouseScroll(float yOffset) override{}

protected:
    void updateCameraVectors() override
    {
        glm::quat qYaw = glm::angleAxis(glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat qPitch = glm::angleAxis(glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat qRoll = glm::angleAxis(glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));

        orientation = qYaw * qPitch * qRoll;
        orientation = glm::normalize(orientation);

        front = glm::rotate(orientation, glm::vec3(0.0f, 0.0f, 1.0f));
        up = glm::rotate(orientation, glm::vec3(0.0f, 1.0f, 0.0f));
        right = glm::normalize(glm::cross(front, up));
    }
};

#endif //BLACKHOLERAYTRACER_FREECAMERA_H
