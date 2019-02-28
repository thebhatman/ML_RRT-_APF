import sys
import math
import subprocess
# import preprocess
ideal_path = open('data/finaloutput.txt')
file_arg = "apf.cpp "+str(300000)+" "+str(1)
subprocess.call(["g++",file_arg])
subprocess.call("./a.out")
text = open('apf_out.txt',"r")
array = []
for line in text:
	lineSplit=line.split(" ")
	for word in lineSplit:
		array.append(int(word))
print(lossfunction(array,1))

def lossfunction(epoch_array,num):
	step_down = 100
	error = 0
	lineSplit=ideal_path[num].split(" ")
	i = 0
	ideal = 0
	for word in lineSplit:
		if word == '(' or word == ',' or word == ')':
			continue
		else:
			ideal = epoch_array[i]
			i+=1
			error += (int(ideal) - int(word))*(int(ideal) - int(epochx)) 
			
	return error/step_down