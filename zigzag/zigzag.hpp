#ifndef ZIGZAG_HPP
#define ZIGZAG_HPP

#include "../src/turtle.hpp"

void zigzag(Turtle& t, const Point& start, const Point& end, float amplitude, float segmentLength);

void testZigzagLowAmplitude();
void testZigzagHighAmplitude();
void testZigzagShortSegment();
void testZigzagLongSegment();

void testHalfCircles();


#endif // ZIGZAG_HPP
