#!/usr/bin/env python
import roslib
roslib.load_manifest('roboga')
import rospy
import tf
import tf_conversions
from nav_msgs.msg import Odometry
import math



def handle_pose_msg(odomMsg, robot):
	br = tf.TransformBroadcaster()
	time =  rospy.Time.now()

	#br.sendTransform(
	#	      (odomMsg.pose.pose.position.x,
	#		odomMsg.pose.pose.position.y,
	#		odomMsg.pose.pose.position.z),
	#	      (odomMsg.pose.pose.orientation.x,
	#		odomMsg.pose.pose.orientation.y,
	#		odomMsg.pose.pose.orientation.z,
	#		odomMsg.pose.pose.orientation.w),
	#	       time,
	#	      '%s_base' % robot,
	#	      '%s_odom' % robot)

	br.sendTransform(
		      (0.189, 0, 0.254),
		      tf_conversions.transformations.quaternion_from_euler(0, 0, math.pi),
		      time,
		      '%s_lidar' % robot,
		      '%s_base' % robot)


	br.sendTransform(
		      (0.258, 0, 0.216),
		      (tf_conversions.transformations.quaternion_from_euler(0, 0, 0)),
		      time,
		      '%s_zed' % robot,
		      '%s_base' % robot)



if __name__ == '__main__':
	rospy.init_node('tf_broadcaster')
	robot = rospy.get_param('~robot_name')
	#rospy.Subscriber('/%s/odom_wheels' % robot,
	#		 Odometry,
        #		         handle_pose_msg,
	#	         robot)

	rate = rospy.Rate(100) # 30hz
    	while not rospy.is_shutdown():
		br = tf.TransformBroadcaster()
		time =  rospy.Time.now()
		br.sendTransform(
			      (0.189, 0, 0.254),
			      tf_conversions.transformations.quaternion_from_euler(0, 0, math.pi),
			      time,
			      '%s_lidar' % robot,
			      '%s_base' % robot)
		br.sendTransform(
			      (0.258, 0, 0.216),
			      (tf_conversions.transformations.quaternion_from_euler(0, 0, 0)),
			      time,
			      '%s_zed' % robot,
			      '%s_base' % robot)
		rate.sleep()
