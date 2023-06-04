#include "Vec2.h"
#include <cmath>

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float xin, float yin) : x(xin), y(yin) {}

bool Vec2::operator == (const Vec2& rhs) const
{
    return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
    return !(*this == rhs);
}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator * (const float val) const
{
    return Vec2(x * val, y * val);
}

Vec2 Vec2::operator / (const float val) const
{
    return Vec2(x / val, y / val);
}

void Vec2::operator += (const Vec2& rhs)
{
    x += rhs.x;
    y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator *= (const float val)
{
    x *= val;
    y *= val;
}

void Vec2::operator /= (const float val)
{
    x /= val;
    y /= val;
}

float Vec2::dist(const Vec2& rhs) const
{
    float dx = x - rhs.x;
    float dy = y - rhs.y;
    return std::sqrt(dx * dx + dy * dy);
}

float Vec2::length() const
{
    return std::sqrt(x * x + y * y);
}

void Vec2::normalize()
{
    float len = length();
    if (len != 0)
    {
        x /= len;
        y /= len;
    }
}

// function to rotate a vector by angle theta in degrees
void Vec2::rotate(float theta)
{
    float rad = theta * 3.14159265 / 180.0f;
    float cosine = cos(rad);
    float sine = sin(rad);

    float xnew = x * cosine - y * sine;
    float ynew = x * sine + y * cosine;

    x = xnew;
    y = ynew;
}