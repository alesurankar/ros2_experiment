#pragma once
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

class LidarProcessor
{
public:
  explicit LidarProcessor(rclcpp::Node* node);
  bool obstacleDetected() const;
  float getFrontDistance() const;
private:
  void scanCallback(const sensor_msgs::msg::LaserScan & msg);
private:
  rclcpp::Node* node_;
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;
  bool obstacle_detected_;
  float front_distance_;
};