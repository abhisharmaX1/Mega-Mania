#include "Animation.h"
#include <cmath>
#include <iostream>


Animation::Animation()
    : m_frameCount(0), m_speed(0), m_currentFrame(0), m_name("none")
{
    // Default constructor implementation
}

Animation::Animation(const std::string& name, const sf::Texture& texture)
    : m_frameCount(1), m_speed(1), m_currentFrame(0), m_name(name)
{
    m_size = Vec2(texture.getSize().x, texture.getSize().y);
    m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
    m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
}

Animation::Animation(const std::string& name, const sf::Texture& texture, size_t frameCount, size_t speed)
    : m_frameCount(frameCount), m_speed(speed), m_currentFrame(0), m_name(name), m_sprite(texture)
{
    m_size = Vec2((float)texture.getSize().x / frameCount, (float)texture.getSize().y);
    m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
    m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
}


// update the animation to show the next frame, depending on its speed
// animation loops when it reaches the end
void Animation::update()
{
    m_currentFrame++;
    if (m_speed == 0) return;
    int frame = (m_currentFrame / m_speed) % m_frameCount; // safe code would check for 'divide by zero' error
    sf::IntRect rect = sf::IntRect(frame*(m_size.x), 0, m_size.x, m_size.y);
    
    m_sprite.setTextureRect(rect);

}

bool Animation::hasEnded() const
{
    // TODO: detect when animation has ended (last frame was played) and return true
    if (m_speed == 0) return false;
    if (m_currentFrame >= m_frameCount * m_speed) {
        return true;
    }
    return false;
}

const std::string& Animation::getName() const
{
    return m_name;
}

const Vec2& Animation::getSize() const
{
    return m_size;
}

sf::Sprite& Animation::getSprite()
{
    return m_sprite;
}
