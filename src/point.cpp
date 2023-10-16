#include <math.h>
#include "point.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

Point::Point(float x, float y) : x_{x}, y_{y} {
    // nothing (else) to do
}

Point::Point(const Point& other) : x_{other.x_}, y_{other.y_} {
    // nothing else to do
}

string Point::tostr(int precision) const {
    if (precision >= 0) {
        std::stringstream x_as_str;
        std::stringstream y_as_str;
        x_as_str << std::fixed << std::setprecision(precision) << x_;
        y_as_str << std::fixed << std::setprecision(precision) << y_;
        return x_as_str.str() + "x" + y_as_str.str();
    } else {
        return to_string(x_) + "x" + to_string(y_);
    }
}

Point& Point::operator=(const Point& other) {
    x_ = other.x_;
    y_ = other.y_;
    return *this;
}

Point& Point::operator+=(const Point& other) {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
}

Point Point::operator+(const Point& other) const {
    return Point(x_ + other.x_, y_ + other.y_);
}

Point Point::operator-(const Point& other) const {
    return Point(x_ - other.x_, y_ - other.y_);
}

Point Point::operator*(const float scalar) const {
    return Point(x_ * scalar, y_ * scalar);
}

Point Point::operator/(const float scalar) const {
    return Point(x_ / scalar, y_ / scalar);
}

bool Point::operator==(const Point& other) const {
    return ((x_ == other.x_) && (y_ == other.y_));
}

bool Point::operator<(const Point& other) const {
    if (x_ != other.x_) {
        return x_ < other.x_;
    }
    return y_ < other.y_;
}

bool Point::operator>(const Point& other) const {
    if (x_ != other.x_) {
        return x_ > other.x_;
    }
    return y_ > other.y_;
}

bool Point::operator>=(const Point& other) const {
    if (x_ != other.x_) {
        return x_ >= other.x_;
    }
    return y_ >= other.y_;
}

bool Point::operator<=(const Point& other) const {
    if (x_ != other.x_) {
        return x_ <= other.x_;
    }
    return y_ <= other.y_;
}

float Point::length() const {
    return sqrt(pow(x_, 2) + pow(y_, 2));
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "Point(" << p.x_ << "," << p.y_ << ")";
    return os;
}
