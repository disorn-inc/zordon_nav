#!/usr/bin/env python
import rospy
import actionlib       # Use the actionlib package for client and server
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from zordon_nav.srv import SetGetRoute


# Define Goal Points and orientations for TurtleBot in a list
#GoalPoints = [ [(3.0, 0.0, 0.0), (0.0, 0.0, 0.0, 1.0)] , [(3.0, 3.6, 0.0), (0.0, 0.0, 0.707, 0.707)]]

def get_points():
    goal_points = []
    rospy.wait_for_service('/route/get_points')
    try:
        get_route = rospy.ServiceProxy('/route/get_points', SetGetRoute)
        points = get_route([])  # Faz requisicao ao service, passando uma lista vazia como parametro

        # Obtem os dados e joga em uma lista
        for p in points.point_array:
            goal_points.append([(float(p.x), float(p.y), 0.0), (0.0, 0.0, 0.0, 1.0)])
        
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

    return goal_points

# The function assign_goal initializes the goal_pose variable as a MoveBaseGoal action type.
#
def assign_goal(pose):
    goal_pose = MoveBaseGoal()
    goal_pose.target_pose.header.frame_id = 'map'
    goal_pose.target_pose.pose.position.x = pose[0][0]
    goal_pose.target_pose.pose.position.y = pose[0][1]
    goal_pose.target_pose.pose.position.z = pose[0][2]
    goal_pose.target_pose.pose.orientation.x = pose[1][0]
    goal_pose.target_pose.pose.orientation.y = pose[1][1]
    goal_pose.target_pose.pose.orientation.z = pose[1][2]
    goal_pose.target_pose.pose.orientation.w = pose[1][3]

    return goal_pose


if __name__ == '__main__':
    # Inicializa o node
    rospy.init_node('MoveTBtoGoalPoints')

    # Create a SimpleActionClient of a move_base action type and wait for server.
    client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
    client.wait_for_server()

    # Obtem a lista dos goal points
    GoalPoints_inicial = []

    rate = rospy.Rate(2) # 2hz
    while not rospy.is_shutdown():
        # Obtem a lista dos goal points
        GoalPoints = get_points()

        if (len(GoalPoints) > 0):
            if (GoalPoints != GoalPoints_inicial):
                GoalPoints_inicial = GoalPoints
                # Seta como goal cada ponto da lista
                for TBpose in GoalPoints:
                    TBgoal = assign_goal(TBpose)   # For each goal point assign pose
                    client.send_goal(TBgoal)
                    client.wait_for_result()

                    if (client.get_state() == actionlib.GoalStatus.SUCCEEDED):
                        rospy.loginfo("success")
                    else:
                        rospy.loginfo("failed")
                    
        rate.sleep()
