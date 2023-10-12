##!/usr/bin/env python3

import triad_openvr
import time
import sys

import numpy as np
import math
# import pdb
import socket
import struct

def vive_tracker():
    # rospy.init_node('vive_tracker_frame')
    broadcaster = { }
    publisher = { }
    # listener = tf.TransformListener()
    # rate = rospy.Rate(90) # 10hz]
    deviceCount = 0

    try:
      v = triad_openvr.triad_openvr()
    except Exception as ex:
      if (type(ex).__name__ == 'OpenVRError' and ex.args[0] == 'VRInitError_Init_HmdNotFoundPresenceFailed (error number 126)'):
        print('Cannot find the tracker.')
        print('Is SteamVR running?')
        print('Is the Vive Tracker turned on, connected, and paired with SteamVR?')
        print('Are the Lighthouse Base Stations powered and in view of the Tracker?\n\n')
      else:
        template = "An exception of type {0} occurred. Arguments:\n{1!r}"
        message = template.format(type(ex).__name__, ex.args)
        print (message)
      #print(ex.args)
      quit()

    v.print_discovered_objects()
    P = np.mat([[1e-6, 0, 0], [0, 1e-6, 0], [0, 0, 1e-3]])
    p_cov = np.zeros((6, 6))
    # position covariance
    p_cov[0:2,0:2] = P[0:2,0:2]
    # orientation covariance for Yaw
    # x and Yaw
    p_cov[5,0] = p_cov[0,5] = P[2,0]
    # y and Yaw
    p_cov[5,1] = p_cov[1,5] = P[2,1]
    # Yaw and Yaw
    p_cov[5,5] = P[2,2]

    p_cov[0,:] =   [0.0000349162103240595,  -0.0000018202960310455,  -0.0000339898160507969,  -0.0000081126791170800,   0.0000001353045808767,   0.0000032202291901186]
    p_cov[1,:] =    [-0.0000018202960310455,   0.0000011910722363973,   0.0000020423436706964,   0.0000010961526869235,  -0.0000000333091396801,  -0.0000001408541892558]
    p_cov[2,:] =    [-0.0000339898160507969,   0.0000020423436706964,   0.0000341312090595451,   0.0000060715616751347,  -0.0000000237628610568,  -0.0000029217229365340]
    p_cov[3,:] =    [-0.0000081126791170800,   0.0000010961526869235,   0.0000060715616751347,   0.0000165832615351042,  -0.0000004759697840205,  -0.0000024486872043021]
    p_cov[4,:] =    [0.0000001353045808767,  -0.0000000333091396801,  -0.0000000237628610568,  -0.0000004759697840205,   0.0000003366392930324,  -0.0000000030521109214]
    p_cov[5,:] =    [0.0000032202291901186,  -0.0000001408541892558,  -0.0000029217229365340,  -0.0000024486872043021,  -0.0000000030521109214,   0.0000007445433570531]
    # rospy.loginfo(p_cov)
    # while not rospy.is_shutdown():
        #if (deviceCount != v.get_device_count()):
        #    v = triad_openvr.triad_openvr()
        #    deviceCount = v.get_device_count()
        # For each Vive Device 

    # ip_port = ('127.0.0.1', 9999)
    # ip_port = ('127.0.0.1', 9999)

    # s = socket.socket()     # 创建套接字

    # s.connect(ip_port)      # 连接服务器
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('localhost', 9999)  # 服务器的IP地址和端口号
    client_socket.connect(server_address)
    while 1:
      for deviceName in v.devices:
          # print(deviceName)
          if 'Null' in v.devices[deviceName].get_serial():
            del v.devices[deviceName]
            break
          if deviceName == "tracker_1":
            publish_name_str = v.devices[deviceName].get_serial().replace("-","_")

            # Broadcast the TF as a quaternion
            [x, y, z, qx, qy, qz, qw] = v.devices[deviceName].get_pose_quaternion()
            # print([x, y, z, qx, qy, qz, qw])
            # print([x, y, z])
            
            # b = [float('{:.4f}'.format(i)) for i in [x, y, z, qx, qy, qz, qw]]
            # print(b)
            
            packed_data = struct.pack(f'{len([x, y, z, qx, qy, qz, qw])}f', *[x, y, z, qx, qy, qz, qw])
            # packed_data = struct.pack(f'{len(b)}f', *b)

            # s.sendall("".join([str(_) for _ in [x, y, z, qx, qy, qz, qw]]).encode())
            client_socket.sendall(packed_data)
            # time.sleep(0.00001)



if __name__ == '__main__':
    # try:
    #     vive_tracker()
    # except rospy.ROSInterruptException:
    #     pass
    vive_tracker()
