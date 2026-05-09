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

  float dist = lidar_.frontDistance();
  if (!std::isfinite(dist)) {
    dist = 10.0f;
  }
  geometry_msgs::msg::Twist cmd;
  SafetyState new_state;

  // --- STOP ZONE ---
  if (dist < 0.2f) {
    new_state = SafetyState::STOP;
    cmd.linear.x = 0.0;
    cmd.angular.z = 0.0;
  }

  // --- SLOW ZONE ---
  else if (dist < 0.5f) {
    new_state = SafetyState::SLOW;
    float scale = std::clamp((dist - 0.2f) / 0.3f, 0.0f, 1.0f);
    float max_speed = 0.3f;

    cmd.linear.x = std::clamp(msg.linear_x * scale, -max_speed, max_speed);
    cmd.angular.z = std::clamp(msg.angular_z * scale, -1.0f, 1.0f);
  }

  // --- NORMAL ZONE ---
  else {
    new_state = SafetyState::NORMAL;
    cmd.linear.x = msg.linear_x;
    cmd.angular.z = msg.angular_z;
  }
  // --- ONLY LOG ON CHANGE ---
  if (new_state != last_state_) {
    switch (new_state) {
      case SafetyState::STOP:
        RCLCPP_WARN(this->get_logger(), "ENTER STOP ZONE (%.2f m)", dist);
        break;
      case SafetyState::SLOW:
        RCLCPP_WARN(this->get_logger(), "ENTER SLOW ZONE (%.2f m)", dist);
        break;
      case SafetyState::NORMAL:
        RCLCPP_INFO(this->get_logger(), "ENTER NORMAL ZONE");
        break;
    }
    last_state_ = new_state;
  }

  publisher_->publish(cmd);
}

void MotionFilter::watchdogCallback()
{
  if ((this->now() - lastCmdTime_).seconds() > 0.5 && !stopped_) {
    stopRobot();
    stopped_ = true;
  }
}