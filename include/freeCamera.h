#ifndef BLACKHOLERAYTRACER_FREECAMERA_H
#define BLACKHOLERAYTRACER_FREECAMERA_H

#include "cameraBase.h"

class FreeCamera : public CameraBase
{
public:
    FreeCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float dummyYaw = YAW, float dummyPitch = PITCH, float dummyRoll = ROLL)
            : CameraBase(position, up, 0.f, 0.f, 0.f)
    {}

    glm::mat4 getViewMatrix() override
    {
        glm::vec3 frontVec = glm::rotate(orientation, glm::vec3(0.f, 0.f, 1.f));
        glm::vec3 upVec = glm::rotate(orientation, glm::vec3(0.f, 1.f, 0.f));

        return glm::lookAt(position, position + frontVec, upVec);
    }

    void processKeyboard(CameraMovement direction, float deltaTime) override
    {
        float velocity = movementSpeed * deltaTime;

        glm::vec3 frontVec = glm::rotate(orientation, glm::vec3(0.f, 0.f, 1.f));
        glm::vec3 rightVec = glm::rotate(orientation, glm::vec3(1.f, 0.f, 0.f));
        glm::vec3 upVec = glm::rotate(orientation, glm::vec3(0.f, 1.f, 0.f));

        if (direction == FORWARD)
            position += frontVec * velocity;
        if (direction == BACKWARD)
            position -= frontVec * velocity;
        if (direction == LEFT)
            position -= rightVec * velocity;
        if (direction == RIGHT)
            position += rightVec * velocity;
        if (direction == UP)
            position += upVec * velocity;
        if (direction == DOWN)
            position -= upVec * velocity;

        if (direction == ROLL_LEFT)
        {
            glm::quat rollQ = glm::angleAxis(glm::radians(57.3f * deltaTime), frontVec);
            orientation = glm::normalize(rollQ * orientation);
        }
        if (direction == ROLL_RIGHT)
        {
            glm::quat rollQ = glm::angleAxis(glm::radians(-57.3f * deltaTime), frontVec);
            orientation = glm::normalize(rollQ * orientation);
        }

        updateCameraVectors();
    }

    void processMouseMovement(float xOffset, float yOffset) override
    {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        glm::vec3 localUp = glm::rotate(orientation, glm::vec3(0.f, 1.f, 0.f));
        glm::vec3 localRight = glm::rotate(orientation, glm::vec3(1.f, 0.f, 0.f));

        glm::quat yawQ = glm::angleAxis(glm::radians(-xOffset), localUp);

        glm::quat pitchQ = glm::angleAxis(glm::radians(-yOffset), localRight);

        glm::quat combined = yawQ * pitchQ;
        orientation = glm::normalize(combined * orientation);
    }

    void processMouseScroll(float yOffset) override
    {}

protected:
    void updateCameraVectors() override
    {
        front = glm::rotate(orientation, glm::vec3(0.f, 0.f, 1.f));
        up = glm::rotate(orientation, glm::vec3(0.f, 1.f, 0.f));
        right = glm::normalize(glm::cross(front, up));
    }
};

#endif //BLACKHOLERAYTRACER_FREECAMERA_H
