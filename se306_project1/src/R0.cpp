#include "ros/ros.h"
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/LaserScan.h>

#include <sstream>
#include "math.h"
#include "time_conversion.hpp"
#include <cmath>
#include <stdlib.h>

//velocity of the robot
double linear_x;
double angular_z;

//goal pose and orientation
double goal_x;
double goal_y;
double goal_angle;
bool isSet = false;

//current pose and orientation of the robot
double px;
double py;
double cur_angle;

int cc = 1; //current_checkpoint = 0;

std::pair<double, double> ret;	

int checkpoints[5][2] = {  
{30, 25}, 
{35, 35}, 
{12, 42},
{30, 42},
{30, 25}  
};

std::pair<double, double> move(double goal_x, double goal_y, double cur_angle, double goal_angle, double px, double py);
double calc_goal_angle(double goal_x, double goal_y, double cur_angle, double px, double py); 
void StageOdom_callback(nav_msgs::Odometry msg); 


void StageOdom_callback(nav_msgs::Odometry msg)
{
	ret = std::make_pair(0, 0); //initialize pair. Used to get return.

	//Converting from quaternion to radians
	cur_angle = acos(msg.pose.pose.orientation.w) * 2;
	if (msg.pose.pose.orientation.z > 0) {
		cur_angle = (2*M_PI)-acos(msg.pose.pose.orientation.w) * 2;
	}

	//Rounding to 3 decimal places
	cur_angle = ((int)(cur_angle * 1000 + .5) / 1000.0);
	
	//Update the current position
	px = msg.pose.pose.position.x + checkpoints[0][0];
	py = msg.pose.pose.position.y + checkpoints[0][1];
	
	//When goal reached
	if ((px <= goal_x + 0.5) && (px >= goal_x - 0.5) && (py <= goal_y + 0.5) && (py >= goal_y - 0.5)) {
	isSet = false;
		if (cc == 4) { //If at last checkpoint
			linear_x = 0;
		} else {
			cc++; //Increment checkpoint index
		}
		goal_x = checkpoints[cc][0];
		goal_y = checkpoints[cc][1];
	
		//Account for delay by subtracting delay values from current pose and orientation
		goal_angle = calc_goal_angle(goal_x, goal_y, cur_angle - M_PI/20, px - 0.1, py - 0.1);

	} else { //Do this until goal is reached
		ret = move(goal_x, goal_y, cur_angle, goal_angle, px, py);	
		linear_x = ret.first;
		angular_z = ret.second;
	}
}


//Keeps robot moving by changing linear_x and angular_z
std::pair<double, double> move(double goal_x, double goal_y, double cur_angle, double goal_angle, double px, double py) 
{		
	
	std::pair<double,double>_ret = std::make_pair(0, 0); //initialize pair. Used to get return.
	double moveSpeed = M_PI/2;
	moveSpeed = ((int)(moveSpeed * 1000 + .5) / 1000.0);

	//When the robot is facing the correct direction, start moving
	double threshold = cur_angle-moveSpeed/10;
	threshold = ((int)(threshold * 1000 + .5) / 1000.0);

	if ((goal_angle  == threshold) || isSet) {
		_ret.first = 5; //linear_x
		_ret.second = 0; //angular_z
		isSet = true;
	} else if ((goal_angle <= cur_angle + 0.3) && (goal_angle >= cur_angle - 0.3) )  {
		_ret.first = 0; //linear_x
		_ret.second = fabs(goal_angle - cur_angle); //angular_z
		if (goal_angle == cur_angle) {
			isSet = true;		
		}

	} else {
		_ret.first = 0; //linear_x
		_ret.second = moveSpeed; //angular_z
		isSet = false;
	}


	if ((px-0.1 <= goal_x + 0.5) && (px-0.1 >= goal_x - 0.5) && (py-0.1 <= goal_y + 0.5) && (py-0.1 >= goal_y - 0.5)) {	
			_ret.first = 0; 
			isSet = false;
	}

	return _ret; 
}

double calc_goal_angle(double goal_x, double goal_y, double cur_angle, double px, double py) 
{

	//Initial and goal vectors used to calculate goal theta
	double init_vector_x;
	double init_vector_y;
	double goal_vector_x;
	double goal_vector_y;
	double goal_angle;
	
	//Finding the vector that the robot is facing and the goal vector
	init_vector_x = cos(cur_angle);
	init_vector_y = sin(cur_angle);
	goal_vector_x = goal_x - px;
	goal_vector_y = goal_y - py;
	
	goal_angle = atan2(goal_vector_y, goal_vector_x); //pi <= goal_angle < -pi
	if (goal_angle < 0) {
		goal_angle = 2 * M_PI + goal_angle; //Remove sign, then add to pi
	} else if (goal_angle == 2 * M_PI) { //New goal angle =>   >0 to 6.283
		goal_angle = 0;
	}
	goal_angle = (2* M_PI) - goal_angle;

	//rounding goal_angle to three decimal places
	goal_angle = ((int)(goal_angle * 1000 + .5) / 1000.0);
	
	return goal_angle;
}

