#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <zordon_msgs/OdomCompacted.h>
#include <zordon_msgs/TwistCompacted.h>

#ifndef BASE_CONTROLLER_CONVERTER_H
#define BASE_CONTROLLER_CONVERTER_H

const int a = 4;
namespace zordon_nav
{
	class BaseControllerConverterNodelet: public nodelet::Nodelet
	{
		public:
			BaseControllerConverterNodelet() : wheel_distance_(0) {}
			virtual void onInit();
			void twistCb(const geometry_msgs::Twist & twist_msg);
			void odomCompactedCb(const zordon_msgs::OdomCompacted & odom_compacted_msg);

		private:
			ros::Publisher odom_pub_;
			ros::Publisher twist_compacted_pub_;
			ros::Subscriber odom_compacted_sub_;
			ros::Subscriber twist_sub_;

			nav_msgs::Odometry odom_msg_;
			zordon_msgs::TwistCompacted twist_compacted_msg_;
			float wheel_distance_;
			float wheel_radius_;
	};
}

#endif
