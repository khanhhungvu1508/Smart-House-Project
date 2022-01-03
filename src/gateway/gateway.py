import serial
from time import sleep
import sys
from Adafruit_IO import MQTTClient

AIO_USERNAME = "KhanhNguyen"
AIO_KEY = "aio_xeqf11kxn9Lcqtc3eiiuQjdBhbJy"
AIO_SMARTHOME = ["AC_Power", "AC_Increase", "AC_Decrease", "LED", "Temperature", "Humidity"]

arduino = serial.Serial(port='COM2', baudrate=9600, timeout=.1)

def  connected(client):
    print("Connected...")
    for feed in AIO_SMARTHOME:
        client.subscribe(feed)

def  subscribe(client , userdata , mid , granted_qos):
    print("Subscribe successfully...")

def  disconnected(client):
    print("Disconnected...")
    sys.exit (1)

def  message(client , feed_id , payload):
    print("Received data: " + payload)
    if(payload == "FAN"):
        arduino.write(bytes("#FAN_ON*", 'UTF-8'))
    elif(payload == "AUTO"):
        arduino.write(bytes("#FAN_AUTO*", 'UTF-8'))
    elif(payload == "INC"):
        arduino.write(bytes("#FAN_INC*", 'UTF-8'))
    elif(payload == "LED"):
        arduino.write(bytes("#LED_ON*", 'UTF-8'))

client = MQTTClient(AIO_USERNAME , AIO_KEY)
client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message
client.on_subscribe = subscribe
client.connect()
client.loop_background()

def data_processing(data):
    if data.endswith("#\\r\\n") & data.startswith("*"):
        data = data.replace ("#\\r\\n", "")
        data = data.replace ("*", "")
        return data.split()
        

def send_data(data):
    if(len(data)):
        data = data.replace ("b'", "")
        data = data.replace ("'", "")
        d = data_processing(data)
        if (d != None):
            t = float(d[0])
            h = float(d[1])
            print("Update: temperature =", t, " humidity = ", h)
            client.publish(AIO_SMARTHOME[4], t)
            client.publish(AIO_SMARTHOME[5], h)

while True:
    if(arduino.in_waiting):
        data = str(arduino.readline())
        send_data(data)
    sleep(1)