#include "ros/ros.h"
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include "se306_project1/ResidentMsg.h"
#include "se306_project1/AssistantMsg.h"
#include <sstream>
#include "math.h"
#include "time_conversion.hpp"
#include "Assistant.h"
#include <se306_project1/src/Agent.h>

/**
*	@brief Callback function that unpacks and processes resident status messages.
* 	Calls other callback (do) functions.
*	Assistant should subscribe to the ResidentMsg topic in order for this callback to be called. ResidentMsg is published by the Resident.
*	@param msg A custom ResidentMsg message that contains information about the resident's current status.
*/
void Assistant::delegate(se306_project1::ResidentMsg msg) {
	// Resident status will be a string - one among SILL, ILL, HUNGRY, TIRED BORED, and HEALTHCARE - see Mustafa's pq.
	// alternatively we could send the status in another format.
}

/**
*	@brief Periodic callback for the provision of medication.
*	Called by the ros::Timer in the run() function. Can specify start time, end time, and period.
*	@note The callback is called at the end of the duration specified for the timer.
*	@param TimerEvent& TimerEvent generated by a ros::Timer.
*	@returns true if behaviour was successful, false otherwise
*/
bool Assistant::doMedication(const ros::TimerEvent&) {
	return true;
}

/**
*	@brief Causes assistant to cook for resident and return to them with the food, feeding them.
*	@returns true if behaviour was successful, false otherwise
*/
bool Assistant::doCooking() {
	return true;
}

/**
*	@brief Causes assistant to clean the house.
*	@returns true if behaviour was successful, false otherwise
*/
bool Assistant::doCleaning() {
	return true;
}	

/**
*	@brief Causes assistant to entertain the resident.
*	@returns true if behaviour was successful, false otherwise
*/
bool Assistant::doEntertainment() {
	return true;
}

/**
*	@brief Assistant co-ordinates with other assistants in deciding which one picks up a task.
*	@returns true if this robot is to perform the task, and false if some other assistant is to do it.
*	@note Not totally sure if this is the implementation we're going for or not.
*/
bool Assistant::coordinate() {
	return true;
} 

/**
*	@brief Main function for the Assistant process.
*	Controls node setup and periodic events.
*/
int Assistant::run(int argc, char **argv) {

	/* -- Initialisation -- */
	
	//You must call ros::init() first of all. ros::init() function needs to see argc and argv. The third argument is the name of the node
	ros::init(argc, argv, "Assistant");

	//NodeHandle is the main access point to communicate with ros.
	ros::NodeHandle n;

	ros::Rate loop_rate(10);


	/* -- Publish / Subscribe -- */

	//advertise() function will tell ROS that you want to publish on a given topic_
	//to stage
	ros::Publisher RobotNode_stage_pub = n.advertise<geometry_msgs::Twist>("robot_0/cmd_vel",1000); 

	//subscribe to listen to messages coming from stage
	ros::Subscriber StageOdo_sub = n.subscribe<nav_msgs::Odometry>("robot_0/odom",1000, &Agent::StageOdom_callback,this);

	////messages
	//velocity of this RobotNode
	geometry_msgs::Twist RobotNode_cmdvel;
	

	while (ros::ok())
	{
		//messages to stage
		//RobotNode_cmdvel.linear.x = linear_x;
		//RobotNode_cmdvel.angular.z = angular_z;
			
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
	Assistant *a = new Assistant();
	a->Assistant::run(argc, argv);
}