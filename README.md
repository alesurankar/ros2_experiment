## Overview

This is a simple ROS2 C++ package that demonstrates a basic publisher node.
It publishes string messages at a fixed interval using ROS2 topics.


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
ros2 run cpp_test talker
```


## Environment

- OS: Ubuntu 22.04 (WSL2 on Windows 11)
- ROS2: Humble