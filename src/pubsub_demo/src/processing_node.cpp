#include "rclcpp/rclcpp.hpp"
#include "pubsub_demo/processing_node.hpp"

using namespace std::chrono_literals;

ProcessingNode::ProcessingNode()
  : 
  Node("processing_node")
{
  subscription_ = this->create_subscription<std_msgs::msg::String>(
    "chatter",
    10,
    std::bind(&ProcessingNode::callback, this, std::placeholders::_1)
  );

  publisher_ = this->create_publisher<std_msgs::msg::String>(
    "processed_chatter",
    10
  );
}

void ProcessingNode::callback(const std_msgs::msg::String::SharedPtr msg)
{
  std_msgs::msg::String out;

  out.data = "Processed: " + msg->data;

  RCLCPP_INFO(this->get_logger(), "Received: '%s'", msg->data.c_str());
  RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", out.data.c_str());

  publisher_->publish(out);
}