#pragma once
#include "rclcpp/rclcpp.hpp"
#include "motion_test/msg/motion_command.hpp"


class MotionFilter : public rclcpp::Node
{
public:
  MotionFilter();
  void stopRobot();
private:
  void topicCallback(/*MotionCommand.msg*/);
  void timerCallback();
  void watchdogCallback();
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::TimerBase::SharedPtr watchdogTimer_;
  rclcpp::Time lastCmdTime_;
};