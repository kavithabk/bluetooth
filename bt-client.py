#from bluetooth import *

# Create the client socket
#client_socket=BluetoothSocket( RFCOMM )
#
#client_socket.connect(("00:12:D2:5A:BD:E4", 3))
#
#client_socket.send("Hello World")
#
print "Finished"
#
#client_socket.close()
import socket
 
 hostMACAddress = '00:1B:10:00:2A:EC' # The MAC address of a Bluetooth adapter
 port = 3   # 3 is an arbitrary choice. However, it must match the port used
 size = 1024
 s = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM,
		 socket.BTPROTO_RFCOMM)
socket.connect((hostMACAddress, 3))
socket.send("Hello World")
print "Finished"
#
socket.close()
