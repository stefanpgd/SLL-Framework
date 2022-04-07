#pragma once
/* This the precompiled header for this project. Each cpp file should include this on the very first line (otherwise a C1010 compilation error will be thrown)
 *
 * Meant for including _STABLE_ headers that are commonly used in your project to help speed up compilation.
 * In this case it is also used to ensure that the classes defined in the template.h / surface.h files are always to reduce complexity.
 *
 * Compilation is sped up by the included headers only being parsed once, and the parsed state is save to be re-used for every compilation.
 * The parsed state will be re-used until a change is detected in an included file.
 * Stable meaning the headers don't change often, or don't change at all over the course of your project.
 * Examples include standard headers that ship with the compiler or third party library headers
 *
 * This is _NOT_ a dumping ground for all headers in your project.
 * Any time anything is changed in a header that is included in this file it will trigger a full rebuild of your project.
 *
 * This is also not some magic file, the settings can be found in Project Properties -> C++ -> Precompiled Headers
 */

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <limits>
#include <random>
#include <memory>
#include <iostream>

#include "template.h"
#include "surface.h"

const float infinity = std::numeric_limits<float>::infinity();
const float Pi = 3.1415926535;
const float RadToDeg = 180.0f / Pi;
const float DegToRad = Pi / 180.0f;

using std::sqrtf;
using namespace Tmpl8;

#define LOG_TIME true

#include "Ray.h"
#include "Utilities.h"