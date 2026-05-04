#pragma once
#include "rclcpp/rclcpp.hpp"
#include "motion_test/msg/motion_command.hpp"


class MotionCommander : public rclcpp::Node
{
public:
  MotionCommander();
private:
  void timerCallback();
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};