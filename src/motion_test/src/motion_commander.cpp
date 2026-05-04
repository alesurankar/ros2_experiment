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
}

void MotionCommander::timerCallback()
{
  // TODO: auto msg = MotionCommand.msg 
  // publisher_->publish(msg);
}