
1. Start RViz2
```bash
cd ~/ros2_ws
colcon build --packages-select walker_s2_controller
source install/setup.bash
ros2 launch walker_s2_description display.launch.py
```

2. Start motion player
```bash
cd /home/alesurankar/ros2_ws
colcon build --packages-select motion_player
source install/setup.bash
ros2 run walker_s2_controller motion_player
```

2. Configure RViz2 (if robot doesn't appear)
- Click "Add" button
- Select "RobotModel" → "OK"
- Set "Fixed Frame" to base_link
- Set "Robot Description Topic" to /robot_description

3. Verify Everything Works
```bash
# Check robot description
ros2 topic echo /robot_description

# Check TF frames
ros2 run tf2_tools view_frames

# Check joint states
ros2 topic echo /joint_states
```

4. Build the controller
```bash
cd ~/ros2_ws
colcon build --packages-select walker_s2_controller
source install/setup.bash
ros2 run walker_s2_controller walker_controller
```

5. Monitor topics
```bash
ros2 topic echo /joint_states
```



# GAZEBO

## Run Gazebo
```bash
cd ~/ws_robot
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 launch gazebo_ros gazebo.launch.py
```

## Spawn Walker S2
```bash
source /opt/ros/humble/setup.bash
cd ~/ws_robot
source install/setup.bash
ros2 run gazebo_ros spawn_entity.py -entity walker_s2 -file ~/ws_robot/src/walker_s2_description/urdf/s2/s2_noheader.urdf -x 0 -y 0 -z 0.5
```

## publish robot kinematic frames so ROS tools know where each link is.
```bash
source /opt/ros/humble/setup.bash
cd ~/ws_robot
source install/setup.bash

python3 - << 'PY'
from pathlib import Path
urdf = Path('/home/alesurankar/ws_robot/src/walker_s2_description/urdf/s2/s2_noheader.urdf').read_text()
out = Path('/tmp/walker_rsp_params.yaml')
out.write_text(
    "robot_state_publisher:\n"
    "  ros__parameters:\n"
    "    robot_description: |\n" +
    "".join(f"      {line}\n" for line in urdf.splitlines())
)
print(out)
PY

ros2 run robot_state_publisher robot_state_publisher --ros-args --params-file /tmp/walker_rsp_params.yaml
```
