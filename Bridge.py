"""
###########FOR TRAINING#########
from ctypes import *

mydll = cdll.LoadLibrary("E:\\community-sdk-master\\examples_basic\\C++\\Debug\\MentalCommandWithLocalProfile.dll");

mydll.getCommands(2)

"""
#####FOR MENTAL COMMAND DETECTION######

from time import sleep
import serial
from ctypes import *
print "Loading DLLs...\n"
mydll = cdll.LoadLibrary("E:\\community-sdk-master\\examples_basic\\C++\\Debug\\MentalCommandWithLocalProfile.dll");

ser = serial.Serial('COM3', 9600)

sleep(1)
ser.readline()
ser.flush()
#x = 0.2
print "Serial Connection Established and Handshaking completed\n"

err = mydll.initEngine()
if(err != 0):
    print "Error in engine initialising. Exiting..."
    exit()
else:
    print "Engine Init success!\n"
sleep(1)
while(1):
    x = mydll.getCommands(1)
    print "x is "
    print x
    print '\n'
    if(x != 0):
        ser.write(str(x))
        print "1 written\n"
        data =  ser.read(ser.inWaiting())
        print data
    else:
        ser.write(str(0))
        print "0 written"
        data =  ser.read(ser.inWaiting())
        print data
    sleep(1)
    ser.flush()

ser.close()
mydll.closeEngine()


