#include "motion_test/motion_commander.hpp"


using namespace std::chrono_literals;

MotionCommander::MotionCommander()
  : 
  Node("motion_commander_node")
{
  this->declare_parameter("linear_x", 0.5);
  this->declare_parameter("angular_z", 0.2);

  linear_x_ = this->get_parameter("linear_x").as_double();
  angular_z_ = this->get_parameter("angular_z").as_double();

  publisher_ = this->create_publisher<motion_test::msg::MotionCommand>(
    "motion_command", 10
  );

  timer_ = this->create_wall_timer(
    500ms,
    [this]() { this->timerCallback(); }
  );
}

void MotionCommander::timerCallback()
{
  auto msg = motion_test::msg::MotionCommand();

  msg.linear_x = linear_x_;
  msg.angular_z = angular_z_;

  // Logging ///
  RCLCPP_INFO(this->get_logger(),
    "Publishing: linear_x=%.2f angular_z=%.2f",
    msg.linear_x, msg.angular_z);
  //////////////

  publisher_->publish(msg);
}