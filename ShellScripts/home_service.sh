#!/bin/sh

xterm  -e  " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=`rospack find turtlebot_gazebo`/../../World/MyWorld.world" &
sleep 5
xterm  -e  " roslaunch turtlebot_gazebo amcl_demo.launch map_file:=`rospack find turtlebot_gazebo`/../../World/my_map.yaml" &
sleep 5
xterm -e " rosrun rviz rviz -d `rospack find turtlebot_gazebo`/../../RvizConfig/home_service_bot.rviz " &
sleep 5
xterm -e "rosrun add_markers add_markers " &
sleep 5
xterm -e "rosrun pick_objects pick_objects "
