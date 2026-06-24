#pragma once

enum class BubbleColor      // Types of bubble color
{
    Red,
    Green,
    Blue,
    Yellow
};

enum class BubbleState      // Possible bubble states
{
    Moving,
    InGrid,
    Ready
};
class Bubble
{
public:
    Bubble(float x, float y, float scale, BubbleColor color);
    ~Bubble();

    void Update(float dt);

    // Set bubble position
    void SetPosition(float x, float y);
    // Set bubble scale
    void SetScale(float scale);
    // Set bubble velocity
    void SetVelocity(float vx, float vy);
    // Refletes Velocity x axis
    void BounceX(float bounceMultiplier);
    // Refletes Velocity y axis
    void BounceY(float bounceMultiplier);
    // Stops Bubble in grid cell
    void StopAtGrid();

    float GetX() const { return m_X; }
    float GetY() const { return m_Y; }
    float GetScale() const { return m_Scale; }
    BubbleColor GetColor() const { return m_Color; }
    BubbleState GetState() const { return m_State; }

private:
    // Position
    float m_X;
    float m_Y;

    // Scale;
    float m_Scale;

    // Color
    BubbleColor m_Color;

    // Movement
    float m_VelocityX = 0.0f;
    float m_VelocityY = 0.0f;
    BubbleState m_State = BubbleState::Ready;

    // Animation
    int m_CurrentFrame = 0;
    int m_TotalFrames = 4;  // number of frames per bubble animation
    float m_FrameTime = 0.1f;
    float m_AnimationTimer = 0.0f;
};