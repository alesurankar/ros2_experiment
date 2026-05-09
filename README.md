# ROS2 Lab

This repository contains a collection of ROS2 C++ experiments.

It is used to explore:
- ROS2 publisher/subscriber communication
- motion control experiments
- sensor processing tests

---

## Environment Setup

Before using ROS2 commands in a new terminal, you must source the environment:

```bash
# ROS2 system
source /opt/ros/humble/setup.bash

# Your workspace
source ~/ros2_ws/install/setup.bash
```

---

## Build Instructions

From the workspace root:
```bash
colcon build
```

---

## Run

From the workspace root:
```bash
ros2 run <project_name> <node_executable>
```

---

## 🏗️ structure

```text
ros2_ws/
└─ src/
    └─ motion_test/
        ├─ include/
        │   └─ motion_test/
        │       ├─ lidar_processor.hpp
        │       ├─ motion_commander.hpp
        │       └─ motion_filter.hpp
        ├─ msg/
        ├─ src/
        ├─ CMakeLists.txt
        └─ package.xml
```

---

## Environment

- OS: Ubuntu 22.04 (WSL2 on Windows 11)
- ROS2: Humble


## Run Gazebo
```bash
source /opt/ros/humble/setup.bash
export TURTLEBOT3_MODEL=burger
killall gzserver gzclient 2>/dev/null || true
ros2 launch turtlebot3_gazebo turtlebot3_world.launch.py
```

## Spawn Robot

if not robot in the scene after opening gazebo, then run this:

```bash
ros2 run gazebo_ros spawn_entity.py \
  -entity burger \
  -file /opt/ros/humble/share/turtlebot3_gazebo/models/turtlebot3_burger/model.sdf \
  -x -2.0 \
  -y -0.5 \
  -z 0.0
```