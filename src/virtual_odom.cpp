#include "ros/ros.h"
#include <nav_msgs/Odometry.h>
#include <math.h>

#define WHEEL_RADIUS 0.0251
#define WHEELS_DISTANCE 0.25

const std::string PREFIX_MSG = "Mirela";

nav_msgs::Odometry odometry_msg;

const std::string odometry_msg_name = PREFIX_MSG + "/wheels_odom";
const std::string odometry_msg_child_frame_id = PREFIX_MSG + "_base";
const std::string odometry_msg_frame_id = PREFIX_MSG + "_odom";

const std::string cmd_vel_msg_name = PREFIX_MSG + "/cmd_vel";

struct Velocity {
	double linear;
	double angular;
};

struct Pose {
	double x;
	double y;
	double theta;
};

struct Velocity baseVelocity = {0,0};
struct Pose basePose = {0,0,0};

void commandVelocityCallback(const geometry_msgs::Twist& msg)
{
	baseVelocity.linear = msg.linear.x;
	baseVelocity.angular = msg.angular.z;	
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "virtual_odom");
	ros::NodeHandle n;
	ros::Publisher odometry_msg_pub = n.advertise<nav_msgs::Odometry>("Mirela/wheels_odom", 1);
	ros::Subscriber cmd_vel_sub = n.subscribe("Mirela/cmd_vel", 1000, &commandVelocityCallback);
	ros::Rate loop_rate(100);


	double time_last = 0;	

	while (ros::ok()) {
		
		/*** Calculo da posicao e da orientacao
			basePose.x = basePose.x + time_elapsed * WHEEL_RADIUS * baseVelocity.linear * cos(basePose.theta);
			basePose.y = basePose.y + time_elapsed * WHEEL_RADIUS * baseVelocity.linear * sin(basePose.theta);
			basePose.theta = basePose.theta + time_elapsed * baseVelocity.angular * WHEEL_RADIUS;
		*/
		double time_now = ros::Time::now().toSec();
		double time_elapsed = time_now - time_last;
		time_last = time_now;
		
		basePose.x = basePose.x + time_elapsed * baseVelocity.linear * cos(basePose.theta);
		basePose.y = basePose.y + time_elapsed * baseVelocity.linear * sin(basePose.theta);
		basePose.theta = basePose.theta + time_elapsed * baseVelocity.angular  * 2;

		odometry_msg.twist.twist.linear.x = baseVelocity.linear;
		odometry_msg.twist.twist.angular.z = baseVelocity.angular;
		//odometry_msg.twist.twist.angular.x = left_wheel.velocity;
		//odometry_msg.twist.twist.angular.y = right_wheel.velocity;


		odometry_msg.pose.pose.position.x = basePose.x;
		odometry_msg.pose.pose.position.y = basePose.y;
		odometry_msg.pose.pose.position.z = 0.0;//pose.theta *180/3.1415;
		odometry_msg.pose.pose.orientation.x = 0.0;//goal_vel.angular[2];
		odometry_msg.pose.pose.orientation.y = 0.0;//dxl_wb.itemRead(DXL_ID, "Present_Position");
		odometry_msg.pose.pose.orientation.z = sin(basePose.theta/2.0);
		odometry_msg.pose.pose.orientation.w = cos(basePose.theta/2.0);
		//odometry_msg.pose.covariance[0]  = 0.2; ///< x
		//odometry_msg.pose.covariance[7]  = 0.2; ///< y
		//odometry_msg.pose.covariance[35] = 0.4; ///< yaw
		odometry_msg.header.stamp = ros::Time::now();

		odometry_msg_pub.publish(odometry_msg);

		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
