#include "rclcpp/rclcpp.hpp"
#include "motion_test/motion_filter.hpp"
#include <memory>


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MotionFilter>();
  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}