# test.py>
 
# function
# def displayText():
#     print( "Geeks 4 Geeks !")

# functions


# import pyrebase
#import keras.backend.tensorflow_backend as tb
# tb._SYMBOLIC_SCOPE.value = True
#Import required libraries 
import keras #library for neural network
import pandas as pd #loading data in table form  
#import seaborn as sns #visualisation 
#import matplotlib.pyplot as plt #visualisation
import numpy as np # linear algebra
# data processing, CSV file I/O (e.g. pd.read_csv)
from sklearn.preprocessing import normalize #machine learning algorithm library

#Neural network module
from keras.models import Sequential 
from keras.layers import Dense,Activation,Dropout 
# from keras.layers.normalization import BatchNormalization 
from keras.utils import np_utils
from tensorflow.keras.models import model_from_json

import os


# importing the csv module 
import csv 



def loadmodel():
    #load in server
    json_file = open('forcemodel_shuffled_31__2021.json', 'r')
    loaded_forcemodel_new_json = json_file.read()
    json_file.close()
    loaded_forcemodel = model_from_json(loaded_forcemodel_new_json)
    # load weights into new model
    loaded_forcemodel.load_weights("forcemodel_shuffled_31__2021.h5")
    print("Loaded model from disk")
    
    # evaluate loaded model on test data
    loaded_forcemodel.compile(loss='mean_squared_error', optimizer='sgd')




def pred_force(position):


    position_max = 0.045
    position_min = 0
    force_min = 0 
    force_max = 15

    # position = position_x

    position_norm = (position-position_min)/(position_max-position_min)

    print("position_norm : ",position_norm)


    data = [[position_norm]] 
    # Create the pandas DataFrame  
    df = pd.DataFrame(data, columns = ['position_x'])    
    y=df.head(1)
    force_predict_nom = loaded_forcemodel.predict(y)


    # Predicted_Force_tp = (y_predict_tp*(df_train_reduced.Force_Response.max()-df_train_reduced.Force_Response.min()))+ df_train_reduced.Force_Response.min()
    Predicted_Force_tp = (force_predict_nom*(force_max-force_min))+ force_min
    print("Predicted_Force_tp : ", Predicted_Force_tp)

   
    return Predicted_Force_tp