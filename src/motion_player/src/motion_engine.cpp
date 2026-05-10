#include "motion_player/motion_engine.hpp"


MotionFrame MotionEngine::interpolate(
  const MotionFrame& f0,
  const MotionFrame& f1, 
  double t)
{
  MotionFrame out;
  out.timestamp = f0.timestamp + t * (f1.timestamp - f0.timestamp);

  for (const auto& [joint, v0] : f0.joints) {
    auto it1 = f1.joints.find(joint);
    if (it1 == f1.joints.end()) {
      continue;
    }

    double v1 = it1->second;
    out.joints[joint] = v0 + t * (v1 - v0);
  }

  return out;
}