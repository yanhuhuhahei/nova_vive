# 获取vive tracker 和SenseGlove_nova数据
## 步骤
1. 安装steamVR，以连接vive tracker  
2. [SenseGlove](https://github.com/Adjuvo/SenseGlove-API)连接需要蓝牙和SenseCom软件  


## vive tracker连接
### 硬件
基站、vive tracker、信号接受器

### SteamVR
1. 从ubuntu software上下载Steam installer。进入steam，从library中下载steamVR[linux_v1.14]。  
2. 需要设置steamVR，让它可以在没有头盔显示器的状态下使用：  
    - 右击steamVR -> Manage -> Browse local files  
    - 打开 drivers/null/resources/settings/default.vrsettings，将其中的enable改为true
    - 打开 resources/settings/default.vrsettings，在steamvr中修改:  
  
                "requireHmd": false,  
                "forcedDriver": "null",  
                 "activateMultipleDrivers": true,   
   -  到~/.steam/debian-installation/config/steamvr.vrsetting, 在steamvr中加入：

                "requireHmd": false,  
                "forcedDriver": "null", 
    
3. 设置完成后，打开vive基站和tracker，再launch SteamVR，等待三角花瓣图案变成绿色  
   ![2023-10-12 21-22-04 的屏幕截图.png](https://s2.loli.net/2023/10/12/VGfUdPQN6LJeuiR.png)
4. 为验证tracker是否连接成功，可以在triad_openvr文件夹下，打开终端，运行  
   
        python3 tracker_test.py
    若能看到变化的数据（x,y,z,yaw,pitch,roll），则说明连接成功.
    ![2023-10-12 21-25-49 的屏幕截图.png](https://s2.loli.net/2023/10/12/8Fq5QwTELk6CsUn.png)
   

## SenseGlove nova连接
### 硬件
按下按键，蓝色灯闪烁
### 连接蓝牙和软件

1. 将ubuntu当前用户加入串口所在组  
	
        sudo adduser $USER dialout  
   $USER为当前Ubuntu用户名，完成后注销Ubuntu重新登录 
2. 打开终端，  
   
        cd ~/nova_vive/SenseGlove-API/SenseCom/SenseCom_Linux/SenseCom
3. ctrl+shift+o分屏，在第一个终端中运行  
   
        ./sgc_l.sh  
   显示Press CTRL-C for hangup，并且蓝色光从闪烁变为常亮
   ![2023-10-12 21-28-48 的屏幕截图.png](https://s2.loli.net/2023/10/12/haVxf4OUsb9GmKN.png)
4. 在第二个终端中运行  
   
        chmod a+x SenseCom.x86_64
       ./SenseCom.x86_64  
   启动SenseCom软件，手掌颜色为实心蓝色，且有wifi标识。  
   ![2023-10-12 21-28-57 的屏幕截图.png](https://s2.loli.net/2023/10/12/7hGOBZJXWcFaIKm.png)  
5. SenseCom软件校准在linux中不准确，需要进行程序校准，软件中的自动校准可以关掉。

## 运行数据收发程序
### Senseglove nova
					
1. 打开终端   
   
        cd ~/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/build
2. 运行  ./SGCore_Sonny 按照窗口提示进行校准，即可看到手指数据更新。  
   * SGCore_Sonny是经过编译之后的可执行程序，可修改main_sony.cpp（~/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp），再进入到build中，执行：
  
     		cmake ..  
        	make
3. main_sonny.cpp中的handPose储存手指关节信息
4. 具体API参考[官方文档](https://senseglove.gitlab.io/SenseGloveDocs/native/cpp-reference.html)  

### vive tracker

  ~/nova_vive/vive_tracker/scripts下的vive_tracke.py 程序用来获取vive tracker的位置信息

### 获得末端位置
1. 打开两个终端:  
    第一个：
        
        cd ~/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/build
    第二个：

        cd ~/nova_vive/vive_tracker/scripts

2. 先在第一个终端中运行：  
   
        ./SGCore_Sonny
    然后在第二个终端中运行：

        python3 vive_tracke.py
3. 运行成功之后，可在第一个终端的窗口中看到 Wrist Position 和 Wrist Rotation
   ![2023-10-12 22-02-24 的屏幕截图.png](https://s2.loli.net/2023/10/12/yTIBanW7qPkuVbK.png)



   
