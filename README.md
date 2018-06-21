# BCI-with-Emotiv
The objective is to create a system where in which any motor driven structure (say, a prosthetic arm) can be controlled using EEG signals. The headset used was the EMOTIV EPOC, along with the supplied SDK. Since at the time of this commit, there is no support for Mental Command with Local Profile with Python or sufficient linux support, this implementation uses the local profile implementation on c++ and uses python to create a bridge between processing on the host PC and actuation on the arduino 

The project is built on the SDK provided by EMOTIV inc:
https://github.com/Emotiv/community-sdk

The C++ script found in community-sdk-master/examples_basic/C++/MentalCommandWithLocalProfile/ was modified to expose some APIs and compiled into a DLL file using Visual Studio

This DLL was then used in a python script used to bridge the EEG processing done on the (Windows) PC, with the motor actuation on an Arduino 
