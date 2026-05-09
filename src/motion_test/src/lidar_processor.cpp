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

void LidarProcessor::scanCallback(const sensor_msgs::msg::LaserScan& msg)
{
  float angle = 0.0f;
  int index = (int)((angle - msg.angle_min) / msg.angle_increment);

  int window = 10;
  float min_distance = std::numeric_limits<float>::infinity();

  for (int i = index - window; i <= index + window; i++) {

    if (i < 0 || i >= (int)msg.ranges.size()) continue;

    float d = msg.ranges[i];

    if (std::isfinite(d) &&
        d > msg.range_min &&
        d < msg.range_max)
    {
      min_distance = std::min(min_distance, d);
    }
  }

  front_distance_ = min_distance;

  obstacle_detected_ = (front_distance_ < 0.2f);
}