#include "motion_player/motion_player.hpp"
#include "motion_player/motion_loader.hpp"
#include "motion_player/motion_engine.hpp"
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <chrono>
#include <functional>


MotionPlayer::MotionPlayer()
  : 
  Node("motion_player"),
  is_playing_(false),
  start_time_(std::chrono::steady_clock::now())
{
  pub_ = create_publisher<sensor_msgs::msg::JointState>("/joint_states", 10);

  std::string path =
  ament_index_cpp::get_package_share_directory("motion_player")
  + "/walker_s2_motion.json";

motion_data_ = MotionLoader::load(path);

  timer_ = create_wall_timer(
    std::chrono::milliseconds(33),
    std::bind(&MotionPlayer::playback_callback, this));

  RCLCPP_INFO(get_logger(), "Loaded %zu frames", motion_data_.frames.size());
}

void MotionPlayer::start_playback()
{
  if (motion_data_.frames.empty()) {
    RCLCPP_ERROR(get_logger(), "No motion loaded");
    return;
  }

  is_playing_ = true;
  start_time_ = std::chrono::steady_clock::now();
}

void MotionPlayer::playback_callback()
{
  if (!is_playing_ || motion_data_.frames.size() < 2) {
    return;
  }

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

void MotionPlayer::publish_joint_state(const MotionFrame& frame)
{
  sensor_msgs::msg::JointState msg;
  msg.header.stamp = this->get_clock()->now();

  for (const auto& [name, value] : frame.joints) {
    msg.name.push_back(name);
    msg.position.push_back(value);
  }

  msg.velocity.resize(msg.name.size(), 0.0);
  msg.effort.resize(msg.name.size(), 0.0);

  pub_->publish(msg);
}
