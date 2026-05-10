#include "robot_controller/motion_player.hpp"
#include "robot_controller/motion_loader.hpp"
#include "robot_controller/motion_engine.hpp"

#include <chrono>

MotionPlayer::MotionPlayer()
: 
Node("motion_player"),
is_playing_(false)
{
    pub_ = create_publisher<sensor_msgs::msg::JointState>("/joint_states", 10);

    motion_data_ =
        MotionLoader::load("/home/alesurankar/ros2_ws/walker_s2_motion.json");

    timer_ = create_wall_timer(
        std::chrono::milliseconds(33),
        std::bind(&MotionPlayer::playback_callback, this));

    RCLCPP_INFO(get_logger(), "Loaded %zu frames", motion_data_.frames.size());
}

void MotionPlayer::playback_callback()
{
    if (!is_playing_ || motion_data_.frames.size() < 2)
        return;

    double elapsed =
        std::chrono::duration<double>(
            std::chrono::steady_clock::now() - start_time_
        ).count();

    const auto& frames = motion_data_.frames;

    if (elapsed >= frames.back().timestamp)
    {
        publish_joint_state(frames.back());
        return;
    }

    size_t i = 0;
    while (i + 1 < frames.size() &&
           elapsed > frames[i + 1].timestamp)
        i++;

    double dt = frames[i + 1].timestamp - frames[i].timestamp;
    double t = (elapsed - frames[i].timestamp) / dt;

    MotionFrame interp =
        MotionEngine::interpolate(frames[i], frames[i + 1], t);

    publish_joint_state(interp);
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MotionPlayer>();

    node->start_playback();
    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}