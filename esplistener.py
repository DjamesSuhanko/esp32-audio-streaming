#!/usr/bin/env python
# # -*- coding: UTF-8 -*-
# Author: Djames Suhanko <djames.suhanko@gmail.com>
# Created: qui 31 jan 2019 12:51:56 -02
# Last Update: qui 31 jan 2019 12:51:56 -02
# File: <name>
# Notes: 
import pyaudio
import socket

# troque o ip abaixo e defina a porta desejada, acima de 1024
TCP_IP = "192.168.1.2"
TCP_PORT = 8888

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.bind((TCP_IP, TCP_PORT))

serversocket.listen(1)

print("Server {}", TCP_IP)
connection, client_address = serversocket.accept()

p = pyaudio.PyAudio()
stream = p.open(format=32, channels=1, rate=8000, output=True)

try:
    while True:
        data = connection.recv(800)
        stream.write(data)
        #print(data)


except KeyboardInterrupt:
    print("Finalizado com Ctrl+C")
    stream.stop_stream()
    stream.close()
    p.terminate()

