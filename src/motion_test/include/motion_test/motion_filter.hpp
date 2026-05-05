#pragma once
#include "rclcpp/rclcpp.hpp"
#include "motion_test/msg/motion_command.hpp"
#include "geometry_msgs/msg/twist.hpp"


class MotionFilter : public rclcpp::Node
{
public:
  MotionFilter();
  void stopRobot();
private:
  void topicCallback(motion_test::msg::MotionCommand);
  void timerCallback();
  void watchdogCallback();
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Subscription<motion_test::msg::MotionCommand>::SharedPtr subscription_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::TimerBase::SharedPtr watchdogTimer_;
  rclcpp::Time lastCmdTime_;
};