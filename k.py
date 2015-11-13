#! /usr/bin/python
 
import serial
import socket
import fcntl
import struct


def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,  # SIOCGIFADDR
        struct.pack('256s', ifname[:15])
    )[20:24])

 

bluetoothSerial = serial.Serial( "/dev/rfcomm0", baudrate=9600 )
 
a = None
while a == None:
    try:
        a = float( raw_input( "Please enter the first number: " ) )
    except:
        pass    # Ignore any errors that may occur and try again
 
b = None
while b == None:
    try:
        b = float( raw_input( "Please enter the second number: " ) )
    except:
        pass    # Ignore any errors that may occur and try again
 
bluetoothSerial.write( "{0} {1} {2}".format( a, b, get_ip_address('eth0') ) )
print bluetoothSerial.readline()
