import re
import numpy as np
import sys

def potential(filename):
	distance = open(filename,"r")
	pot = []
	for line in distance:
		lineSplit=line.split(" ")
		pot.append(lineSplit[0])

	return pot

# def number(filename):
# 	num = open(filename,"r")
# 	number = []
# 	for line in num:
# 		lineSplit=line.split(" ")
# 		number.append(lineSplit[0])
# 	return number

def features(filename):
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
	
