#pragma once
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"


class LidarProcessor
{
public:
  explicit LidarProcessor(rclcpp::Node& node);
  float frontDistance() const;
  bool hasObstacle() const;
private:
  void scanCallback(const sensor_msgs::msg::LaserScan& msg);
private:
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;
  bool obstacle_detected_ = false;
  float front_distance_ = 0.0f;
};