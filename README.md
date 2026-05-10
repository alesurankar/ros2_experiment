
Start RViz2
```bash
cd ~/ros2_ws
colcon build
source install/setup.bash
ros2 launch walker_s2_description display.launch.py
```


Configure RViz2 (if robot doesn't appear)
- Click "Add" button
- Select "RobotModel" → "OK"
- Set "Fixed Frame" to base_link
- Set "Robot Description Topic" to /robot_description


Verify Everything Works
```bash
# Check robot description
ros2 topic echo /robot_description

# Check TF frames
ros2 run tf2_tools view_frames

# Check joint states
ros2 topic echo /joint_states
```


Monitor topics
```bash
ros2 topic echo /joint_states
```


# clear builds
```bash
cd ~/ros2_ws
rm -rf build install log
```

# generate compile commands
```bash
cd ~/ros2_ws
colcon build --cmake-args -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```
