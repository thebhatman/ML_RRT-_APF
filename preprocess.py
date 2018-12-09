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

def obstacles(filename):
	obs = open(filename,"r")
	x_centre = []
	y_centre = []
	area = []
	
