#include "Vector2d.h"

Vector2d::Vector2d()
{

    x = 0;
    y = 0;

}
Vector2d::Vector2d(float x, float y)
{
    this->x = x;
    this->y = y;
}
Vector2d::~Vector2d()
{



}

Vector2d& Vector2d::Add(const Vector2d& v)
{
    this->x += v.x;
    this->y += v.y;

    return *this;

}
Vector2d& Vector2d::Subtract(const Vector2d& v)
{

    this->x -= v.x;
    this->y -= v.y;

    return *this;

}
Vector2d& Vector2d::Multipy(const Vector2d& v)
{
    this->x *= v.x;
    this->y *= v.y;

    return *this;


}
Vector2d& Vector2d::Divide(const Vector2d& v)
{

    this->x /= v.x;
    this->y /= v.y;

    return *this;

}

Vector2d& operator+(Vector2d& v1, const Vector2d& v2)
{
    return v1.Add(v2);
}
Vector2d& operator-(Vector2d& v1, const Vector2d& v2)
{
    return v1.Subtract(v2);


}
Vector2d& operator*(Vector2d& v1, const Vector2d& v2)
{

    return v1.Multipy(v2);

}
Vector2d& operator/(Vector2d& v1, const Vector2d& v2)
{

    return v1.Divide(v2);

}

Vector2d& Vector2d::operator+=(Vector2d& v2)
{

    return this->Add(v2);

}
Vector2d& Vector2d::operator-=(Vector2d& v2)
{

    return this->Subtract(v2);


}
Vector2d& Vector2d::operator*=(Vector2d& v2)
{

    return this->Multipy(v2);


}
Vector2d& Vector2d::operator/=(Vector2d& v2)
{

    return this->Divide(v2);


}