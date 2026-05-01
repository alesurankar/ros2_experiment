#include "pubsub_demo/talker.hpp"

using namespace std::chrono_literals;

TalkerNode::TalkerNode()
  : 
  Node("talker_node"), counter_(0)
{
  publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);

  timer_ = this->create_wall_timer(
    500ms,
    std::bind(&TalkerNode::timerCallback, this)
  );
}

void TalkerNode::timerCallback()
{
  std_msgs::msg::String msg;
  msg.data = "Hello ROS2: " + std::to_string(counter_++);

  RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msg.data.c_str());

  publisher_->publish(msg);
}