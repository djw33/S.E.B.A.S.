# first of all import the socket library
import socket               
 
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
       data = str(c.recv(1024))
       if "1" in data:
           print("up")
       elif "2" in data:
           print("down")
       elif "3" in data:
           print("right")
       elif "4" in data:
           print("left")
       else:
           break
    
   # Close the connection with the client
   c.close()
   break
