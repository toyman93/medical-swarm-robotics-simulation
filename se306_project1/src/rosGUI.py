#!/usr/bin/env python
# Using Tkinter to create a GUI

import os
from multiprocessing import Process, Pipe
import roslib
roslib.load_manifest('se306_project1')
import rospy
from std_msgs.msg import String
#from se306_project1 import ResidentMsg
import Tkinter	

callbackData = 'default'
STDIN = 0
STDOUT = 1
STDERR = 2

subscriber = None
perc = 1
# We inherit from Tkinter.Tk, which is the base class for standard windows.
class simpleapp_tk(Tkinter.Tk):
    # Constructor
	def __init__(self,parent):
		Tkinter.Tk.__init__(self,parent)
		# Keep a reference to our parent.
		self.parent = parent
		#self.setupNode()
		self.initialize()

	def initialize(self):
		self.grid()
        
		# Image of Resident
		img = Tkinter.Button(self, text="Photo goes here", relief="raised", bitmap="error", height=80, width=80)
		img.grid(column=0, row=0)
        
		# Name of resident
		name = Tkinter.Label(self, text="Sean Kim", font=(20)) #Declare size of font and text
		name.grid(column=0, row=1)
        
		#====Start of statusFrame====#
        
		# Frame containing status of resident (health, hunger)
		statusFrame = Tkinter.Frame(self, bd=2, relief="groove")
		statusFrame.grid(column=0, row=2, padx=5)
        
		# Status title
		statusText = Tkinter.Label(statusFrame, text="Status", font=(18), anchor="center")
		statusText.grid(column=0, row=0, sticky="W")
        
		# Health
		healthText = Tkinter.Label(statusFrame, text="Health : ")
		healthText.grid(column=0, row=1, sticky="W")
        
		# Hunger
		hungerText = Tkinter.Label(statusFrame, text="Hunger : ")
		hungerText.grid(column=0, row=2, sticky="W")
        
		# Health Bar (Parent)
		self.healthBar = Tkinter.Canvas(statusFrame, bg="blue", height=16, width=200, highlightbackground="black")
		self.healthBar.grid(column=1, row=1, sticky="E", padx=3)
		# Health Bar (Filler)
		self.healthFiller = Tkinter.Canvas(statusFrame, bg="red", height=16, width=0, highlightbackground="black")
		self.healthFiller.grid(column=1, row=1, sticky="E", padx=3)
		# Hunger Bar (Parent)
		self.hungerBar = Tkinter.Canvas(statusFrame, bg="blue", height=16, width=200, highlightbackground="black")
		self.hungerBar.grid(column=1, row=2, sticky="E", padx=3)
		# Hunger Bar (Filler)
		self.hungerFiller = Tkinter.Canvas(statusFrame, bg="red", height=16, width=0, highlightbackground="black")
		self.hungerFiller.grid(column=1, row=2, sticky="E", padx=3)
        
        # Position
		posText = Tkinter.Label(statusFrame, text="Position : ")
		posText.grid(column=0, row=3, sticky="W")
        
        # Position values e.g.(x,y)
		posVal = Tkinter.Label(statusFrame, text="(x,y)") #Dummy at the moment
		posVal.grid(column=1, row=3, sticky="E", padx=2)
        
        # State of Resident (Text)
		stateText = Tkinter.Label(statusFrame, text="State : ")
		stateText.grid(column=0, row=4, sticky="W")

		# State of Resident (Subscribed state)
		stateResident = Tkinter.Label(statusFrame, text="SICK", fg="red")
		stateResident.grid(column=1, row=4, sticky="E", padx=2)

		#====End of statusFrame====#
		
		#====Start of Time Frame====#
        
		timeFrame = Tkinter.Frame(self, bd=2, relief="groove")
		timeFrame.grid(column=0, row=3, padx=5, pady=5)
		
		timeText = Tkinter.Label(timeFrame, text="Time", font=(18), anchor="center")
		timeText.grid(column=0, row=0, sticky="W")
        
		currentTimeText = Tkinter.Label(timeFrame, text="Current Time : ")
		currentTimeText.grid(column=0, row=1)
        
		currentTime = Tkinter.Label(timeFrame, text="11:08PM, 27 August 2014")
		currentTime.grid(column=1, row=1, sticky="E", padx=5)

	def setupNode(self):
		print('boji listener')
		rospy.init_node('listener', anonymous=True)
		if subscriber is None:
			rospy.Subscriber("python/gui", String, self.residentStatusCallback)
	
	def residentStatusCallback(self, GUImsg):
		global callbackData
		callbackData = GUImsg.data
		print('in boji callback')
		rospy.loginfo(rospy.get_caller_id()+"I heard %s", GUImsg.data)
		#TODO self.entry.delete(0, Tkinter.END)
		#TODO self.entry.insert(0, GUImsg.data)

	def decreaseHealth(self, value):
		self.healthFiller.config(width=200*(1-value))
		self.hungerFiller.config(width=200*(1-value))

def task():
	global perc
	perc -= 0.05
	if perc > -0.05: # There is no 0 in python...
		app.decreaseHealth(perc)
	app.after(1000,task)  # reschedule event in 1 seconds
	

if __name__ == "__main__":
    # Instantiate class
	app = simpleapp_tk(None)
    # Give a title to window
	app.title('my application')
	app.after(1000,task)
	app.mainloop()



