
# ROS2 Lab

This repository contains a collection of ROS2 C++ experiments.

It is used to explore:
- ROS2 publisher/subscriber communication
- motion control experiments
- sensor processing tests


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
ros2 run pubsub_demo talker
```

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

## Environment

- OS: Ubuntu 22.04 (WSL2 on Windows 11)
- ROS2: Humble