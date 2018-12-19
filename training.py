#import pandas as pd
import numpy as np
from sklearn.preprocessing import scale
from sklearn.metrics import mean_squared_error
#from sklearn.cross_validation import train_test_split
from sklearn.ensemble import RandomForestRegressor
from sklearn.svm import SVC
import preprocess
import math

pot_func = preprocess.potential("data/smoke_pot.txt")
#shortest_distance = preprocess.shortest_dist("data/dist.txt")
features = preprocess.all_circles("data/obst.txt")



training_set_size = 100000
"""=
X_all = np.zeros(shape = (25, int(training_set_size*0.9)))
Y_all = np.zeros(shape = (1,int(training_set_size*0.9)))

X_test = np.zeros(shape = (25, int(training_set_size*0.1)))
Y_test = np.zeros(shape = (1,int(training_set_size*0.1)))
"""
X_all = np.zeros(shape = (int(training_set_size*0.9), 25))
Y_all = np.zeros(shape = (int(training_set_size*0.9),1))

X_test = np.zeros(shape = (int(training_set_size*0.1), 25))
Y_test = np.zeros(shape = (int(training_set_size*0.1),1))
image = 0
train_size = int(training_set_size*0.9)
for i in range(train_size):
	for j in range(25):
		#print("fa",features[i][j])
		X_all[i][j] = float(features[i][j])
	Y_all[i][0] = float(pot_func[i][:-1])

for i in range(training_set_size-train_size):
	# if(i>number[image]):
	# 	shortest_dist = shortest_dist[image]
	for j in range(25):
		X_test[i][j] = float(features[i+train_size][j])
	Y_test[i][0] = float(pot_func[train_size+i][:-1])
#X_train = np.transpose(X_all)
#print(X_train)
#Y_train = np.transpose(Y_all)

forest_clf = RandomForestRegressor(n_estimators = 100, max_depth = 5, random_state = 0)
forest_clf.fit(X_all, Y_all.ravel())
Y_pred = forest_clf.predict(X_test)
print(Y_pred)
#rms_error=mean_squared_error(Y_test.ravel(),Y_pred)
manual_rms_error=0.0
for i in range(training_set_size-train_size):
	manual_rms_error+=(Y_test[i][0]-Y_pred[i])**2
manual_rms_error=manual_rms_error/10000
manual_rms_error=math.sqrt(manual_rms_error)
print ("RMS error of model",manual_rms_error)
np.savetxt('RFC_output.txt', Y_pred)
f = open("RFC_output.txt", "a")
f.write("\n \n RMS error of model:")
f.write("%f" % (manual_rms_error))
f.close()

	