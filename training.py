#import pandas as pd
import numpy as np
from sklearn.preprocessing import scale
from sklearn.cross_validation import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.svm import SVC
import preprocess

pot_func = preprocess.number("data/potential.txt")
#shortest_distance = preprocess.shortest_dist("data/dist.txt")
x,y,area = preprocess.obstacles("data/features.txt")

training_set_size = 10000

X_all = np.zeros(shape = (3, training_set_size))
Y_all = np.zeroes(shape = (1,training_set_size))

image = 0

for i in range(training_set_size):
	# if(i>number[image]):
	# 	shortest_dist = shortest_dist[image]
	X_all[0][i] = x[i]
	X_all[1][i] = y[i]
	X_all[2][i] = area[i]
	Y_all[0][i] = pot_func[i]

forest_clf = RandomForestClassifier(n_estimators = 100, max_depth = 20, random_state = 0)
forest_clf.fit(x_train, y_train)
