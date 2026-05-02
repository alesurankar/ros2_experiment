#include "rclcpp/rclcpp.hpp"
#include "pubsub_demo/listener_node.hpp"

using namespace std::chrono_literals;

ListenerNode::ListenerNode()
  : 
  Node("listener_node")
{
  subscription_ = this->create_subscription<std_msgs::msg::String>(
    "processed_chatter",
    10,
    [this](const auto msg) {
      this->topicCallback(msg);
    }
  );
}

void ListenerNode::topicCallback(const std_msgs::msg::String::SharedPtr msg)
{
  RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
}