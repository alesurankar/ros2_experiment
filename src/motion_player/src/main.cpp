#include "motion_player/motion_player.hpp"
#include <rclcpp/rclcpp.hpp>


int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<MotionPlayer>();
    node->start_playback();

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}