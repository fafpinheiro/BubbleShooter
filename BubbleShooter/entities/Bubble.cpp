#include "Bubble.h"
#include <iostream>

Bubble::Bubble(float x, float y, float scale, BubbleColor color)
{
    m_X = x;
    m_Y = y;
    m_Scale = scale;
    m_Color = color;
}

Bubble::~Bubble()
{
}

void Bubble::Update(float dt)
{
    if (m_State == BubbleState::Moving)
    {
        m_X += m_VelocityX * dt;
        m_Y += m_VelocityY * dt;
    }

    // Animation update
    m_AnimationTimer += dt;
    if (m_AnimationTimer >= m_FrameTime)
    {
        m_CurrentFrame = (m_CurrentFrame + 1) % m_TotalFrames;
        m_AnimationTimer = 0.0f;
    }
}

void Bubble::SetPosition(float x, float y)
{
    m_X = x;
    m_Y = y;
}

void Bubble::SetScale(float scale)
{
    m_Scale = scale;
}

void Bubble::SetVelocity(float vx, float vy)
{
    m_VelocityX = vx;
    m_VelocityY = vy;
    m_State = BubbleState::Moving;
}

void Bubble::BounceX(float bounceMultiplier)
{
    m_VelocityX = -m_VelocityX * bounceMultiplier;
}

void Bubble::BounceY(float bounceMultiplier)
{
    m_VelocityY = -m_VelocityY * bounceMultiplier;

}

void Bubble::StopAtGrid()
{
    m_VelocityX = 0.0f;
    m_VelocityY = 0.0f;
    m_State = BubbleState::InGrid;
}
