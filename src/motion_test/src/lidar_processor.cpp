#include "motion_test/lidar_processor.hpp"
#include <limits>
#include <cmath>


LidarProcessor::LidarProcessor(rclcpp::Node& node)
{
  scan_sub_ = node.create_subscription<sensor_msgs::msg::LaserScan>(
    "/scan",
    10,
    [this](const sensor_msgs::msg::LaserScan::SharedPtr msg) {
      this->scanCallback(*msg);
    }
  );
}

float LidarProcessor::frontDistance() const
{
  return front_distance_;
}

bool LidarProcessor::hasObstacle() const
{
  return obstacle_detected_;
}

void LidarProcessor::scanCallback(const sensor_msgs::msg::LaserScan & msg)
{
  // middle beam = front direction
  int center_index = msg.ranges.size() / 2;

  float distance = msg.ranges[center_index];

  // ignore invalid lidar values
  if (std::isnan(distance) || std::isinf(distance)) {
    return;
  }

  front_distance_ = distance;

  // obstacle threshold
  obstacle_detected_ = front_distance_ < 0.5f;
}