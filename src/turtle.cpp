#include <iostream>
#include <math.h>
#include <algorithm>
#include <string>

#include "format-dst.h"
#include "emb-pattern.h"
#include "turtle.hpp"
#include "alphabet.hpp"

using namespace std;

// Constructors, destructors

/** Constructor: Only a default constructor is provided. */
Turtle::Turtle()
    : emb_{embPattern_create()},
      stepsize_{2},
      color_{0},
      pen_is_down_{false},
      satin_is_on_{false},
      satin_delta_{.5},
      dir_{Point(1, 0)},
      position_{Point(0, 0)},
      density_error_{false},
      density_warning_{false} {
    embPattern_addThread(
        emb_, EmbThread{embColor_make(0, 0, 0), "Default color", "0"});
    embPattern_changeColor(emb_, 1);
}

/** Destructor: The destructor calls the appropriate libembroidery cleanup. */
Turtle::~Turtle() {
    embPattern_free(emb_);
}

// Functions for setting properties of Turtle

/** Sets the distance that the Turtle will move without putting the needle down.
 * When satin stitch is off, this sets how far the needle will travel for each
 stitch. For example, if step size is set to 2 and the Turtle moves 10, stitches
 will be added at 2, 4, 6, 8, and 10.
 * When satin sttich is on, this sets the *width* of the satin stitch.
*/
void Turtle::setStepSize(float step) {
    stepsize_ = step;
}

/** Enables satin stitch mode.
 * When satin stitch is on, the Turtle will move side to side,
 * creating the effect of a wider line of thread.
 */
void Turtle::satinon(float delta) {
    satin_is_on_ = true;
    satin_delta_ = delta;
}

/** Disables satin stitch mode.
 * When satin stitch is off, the Turtle will move in a straight
 * stitch line, using the current step size.
 */
void Turtle::satinoff() {
    satin_is_on_ = false;
}

/** Puts the turtle's pen down.
 * When the pen is down, the turtle will add stitches along any
 * path it follows.
 */
void Turtle::pendown() {
    pen_is_down_ = true;
}

/** Puts the turtle's pen up.
 * When the pen is up, the turtle will "jump" from one location
 * to another without adding any stitches.
 */
void Turtle::penup() {
    pen_is_down_ = false;
}

/** Returns the Turtle's current position.
 * The position is returned as a Point object.
 */
Point Turtle::position() {
    return position_;
}

// Functions for turning the Turtle

/** Turn to face `pos`.
 * The turtle's direction is changed so that it is facing in the direction of
 * the Point `pos`. The turtle's location is not changed.
 */
void Turtle::turntoward(const Point& pos) {
    Point delta = pos - position_;
    dir_ = delta.normalize(1);
}

/** Turn to face `(x,y)`.
 * The turtle's direction is changed so that it is facing in the direction of
 * the point `(x,y)`. The turtle's location is not changed.
 */

void Turtle::turntoward(const float x, const float y) {
    turntoward(Point(x, y));
}

/** Directly set the Turtle's direction.
 * The direction is set to the (normalized) value of
 * the point `pos`.
 */
void Turtle::setdir(const Point& pos) {
    dir_ = pos.normalize(1);
}

/** Directly set the Turtle's direction.
 * The direction is set to the (normalized) value of
 * the point `(x,y)`.
 */
void Turtle::setdir(const float x, const float y) {
    setdir(Point(x, y));
}

/** Turns the Turle `degreesccw` degrees. */
void Turtle::turn(const float degreesccw) {
    float radcw = -degreesccw / 180.0 * 3.141592653589;
    dir_ = Point(cos(radcw) * dir_.x_ - sin(radcw) * dir_.y_,
                 sin(radcw) * dir_.x_ + cos(radcw) * dir_.y_);
}

/** Turns the turtle right `degreesccw`.
 * This is a convenience alias for `turn`.
 */
void Turtle::right(float degreesccw) {
    turn(degreesccw);
}

/** Turns the turtle left `degreesccw`.
 * This is a convenience alias for `turn(-degreesccw)`.
 */
void Turtle::left(float degreesccw) {
    turn(-degreesccw);
}

// Functions for moving the Turtle

/** Relative move from Turtle's current position
 * Moves the Turtle to its current location + `delta`,
 * using the Turtle's current move settings.
 */
void Turtle::move(const Point& delta) {
    gotopoint(position_ + delta);
}

/** Relative move from Turtle's current position
 * Moves the Turtle to its current location + `Point(x, y)`,
 * using the Turtle's current move settings. Convenience
 * wrapper to the version of move that takes a Point as its
 * only argument.
 */

void Turtle::move(const float x, const float y) {
    move(Point(x, y));
}

/** Absolute move
 * Moves the Turtle to the point `(x, y)` using the Turtle's
 * current move settings.
 */
void Turtle::gotopoint(const float x, const float y) {
    gotopoint(Point(x, y));
}

