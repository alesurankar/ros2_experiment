#pragma once
#include "rclcpp/rclcpp.hpp"
#include "motion_test/msg/motion_command.hpp"
#include "motion_test/lidar_processor.hpp"
#include "geometry_msgs/msg/twist.hpp"


class MotionFilter : public rclcpp::Node
{
public:
  MotionFilter();
  void stopRobot();
private:
  LidarProcessor lidar_;
  void topicCallback(const motion_test::msg::MotionCommand& msg);
  void watchdogCallback();
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Subscription<motion_test::msg::MotionCommand>::SharedPtr subscriber_;
  rclcpp::TimerBase::SharedPtr watchdogTimer_;
  rclcpp::Time lastCmdTime_;
  bool stopped_ = false;
};