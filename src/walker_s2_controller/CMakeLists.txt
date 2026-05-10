cmake_minimum_required(VERSION 3.8)
project(walker_s2_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# =========================================
# Dependencies
# =========================================

find_package(ament_cmake REQUIRED)

find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

find_package(nlohmann_json REQUIRED)

# =========================================
# Testing
# =========================================

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)

  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)

  ament_lint_auto_find_test_dependencies()
endif()

# =========================================
# Walker Controller
# =========================================

add_executable(walker_controller
  src/walker_controller.cpp
)

ament_target_dependencies(
  walker_controller
  rclcpp
  sensor_msgs
  geometry_msgs
)

# =========================================
# Motion Player
# =========================================

add_executable(motion_player
  src/motion_player.cpp
)

ament_target_dependencies(
  motion_player
  rclcpp
  sensor_msgs
)

target_link_libraries(
  motion_player
  nlohmann_json::nlohmann_json
)

# =========================================
# Install
# =========================================

install(TARGETS
  walker_controller
  motion_player
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()