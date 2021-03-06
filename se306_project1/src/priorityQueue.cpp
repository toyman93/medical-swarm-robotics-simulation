/*
 * This class implements a reverse order PQ so its 7,6,5 not 5,6,7
 * This is done to make use of the 0(1) methods that c++ vector provide
 */

#include "priorityQueue.h"
#include <vector>
#include <iostream>
#include "ros/ros.h"

struct statusObj { //
int priority;
residentStates state;
}; /*!< Associates priority with resident state. */

std::vector<statusObj> PQ; /*!< The priority queue takes the form of a vector of statuses (state + priority) */

priorityQueue::priorityQueue() {
	addToPQ(tired);

}

priorityQueue::~priorityQueue() {
	// TODO Auto-generated destructor stub
}

/**
*	@brief Returns true if the given state is in the priority queu.
*	@param currentState the state to find.
*	@returns true if the state is in the queue, false otherwise.
*/
bool priorityQueue::isStateInPQ(residentStates currentState) {


	for (unsigned i=0; i<PQ.size();i++) {
		if (PQ.at(i).state == currentState) {
			return true;
		}
		if (i > 8 ) {
			break;
		}
	}
	return false;

}

/**
*	@brief Returns the current state (highest priority) as a string.
*	@returns The top of the priority queue as a string.
*/
std::string priorityQueue::checkCurrentState() {
	if (PQ.empty()) {
		return stateConvertString(idle);
	} else {
		return stateConvertString(PQ.back().state);
	}
}

/**
*	@brief Removes a given state from the priority queue.
*	@param unwantedState The state to remove.
*/
void priorityQueue::removeState(residentStates unwantedState) {
	if (!isStateInPQ(unwantedState)) {
		return;
	}
	for (unsigned i=0; i<PQ.size();i++) {
		if (PQ.at(i).state == unwantedState) {
			ROS_INFO("removed");
			PQ.erase(PQ.begin() + i);
			return;
		}
	}

}

/**
*	@brief Adds a given state to the queue.
*	@param currentState The state to be added.
*/
void priorityQueue::addToPQ(residentStates currentState) {
	if (isStateInPQ(currentState)) { // if state is in PQ, don't add again
		return;
	}

	if (currentState == emergency) {
		removeState(healthLow);
	}
	if (currentState == healthLow) {
		if(isStateInPQ(emergency)) {
			return;
		}
	}



	statusObj currResidentStatus;
	currResidentStatus.state = currentState;

	switch(currentState) // assigning the priority
	{
	case (emergency):
			currResidentStatus.priority = 1;
			break;
	case (healthLow):
			currResidentStatus.priority = 2;
			break;
	case (caregiver):
			currResidentStatus.priority = 3;
			break;
	case(medication):
			currResidentStatus.priority = 4;
			break;
	case (friends):
			currResidentStatus.priority = 5;
			break;
	case (hungry):
			currResidentStatus.priority = 6;
			break;
	case (tired):
			currResidentStatus.priority = 7;
			break;
	case (bored):
			currResidentStatus.priority = 8;
			break;
	case (idle):
			break;
	}

	if (currentState != idle) {

		if (PQ.empty()) { // if PQ is empty, add to the back.

			PQ.push_back(currResidentStatus);

		} else {

			bool statusInserted = false;

			for (unsigned i=0; i<PQ.size();i++) {

				if (currResidentStatus.priority > PQ.at(i).priority) {

					PQ.insert(PQ.begin() + i, currResidentStatus);
					statusInserted = true;
					break;

				}

				if (i > 8 ) {
					break;
				}

			}
			if (statusInserted == false) {
				PQ.push_back(currResidentStatus);
			}
		}

	}
}

/**
*	@brief Pops the highest priority off the priority queue.
*	@returns the highest priority resident status.
*/
residentStates priorityQueue::popFromPQ() {
	if (PQ.empty()) {
		return idle;

	} else {
		residentStates state = PQ.back().state;
		PQ.pop_back();
		return state;
	}
}

/**
*	@brief Converts a state to a string.
*	@param currentState The state to be converted
*	@returns The state as a string
*/
std::string priorityQueue::stateConvertString(residentStates currentState){
	switch(currentState) // assigning the priority
		{
		case (emergency):
				return "emergency";
				break;
		case (healthLow):
				return "healthLow";
				break;
		case (caregiver):
				return "caregiver";
				break;
		case(medication):
				return "medication";
				break;
		case (friends):
				return "friends";
				break;
		case (hungry):
				return "hungry";
				break;
		case (tired):
				return "tired";
				break;
		case (bored):
				return "bored";
				break;
		case (idle):
				return "idle";
				break;
		}
	return "Can't convert!!"; //
}

int somePQDummyTest(int a, int b) {
	return a;
}
