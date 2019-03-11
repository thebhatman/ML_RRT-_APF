import sys
import math
import subprocess
import os
# import preprocess
	
ideal_path = open('data/finaloutput.txt')
file_arg = str(300000)+" "+str(1)
os.system(("./apf "+file_arg))
text = open('apf_out.txt',"r")
array = []

def lossfunction(epoch_array,num):
	step_down = 10000
	error = 0
	count = 0
	for line in ideal_path:
		if(count==num):
			lineSplit = line.split(" ")
			break
		count+=1
	i = 0
	ideal = 0
	count = 1
	#print(lineSplit)
	for word in lineSplit:
		count+=1
		if word == '(' or word == ',' or word == ')' or word == ' ' or word == '':
			continue
		else:
			if(count<=len(lineSplit)) and i < len(epoch_array):
				ideal = epoch_array[i]
				i+=1
				#print(i)
				#print(word)
				error += (int(ideal) - int(word))*(int(ideal) - int(word))

	return error/step_down 

for line in text:
	lineSplit=line.split(" ")
	count = 1
	# print(len(lineSplit))
	for word in lineSplit:
		count+=1
		if(count<=len(lineSplit)):
			# print(word)
			array.append(int(word))
print(lossfunction(array,1))

