#!/usr/bin/env python
from jacto.srv import *
import rospy


# Variaveis globais
#point_array = SetGetRoute()
point_array = []


def handle_set_points(req):
    global point_array

    print("SET")
    point_array = req.point_array

    return SetGetRouteResponse(req.point_array)


def handle_get_points(req):
    global point_array

    print("GET")
    print(point_array)

    return SetGetRouteResponse(point_array)


def route_server():
    rospy.init_node('route_server')
    s = rospy.Service('/route/set_points', SetGetRoute, handle_set_points)
    g = rospy.Service('/route/get_points', SetGetRoute, handle_get_points)

    print("Ready to set and get points.")
    rospy.spin()

if __name__ == "__main__":
    route_server()
