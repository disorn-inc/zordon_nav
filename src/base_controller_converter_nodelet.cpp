#include "zordon_nav/base_controller_converter_nodelet.h"

#define WHEEL_DISTANCE 0.25f
#define WHEEL_RADIUS 0.024f

namespace zordon_nav {
	void BaseControllerConverterNodelet::onInit()
	{
		ros::NodeHandle &nh = getNodeHandle();
		ros::NodeHandle & private_nh = getPrivateNodeHandle();

		private_nh.param("wheel_distance", wheel_distance_, WHEEL_DISTANCE);
		private_nh.param("wheel_radius", wheel_radius_, WHEEL_RADIUS);

		this->odom_pub_ = nh.advertise<nav_msgs::Odometry>("odom", 10);
		this->twist_compacted_pub_ = nh.advertise<zordon_msgs::TwistCompacted>("cmd_vel_compacted", 10);

		this->odom_compacted_sub_ = nh.subscribe("/base_controller/odom", 10, &BaseControllerConverterNodelet::odomCompactedCb, this);
		twist_sub_ = nh.subscribe("cmd_vel", 10, &BaseControllerConverterNodelet::twistCb, this);
	}

	void BaseControllerConverterNodelet::twistCb(const geometry_msgs::Twist & twist_msg)
	{
		twist_compacted_msg_.linear = twist_msg.linear.x;
		twist_compacted_msg_.angular = twist_msg.angular.z;

		twist_compacted_pub_.publish(twist_compacted_msg_);
	}

	void BaseControllerConverterNodelet::odomCompactedCb(const zordon_msgs::OdomCompacted & odom_compacted_msg)
	{
		odom_msg_.twist.twist.linear.x = (odom_compacted_msg.wl + odom_compacted_msg.wr)*wheel_radius_/2;
		odom_msg_.twist.twist.angular.z = (odom_compacted_msg.wr - odom_compacted_msg.wl)*wheel_radius_/wheel_distance_;
;
		odom_msg_.pose.pose.position.x = odom_compacted_msg.x;
		odom_msg_.pose.pose.position.y = odom_compacted_msg.y;

		//odom_msg_.pose.pose.orientation.x = 0.0;
		odom_msg_.pose.pose.orientation.x = 0.0;
		odom_msg_.pose.pose.orientation.y = 0.0;
		odom_msg_.pose.pose.orientation.z = sin(odom_compacted_msg.yaw / 2.0);
		odom_msg_.pose.pose.orientation.w = cos(odom_compacted_msg.yaw / 2.0);

		odom_msg_.header.stamp = ros::Time::now();	
		odom_pub_.publish(odom_msg_);
	}
}
#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(zordon_nav::BaseControllerConverterNodelet,nodelet::Nodelet)
