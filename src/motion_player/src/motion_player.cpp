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
  last_index_(0)
{
  pub_ = create_publisher<sensor_msgs::msg::JointState>("/joint_states", 10);

  std::string path =
  ament_index_cpp::get_package_share_directory("motion_player")
  + "/walker_s2_motion.json";

  motion_data_ = MotionLoader::load(path);

  timer_ = create_wall_timer(std::chrono::milliseconds(10),
    std::bind(&MotionPlayer::playback_callback, this));

  joint_names_.clear();
  for (const auto& [name, _] : motion_data_.frames[0].joints) {
    joint_names_.push_back(name);
  }

  RCLCPP_INFO(get_logger(), "Loaded %zu frames", motion_data_.frames.size());
  start_playback();
}

void MotionPlayer::start_playback()
{
  if (motion_data_.frames.empty()) {
    RCLCPP_ERROR(get_logger(), "No motion loaded");
    return;
  }

  is_playing_ = true;
  last_index_ = 0;
  start_time_ = this->get_clock()->now();
}

void MotionPlayer::playback_callback()
{
  if (!is_playing_ || motion_data_.frames.size() < 2) {
    return;
  }

  double elapsed = (this->get_clock()->now() - start_time_).seconds();

  const auto& frames = motion_data_.frames;

  RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 500,
    "elapsed=%.3f last_index=%zu frame_count=%zu",
    elapsed, last_index_, frames.size());

  if (elapsed >= frames.back().timestamp) {
    publish_joint_state(frames.back());

    is_playing_ = false;
    return;
  }

  // find segment
  while (last_index_ + 1 < frames.size() &&
         elapsed >= frames[last_index_ + 1].timestamp) {
    last_index_++;
    RCLCPP_INFO(get_logger(), "Advancing index -> %zu (t=%.3f)",
      last_index_, frames[last_index_].timestamp);
  }

  size_t i = last_index_;

  if (i + 1 >= frames.size()) {
    RCLCPP_WARN(get_logger(), "Index at end -> publishing last frame");
    publish_joint_state(frames.back());
    return;
  }

  double dt = frames[i + 1].timestamp - frames[i].timestamp;

  if (dt <= 1e-6) {
    RCLCPP_WARN(get_logger(),
      "Zero dt at i=%zu (%.3f -> %.3f)",
      i, frames[i].timestamp, frames[i + 1].timestamp);

    publish_joint_state(frames[i]);
    return;
  }

  double t = (elapsed - frames[i].timestamp) / dt;
  t = std::clamp(t, 0.0, 1.0);

  RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 500,
    "segment i=%zu t=%.3f (%.3f -> %.3f)",
    i, t, frames[i].timestamp, frames[i + 1].timestamp);

  MotionFrame interp =
    MotionEngine::interpolate(frames[i], frames[i + 1], t);

  publish_joint_state(interp);
}

void MotionPlayer::publish_joint_state(const MotionFrame& frame)
{
  sensor_msgs::msg::JointState msg;
  msg.header.stamp = this->get_clock()->now();

  msg.name = joint_names_;

  msg.position.reserve(joint_names_.size());
  msg.velocity.reserve(joint_names_.size());

  static MotionFrame prev_frame;
  static bool has_prev = false;

  double dt = 0.033;

  for (const auto& name : joint_names_) {
    double pos = frame.joints.at(name);
    msg.position.push_back(pos);

    double vel = 0.0;
    if (has_prev) {
      double prev = prev_frame.joints.at(name);
      vel = (pos - prev) / dt;
    }

    msg.velocity.push_back(vel);
  }

  prev_frame = frame;
  has_prev = true;

  msg.effort.resize(msg.name.size(), 0.0);
  pub_->publish(msg);
}