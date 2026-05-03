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
source install/setup.bash
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
 ├── src/
 │    ├── pubsub_demo/
 │    │    ├── CMakeLists.txt
 │    │    ├── package.xml
 │    │    ├── src/
 │    │    └── include/
 │    │
 │    ├── sensor_playground/
 │    │    ├── CMakeLists.txt
 │    │    ├── package.xml
 │    │    ├── src/
 │    │
 │    ├── motion_test/
 │         ├── CMakeLists.txt
 │         ├── package.xml
 │         ├── src/
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