/** Absolute move
 * Moves the Turtle to the point `(pos)` using the Turtle's
 * current move settings.
 */
void Turtle::gotopoint(const Point& pos) {
    // if (pos == position_) {
    //    return;
    //}
    if (pen_is_down_) {
        if (satin_is_on_) {
            satin_stitch_to(pos);
        } else {
            stitch_to(pos);
        }
    } else {
        jump_to(pos);
    }
}

/** Relative move forward
 * Moves the turtle `dist` in the direction it
 * is currently facing.
 */
void Turtle::forward(const float dist) {
    move(dir_ * dist);
}

/** Relative move backward
 * Moves the turtle `-dist` in the direction it
 * is currently facing.
 */
void Turtle::backward(const float dist) {
    move(dir_ * dist * -1);
}

// Functions for drawing text

/** Draws the text `message`.
 * All letters are upper-cased.
 * `x` and `y` determine the location where the text will be drawn.
 * They behave differently depending on `scale`.
 * Scale determines the size of the letters that are drawn.
 */

bool notInAlphabet(char c) {
    return Alphabet.find(c) == Alphabet.end();
}

void Turtle::displayMessage(string message, float scale) {
    transform(message.begin(), message.end(), message.begin(), ::toupper);
    message.erase(remove_if(message.begin(), message.end(), notInAlphabet),
                  message.end());

    bool pen_was_down = pen_is_down_;
    float start_y = position_.y_;
    float max_x = position_.x_;

    for (char& nextChar : message) {
        penup();
        for (const Point& p : Alphabet[nextChar]) {
            move(p * scale);
            if (position_.x_ > max_x)
                max_x = position_.x_;
            pen_is_down_ = pen_was_down;
        }
        penup();  // raise pen to jump to next letter
        gotopoint(max_x + 3 * scale, start_y);
        pen_is_down_ = pen_was_down;
    }
}

// Utility functions for pringint, ending and saving embroidery files

/** Print a Turtle object.
 * Useful for debugging.
 */
std::ostream& operator<<(std::ostream& os, const Turtle& t) {
    os << "Turtle(position=" << t.position_ << ",Direction=" << t.dir_ << ")";
    return os;
}

/** Add an end marker to the embroidery file.
 * This is very important -- without it, the embroidery machine
 * doesn't read the file correctly.
 */
void Turtle::end() {
    embPattern_addStitchRel(emb_, 0, 0, END, color_);
}

/** Save to `fname`.
 * Writes the Turtle's moves to an embroidery file called `fname`.
 * The extension on `fname` determines the embroidery format that is used.
 * For CS70, we will always use the `.dst` extension.
 */
void Turtle::save(std::string fname) {
    if (density_error_) {
        cerr << "Not writing output file because density errors occurred:"
             << endl;
        for (const auto& entry : density_) {
            if (entry.second > DENSITY_ERROR_LIMIT) {
                cerr << "    " << entry.second << " stitches at " << entry.first
                     << endl;
            }
        }
        return;
    }

    if (density_warning_) {
        cerr << "Use caution with output file; density warnings occurred:"
             << endl;
        for (const auto& entry : density_) {
            if (entry.second > DENSITY_WARN_LIMIT) {
                cerr << "    * " << entry.second << " stitches at "
                     << entry.first << endl;
            }
        }
    }
    writeDst(emb_, fname.c_str());
}

// Lower-level functions, set to private

void Turtle::stitch(const Point& pos) {
    embPattern_addStitchRel(emb_, pos.x_, pos.y_, 0, color_);
    check_density(position_ + pos);
    position_ += pos;
}

void Turtle::stitch_abs(const Point& pos) {
    embPattern_addStitchAbs(emb_, pos.x_, pos.y_, 0, color_);
    check_density(pos);
    position_ = pos;
}

void Turtle::jump_to(const Point& pos) {
    embPattern_addStitchAbs(emb_, pos.x_, pos.y_, JUMP, color_);
    // check_density(pos);
    position_ = pos;
}

void Turtle::stitch_to(const Point& pos) {
    float total_length = (position_ - pos).length();
    size_t num_stitches = abs(int(total_length / stepsize_));

    Point step = (pos - position_) / num_stitches;

    for (size_t i = 0; i < num_stitches; ++i) {
        stitch(step);
    }
    stitch_abs(pos);
}

void Turtle::satin_stitch_to(const Point& pos) {
    float total_length = (position_ - pos).length();
    size_t num_stitches = abs(int(total_length / satin_delta_));

    Point step = (pos - position_) / num_stitches;
    Point old_dir = dir_;

    if (num_stitches > 0) {
        turntoward(pos);
        left(90);
        stitch(dir_ * -stepsize_ / 2);
    }

    for (size_t i = 0; i < num_stitches; ++i) {
        stitch(dir_ * stepsize_);
        stitch(step + dir_ * -stepsize_);
    }
    stitch_abs(pos);

    dir_ = old_dir;
}

