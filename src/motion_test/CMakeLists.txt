cmake_minimum_required(VERSION 3.8)
project(motion_test)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

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

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/MotionCommand.msg"
)

# motion commander node
add_executable(motion_commander_node
  src/motion_commander_node.cpp
  src/motion_commander.cpp
)

target_include_directories(motion_commander_node PUBLIC
  include
)

ament_target_dependencies(motion_commander_node
  rclcpp
)

rosidl_target_interfaces(motion_commander_node
  ${PROJECT_NAME} "rosidl_typesupport_cpp")

# motion filter node
add_executable(motion_filter_node
  src/motion_filter_node.cpp
  src/motion_filter.cpp
)

target_include_directories(motion_filter_node PUBLIC
  include
)

ament_target_dependencies(motion_filter_node
  rclcpp
  geometry_msgs
)

rosidl_target_interfaces(motion_filter_node
  ${PROJECT_NAME} "rosidl_typesupport_cpp")

install(TARGETS
  motion_commander_node
  motion_filter_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_export_dependencies(rosidl_default_runtime)

ament_package()
