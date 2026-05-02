cmake_minimum_required(VERSION 3.8)
project(pubsub_demo)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# TALKER NODE
add_executable(talker_node
  src/main_talker.cpp
  src/talker_node.cpp
)

target_include_directories(talker_node PUBLIC
  include
)

ament_target_dependencies(talker_node
  rclcpp
  std_msgs
)

# LISTENER NODE
add_executable(listener_node
  src/main_listener.cpp
  src/listener_node.cpp
)

target_include_directories(listener_node PUBLIC
  include
)

ament_target_dependencies(listener_node
  rclcpp
  std_msgs
)

# INSTALL NODES
install(TARGETS
  talker_node
  listener_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
