#pragma once
#include "rclcpp/rclcpp.hpp"
#include "motion_test/msg/motion_command.hpp"


class MotionCommander : public rclcpp::Node
{
public:
  MotionCommander();
private:
  void timerCallback();
  void keyboardLoop();
private:
  rclcpp::Publisher<motion_test::msg::MotionCommand>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  double linear_x_;
  double angular_z_;
};