// Copyright (c) 2015, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2015-09-10
// Description:
//

#include <iostream>
#include "swigshape.h"

void Shape::move(double dx, double dy)
{
    std::cout << "Shape: dx: " << dx << " dy:" << dy << std::endl;
}

// Circle::~Circle() {}

double Circle:: area()
{
    std::cout << "Circle area " << 3.14 * radius * radius << std::endl;
    return 3.14 * radius * radius;
}

double  Circle::perimeter()
{
    std::cout << "Circle perimeter " << 2 * 3.14 * radius << std::endl;
    return 2 * 3.14 * radius;
}

// Square::~Square() {}

double Square::area()
{
    std::cout << "Square area " << width * width << std::endl;
    return width * width;
}

double Square::perimeter()
{
    std::cout << "Square perimeter " << 4 * width << std::endl;
    return 4 * width;
}
