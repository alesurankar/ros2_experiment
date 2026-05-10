
Start motion player
```bash
cd ~/ros2_ws
colcon build --packages-select motion_player
source install/setup.bash
ros2 run motion_player motion_player
```

to close GUI publisher
```bash
pkill -f joint_state_publisher_gui
```

to open GUI publisher
```bash
ros2 run joint_state_publisher_gui joint_state_publisher_gui
```

open IK setup
```bash
ros2 run moveit_setup_assistant moveit_setup_assistant
```