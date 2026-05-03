#include "motion_test/motion_commander.hpp"


using namespace std::chrono_literals;

MotionCommander::MotionCommander()
  : 
  Node("motion_commander_node")
{
  publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(
    "/cmd_vel", 10
  );

  timer_ = this->create_wall_timer(
    500ms,
    [this]() { this->timerCallback(); }
  );

  watchdogTimer_ = this->create_wall_timer(
    100ms,
    [this]() { this->watchdogCallback(); }
  );

  lastCmdTime_ = this->now();
}

void MotionCommander::stopRobot()
{
  geometry_msgs::msg::Twist stop;

  stop.linear.x = 0.0;
  stop.angular.z = 0.0;

  publisher_->publish(stop);
  RCLCPP_INFO(this->get_logger(), "STOP sent on shutdown");
}

void MotionCommander::timerCallback()
 {
    geometry_msgs::msg::Twist msg;

    msg.linear.x = 0.2;
    msg.angular.z = 0.01;

    publisher_->publish(msg);
    lastCmdTime_ = this->now();
    RCLCPP_INFO(this->get_logger(), "Moving forward...");
  }

void MotionCommander::watchdogCallback()
{
  if ((this->now() - lastCmdTime_).seconds() > 0.5) {
    stopRobot();
  }
}