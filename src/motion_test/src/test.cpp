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

void MotionTest::timerCallback()
 {
    geometry_msgs::msg::Twist msg;

    msg.linear.x = 0.2;   // forward speed
    msg.angular.z = 0.0;  // no rotation

    publisher_->publish(msg);

    RCLCPP_INFO(this->get_logger(), "Moving forward...");
  }