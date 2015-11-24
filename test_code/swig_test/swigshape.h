#ifndef TEST_CODE_SWIG_TEST_SHAPE_H
#define TEST_CODE_SWIG_TEST_SHAPE_H
#pragma once

class Shape {
public:
    Shape() {
        nshapes++;
    }
    virtual ~Shape() {
        nshapes--;
    };
    double  x, y;
    void    move(double dx, double dy);
    virtual double area() = 0;
    virtual double perimeter() = 0;
    int nshapes;
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) { };
    ~Circle() {};
    virtual double area();
    virtual double perimeter();
};

class Square : public Shape {
private:
    double width;
public:
    Square(double w) : width(w) { };
    ~Square() {};
    virtual double area();
    virtual double perimeter();
};

#endif // TEST_CODE_SWIG_TEST_SHAPE_H
