#pragma once
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>


class RobotController : public rclcpp::Node 
{
public:
  RobotController();
private:
  void joint_callback(const sensor_msgs::msg::JointState::SharedPtr msg);
private:
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_sub_;
};
