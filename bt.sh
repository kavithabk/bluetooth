#!/bin/sh

sudo rfcomm release 0
sudo rfcomm bind /dev/rfcomm0 CC:07:AB:69:E4:46 6
sudo rfcomm listen 6 &
#sudo python k.py
