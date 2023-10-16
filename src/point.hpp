#ifndef pointhppincluded
#define pointhppincluded

#include <iostream>

class Point {
 public:
    Point(float x, float y);
    Point(const Point& other);
    Point& operator=(const Point& other);
    Point operator+(const Point& other) const;
    Point& operator+=(const Point& other);
    Point operator-(const Point& other) const;
    Point operator*(const float scalar) const;
    Point operator/(const float scalar) const;
    bool operator==(const Point& other) const;

    bool operator<(const Point& other) const;
    bool operator>(const Point& other) const;
    bool operator>=(const Point& other) const;
    bool operator<=(const Point& other) const;
    float length() const;
    std::string tostr(int precision) const;

    friend std::ostream& operator<<(std::ostream& os, const Point& p);
    friend class Turtle;

 private:
    float x_;
    float y_;
};

#endif
