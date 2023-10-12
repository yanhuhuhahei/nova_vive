/*
A console application demonstrating the intended use of a SenseGlove interface.
Used to compile the programs in the SGCoreCpp/test folder.
*/

#include <iostream> //Output to console

#include <thread>  //To pause the main() while vibrating
#include <chrono>  //To pause the thread for std::chrono::seconds

#include "Library.h" //Contains version information on SGCore / SGConnect Libraries
#include "SenseCom.h" //Functions to check scanning process - and to start it if need be.

#include "HapticGlove.h" //Haptic Glove Interfacing
#include "Tracking.h" //To calculate wrist location based on fixed hardware offsets.
#include "SG_FFBCmd.h" //force-feedback command(s)
#include "SG_BuzzCmd.h" //vibration command(s)
#include "SenseGlove.h"

#include <string>
#include <vector>
// #include <winsock2.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include<iostream>
using namespace std;


// #pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
// #include <thread>

//ȫ�ֱ���
std::vector<float> vive_data(7);

// Socketͨ�����̺߳���
void SocketThread() {
	// initialize Winsock
	// WSADATA wsaData;
	// if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
	// {
	// 	std::cerr << "Failed to initialize Winsock." << std::endl;
	// 	return;
	// }


	// ���� Socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		std::cerr << "Failed to create socket." << std::endl;
		// WSACleanup();
		return;
	}

	// 绑定ip和端口
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(3333);
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		std::cerr << "Failed to bind socket." << std::endl;
		close(serverSocket);
		// WSACleanup();
		return;
	}

	// ��������
	if (listen(serverSocket, 1) == -1)
	{
		std::cerr << "Failed to listen on socket." << std::endl;
		close(serverSocket);
		// WSACleanup();
		return;
	}

	std::cout << "Waiting for incoming connection..." << std::endl;

	// ��������
	int clientSocket = accept(serverSocket, NULL, NULL);
	if (clientSocket == -1)
	{
		std::cerr << "Failed to accept connection." << std::endl;
		close(serverSocket);
		// WSACleanup();
		return;
	}

	std::cout << "Connected with client." << std::endl;

	//while (1) {
	//	int data_Length = 7;
	//	//std::vector<float> data(dataLength);
	//	for (unsigned int i = 0; i < data_Length; i++) {
	//		int bytesReceived = recv(clientSocket, (char*)&vive_data[i], sizeof(float), 0);
	//		if (bytesReceived != sizeof(float)) {
	//			std::cerr << "Failed to receive data." << std::endl;
	//			break;
	//		}
	//	}
	//	std::cout << "Received data: ";
	//	//for (float value : vive_data) {
	//	//	std::cout << value << " ";
	//	//}
	//	std::cout << vive_data[0] << " ";
	//	std::cout << std::endl;
	//}
	while (true) {
		int bytesReceived = recv(clientSocket, (char*)vive_data.data(), 7 * sizeof(float), 0);
		if (bytesReceived != 7 * sizeof(float)) {
			std::cerr << "Failed to receive data." << std::endl;
		}
		else {
			 //解析数据，确保使用大端序
			for (int i = 0; i < 7; ++i) {
				unsigned char* bytes = reinterpret_cast<unsigned char*>(&vive_data[i]);
				std::swap(bytes[0], bytes[3]);
				std::swap(bytes[1], bytes[2]);
			}

			//std::cout << "Received data: ";
			//for (float value : vive_data) {
			//	std::cout << value << " ";
			//}
			//std::cout << std::endl;
		}
	}
	close(clientSocket);
	close(serverSocket);
	// WSACleanup();
}


