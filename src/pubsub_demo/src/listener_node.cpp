#include "rclcpp/rclcpp.hpp"
#include "pubsub_demo/listener_node.hpp"

using namespace std::chrono_literals;

ListenerNode::ListenerNode()
  : 
  Node("listener_node")
{
  subscription_ = this->create_subscription<std_msgs::msg::String>(
    "chatter",
    10,
    std::bind(&ListenerNode::topicCallback, this, std::placeholders::_1)
  );
}

void ListenerNode::topicCallback(const std_msgs::msg::String::SharedPtr msg)
{
  RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
}