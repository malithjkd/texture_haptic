# clientpy.py
# python3

import socket
import time
import json
import pickle

IP = "127.0.0.1"  # Standard loopback interface address (localhost)
PORT = 4455  # Port to listen on (non-privileged ports are > 1023)

m = {"id": 2, "name": "abc"} # a real dict //or data = json.load(open("data.json"))

data = json.dumps(m)
# data = pickle.dumps(m)

# IP = "127.0.0.1"
# PORT = 4455
# ADDR = (IP, PORT)
SIZE =  1024
FORMAT = "utf-8"

# Create client socket.
client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to server (replace 127.0.0.1 with the real server IP).
client_sock.connect((IP, PORT))



# Send some data to server.
# client_sock.sendall(b'Hello, world')

""" Send data """
# data = "From CLIENT"
# client_sock.send(data.encode(FORMAT))
print("[client]\n", data);
client_sock.sendall(bytes(data,encoding="utf-8"))

print("time to send data from client")
# ts stores the time in seconds
ts = time.time()
# print the current timestamp
print(ts)


client_sock.shutdown(socket.SHUT_WR)




# Receive some data back.
data = client_sock.recv(SIZE).decode(FORMAT)
print(f"[SERVER] {data}")

print("time to recieve data to client")
# ts stores the time in seconds
ts = time.time()
# print the current timestamp
print(ts)


# chunks = []
# while True:
#     data = client_sock.recv(2048)
#     if not data:
#         break
#     chunks.append(data)
# print('Received', repr(b''.join(chunks)))

# Disconnect from server.
client_sock.close()


# # echo-client.py

# import socket

# HOST = "127.0.0.1"  # The server's hostname or IP address
# PORT = 65434  # The port used by the server

# with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
#     s.connect((HOST, PORT))
#     s.sendall(b"Hello, world")
#     data = s.recv(1024)

# print(f"Received {data!r}")


# # Import socket module
# import socket            
 
# # Create a socket object
# s = socket.socket()        
 
# # Define the port on which you want to connect
# port = 12345               
 
# # connect to the server on local computer
# s.connect(('127.0.0.1', port))
 
# # receive data from the server and decoding to get the string.
# print (s.recv(1024).decode())
# # close the connection
# s.close()   
     