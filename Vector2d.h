#ifndef VECTOR2D_H
#define VECTOR2D_H


class Vector2d
{

public:
    float x, y;


    Vector2d();
    Vector2d(float x, float y);
    ~Vector2d();


    Vector2d& Add(const Vector2d& v);
    Vector2d& Subtract(const Vector2d& v);
    Vector2d& Multipy(const Vector2d& v);
    Vector2d& Divide(const Vector2d& v);

    friend Vector2d& operator+(Vector2d& v1, const Vector2d& v2);
    friend Vector2d& operator-(Vector2d& v1, const Vector2d& v2);
    friend Vector2d& operator*(Vector2d& v1, const Vector2d& v2);
    friend Vector2d& operator/(Vector2d& v1, const Vector2d& v2);

    Vector2d& operator+=(Vector2d& v2);
    Vector2d& operator-=(Vector2d& v2);
    Vector2d& operator*=(Vector2d& v2);
    Vector2d& operator/=(Vector2d& v2);
};



#endif