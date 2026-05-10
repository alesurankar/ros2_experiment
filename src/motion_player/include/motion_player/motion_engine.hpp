#pragma once
#include "motion_data.hpp"


class MotionEngine
{
public:
  static MotionFrame interpolate(
    const MotionFrame& f0,
    const MotionFrame& f1,
    double t);
};