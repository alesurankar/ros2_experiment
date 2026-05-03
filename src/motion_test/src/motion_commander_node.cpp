#include "rclcpp/rclcpp.hpp"
#include "motion_test/motion_commander.hpp"
#include <memory>


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MotionCommander>();
  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}