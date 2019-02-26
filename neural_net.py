import cv2
import matplotlib.pyplot as plt 
import numpy as np 
import pandas as pd
import sklearn 
import sklearn.datasets 
import sklearn.linear_model 
import matplotlib 
import sys
import math
import subprocess
import preprocess

img = cv2.imread('images/img0.jpg')
ideal_path = open('data/finaloutput.txt')

def ispath(x,y):
	if img[x,y]<200 and img[x,y]>100:
		return True
	else:
		return False

def loss(A_L,num):
	#make graph and array
	#print("apf.cpp "+str(A_L) +" "+str(num))
	file_arg = "apf.cpp "+str(A_L)+" "+str(num)
	subprocess.call(["g++",file_arg])
	subprocess.call("./a.out")
	text = open('file.txt',"r")
	array = []
	for line in text:
		lineSplit=line.split(" ")
		for word in lineSplit:
			array.append(int(word))
	return lossfunction(array,num)

def lossfunction(epoch_array,num):
	step_down = 100
	error = 0
	lineSplit=ideal_path[num].split(" ")
	i = 0
	ideal = 0
	for word in line:
		if word == '(' or word == ',' or word == ')':
			continue
		else:
			ideal = epoch_array[i]
			i+=1
			error += (int(ideal) - int(word))*(int(ideal) - int(epochx)) 
			
	return error/step_down

def sigmoid(Z):
	return 1/(1 + np.exp(-Z))

def relu(Z):
	A = np.maximum(0,Z)
	cache = Z 
	return A, cache

def relu_backward(dA, cache):
	Z = cache
	dZ = np.array(dA, copy=True)  
	dZ[Z <= 0] = 0
	return dZ

def initialize_parameters_deep(layer_dims):
	parameters = {}
	L = len(layer_dims)            
	for l in range(1, L):
		parameters['W' + str(l)] = np.random.randn(layer_dims[l], layer_dims[l-1]) / np.sqrt(layer_dims[l-1]) 
		parameters['b' + str(l)] = np.zeros((layer_dims[l], 1))
		
		assert(parameters['W' + str(l)].shape == (layer_dims[l], layer_dims[l-1]))
		assert(parameters['b' + str(l)].shape == (layer_dims[l], 1))
	return parameters

def linear_activation_forward(A_prev, W, b, activation):
	if activation == "Output":
		Z = W.dot(A_prev) + b
		linear_cache = (A_prev, W, b)
		A, activation_cache = Z,Z
	
	elif activation == "relu":
		Z = W.dot(A_prev) + b
		linear_cache = (A_prev, W, b)
		A, activation_cache = relu(Z)
	
	cache = (linear_cache, activation_cache)
	return A, cache

def forward_prop(X,weights):
	caches = []
	A = X
	L = len(weights)//2
	#print(L, "LLLL")                
	for l in range(1, L):
		A_prev = A 
		A, cache = linear_activation_forward(A_prev, weights['W' + str(l)], weights['b' + str(l)], activation = "relu")
		caches.append(cache)
		#print("hiiiiiiii")
		#print((np.transpose(A))[0].shape)
		#print("hiiiiiiii")
 
	A_L, cache = linear_activation_forward(A, weights['W' + str(L)], weights['b' + str(L)], activation = "Output")
	caches.append(cache)
	
	return A_L, caches


def linear_activation_backward(dA, cache, activation):
	linear_cache, activation_cache = cache
	
	if activation == "relu":
		dZ = relu_backward(dA, activation_cache)
		A_prev, W, b = linear_cache
		m = A_prev.shape[1] 
		dW = 1./m * np.dot(dZ,A_prev.T)
		db = 1./m * np.sum(dZ, axis = 1, keepdims = True)
		dA_prev = np.dot(W.T,dZ)
		
	elif activation == "Output":
		dZ = dA
		A_prev, W, b = linear_cache
		m = A_prev.shape[1] 
		dW = 1./m * np.dot(dZ,A_prev.T)
		db = 1./m * np.sum(dZ, axis = 1, keepdims = True)
		dA_prev = np.dot(W.T,dZ)
	
	return dA_prev, dW, db

def backward_prop(A_L,caches,i):
	grads = {}
	L = len(caches) 
	m = A_L.shape[1]
	print((np.transpose(A_L))[0][0])    
	dA_L = loss(np.transpose(A_L)[0][0],i)
	current_cache = caches[L-1]
	grads["dA" + str(L)], grads["dW" + str(L)], grads["db" + str(L)] = linear_activation_backward(dA_L, current_cache, activation = "Output")
	
	for l in reversed(range(L-1)):
		current_cache = caches[l]
		dA_prev_temp, dW_temp, db_temp = linear_activation_backward(grads["dA" + str(l + 2)], current_cache, activation = "relu")
		grads["dA" + str(l + 1)] = dA_prev_temp
		grads["dW" + str(l + 1)] = dW_temp
		grads["db" + str(l + 1)] = db_temp

	return grads

def update_parameters(parameters, grads, learning_rate):
	L = len(parameters) // 2 # number of layers in the neural network

	# Update rule for each parameter 
	for l in range(L):
		parameters["W" + str(l+1)] = parameters["W" + str(l+1)] - learning_rate * grads["dW" + str(l+1)]
		parameters["b" + str(l+1)] = parameters["b" + str(l+1)] - learning_rate * grads["db" + str(l+1)]
		
	return parameters

def final_model(X, layers_dims, learning_rate, iterations, print_cost=False):
	costs = [] 
	weights = initialize_parameters_deep(layers_dims)
	
	for i in range(0, iterations):
		A_L, caches = forward_prop(X, weights)
		#print(A_L)
		grads = backward_prop(A_L, caches,i)
		parameters = update_parameters(weights, grads, learning_rate)

number_of_features = 57
features = preprocess.all_circles("data/obst.txt")
training_set_size = 50000
X_all = np.zeros(shape = (training_set_size, number_of_features))
#Y_all = np.zeros(shape = (train_size,1))

for i in range(training_set_size):
	for j in range(number_of_features):
		#print("fa",features[i][j])
		X_all[i][j] = float(features[i][j])

#X_all = np.transpose(X_all)

learning_rate = 0.01
iterations = 1
n_x = 57 
#print(n_x," nnxnxnnxnxn")  
n_y = 1
print(np.transpose(X_all).shape)
layer_dims = (n_x, 19,9,3, n_y)
final_model(np.transpose(X_all),layer_dims,learning_rate,iterations)

