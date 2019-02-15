#!/usr/bin/env python
import roslib
roslib.load_manifest('roboga')
import rospy
import tf_conversions
from nav_msgs.msg import Odometry
from std_msgs.msg import Header
seq_counter = 0


def get_header(robot):
	global seq_counter
	header = Header()
	header.seq = seq_counter
	header.stamp = rospy.Time.now()
	header.frame_id = '%s_odom' % robot
	seq_counter = seq_counter + 1
	return header

if __name__ == '__main__':
	rospy.init_node('odom_broadcaster')
	robot = rospy.get_param('~robot_name')
	child_frame_id = '%s_base' % robot
	pub = rospy.Publisher('/%s/odom_wheels' % robot, Odometry, queue_size=1)
	rate = rospy.Rate(100) # 30hz
    	while not rospy.is_shutdown():
		odom = Odometry()
		odom.header = get_header(robot)
		odom.child_frame_id = child_frame_id
		odom.pose.pose.position.x = 1
		odom.pose.pose.position.y = 1
		odom.pose.pose.position.z = 0
		q = tf_conversions.transformations.quaternion_from_euler(0, 0, 0)
		odom.pose.pose.orientation.x = q[0]
		odom.pose.pose.orientation.y = q[1]
		odom.pose.pose.orientation.z = q[2]
		odom.pose.pose.orientation.w = q[3]
		pub.publish(odom)
		odom.twist.twist.linear.x = 0
		odom.twist.twist.linear.y = 0
		odom.twist.twist.linear.z = 0
		odom.twist.twist.angular.x = 0
		odom.twist.twist.angular.y = 0
		odom.twist.twist.angular.z = 0
		rate.sleep()
