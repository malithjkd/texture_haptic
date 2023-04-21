# server_py.py
# python3

import socket
# fuser -k 4455/tcp


# using time module
import time
import json
import pickle


from model_py import *
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
    if position < 0 :
        position = 0 
    print("position : ",position)
    position_norm = (position-position_min)/(position_max-position_min)

    print("position_norm : ",position_norm)


    data = [[position_norm]] 
    # Create the pandas DataFrame  
    df = pd.DataFrame(data, columns = ['position_x'])    
    y=df.head(1)
    force_predict_nom = loaded_forcemodel.predict(y)


    # Predicted_Force_tp = (y_predict_tp*(df_train_reduced.Force_Response.max()-df_train_reduced.Force_Response.min()))+ df_train_reduced.Force_Response.min()
    Predicted_Force_tp = (force_predict_nom*(force_max-force_min))+ force_min
    pred_force= Predicted_Force_tp[0][0].item()
    print("Predicted_Force_tp : ", pred_force)

    
    return pred_force

  
# IP = "127.0.0.1"
# PORT = 4455
# ADDR = (IP, PORT)
SIZE = 1024
FORMAT = "utf-8"

IP = "127.0.0.1"  # Standard loopback interface address (localhost)
PORT = 4455  # Port to listen on (non-privileged ports are > 1023)

# Create server socket.
serv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, proto=0)

# Bind server socket to loopback network interface.
serv_sock.bind((IP, PORT))

# Turn server socket into listening mode.
serv_sock.listen()
# serv_sock.listen(10)


# # calling functions
# loadmodel()




while True:
    # Accept new connections in an infinite loop.
    print("Server Start \n")
    client_sock, client_addr = serv_sock.accept()
    print('New connection from', client_addr)

    # Receive some data back.
    # dict = pickle.loads(tcp_recieve())
    data = client_sock.recv(SIZE).decode(FORMAT)
    print(f"[SERVER] {data}")
    # received = client_sock.recv(1024)
    # received = received.decode("utf-8")
    # parsed_data = json.dumps(data)
    
    print(type(data)) #string
    print(data)
    # parse data:
    y = json.loads(data)

    # the result is a Python dictionary:
    print(y["position_x"])

    position_x = float(y["position_x"])
    print(position_x)
    print(f"force_pred: {pred_force(position_x)} .")

    # #convert string to  object
    # data_json = json.loads(data)

    # #check new data type
    # print(type(data_json))

    #output
    #<class 'dict'>

    #access first_name in dictionary
    # print(parsed_data["function_exicution_time"]

    # print(parsed_data["position_x"])

    # print(type(data)) #string
    # print(data) 
    # data =data.replace("{","")
    # data =data.replace("}","")
    # # # data =data.replace("","")
    # # print(data)
    # # # Converting string into dictionary
    # # # using dict comprehension
    # res = dict(item.split(":") for item in data.split(","))
            
    # # Printing resultant string
    # print ("Resultant dictionary", str(res))
    # print(type(res))
    # position_x = float(res.get('\n    "position_x"'))
    # print(position_x)
    # print(res.keys())
    # print(res.values())




      
    # print(res.get('"position_x"'))

    # print(type(res.get('"position_x"')))
    # # convert a using float
    # # a = float()
    
    # # convert b using int
    # a = int(res.get('"id"'))
    # print(a)

    # print(type(a))



    


    print("time to recieve data to server")
    # ts stores the time in seconds
    ts = time.time()
    # print the current timestamp
    print(ts)

    # chunks = []
    # while True:
    #     # Keep reading while the client is writing.
    #     data = client_sock.recv(2048)
    #     if not data:
    #         # Client is done with sending.
    #         break
    #     chunks.append(data)

    """ Send data """
    # m = {
    # "position": 15325.299805,
    # "force": 15325.299805,
    # "cycle no.": 3
    # }
    force_pred = pred_force(position_x)
    print(type(force_pred))
    # force = 15325.299805
    # print(type(force))
    
    m = {"force_pred": force_pred}
    
    print(type(m))
    # convert into JSON:
    data = json.dumps(m)
    print(type(data))
    # data = "From server"
    print("[server]\n", data);
    # client_sock.send(data.encode(FORMAT))
    client_sock.sendall(bytes(data,encoding="utf-8"))

    print("time to send data from sever")
    # ts stores the time in seconds
    ts = time.time()
    # print the current timestamp
    print(ts)

    # client_sock.sendall(b''.join(chunks))
    client_sock.close()



    # output
    # Server Start 

    # New connection from ('127.0.0.1', 40050)
    # [SERVER] {
    #     "position_x": 5.000000,
    #     "function_exicution_time": 5.000000,
    #     "F_ref": 5.000000,
    #     "rtob_force": 5.000000
    # }
    # Resultant dictionary {'\n    "position_x"': ' 5.000000', '\n    "function_exicution_time"': ' 5.000000', '\n    "F_ref"': ' 5.000000', '\n    "rtob_force"': ' 5.000000\n'}
    # <class 'dict'>
    # 5.0
    # dict_keys(['\n    "position_x"', '\n    "function_exicution_time"', '\n    "F_ref"', '\n    "rtob_force"'])
    # dict_values([' 5.000000', ' 5.000000', ' 5.000000', ' 5.000000\n'])
    # time to recieve data to server
    # 1653640388.7977781
    # [server]
    # {"position": 15325.299805, "force": 15325.299805, "cycle no.": 3}
    # time to send data from sever
    # 1653640388.7978957
