#include "Physics.h"
#include "Components.h"
#include "Entity.h"

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    // TODO: return the overlap rectangle size of the bounding boxes of entity a and b
    if (!a->hasComponent<CBoundingBox>() || !b->hasComponent<CBoundingBox>()) return Vec2(0, 0);
    float x1 = a->getComponent<CTransform>().pos.x; 
    float y1 = a->getComponent<CTransform>().pos.y; 
    float x2 = b->getComponent<CTransform>().pos.x; 
    float y2 = b->getComponent<CTransform>().pos.y;

    float h1 = a->getComponent<CBoundingBox>().halfSize.y;
    float w1 = a->getComponent<CBoundingBox>().halfSize.x;
    float h2 = b->getComponent<CBoundingBox>().halfSize.y;
    float w2 = b->getComponent<CBoundingBox>().halfSize.x;

    Vec2 delta = {(float)abs(x1 - x2), (float)abs(y1 - y2)};
    float ox = w1 + w2 - delta.x;
    float oy = h1 + h2 - delta.y;
    
    return Vec2(ox, oy);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    // TODO: return the previous overlap rectangle size of the bounding boxes of entity a and b
    //       previous overlap uses the entity's previous position
    if (!a->hasComponent<CBoundingBox>() || !b->hasComponent<CBoundingBox>()) return Vec2(0, 0);
    float x1 = a->getComponent<CTransform>().prevPos.x; 
    float y1 = a->getComponent<CTransform>().prevPos.y; 
    float x2 = b->getComponent<CTransform>().prevPos.x;
    float y2 = b->getComponent<CTransform>().prevPos.y;

    float h1 = a->getComponent<CBoundingBox>().halfSize.y;
    float w1 = a->getComponent<CBoundingBox>().halfSize.x;
    float h2 = b->getComponent<CBoundingBox>().halfSize.y;
    float w2 = b->getComponent<CBoundingBox>().halfSize.x;

    Vec2 delta = {(float)abs(x1 - x2), (float)abs(y1 - y2)};
    float ox = w1 + w2 - delta.x;
    float oy = h1 + h2 - delta.y;
    
    return Vec2(ox, oy);
}