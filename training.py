#import pandas as pd
import numpy as np
from sklearn.preprocessing import scale
from sklearn.cross_validation import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.svm import SVC
import preprocess

number = preprocess.number("number.txt")
shortest_distance = preprocess.shortest_dist("dist.txt")
x,y,area = preprocess.obstacles("obst.txt")

for i in range(len(number))-1:
	number[i+1]=number[i]+number[i+1]

training_set_size = number[len(number)-1]

X_all = np.zeros(shape = (3, training_set_size))
Y_all = np.zeroes(shape = (1,training_set_size))

image = 0

for i in range(training_set_size):
	if(i>number[image]):
		shortest_dist = shortest_dist[image]
	X_all[0][i] = x[i]
	X_all[1][i] = y[i]
	X_all[2][i] = area[i]
