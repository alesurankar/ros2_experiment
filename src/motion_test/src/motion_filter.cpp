#include "motion_test/motion_filter.hpp"


using namespace std::chrono_literals;

MotionFilter::MotionFilter()
  : 
  Node("motion_filter_node")
{
  // publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(
  //   "/cmd_vel", 10
  // );

  subscription_ = this->create_subscription<motion_test::msg::MotionCommand>(
    "motion_command", 10,
    [this](const motion_test::msg::MotionCommand::SharedPtr msg) {
      this->topicCallback(*msg);
    }
  );

  // timer_ = this->create_wall_timer(
  //   500ms,
  //   [this]() { this->timerCallback(); }
  // );

  // watchdogTimer_ = this->create_wall_timer(
  //   100ms,
  //   [this]() { this->watchdogCallback(); }
  // );

  // lastCmdTime_ = this->now();
}

// void MotionFilter::stopRobot()
// {
//   geometry_msgs::msg::Twist stop;

//   stop.linear.x = 0.0;
//   stop.angular.z = 0.0;

//   publisher_->publish(stop);
//   RCLCPP_INFO(this->get_logger(), "STOP sent on shutdown");
// }

void MotionFilter::topicCallback(const motion_test::msg::MotionCommand & msg)
{
  RCLCPP_INFO(this->get_logger(),
    "Received: linear_x=%.2f angular_z=%.2f",
    msg.linear_x, msg.angular_z);
}

// void MotionFilter::timerCallback()
// {
//   geometry_msgs::msg::Twist msg;

//   msg.linear.x = 0.2;
//   msg.angular.z = 0.01;

//   publisher_->publish(msg);
//   lastCmdTime_ = this->now();
//   RCLCPP_INFO(this->get_logger(), "Moving forward...");
// }

// void MotionFilter::watchdogCallback()
// {
//   if ((this->now() - lastCmdTime_).seconds() > 0.5) {
//     stopRobot();
//   }
// }