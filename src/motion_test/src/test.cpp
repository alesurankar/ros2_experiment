#include "motion_test/test.hpp"

Test::Test()
  : Node("test_node")
{
  subscription_ = this->create_subscription<std_msgs::msg::String>(
    "chatter",
    10,
    [this](const std_msgs::msg::String::SharedPtr msg) {
      this->subscriberCallback(msg);
    }
  );
}

void Test::subscriberCallback(const std_msgs::msg::String::SharedPtr msg)
{
  RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
}