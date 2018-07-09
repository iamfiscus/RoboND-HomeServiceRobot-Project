#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
//#include <nav_msgs/OccupancyGrid.h>

bool pickup = false;
bool dropoff = false;

void odom_Callback(const nav_msgs::Odometry::ConstPtr &msg)
{
  float x = msg->pose.pose.position.x;
  float y = msg->pose.pose.position.y;
  float z = msg->pose.pose.position.z;

  if ((x < -4.36 && x > -4.86) && (y < 3.58 && y > 4.08) && !pickup && !dropoff)
  {
    pickup = true;
  }
  else if ((x < 4.0 && x > 3.5) && (y < 4.1 && y > 3.7) && pickup && !dropoff)
  {
    pickup = false;
    dropoff = true;
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber odom_sub = n.subscribe("/odom", 100, odom_Callback);

  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CYLINDER;

  visualization_msgs::Marker marker;
  // Set the frame ID and timestamp.  See the TF tutorials for information on these.
  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();

  // Set the namespace and id for this marker.  This serves to create a unique ID
  // Any marker sent with the same namespace and id will overwrite the old one
  marker.ns = "marker_base";
  marker.id = 0;

  // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
  marker.type = shape;

  // Set the scale of the marker -- 1x1x1 here means 1m on a side
  marker.scale.x = 0.3;
  marker.scale.y = 0.3;
  marker.scale.z = 0.3;

  // Set the color -- be sure to set alpha to something non-zero!
  marker.color.r = 1.0f;
  marker.color.g = 0.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;

  while (ros::ok())
  {
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }

    if (pickup)
    {
      marker.pose.position.x = -3.6;
      marker.pose.position.y = 7.0;
      marker.action = visualization_msgs::Marker::DELETE;
    }

    if (dropoff)
    {
      marker.pose.position.x = 4.0;
      marker.pose.position.y = 4.0;
      marker.action = visualization_msgs::Marker::ADD;
    }

    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    marker.lifetime = ros::Duration();

    marker_pub.publish(marker);
  }
}