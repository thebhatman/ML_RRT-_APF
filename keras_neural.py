import cv2
import numpy as np
import tensorflow as tf
from keras.models import Sequential
from keras.layers import Dense
from keras.wrappers.scikit_learn import KerasRegressor
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import KFold
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
import sys
import math
import subprocess
import preprocess
import os
import keras.backend as K
# import theano.tensor as T
# from theano import function

first_c = 0

img = cv2.imread('dataset/img0.jpg')
ideal_path = open('data/finaloutput.txt')

image_num = 0

def ispath(x,y):
	if img[x,y]<200 and img[x,y]>100:
		return True
	else:
		return False

def custom_loss(y_true,y_pred):
	#make graph and array
	#print("apf.cpp "+str(A_L) +" "+str(num))
	global image_num,first_c
	print((y_true.shape))
	file_arg = str(y_pred)+" "+str(image_num)
	
	print(file_arg)
	array = []
	if(first_c):
		print("Second!!!!")
		image_num+=1
		os.system("./apf "+ file_arg)
		text = open('apf_out.txt',"r")
		
		for line in text:
			lineSplit=line.split(" ")
			count = 1
			# print(len(lineSplit))
			for word in lineSplit:
				count+=1
				if(count<=len(lineSplit)):
					# print(word)
					array.append(int(word))
		return lossfunction(array,image_num-1)
	first_c = 1
	print("FIRSTTTT")
	for i in range(100):
		array.append(99)
	return lossfunction(array,1)
	

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
	#Y = np.zeros(shape = (1, 1))
	#print(lineSplit)
	error = []
	real = []
	for word in lineSplit:
		count+=1
		if word == '(' or word == ',' or word == ')' or word == ' ' or word == '':
			continue
		else:
			if(count<=len(lineSplit)) and i < len(epoch_array):
				# ideal = epoch_array[i]
				i+=1
				real.append(int(word))
				#print(i)
				#print(word)
				# error.append((int(ideal) - int(word))*(int(ideal) - int(word)))
	# print("error = "+str(error))
	# tens_arr = tf.convert_to_tensor(error,dtype=tf.float32)
	# Y = tf.reduce_mean(tens_arr)
	Y = tf.losses.mean_squared_error(epoch_array,real)
	print(Y)
	return Y 

number_of_features = 57
features = preprocess.all_circles("data/obst.txt")
training_set_size = 50000
X_all = np.zeros(shape = (training_set_size, number_of_features))
Y = np.zeros(shape = (training_set_size, 1))
#Y_all = np.zeros(shape = (train_size,1))

for i in range(training_set_size):
	for j in range(number_of_features):
		#print("fa",features[i][j])
		X_all[i][j] = float(features[i][j])

model = Sequential()
model.add(Dense(18, input_dim=57, kernel_initializer='normal', activation='relu'))
# model.add(Dense(18, kernel_initializer='normal', activation='relu'))
model.add(Dense(9, kernel_initializer='normal', activation='relu'))
model.add(Dense(1, kernel_initializer='normal'))
model.compile(loss='binary_crossentropy', optimizer='adam')
print("going to fit !!!")
model.fit(X_all, Y, epochs=150, batch_size=1,  verbose=2)
predictions = model.predict(X)