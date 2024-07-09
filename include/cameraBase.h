#ifndef BLACKHOLERAYTRACER_CAMERA_H
#define BLACKHOLERAYTRACER_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ROLL_LEFT,
    ROLL_RIGHT
};

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float ROLL = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

class CameraBase
{
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw, pitch, roll;
    float movementSpeed;

    float mouseSensitivity;
    float zoom;

    CameraBase(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float roll = ROLL)
            : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->roll = roll;
    }

    virtual glm::mat4 getViewMatrix() = 0;

    virtual void processKeyboard(CameraMovement direction, float deltaTime) = 0;

    virtual void processMouseMovement(float xOffset, float yOffset) = 0;

    virtual void processMouseScroll(float yOffset) = 0;

protected:
    glm::quat orientation;

    virtual void updateCameraVectors() = 0;
};

#endif //BLACKHOLERAYTRACER_CAMERA_H
