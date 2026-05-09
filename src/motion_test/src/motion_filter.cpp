#include "motion_test/motion_filter.hpp"


using namespace std::chrono_literals;

MotionFilter::MotionFilter()
  : 
  Node("motion_filter_node"),
  lidar_(*this)
{
  publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(
    "/cmd_vel", 10
  );

  subscriber_ = this->create_subscription<motion_test::msg::MotionCommand>(
    "motion_command", 10,
    [this](const motion_test::msg::MotionCommand::SharedPtr msg) {
      this->topicCallback(*msg);
    }
  );

  watchdogTimer_ = this->create_wall_timer(
    100ms,
    [this]() { this->watchdogCallback(); }
  );

  lastCmdTime_ = this->now();
}

void MotionFilter::stopRobot()
{
  geometry_msgs::msg::Twist stop;
  stop.linear.x = 0.0;
  stop.angular.z = 0.0;

  publisher_->publish(stop);

  RCLCPP_WARN(this->get_logger(), "WATCHDOG: STOP sent!");
}

void MotionFilter::topicCallback(const motion_test::msg::MotionCommand & msg)
{
  lastCmdTime_ = this->now();
  stopped_ = false;

  geometry_msgs::msg::Twist cmd;
  // RCLCPP_INFO(this->get_logger(),
  //   "Received: linear_x=%.2f angular_z=%.2f",
  //   msg.linear_x, msg.angular_z);

  if (lidar_.hasObstacle() && msg.linear_x > 0.0) {
    RCLCPP_WARN(this->get_logger(), 
      "Obstacle detected! Blocking forward motion.");

    cmd.linear.x = 0.0;
    cmd.angular.z = msg.angular_z;

    publisher_->publish(cmd);
    return;
  }

  cmd.linear.x = msg.linear_x;
  cmd.angular.z = msg.angular_z;

  publisher_->publish(cmd);
}

void MotionFilter::watchdogCallback()
{
  if ((this->now() - lastCmdTime_).seconds() > 0.5 && !stopped_) {
    stopRobot();
    stopped_ = true;
  }
}