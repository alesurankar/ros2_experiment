#pragma once
#include "motion_data.hpp"
#include <string>


class MotionLoader
{
public:
  static MotionData load(const std::string& filename);
};