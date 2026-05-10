#include "robot_controller/robot_controller.hpp"


RobotController::RobotController() 
:
Node("robot_controller") 
{
  // Publisher for joint commands
  joint_pub_ = this->create_publisher<sensor_msgs::msg::JointState>(
      "/joint_commands", 10);
  
  // Subscriber to joint states
  joint_sub_ = this->create_subscription<sensor_msgs::msg::JointState>(
      "/joint_states", 10, 
      std::bind(&RobotController::joint_callback, this, std::placeholders::_1));
  
  RCLCPP_INFO(this->get_logger(), "Robot Controller started");
}

void RobotController::joint_callback(const sensor_msgs::msg::JointState::SharedPtr msg) 
{
    RCLCPP_INFO(this->get_logger(), "Received joint states");
    // Process joint states here
    (void)msg;
}

int main(int argc, char** argv) 
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<RobotController>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}