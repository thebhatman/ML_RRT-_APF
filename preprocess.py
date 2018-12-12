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

def func_num(filename):
	num = open(filename,"r")
	number = []
	for line in num:
		lineSplit=line.split(" ")
		number.append(int(lineSplit[0]))
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
	
def all_circles(filename):
	number = func_num("data/number.txt")
	num  = 0
	obs = open(filename,"r")
	k = 0
	array = np.zeros(shape = (100000, 25))
	i =  0
	line = 0
	j = 0
	for i in range(1,len(number)):
		number[i] = number[i] + number[i-1]
	#for i in range(100000):
	
	i = 0
	for line in obs:
		#for j in range(24):
		if (k >= int(number[num])):
			#print("j=",j)
			while(j < 24):
				array[num][j] = 99
				j += 1
			num += 1
			j = 0
			#continue
	
		lineSplit = line.split(" ")
		array[num][j] = lineSplit[0]
		array[num][j+1] = lineSplit[1]
		j += 2
		k += 1
		if(num == 99999):
			while(j < 24):
				array[num][j] = 99
				j += 1
			break
		#print(k)
		#continue
	area = features("data/feature.txt")
	for i in range(100000):
		array[i][24] = area[i]
	return array
		

#all_circles("data/obst.txt")