##!/usr/bin/env python3

import triad_openvr
import time
import sys

import numpy as np
import math
# import pdb
import socket
import struct
from scipy.spatial.transform import Rotation as R
# import matplotlib.pyplot as plt


def vive_tracker():
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

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('127.0.0.1', 3333)  # 服务器的IP地址和端口号
    client_socket.connect(server_address)


    while 1:
      for deviceName in v.devices:
          # print(deviceName)
          if 'Null' in v.devices[deviceName].get_serial():
            del v.devices[deviceName]
            break
          if deviceName == "tracker_1":
            # publish_name_str = v.devices[deviceName].get_serial().replace("-","_")

            # Broadcast the TF as a quaternion
            [x, y, z, _qx, _qy, _qz, _qw] = v.devices[deviceName].get_pose_quaternion()

            if "LHR" in v.devices[deviceName].get_serial() and _qw !=1:
              r0 = R.from_quat([_qx, _qy, _qz, _qw])
              r1 = R.from_euler('x', -90, degrees=True)
              r2 = r0.as_matrix().dot(r1.as_matrix())
              [qx, qy, qz, qw] = R.from_matrix(r2).as_quat()
            if qw !=0:
              
              # packed_data = struct.pack(f'{len([_x, _y, _z, qx, qy, qz, qw])}f', *[_x, _y, _z, qx, qy, qz, qw])
              packed_data = struct.pack('!{}f'.format(len([x, y, z, qx, qy, qz, qw])), *[x, y, z, qx, qy, qz, qw])
              # print(x)
              # s.sendall("".join([str(_) for _ in [x, y, z, qx, qy, qz, qw]]).encode())
              client_socket.sendall(packed_data)
            time.sleep(0.00001)



if __name__ == '__main__':
    vive_tracker()
