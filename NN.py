import numpy as np 
import pandas as pd
import math
import keras
from sklearn.preprocessing import scale
from keras.layers import Activation, Dense
from keras.models import Sequential
from keras.optimizers import SGD
import preprocess

size = 100000

no_feat = 8
no_grids = 12

train_size = int(size*0.9)
test_size = int(size*0.1)

pot_func = preprocess.potential("data/smoke_pot_val.txt",size)
features = preprocess.feautures_ssl("data/ssl_field_features.txt",size)

pot_func = pd.DataFrame(pot_func)
features = pd.DataFrame(features)

# for i in range(no_grids):
# 	# print(i)
# 	pot_func.loc[0:size, i] = scale(pot_func.loc[0:size, i])

for i in range(no_feat):
	# print(i)
	features.loc[0:size, i] = (scale(features.loc[0:size, i]))*100000


X_train = np.zeros(shape = (train_size, no_feat))
Y_train = np.zeros(shape = (train_size,no_grids))

X_test = np.zeros(shape = (test_size, no_feat))
Y_test = np.zeros(shape = (test_size,no_grids))

for i in range(train_size):
	for j in range(no_feat):
		#print("fa",features[i][j])
		X_train[i][j] = float(features.loc[i,j])
	for j in range(no_grids):
		#print("fa",features[i][j])
		Y_train[i][j] = float(pot_func.loc[i,j])
	#Y_all[i][0] = float(pot_func[i][:-1])

for i in range(test_size):
	# if(i>number[image]):
	# 	shortest_dist = shortest_dist[image]
	for j in range(no_feat):
		X_test[i][j] = float(features.loc[i+train_size,j])
	for j in range(no_grids):
		Y_test[i][j] = float(pot_func.loc[i+train_size,j])
	#Y_test[i][0] = float(pot_func[train_size+i][:-1])

print(X_test[0])
print(Y_test[0])

#neural net
model = Sequential()
model.add(Dense(10, input_dim=8, activation='relu'))
model.add(Dense(10, activation='relu'))
model.add(Dense(12, activation='sigmoid'))
sgd = SGD(lr=0.001, momentum=0.9, decay=0.0, nesterov=False)
model.compile(loss='binary_crossentropy', optimizer=sgd, metrics=['accuracy'])

model.fit(X_train, Y_train, epochs=1, batch_size=128)
scores = model.evaluate(X_test, Y_test)
Y = model.predict(X_test)
# print("\n%s: %.2f%%" % (model.metrics_names[1], scores[1]*100))
print(Y)