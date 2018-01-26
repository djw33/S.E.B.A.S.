# first of all import the socket library
import socket
import RPi.GPIO as GPIO
import time
import subprocess

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.IN)
GPIO.setup(18, GPIO.OUT)
GPIO.setup(27, GPIO.IN)

otationCount = 0;
count = 0;
oldVal1 = 0;
timeStart = 1;
oldTimeStart = 0;
rev = 0;
pwm = GPIO.PWM(18, 200);
dutycycle = 50;
pwm.start(dutycycle);
#pwm.ChangeDutyCycle(50);

timeStart = time.clock();
 
# next create a socket object
s = socket.socket()         
print("Socket successfully created")
 
# reserve a port on your computer
port = 51717
 
# Next bind to the port
# we have not typed any ip in the ip field
# instead we have inputted an empty string
# this makes the server listen to requests 
# coming from other computers on the network
s.bind(('', port))        
print("socket binded to %s" %(port))
 
# put the socket into listening mode
s.listen(5)     
print("socket is listening")
# a forever loop until we interrupt it or 
# an error occurs
while True:
 
   # Establish connection with client.
   c, addr = s.accept()     
   print("Got connection from", addr)

   while True:
       newVal1 = GPIO.input(17);
       newVal2 = GPIO.input(27);
       if (newVal1 != oldVal1):
           count = count + 1;

       if (count > 305.25*12): # there are 12 changes per rotation and 407 rotations per revolution
           count = 0;
           rotationCount = rotationCount + 1;
           print("rotationCount: ", rotationCount);
           timeStart = time.clock();
           revPerSec = 1/(timeStart - oldTimeStart);
           oldTimeStart = timeStart;
           print("RevPerSec: ", revPerSec)

       oldVal1 = newVal1;
	
       data = str(c.recv(1024))
       if "1" in data:
           print("up")
           if (dutycycle < 90):
               dutycycle += 10;
               pwm.ChangeDutyCycle(dutycycle);
           print(dutycycle)
       elif "2" in data:
           print("down")
           if (dutycycle > 10):
               dutycycle -= 10;
               pwm.ChangeDutyCycle(dutycycle);
           print(dutycycle)
       elif "3" in data:
           print("right")
       elif "4" in data:
           print("left")
       else:
           break
    
   # Close the connection with the client
   c.close()
   break
