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
	global image_num
	print((y_true.shape))
	file_arg = str(y_pred)+" "+str(image_num)
	image_num+=1
	os.system(("./apf "+ file_arg))
	text = open('apf_out.txt',"r")
	array = []
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
	Y = tf.constant(error,dtype=tf.float32)
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
model.add(Dense(57, input_dim=13, kernel_initializer='normal', activation='relu'))
model.add(Dense(18, kernel_initializer='normal', activation='relu'))
model.add(Dense(9, kernel_initializer='normal', activation='relu'))
model.add(Dense(1, kernel_initializer='normal'))
model.compile(loss=custom_loss, optimizer='adam')
model.fit(X_all, Y, epochs=150, batch_size=10,  verbose=2)
predictions = model.predict(X)