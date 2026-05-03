#pragma once
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"


class MotionTest : public rclcpp::Node
{
public:
  MotionTest();
  ~MotionTest();
private:
  void timerCallback();
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};