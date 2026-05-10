#pragma once
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include "motion_data.hpp"


class MotionPlayer : public rclcpp::Node
{
public:
  MotionPlayer();
  void start_playback();
private:
  void playback_callback();
  void publish_joint_state(const MotionFrame& frame);
private:
  MotionData motion_data_;
  bool is_playing_;
  std::chrono::steady_clock::time_point start_time_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};