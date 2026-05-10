#pragma once
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include "motion_data.hpp"
#include <vector>


class MotionPlayer : public rclcpp::Node
{
public:
  MotionPlayer();
  void start_playback();
private:
  void playback_callback();
  void publish_joint_state(const MotionFrame& frame);
private:
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  MotionData motion_data_;
  bool is_playing_;
  size_t last_index_;
  rclcpp::Time start_time_;
  std::vector<std::string> joint_names_;
};