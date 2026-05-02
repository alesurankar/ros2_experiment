#include "rclcpp/rclcpp.hpp"
#include "pubsub_demo/processing_node.hpp"
#include <memory>

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ProcessingNode>());
  rclcpp::shutdown();
  return 0;
}