int main()
{   
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // calibration in ubuntu

	std::cout << "Testing SGCore C++ Library - Calibration"<< std::endl;
	std::cout << "=======================================" << std::endl;
    // while (true)
	// if (SGCore::DeviceList::SenseCommRunning()) //check if the Sense Comm is running. If not, warn the end user.
	std::shared_ptr<SGCore::HapticGlove>  testglove;
	SGCore::HandProfile hand_profile_(SGCore::HandProfile::Default(false));
	if (SGCore::SenseCom::ScanningActive()) //check if the Sense Comm is running. If not, warn the end user.
	{
		if (SGCore::HapticGlove::GetGlove(testglove))
		{
			std::cout << "Connected to a " << (testglove->IsRight() ? "right" : "left") << "-handed SenseGlove. Staring calibration" << std::endl;

			/* 
			Our goal is to find the min / max sensor values, which correspond to the user opening their hand and making a fist.
			We can only update this range after parsing sensor data, which happens when accessing sensorData, glovePoses or handPoses.
			In our VR use cases, we pull new hand data each frame, and so this min/max range is updated automatically. 
			In this example, we will update the range twice; once when the hand is 'open', once when it is closed into a fist.
			*/ 		

			// Step 1: Open hand - Calibrates extension
			std::cout << std::endl;
			std::cout << "Step 1: Place your hand on a flat surface, like a table, and spread your thumb and fingers." << std::endl;
			// Once you get the hang of this motion, you can do it without the surface.
			std::cout << "Once your hand is in the right position, press any key to continue" << std::endl; 
			std::cin.get();
			
			// This function updates the calibration range of testglove.
			testglove->UpdateCalibrationRange(); // Instead of this method, you can also use the GetSensorData(), GetGlovePose() or GetHandPose function instead.


			// Step 2: Fist - Calibrates 3-finger flexion
			std::cout << std::endl;
			std::cout << "Step 2: Close your hand into a fist for next 10 seconds. Make sure your fingers aren't wrapped around your thumb." << std::endl;
			std::cout << "Once you are ready, press any key to continue" << std::endl;
			std::cin.get();
		// record start time
		auto start = std::chrono::high_resolution_clock::now();
			// update calibration range for 5 seconds
		while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count() < 10)
		{
		// This function updates the calibration range of testglove.
		testglove->UpdateCalibrationRange(); // Instead of this method, you can also use the GetSensorData(), GetGlovePose() or GetHandPose function instead.
		}
		std::cout << "Step 2 Done." << std::endl;

		// Step 3: Calibrates thumb 
		std::cout << std::endl;
			std::cout << "Step 3: move your thumb to the extreme bounds for next 10 seconds." << std::endl;
			std::cout << "Once you are ready, press any key to continue" << std::endl;
			std::cin.get();
		// record start time
		start = std::chrono::high_resolution_clock::now();
			// update calibration range for 10 seconds
		while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count() < 10)
		{
		// This function updates the calibration range of testglove.
		testglove->UpdateCalibrationRange(); // Instead of this method, you can also use the GetSensorData(), GetGlovePose() or GetHandPose function instead.
		}
		std::cout << "Step 3 Done." << std::endl;

			// At this point, we've collected data while the hand was open, and when it was closed. 
			// The calibration range should now have the two extremes to interpolate between.
			// Let's check & ouput the ranges:
			std::vector<SGCore::Kinematics::Vect3D> minRanges, maxRanges;
			testglove->GetCalibrationRange(minRanges, maxRanges);
			
			// The calibration ranges contain the x, y, z values, which represent the pronation/supination, flexion/extension, and abduction/adduction movements respectively, in radians. 
			// For readability's sake, we'll print out the flexion/extension values in degrees.
			float rad2Deg = 180 / M_PI;
			std::cout << std::endl << "Evaluated the following calibration range for extension/flexion" << std::endl;
			std::cout << "Extensions: ";
			for (int f = 0; f < minRanges.size(); f++)
			{
				std::cout << std::to_string((int)(rad2Deg * minRanges[f].y));
				if (f < minRanges.size() - 1) { std::cout << ", "; }
			}
			std::cout << std::endl << "Flexions: ";
			for (int f = 0; f < maxRanges.size(); f++)
			{
				std::cout << std::to_string((int)(rad2Deg * maxRanges[f].y));
				if (f < maxRanges.size() - 1) { std::cout << ", "; }
			}
			std::cout << std::endl;

			// Now we apply the calibration to the default profile
			testglove->ApplyCalibration(hand_profile_);

			// And can now use it to calculate handPoses
			SGCore::HandPose handPose;
			
			if (testglove->GetHandPose(hand_profile_, handPose))
				// if (testglove->GetHandPose(handPose))
				{
					std::cout << std::endl << "With these ranges, we've calculated the following hand angles:" << std::endl;
					std::cout << handPose.ToString() << std::endl;
				}
			else
				{
					std::cout << "Something went wrong while trying to calucate a handPose. Perhaps a packet was dropped or an exception occurs." << std::endl;
				}
			
			
			
			// Finally, we can store the profile in its serialized form to use later
			std::string serializedProfile = hand_profile_.Serialize();

			// And Deserialize it back into a useable profile
			SGCore::HandProfile loadedProfile = SGCore::HandProfile::Deserialize(serializedProfile);
		}
	}
	else
		std::cout << "SenseComm is not running. Please start it and try again." << std::endl;

	std::cout << "=======================================" << std::endl;
	std::cout << "Press any key to exit calibration." << std::endl;
	std::cin.get();


	std::thread childThread(SocketThread);

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Checking the Library

	// Displaying Library information - Useful to know when asking for any kind of support
	std::cout << "Testing " + SGCore::Library::Version() + ", compiled for " + SGCore::Library::BackEndVersion();
	if (SGCore::Library::GetBackEndType() == SGCore::BackEndType::SharedMemory) // By default, your library will be compiled to use Shared Memory via the SGConnect library
    {
		std::cout << " using " + SGCore::Library::SGConnectVersion(); //If you replace SGConnect.dll, this will give you its current version number.
    }
	std::cout << std::endl;
	std::cout << "The source code for this program is located in the SGCoreCs/test/ folder." << std::endl;
	std::cout << "=========================================================================" << std::endl;


	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Ensuring connectivity

	// Connecting to SenseGlove devices is done in a separate "Connection Process" - contained in the SGConnect library.
	// We can test if this Connection Process is running on this PC. Usually, it runs inside SenseCom.
	// It's good practice to start this process if your user has not sone so yet.
	{
		bool connectionsActive = SGCore::SenseCom::ScanningActive(); //returns true if SenseCom (or another program) has started the SenseGlove Communications Process.
		if (!connectionsActive) // If this process is not running yet, we can "Force-Start" SenseCom. Provided it has run on this PC at least once.
		{
			std::cout << "SenseCom is not yet running. Without it, we cannot connect to SenseGlove devices." << std::endl;
			bool startedSenseCom = SGCore::SenseCom::StartupSenseCom(); //Returns true if the process was started.
			if (startedSenseCom)
			{
				std::cout << "Successfully started SenseCom. It will take a few seconds to connect..." << std::endl;
				connectionsActive = SGCore::SenseCom::ScanningActive(); //this will return false immedeately after you called StartupSenseCom(). Because the program has yet to initialize.
																		// Even if SenseCom started and the connections process is active, there's no guarantee that the user has turned their device(s) on. More on that later.
			}
			else // If StartupSenseCom() returns false, you've either never run SenseCom, or it is already running. But at that point, the ScanningActive() should have returned true. 
			{
				std::cout << "Could not Start the SenseCom process. This is not yet implemented in our C++ API.";
			}
			std::cout << std::endl;
		}
	}




	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Accessing Haptic Gloves

	// You interface with SenseGlove devices through an instance of a HapticGlove class.
	// At any point in your program, you can try to grab a SenseGlove Device. There's no guarantee that one is connected, though.
	// Let's try and grab a Haptic Glove, a class from which both our Nova Glove and SenseGlove DK1 Exoskeletons derive.
	// Using the HapticGlove as opposed to the specific Nova / SenseGlove class allows your software to be compatible with both types of devices (as well as any other types of gloves we add later).
	{
		std::shared_ptr<SGCore::HapticGlove> testGlove; //SGCore.HapticGlove - The class from which all of our gloves derive. For this reason, we use a (smart) pointer, rather than a class itself.
		bool gotHapticGlove = SGCore::HapticGlove::GetGlove(testGlove); //GetGlove returns the first glove connected to your system - doesn't care if it's a left or a right hand.
		while (!gotHapticGlove) //For this exercise, I'll keep trying to connect to a glove.
		{
			std::cout << "Failed to Detect a Haptic Glove on your System. Please ensure your device is paired or connected via USB." << std::endl;
			std::cout << "Press Return to try again..." << std::endl;
			system("pause");
			gotHapticGlove = SGCore::HapticGlove::GetGlove(testGlove);
		}

		// If we were looking for a specific hand, we can pass a boolean to the GetGlove function. I know you have at least one - or you wouldn't have passed this section.
				
		std::shared_ptr<SGCore::HapticGlove> leftGlove, rightGlove;
		bool gotLeft  = SGCore::HapticGlove::GetGlove(false, leftGlove); // passing false returns the first connected left hand
		bool gotRight = SGCore::HapticGlove::GetGlove(true,  rightGlove); // passing true returns the first connected right hand

		// Alternatively, you can get a list of all connected Haptic Gloves on your system
		std::vector<std::shared_ptr<SGCore::HapticGlove>> allGloves = SGCore::HapticGlove::GetHapticGloves();

		std::cout << "There are " + std::to_string(allGloves.size()) + " Haptic Glove(s) connected to your system." << std::endl;

		if (gotLeft)
		{ std::cout << "Your Left hand is " + leftGlove->ToString() << std::endl;; }
		else
		{ std::cout << "You have no left hands connected to your system." << std::endl; }

		if (gotRight)
		{ std::cout << "Your Right hand is " + rightGlove->ToString() << std::endl; }
		else
		{ std::cout << "You have no right hands connected to your system." << std::endl; }

		std::cout << "The first Haptic Glove connected to your system is " + testGlove->ToString() + " - we will use this one for testing." << std::endl;;
		std::cout << std::endl;
	}

	// socket communication

	// // initialize Winsock
	// WSADATA wsaData;
	// if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
	// 	std::cerr << "Failed to initialize Winsock." << std::endl;
	// 	return 1;
	// }


	int novaSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (novaSocket == -1) {
		std::cerr << "Failed to create socket." << std::endl;
		// WSACleanup();
		return 1;
	}

	// ���÷�������ַ�Ͷ˿�
	// struct sockaddr_in novaAddress;
	// novaAddress.sin_family = AF_INET;
	// novaAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	// novaAddress.sin_port = htons(8888);
	// bind(novaSocket, (struct sockaddr*)&novaAddress, sizeof(novaAddress));
	// if (bind(novaSocket, (struct sockaddr*)&novaAddress, sizeof(novaAddress)) == -1) {
	// 	std::cerr << "Failed to bind nova socket." << std::endl;
	// 	close(novaSocket);
	// 	// WSACleanup();
	// 	return 1;
	// }

	// if (connect(novaSocket, (struct sockaddr*)&novaAddress, sizeof(novaAddress)) == -1) {
	// 	std::cerr << "Failed to connect to nova server." << std::endl;
	// 	close(novaSocket);
	// 	// WSACleanup();
	// 	return 1;
	// }

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Using a Haptic Glove instance

	// SenseGlove Devices are chached inside the SGCore library. The GetGloves() function returns a reference to them, not a copy.
	// Let's pretend this is a frame of your simulation
	// Once you have a HapticGlove instance , you request specific data.
	{
		// Basic Device Information
		bool rightHand = true; // I'm going to use this for the next part of this test.
		std::shared_ptr<SGCore::HapticGlove> testGlove;
		if (SGCore::HapticGlove::GetGlove(testGlove)) //remember, if this function returns true, we've succesfully connected to a Glove.
        {
			rightHand = testGlove->IsRight();	//Returns true if this is a right hand. If false, this is a left hand.
			std::string deviceID = testGlove->GetDeviceID();	//The Device's Unique Identifier. Useful if you have multiple of them.
			std::string type = SGCore::SGDevice::ToString(testGlove->GetDeviceType()); //The DeviceType (SenseGlove DK1, SenseGlove Nova, etc.)
			std::cout << deviceID + " is a " + (rightHand ? "Right" : "Left") + " hand whose deviceType is " + type << std::endl;
        }


		std::shared_ptr<SGCore::HapticGlove> glove;

		if (SGCore::HapticGlove::GetGlove(rightHand, glove))
        {
			//Step 1: Hand Pose

			// We want to get the pose of the hand - to animate a virtual model
			SGCore::HandPose handPose; //The handPose class contains all data you'll need to animate a virtual hand
			//for (int i = 0; i < 1000; i++) {
			while(1) {
				// if (glove->GetHandPose(handPose)) //returns the HandPose based on the latest device data, using the latest Profile and the default HandGeometry
				if (testglove->GetHandPose(hand_profile_, handPose))
				{
					//std::cout << "Retrieved the latest Hand Pose from " + glove->ToString() + ". The ToString() function reports important finger angles, in degrees, in our coodinate system:" << std::endl;
					//std::cout << handPose.ToString() << std::endl;
					//std::cout << "x: " << handPose.handAngles[1][0].y << "y: " << handPose.handAngles[1][0].z << "z:  " << handPose.handAngles[1][1].y << "w:" << handPose.handAngles[1][2].y << std::endl;
					// std::cout << handPose.handAngles[1][0].y << std::endl;
					// The HandPose is still in our Coordinate System, relative to the wrist.
					// You'll need to convert our Vect3D and Quats to your own coordinate system. You can find an explanation of our Coordinate system at docs.senseglove.com/

					// We also offer "normalized" angles : Which gives you the total flexion of the finger(s) as a value between 0 and 1,
					// where 0 represents the fingers fully extended, and 1 represents the finger fully flexed.
					std::vector<float> flexions = handPose.GetNormalizedFlexion();
					std::string flexes = "";
					for (int i = 0; i < flexions.size(); i++)
					{
						flexes += std::to_string((int)std::round(flexions[i]));
						if (i < flexions.size() - 1) { flexes += ", "; }
					}
					//std::cout << "Normalized finger flexions [" + flexes + "]" << std::endl;
					//std::cout << std::endl;
				}
				else //This function could return false if no data for this glove is available (yet).
				{
					std::cout << "Something went wrong trying to access the HandPose of " + glove->ToString() + ". Try again later.";
				}
				//std::this_thread::sleep_for(std::chrono::milliseconds(200));
				
				//int dataLength = 7;
				//std::vector<float> data(dataLength);
				//for (unsigned int i = 0; i < dataLength; i++) {
				//	int bytesReceived = recv(clientSocket, (char*)&data[i], sizeof(float), 0);
				//	if (bytesReceived != sizeof(float)) {
				//		std::cerr << "Failed to receive data." << std::endl;
				//		break;
				//	}
				//}

				// ��ӡ���յ��ĸ���������
				//std::cout << "Received data: ";
				//for (float value : data) {
				//	std::cout << value << " ";
				//}
				//std::cout << std::endl;



			//Step 2: Wrist Location

			// The glove does not come with a built-in hand tracking. I want to know the location of the wrist in 3D space - to determine my finger locations, for example.
			// Let's say we've connected a Vive Tracker to the glove - and we know it's position / location
			SGCore::PosTrackingHardware tracker = SGCore::PosTrackingHardware::ViveTracker;
			SGCore::Kinematics::Vect3D position;
			position.x = vive_data[0] * 1000;
			position.y = vive_data[1] * 1000;
			position.z = vive_data[2] * 1000;
			SGCore::Kinematics::Vect3D trackerPosition = position; //set at 0, 0, 0
			//SGCore::Kinematics::Vect3D trackerPosition = SGCore::Kinematics::Vect3D zero; //set at 0, 0, 0
			//SGCore::Kinematics::Quat trackerRotation = SGCore::Kinematics::Quat::identity; //no rotation either.

			SGCore::Kinematics::Quat quat;
			quat.x = vive_data[3];
			quat.y = vive_data[4];
			quat.z = vive_data[5];
			quat.w = vive_data[6];

			//Eigen::Vector3d vector(data[0], data[1], data[2]);

			//Eigen::Vector3d pos = rotation_matrix1 * vector;

			//std::cout << "Result Vector:" << std::endl << pos << std::endl;
			//
			//Eigen::Quaterniond quaternion(data[6], data[3], data[4], data[5]);

			//// ����Ԫ��ת��Ϊ��ת����
			//Eigen::Matrix3d rotation_matrix = quaternion.toRotationMatrix();
			//
			//
			//Eigen::Matrix3d matrix = rotation_matrix1 * rotation_matrix;
			//
			//Eigen::Quaterniond quat_new(matrix);

			//std::cout << "Quaternion:" << std::endl;
			//std::cout << "w: " << quat_new.w() << ", x: " << quat_new.x()
			//	<< ", y: " << quat_new.y() << ", z: " << quat_new.z() << std::endl;

			// ��ӡ��ת����
			//std::cout << "Rotation Matrix:" << std::endl;
			//std::cout << rotation_matrix << std::endl;

			SGCore::Kinematics::Quat trackerRotation = quat; //no rotation either.


			SGCore::Kinematics::Vect3D wristPosition;
			SGCore::Kinematics::Quat wristRotation;
			glove->GetWristLocation(trackerPosition, trackerRotation, tracker, wristPosition, wristRotation);

			//std::cout << "If your tracked device (" + SGCore::Tracking::ToString(tracker) + ") is at position " + trackerPosition.ToString() + ", and rotation " + trackerRotation.ToEuler().ToString()
			//	+ ", your wrist position is " + wristPosition.ToString() + ", with rotation at " + wristRotation.ToEuler().ToString() << std::endl;
			////std::cout << "with rotation at " + wristRotation.ToString() << std::endl;
			//std::cout << std::endl;

			//std::vector<float> sendData = { 1.23f, 4.56f, 7.89f };
			//int sendDataLength = sendData.size();
			//send(novaSocket, (char*)&sendDataLength, sizeof(sendDataLength), 0);
			//for (float value : sendData) {
			//	send(novaSocket, (char*)&value, sizeof(value), 0);
			////}
			//Eigen::Vector3d vector(trackerPosition.x, trackerPosition.y, trackerPosition.z);

			//std::vector<float> dataArray = { wristPosition.x, wristPosition.y, wristPosition.z, wristRotation.ToEuler().x,  wristRotation.ToEuler().y, wristRotation.ToEuler().z };
			std::vector<float> dataArray = 
			{ wristPosition.x, wristPosition.y, wristPosition.z, wristRotation.x,  wristRotation.y, wristRotation.z,  wristRotation.w,
			//{pos_f[0], pos_f[1], pos_f[2], quat_f.coeffs()[3], quat_f.coeffs()[0],  quat_f.coeffs()[1],  quat_f.coeffs()[2],
			handPose.handAngles[0][0].x, handPose.handAngles[0][0].y, handPose.handAngles[0][0].z, handPose.handAngles[0][1].y,handPose.handAngles[0][2].y,
			handPose.handAngles[1][0].y, handPose.handAngles[1][0].z, handPose.handAngles[1][1].y,handPose.handAngles[1][2].y,
			handPose.handAngles[2][0].y, handPose.handAngles[2][0].z, handPose.handAngles[2][1].y,handPose.handAngles[2][2].y,
			handPose.handAngles[3][0].y, handPose.handAngles[3][0].z, handPose.handAngles[3][1].y,handPose.handAngles[3][2].y,
			};
			std::cout << "Wrist Position: (" << wristPosition.x << ", " << wristPosition.y << ", " << wristPosition.z << ")" << std::endl;
			// std::cout << "Wrist Rotation: (" << wristRotation.x << ", " << wristRotation.y << ", " << wristRotation.z << ", " << wristRotation.w << ")" << std::endl;
			// // std::cout << "hand angle: (" << handPose.handAngles[1][0].y << ", " << handPose.handAngles[1][0].z << ", " << handPose.handAngles[1][1].y << ", " << handPose.handAngles[1][2].y << ")" << std::endl;
			std::cout << handPose.ToString() << std::endl;

			// for (float value : dataArray) {
			// 	uint32_t temp;
			// 	memcpy(&temp, &value, sizeof(value));
			// 	uint32_t swapped = __bswap_32(temp);
			// 	// uint32_t swapped = _byteswap_ulong(temp);
			// 	if (send(novaSocket, (const char*)&swapped, sizeof(swapped), 0) == -1){
			// 		std::cout<< "Failed to send data array." << std::endl;
			// 		close(novaSocket);
			// 		// WSACleanup();
			// 		return 1;}
			// 	;
			// }
			//std::this_thread::sleep_for(std::chrono::milliseconds(200));

			}

		}
	}

}

