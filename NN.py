import numpy as np 
import math
import keras
import preprocess

size = 100000

pot_func = preprocess.potential("data/smoke_pot_val.txt",size)
features = preprocess.feautures_ssl("data/ssl_field_features.txt",size)

no_feat = 8
no_grids = 12

train_size = int(size*0.9)
test_size = int(size*0.1)

X_train = np.zeros(shape = (train_size, no_feat))
Y_train = np.zeros(shape = (train_size,no_grids))

X_test = np.zeros(shape = (test_size, no_feat))
Y_test = np.zeros(shape = (test_size,no_grids))

for i in range(train_size):
	for j in range(no_feat):
		#print("fa",features[i][j])
		X_train[i][j] = float(features[i][j])
	for j in range(no_grids):
		#print("fa",features[i][j])
		Y_train[i][j] = float(pot_func[i][j])
	#Y_all[i][0] = float(pot_func[i][:-1])

for i in range(test_size):
	# if(i>number[image]):
	# 	shortest_dist = shortest_dist[image]
	for j in range(no_feat):
		X_test[i][j] = float(features[i+train_size][j])
	for j in range(no_grids):
		Y_test[i][j] = float(pot_func[i+train_size][j])
	#Y_test[i][0] = float(pot_func[train_size+i][:-1])

#neural net
model = Sequential()
model.add(Dense(10, input_dim=8, activation='relu'))
model.add(Dense(10, activation='relu'))
model.add(Dense(12, activation='sigmoid'))
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

model.fit(X, Y, epochs=150, batch_size=10)
scores = model.evaluate(X, Y)
print("\n%s: %.2f%%" % (model.metrics_names[1], scores[1]*100))