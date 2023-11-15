// zigzag.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
#include "zigzag.hpp"

using namespace std;

void zigzag(Turtle& t, const Point& start, const Point& end, float amplitude, float segmentLength) {
    t.pendown();
    t.zigzag(start, end, amplitude, segmentLength);
    t.penup();
}

// Function to test the zigzag patterns
void testZigzagLowAmplitude() {
    Turtle t;
    t.satinon(0.3);

    Point start(0, 0);
    Point end(100, 0);
    float amplitude = 5;
    float segmentLength = 20;

    zigzag(t, start, end, amplitude, segmentLength);

    t.end();
    t.save("zigzag_low_amplitude.dst");
}

void testZigzagHighAmplitude() {
    Turtle t;
    t.satinon(0.3);

    Point start(0, 0);
    Point end(100, 0);
    float amplitude = 20;
    float segmentLength = 20;

    zigzag(t, start, end, amplitude, segmentLength);

    t.end();
    t.save("zigzag_high_amplitude.dst");
}

void testZigzagShortSegment() {
    Turtle t;
    t.satinon(0.3);

    Point start(0, 0);
    Point end(100, 0);
    float amplitude = 10;
    float segmentLength = 10;

    zigzag(t, start, end, amplitude, segmentLength);

    t.end();
    t.save("zigzag_short_segment.dst");
}

void testZigzagLongSegment() {
    Turtle t;
    t.satinon(0.3);

    Point start(0, 0);
    Point end(100, 0);
    float amplitude = 10;
    float segmentLength = 30;

    zigzag(t, start, end, amplitude, segmentLength);

    t.end();
    t.save("zigzag_long_segment.dst");
}

// testing half circle function
void testHalfCircles() {
    Turtle t;
    t.satinon(0.3);

    float radius = 20;
    Point center(50, 50);

    // draws multiple half circles
    for (int i = 0; i < 3; ++i) {
        // Draw a clockwise half-circle
        t.halfCircle(center, radius, true);
        t.penup();

        // next starting point for the counterclockwise half circle
        center = center + Point(radius * 2, 0);
        t.gotopoint(center);

        // draw half circle
        t.halfCircle(center, radius, false);
        t.penup();

        center = center + Point(radius * 2, 0);
    }

    t.end();
    t.save("test_half_circles2.dst");
}



int main() {
    // testZigzagLowAmplitude();
    // testZigzagHighAmplitude();
    testZigzagShortSegment();
    // testZigzagLongSegment();
    testHalfCircles();
    return 0;
}

