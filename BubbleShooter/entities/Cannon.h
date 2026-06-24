#pragma once
#include <algorithm>
#include <iostream>

const float MIN_ANGLE = -170.0f;//-90;//-3.0f; //DELL
const float MAX_ANGLE = -10.0f;//90;// -0.1f;    //DELL
const float PI = 3.14159265f;

class Cannon
{
public:
    Cannon(float x, float y, float rotationSpeed);
    ~Cannon();

    void Update(float targetX, float targetY, float deltaTime);

    // Set cannon pos
    void SetPosition(float x, float y);

    // Set cannon angle
    void SetAngle(float angle);
    // Set cannon rotation speed
    void SetRotationSpeed(float rotationSpeed);

    float GetAngle() const { return m_Angle; };
    float GetX() const { return m_X; };
    float GetY() const { return m_Y; };
    float GetRotationSpeed() const { return m_RotationSpeed; };

private:
    float m_X = 0.0f;   //Cannon posx
    float m_Y = 0.0f;   //Cannon posy

    float m_Angle = -90.0f;         // pointing up
    float m_RotationSpeed = 300.0f; // degrees per second
};