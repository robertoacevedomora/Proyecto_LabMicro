import serial
import paho.mqtt.client as mqtt
import json
import time
import os
from datetime import datetime

ser = serial.Serial('COM6',9600,timeout =0)


ACCESS_TOKEN = 'HjHFOGkcQkt3XN4S0I6g'
broker = 'demo.thingsboard.io'
port = 1883

def on_publish(client, userdata, result):
    print("data published to thingsboard /n")
    pass
client1 = mqtt.Client("Proyecto")
client1.on_publish = on_publish
client1.username_pw_set(ACCESS_TOKEN)
client1.connect(broker,port,keepalive=60)
dict = dict()


while (1):
    ejes = ser.readline().strip().decode('utf-8')
    ejes = ejes.split('\t')
    print(len(ejes))
    dict["Amarillo"] = ejes[0]
    hola = json.dumps(dict)
    client1.publish("v1/devices/me/telemetry",hola)
    print(hola)
    time.sleep(1)