void Turtle::check_density(const Point& pos) {
    string key = pos.tostr(DENSITY_PRECISION);
    auto entry = density_.find(key);

    if (entry == density_.end()) {
        density_[key] = 1;
        entry = density_.find(key);
    } else {
        entry->second++;
    }
    density_warning_ |= (entry->second > DENSITY_WARN_LIMIT);
    density_error_ |= (entry->second > DENSITY_ERROR_LIMIT);
}

/* Disabled functions -- we could use these at some point, but they feel too
high-level for providing to CS70

void Turtle::rectangle(float w, float h) {
    forward(w);
    right(90);
    forward(h);
    right(90);
    forward(w);
    right(90);
    forward(h);
    right(90);
}


void Turtle::circle(float radius) {
    Point start = position_;
    int steps = 2+int(fmin(11+abs(radius)/6.0, 59.0));
    float w = 360.0 / steps;
    float l = 2.0 * abs(radius) / steps;

    for (int i=0; i < steps; ++i) {
        forward(l);
        turn(w);
    }

    gotopoint(start);
}

void Turtle::snowflake(float sidelength, int levels) {
    flakeside(sidelength, levels);
    left(120);
    flakeside(sidelength, levels);
    left(120);
    flakeside(sidelength, levels);
    left(120);
}

void Turtle::flakeside(float sidelength, int levels) {
    if (levels == 0) {
        forward(sidelength);
    }
    else {
        flakeside(sidelength/3, levels-1);
        right(60);
        flakeside(sidelength/3, levels-1);
        left(120);
        flakeside(sidelength/3, levels-1);
        right(60);
        flakeside(sidelength/3, levels-1);
    }
}


void Turtle::squareSpiral(int line) {
    if (line > 0) {
        forward(line);
        right(90);
        squareSpiral(line-1);
    }
}

void Turtle::tree(int level) {
    if (level > 0) {
        forward(level * 10);
        right(20);
        tree(level-3);
        left(40);
        tree(level-3);
        right(20);
        penup();
        backward(level * 10);
        pendown();
    }
}

void Turtle::svtree(float trunklength, int levels, float trunkwidth) {
    if (trunklength < 1 || levels <= 0) return;
    bool satin_was_on = satin_is_on_;
    float old_satin_delta = satin_delta_;
    float old_step_size = stepsize_;

    if (trunkwidth > .1) {
        satinon(.3);
        setStepSize(trunkwidth);
    }
    else {
        satinoff();
    }

    forward(trunklength);
    right(30);
    svtree(.6*trunklength, levels-1, .7*trunkwidth);
    left(60);
    svtree(.6*trunklength, levels-1, .7*trunkwidth);
    right(30);
    penup();
    backward(trunklength);
    pendown();

    setStepSize(old_step_size);
    if (satin_was_on) {
        satinon(old_satin_delta);
    }
    else {
        satinoff();
    }
}

// End disabled functions
*/


/* added functions */
void Turtle::zigzag(const Point& start, const Point& end, float amplitude, float segmentLength) {
    penup();
    gotopoint(start);

    // calc direction and distance from start to end
    Point direction = (end - start).normalize(1);
    float totalLength = (end - start).length();

    int numSegments = int(totalLength / segmentLength);

    if (numSegments % 2 == 0) {
        numSegments--;
    }
    
    // calc perpendicular direction
    Point perpendicular(direction.y_, -direction.x_);

    // draw zigzag here
    pendown();
    Point currentPos = start;
    for (int i = 0; i < numSegments; ++i) {
        if (i % 2 == 0) {
            currentPos += (direction * segmentLength) + (perpendicular * amplitude);
        } else {
            currentPos += (direction * segmentLength) - (perpendicular * amplitude);
        }

        gotopoint(currentPos);
    }

    if (!(currentPos == end)) {
        gotopoint(end);
    }
}

void Turtle::halfCircle(const Point& center, float radius, bool clockwise) {
    penup();

    // calc the start and end angles for the semi circle
    float startAngle = clockwise ? 0 : M_PI;
    float endAngle = clockwise ? M_PI : 0;

    // calc numberer of stitches to approximate semi circle
    int numStitches = static_cast<int>(radius * M_PI / stepsize_);

    float angleIncrement = (endAngle - startAngle) / (clockwise ? numStitches : -numStitches);

    Point startPoint(center.x_ + radius * cos(startAngle), center.y_ + radius * sin(startAngle));

    gotopoint(startPoint);
    pendown();

    for (int i = 1; i <= numStitches; ++i) {
        float angle = startAngle + angleIncrement * i;
        Point stitchPoint(center.x_ + radius * cos(angle), center.y_ + radius * sin(angle));

        if (satin_is_on_) {
            satin_stitch_to(stitchPoint);
        } else {
            stitch_to(stitchPoint);
        }
    }

    penup();
}



