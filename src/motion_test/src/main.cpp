#include "rclcpp/rclcpp.hpp"
#include "motion_test/test.hpp"
#include <memory>


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MotionTest>());
  rclcpp::shutdown();
  return 0;
}