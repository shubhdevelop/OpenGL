#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    static Camera &getInstance()
    {
        static Camera instance;
        return instance;
    }

    void update(float deltaTime)
    {
        if (moveForward)
            position += front * movementSpeed * deltaTime;
        if (moveBackward)
            position -= front * movementSpeed * deltaTime;
        if (moveLeft)
            position -= right * movementSpeed * deltaTime;
        if (moveRight)
            position += right * movementSpeed * deltaTime;
        if (moveUp)
            position += worldUp * movementSpeed * deltaTime;
        if (moveDown)
            position -= worldUp * movementSpeed * deltaTime;
    }

    void setKeyState(int key, bool pressed)
    {
        switch (key)
        {
        case 'W':
            moveForward = pressed;
            break;
        case 'S':
            moveBackward = pressed;
            break;
        case 'A':
            moveLeft = pressed;
            break;
        case 'D':
            moveRight = pressed;
            break;
        case 'Q':
            moveDown = pressed;
            break;
        case 'E':
            moveUp = pressed;
            break;
        }
    }

    void processMouse(float xOffset, float yOffset)
    {
        yaw += xOffset * mouseSensitivity;
        pitch += yOffset * mouseSensitivity;
        pitch = glm::clamp(pitch, -89.0f, 89.0f);
        updateCameraVectors();
    }

    void processScroll(float yOffset)
    {
        fov = glm::clamp(fov - yOffset, 1.0f, 120.0f);
    }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(position, position + front, camUp);
    }

    glm::mat4 getProjectionMatrix(float aspectRatio) const
    {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    glm::vec3 getPosition() const { return position; }

    bool *getCursorEnabledPtr() { return &cursorEnabled; }
    bool *getFirstMousePtr() { return &firstMouse; }
    float *getLastXPtr() { return &lastX; }
    float *getLastYPtr() { return &lastY; }

private:
    Camera()
        : position(0.0f, 0.0f, 3.0f), worldUp(0.0f, 1.0f, 0.0f), yaw(-90.0f), pitch(0.0f), movementSpeed(2.5f), mouseSensitivity(0.1f), fov(45.0f), nearPlane(0.1f), farPlane(100.0f), cursorEnabled(true), firstMouse(true), lastX(400.0f), lastY(400.0f), moveForward(false), moveBackward(false), moveLeft(false), moveRight(false), moveUp(false), moveDown(false)
    {
        updateCameraVectors();
    }

    void updateCameraVectors()
    {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, worldUp));
        camUp = glm::normalize(glm::cross(right, front));
    }

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 camUp;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float fov;
    float nearPlane;
    float farPlane;
    bool cursorEnabled;
    bool firstMouse;
    float lastX;
    float lastY;
    bool moveForward;
    bool moveBackward;
    bool moveLeft;
    bool moveRight;
    bool moveUp;
    bool moveDown;
};