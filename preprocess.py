import re
import numpy as np
import sys

def shortest_dist(filename):
	distance = open(filename,"r")
	sd = []
	for line in distance:
		lineSplit=line.split(" ")
		sd.append(lineSplit[0])

	return sd 

def number(filename):
	num = open(filename,"r")
	number = []
	for line in num:
		lineSplit=line.split(" ")
		number.append(lineSplit[0])
	return number

def obstacles(filename):
	obs = open(filename,"r")
	x_centre = []
	y_centre = []
	area = []
	for line in obs:
		lineSplit=line.split(" ")
		x_centre.append(lineSplit[0])
		y_centre.append(lineSplit[1])
		area.append(lineSplit[2])
	return x_centre,y_centre,area
	

preprocess.py
Displaying training.py.