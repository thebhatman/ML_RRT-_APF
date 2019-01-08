import cv2
import matplotlib.pyplot as plt 
import numpy as np 
import sklearn 
import sklearn.datasets 
import sklearn.linear_model 
import matplotlib 
import sys
import math

img = cv2.imread('images/img0.jpg')
ideal_path = open('data/output.txt')

def ispath(x,y):
	if img[x,y]<200 and img[x,y]>100:
		return True
	else
		return False

def lossfunction(epoch_array):
	step_down = 100
	error = 0
	#for each point in ideal and each epoch:
		error += (idealx - epochx)*(idealx - epochx) + (idealy - epochy)*(idealy - epochy)
	return error


