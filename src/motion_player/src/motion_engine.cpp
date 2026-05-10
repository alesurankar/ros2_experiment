#include "robot_controller/motion_engine.hpp"

MotionFrame MotionEngine::interpolate(
    const MotionFrame& f0,
    const MotionFrame& f1,
    double t)
{
    MotionFrame out = f0;

    for (auto& [joint, value] : out.joints)
    {
        double v0 = f0.joints.at(joint);
        double v1 = f1.joints.at(joint);

        out.joints[joint] = v0 + t * (v1 - v0);
    }

    return out;
}