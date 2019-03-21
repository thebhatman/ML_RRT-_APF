import re
import numpy as np
import sys

DATA = 50000
F_no = 8
G_no = 12

def feautures_ssl(filename,size):
	array = np.zeros(shape = (size,F_no))
	obs = open(filename,"r")
	i = 0
	for line in obs:
		lineSplit = line.split(" ")
		#lineSplit = lineSplit[:-1]
		for cor in range(F_no):
			array[i][cor] = (lineSplit[(cor)])
		i+=1
	return array

def potential(filename,size):
	pot = open(filename,"r")
	array = np.zeros(shape = (size,G_no))
	i = 0
	for line in pot:
		lineSplit=line.split(" ")
		for k in range(G_no):
			array[i][k]=(lineSplit[k])
		i+=1

	return array

def func_num(filename):
	num = open(filename,"r")
	number = []
	i = 0
	for line in num:
		#lineSplit=line.split(" ")
		#print(str(len(line)) + " "+str(i))
		i+=1
		number.append(int(line[0]+line[1]))
		if(i>49999):
			break
		#print(int(line[0]+line[1]))
		
	return number

def features(filename):
	obs = open(filename,"r")
	# x_centre = []
	# y_centre = []
	area = []
	for line in obs:
		lineSplit=line.split(" ")
		# x_centre.append(lineSplit[0])
		# y_centre.append(lineSplit[1])
		area.append(lineSplit[2])
	return area
	

		

#all_circles("data/obst.txt")