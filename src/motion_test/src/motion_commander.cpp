#include "motion_test/motion_commander.hpp"
#include <thread>
#include <chrono>
#include <termios.h>  // linux terminal behavior controller
#include <unistd.h>   // linux system functions provider
#include <fcntl.h>    // for nonblocking keyboard in linux
#include <cstdio>


using namespace std::chrono_literals;

MotionCommander::MotionCommander()
  : 
  Node("motion_commander_node")
{
  this->declare_parameter("linear_x", 0.0);
  this->declare_parameter("angular_z", 0.0);

  linear_x_ = this->get_parameter("linear_x").as_double();
  angular_z_ = this->get_parameter("angular_z").as_double();

  RCLCPP_INFO(this->get_logger(),
    "\nControls:\n"
    "w/s : linear velocity +/-\n"
    "a/d : angular velocity +/-\n"
    "space : stop\n"
  );

  publisher_ = this->create_publisher<motion_test::msg::MotionCommand>(
    "motion_command", 10
  );

  timer_ = this->create_wall_timer(
    500ms,
    [this]() { this->timerCallback(); }
  );

  std::thread(
    [this]() { keyboardLoop(); }
  ).detach();
}

void MotionCommander::timerCallback()
{
  auto msg = motion_test::msg::MotionCommand();

  msg.linear_x = linear_x_;
  msg.angular_z = angular_z_;

  // Logging ///
  RCLCPP_INFO(this->get_logger(),
    "Publishing: linear_x=%.2f angular_z=%.2f",
    msg.linear_x, msg.angular_z);
  //////////////

  publisher_->publish(msg);
}

void MotionCommander::keyboardLoop()
{
  struct termios old_tio, new_tio;

  tcgetattr(STDIN_FILENO, &old_tio);
  new_tio = old_tio;

  new_tio.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

  while (rclcpp::ok()) {

    int c = getchar();

    if (c == EOF) {
      std::this_thread::sleep_for(50ms);
      continue;
    }

    switch (c) {

      case 'w':
        linear_x_ += 0.1;
        break;

      case 's':
        linear_x_ -= 0.1;
        break;

      case 'a':
        angular_z_ += 0.1;
        break;

      case 'd':
        angular_z_ -= 0.1;
        break;

      case ' ':
        linear_x_ = 0.0;
        angular_z_ = 0.0;
        break;
    }

    RCLCPP_INFO(
      this->get_logger(),
      "linear=%.2f angular=%.2f",
      linear_x_,
      angular_z_
    );
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}