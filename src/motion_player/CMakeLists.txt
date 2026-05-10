cmake_minimum_required(VERSION 3.8)
project(motion_player)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nlohmann_json REQUIRED)

# Testing
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# Motion Player
add_executable(motion_player
  src/main.cpp
  src/motion_player.cpp
  src/motion_loader.cpp
  src/motion_engine.cpp
)

ament_target_dependencies(motion_player
  rclcpp
  sensor_msgs
  nlohmann_json
)

target_include_directories(motion_player PUBLIC
  include
)

# Install
install(TARGETS motion_player
  DESTINATION lib/${PROJECT_NAME}
)

# Install headers
install(DIRECTORY include/
  DESTINATION include
)

# Install motion data
install(DIRECTORY data/
  DESTINATION share/${PROJECT_NAME}
)

ament_package()