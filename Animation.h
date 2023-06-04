#pragma once

#include "Vec2.h"


#include <vector>
#include <SFML/Graphics.hpp>

class Animation
{
    sf::Sprite m_sprite;
    Vec2 m_size                 = {1, 1}; // the size of the animation frame
    size_t m_currentFrame       = 0; // the current frame of animation being played
    size_t m_speed              = 0;  // the speed to play this animation
    size_t m_frameCount         = 1; //total number of frames of animation 
    std::string m_name          = "none";

public:

    Animation();
    Animation(const std::string& name, const sf::Texture& texture);
    Animation(const std::string& name, const sf::Texture& texture, size_t frameCount, size_t speed);

    void update();
    bool hasEnded() const;
    const std::string& getName() const;
    const Vec2& getSize() const;
    sf::Sprite& getSprite();
};