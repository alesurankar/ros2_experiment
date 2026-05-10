#include "motion_player/motion_loader.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <rclcpp/rclcpp.hpp>


using json = nlohmann::json;

MotionData MotionLoader::load(const std::string& filename)
{
    MotionData data;

    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open motion file");

    json j;
    file >> j;

    data.total_frames = j["total_frames"];
    data.duration = j["duration"];

    for (const auto& f : j["frames"])
    {
        MotionFrame frame;
        frame.timestamp = f["timestamp"];
        frame.frame_id = f["frame_id"];

        for (auto it = f["joints"].begin(); it != f["joints"].end(); ++it)
            frame.joints[it.key()] = it.value();

        data.frames.push_back(frame);
    }

    return data;
}