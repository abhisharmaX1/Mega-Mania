#pragma once

#include "Vec2.h"
#include <memory>

class Entity;

class Physics
{
public:
    static Vec2 GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
    static Vec2 GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};
