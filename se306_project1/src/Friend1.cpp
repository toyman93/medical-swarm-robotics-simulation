#include "ros/ros.h"
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/LaserScan.h>
#include <sstream>
#include "math.h"
#include "Friend1.h"
#include "se306_project1/ResidentMsg.h"
#include "time_conversion.hpp"

/**
*	@brief Returns the friend to its start point once its interaction with the resident is done.
*/
void Friend1::friendsDoneCallback(const ros::TimerEvent&){

	move("Friend1Origin");

}

/**
*	@brief Callback function that unpacks and processes resident status messages.
*	Friend should subscribe to the ResidentMsg topic in order for this callback to be called. ResidentMsg is published by the Resident.
*	@param msg A custom ResidentMsg message that contains information about the resident's current status.
*/
void Friend1::delegate(se306_project1::ResidentMsg msg) {
	if (msg.state == "emergency") {
		emergency = true; // required variable if timing/scheduling is done within this class
	}
	if (msg.state == "friends") { // resident state when it needs friends to converse with
		move("ResidentSofa");
	}
}

/**
*	@brief Main function for the Friend process.
*	Controls node setup and periodic events.
*/
int Friend1::run(int argc, char *argv[])
{

	/* -- Initialisation -- */
	
	//You must call ros::init() first of all. ros::init() function needs to see argc and argv. The third argument is the name of the node
	ros::init(argc, argv, "Friend1");

	//NodeHandle is the main access point to communicate with ros.
	ros::NodeHandle n;

	ros::Rate loop_rate(10);


	/* -- Publish / Subscribe -- */

	//advertise() function will tell ROS that you want to publish on a given topic_
	//to stage
	ros::Publisher RobotNode_stage_pub = n.advertise<geometry_msgs::Twist>("robot_7/cmd_vel",1000);


	//publish message to Resident once conversation is over
	ros::Publisher friend_pub = n.advertise<std_msgs::String>("visitorConvo",1000);

	//subscribe to listen to messages coming from stage
	ros::Subscriber StageOdo_sub = n.subscribe("robot_7/odom",1000, &Agent::StageOdom_callback, dynamic_cast<Agent*>(this));


	//custom Resident subscriber to "resident/state"
	ros::Subscriber resident_sub = n.subscribe<se306_project1::ResidentMsg>("residentStatus",1000, &Friend1::delegate, this);

	// Periodic callback
	int friendsDone = time_conversion::simHoursToRealSecs(11.5);
	ros::Timer friendsDoneTimer = n.createTimer(ros::Duration(friendsDone), &Friend1::friendsDoneCallback, this);


	////messages
	//velocity of this RobotNode
	geometry_msgs::Twist RobotNode_cmdvel;


	while (ros::ok())
	{
		//messages to stage
		RobotNode_cmdvel.linear.x = linear_x;
		RobotNode_cmdvel.angular.z = angular_z;
			
		//publish the message
		RobotNode_stage_pub.publish(RobotNode_cmdvel);
		
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
	
/**
*	@brief Redirects to main function (run()) of the node.
*/
int main(int argc, char *argv[]) {

	Friend1 *a = new Friend1();
	a->Friend1::run(argc, argv);
}
