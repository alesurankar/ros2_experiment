#pragma once
#include <vector>
#include <map>
#include <string>


struct MotionFrame
{
    double timestamp;
    int frame_id;
    std::map<std::string, double> joints;
};

struct MotionData
{
    int total_frames;
    double duration;
    std::vector<MotionFrame> frames;
};