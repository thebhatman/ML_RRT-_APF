import numpy as np
from keras.models import Sequential
from keras.layers import Dense
from keras.wrappers.scikit_learn import KerasRegressor
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import KFold
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
# import theano.tensor as T
# from theano import function

img = cv2.imread('images/img0.jpg')
ideal_path = open('data/finaloutput.txt')

def ispath(x,y):
	if img[x,y]<200 and img[x,y]>100:
		return True
	else:
		return False

def custom_loss(A_L,num):
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

number_of_features = 57
features = preprocess.all_circles("data/obst.txt")
training_set_size = 50000
X_all = np.zeros(shape = (training_set_size, number_of_features))
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
model.compile(loss=lossfunction, optimizer='adam')
model.fit(X, Y, epochs=150, batch_size=10,  verbose=2)
predictions = model.predict(X)