#include "Cannon.h"
#include "Input/Input.h"

Cannon::Cannon(float x, float y, float rotationSpeed)
{
    m_X = x;
    m_Y = y;
    m_RotationSpeed = rotationSpeed;
}

Cannon::~Cannon()
{
}

void Cannon::Update(float targetX, float targetY, float deltaTime)
{

    // Rotate cannon to point at mouse

    // Compute target angle
    float dx = targetX - m_X;
    float dy = targetY - m_Y;
    float targetAngle = atan2(dy, dx) * 180.0f / PI;

    // Compute shortest angular difference
    float deltaAngle = targetAngle - m_Angle;
    // Rotation adjustment
    while (deltaAngle > 180.0f) deltaAngle -= 360.0f;
    while (deltaAngle < -180.0f) deltaAngle += 360.0f;

    // Rotate toward target
    float rotationStep = m_RotationSpeed * deltaTime;
    if (fabs(deltaAngle) <= rotationStep)
    {
        m_Angle = targetAngle; // reached target
    }
    else 
    {
        m_Angle += (deltaAngle > 0 ? 1 : -1) * rotationStep;
    }

    // Clamp max min angle
    while (m_Angle > MAX_ANGLE) m_Angle= MAX_ANGLE;
    while (m_Angle < MIN_ANGLE) m_Angle = MIN_ANGLE;

}

void Cannon::SetPosition(float x, float y)
{
    m_X = x;
    m_Y = y;
}

void Cannon::SetAngle(float angle)
{
    m_Angle = angle;
}

void Cannon::SetRotationSpeed(float rotationSpeed)
{
    m_RotationSpeed = rotationSpeed;
}