/* 
	Model for all periodic events - start time, stop time, period. The issue with this at the moment is that
	any behaviour defined here will occur at the end of the duration provided when creating the timer. This
	could easily solved if this method had access to the value of that duration - I'll look into solving this
	later. It's more of a usabiity issue than a usefulness one, as you can  adjust the startTime and endTime
	but I'd prefer it to be more intuitive.
	NOT CURRENTLY SUPPORTED: Events on certain days (other than every x number of days), non-periodic events 
							 during the defined period (startTime - endTime)
*/
void testCallback1(const ros::TimerEvent&) {
	int startTime = time_conversion::simHoursToRealSecs(6); // Start callback at 6am
	int endTime = time_conversion::simHoursToRealSecs(12); // Stop callback at 12pm

	int tnow = ros::Time::now().toSec(); // The simulation time now
	int dlen = time_conversion::getDayLength(); // The length of a simulation day, in seconds

	// Behaviour should only occur if the simulation time is between the specified start and end times.
	if (((tnow % dlen) > startTime) && ((tnow % dlen) < endTime)) { // Note that this will run at the end of the duration specified for the timer.
		ROS_INFO("providing medication: "); 
		std::ostringstream s;
		s << tnow; 
		ROS_INFO(s.str().c_str()); // Just shows elapsed seconds
		ros::Duration(5).sleep();
	}
}

void testCallback2(const ros::TimerEvent&) {
	int startTime = time_conversion::simHoursToRealSecs(0); // Start callback at 6am
	int endTime = time_conversion::simHoursToRealSecs(24); // Stop callback at 12pm

	int tnow = ros::Time::now().toSec(); // The simulation time now
	int dlen = time_conversion::getDayLength(); // The length of a simulation day, in seconds

	if (((tnow % dlen) > startTime) && ((tnow % dlen) < endTime)) {
		ROS_INFO("visiting resident: ");
		std::ostringstream s;
		s << tnow; 
		ROS_INFO(s.str().c_str());
	}
}

int main(int argc, char **argv)
{
 //initialize robot parameters

	//Initial pose. This is the same as the pose used in the world file.
	px = checkpoints[cc-1][0];
	py = checkpoints[cc-1][1];
	cur_angle = 0;

	//Set goal pose 
	goal_x = checkpoints[cc][0];
	goal_y = checkpoints[cc][1];

	goal_angle = calc_goal_angle(goal_x, goal_y, cur_angle, px, py);

	//Initial velocities
	linear_x = 0;
	angular_z = 0;

	//You must call ros::init() first of all. ros::init() function needs to see argc and argv. The third argument is the name of the node
	ros::init(argc, argv, "RobotNode0");

	//NodeHandle is the main access point to communicate with ros.
	ros::NodeHandle n;

	//advertise() function will tell ROS that you want to publish on a given topic_
	//to stage
	ros::Publisher RobotNode_stage_pub = n.advertise<geometry_msgs::Twist>("robot_0/cmd_vel",1000); 

	//subscribe to listen to messages coming from stage
	ros::Subscriber StageOdo_sub = n.subscribe<nav_msgs::Odometry>("robot_0/odom",1000, StageOdom_callback);

	//ros::Subscriber StageLaser_sub = n.subscribe<sensor_msgs::LaserScan>("robot_0/base_scan",1000,StageLaser_callback);

	ros::Rate loop_rate(1000);

	//a count of howmany messages we have sent
	int count = 0;

	////messages
	//velocity of this RobotNode
	geometry_msgs::Twist RobotNode_cmdvel;

	// Timers for event scheduling. Timers that run callback at the same time will have those callbacks queued.
	int dur1 = time_conversion::simHoursToRealSecs(4);
	ros::Timer visitTimer = n.createTimer(ros::Duration(dur1), testCallback2);
	int dur2 = time_conversion::simHoursToRealSecs(2); // Perform callback every 2 simulation hours
	ros::Timer medicationTimer = n.createTimer(ros::Duration(dur2), testCallback1); 

	while (ros::ok())
	{
		//messages to stage
		RobotNode_cmdvel.linear.x = linear_x;
		RobotNode_cmdvel.angular.z = angular_z;

		//publish the message
		RobotNode_stage_pub.publish(RobotNode_cmdvel);
		
		ros::spinOnce();

		loop_rate.sleep();
		++count;
	}

return 0;

}
