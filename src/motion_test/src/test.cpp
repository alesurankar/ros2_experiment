#include "motion_test/test.hpp"


using namespace std::chrono_literals;

MotionTest::MotionTest()
  : 
  Node("motion_test_node")
{
  publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(
    "/cmd_vel", 10
  );

  timer_ = this->create_wall_timer(
    500ms,
    [this]() {
      this->timerCallback();
    }
  );
}

MotionTest::~MotionTest()
{
  geometry_msgs::msg::Twist stop;

  stop.linear.x = 0.0;
  stop.angular.z = 0.0;

  publisher_->publish(stop);

  RCLCPP_INFO(this->get_logger(), "STOP sent on shutdown");
}

void MotionTest::timerCallback()
 {
    geometry_msgs::msg::Twist msg;

    msg.linear.x = 0.2;
    msg.angular.z = 0.01;

    publisher_->publish(msg);

    RCLCPP_INFO(this->get_logger(), "Moving forward...");
  }