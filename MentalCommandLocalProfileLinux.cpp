/****************************************************************************
**
** Copyright 2016 by Emotiv. All rights reserved
** Example - Mental command with  local profile
** This example demonstrates how to detect mental command detection with
** a profile.
** This example work on single connection
****************************************************************************/

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cassert>
#include <dlfcn.h>
#include <stdlib.h>
#undef UNICODE
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif

#if __linux__ || __APPLE__
#include <unistd.h>
int _kbhit(void);
#endif

#define EXIT_FAILURE 1


#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"


#ifdef __APPLE__
int _kbhit (void)
{
    struct timeval tv;
    fd_set rdfs;
    
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    FD_ZERO(&rdfs);
    FD_SET (STDIN_FILENO, &rdfs);
    
    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);
}
#endif

#ifdef __linux__
int _kbhit(void)
{
    struct timeval tv;
    fd_set read_fd;
    
    tv.tv_sec=0;
    tv.tv_usec=0;
    
    FD_ZERO(&read_fd);
    FD_SET(0,&read_fd);
    
    if(select(1, &read_fd,NULL, NULL, &tv) == -1)
        return 0;
    
    if(FD_ISSET(0,&read_fd))
        return 1;
    
    return 0;
}
#endif


using namespace std;

typedef unsigned long ulong;

void handleMentalCommandEvent(std::ostream& os, EmoEngineEventHandle MentalCommandEvent);
void loadProfile(int userID);
void showTrainedActions(int userID);
void showCurrentActionPower(EmoStateHandle);
void trainMentalCommandActions(int headsetID);
void setActiveActions(int userID);
void setMentalCommandActions(int, IEE_MentalCommandAction_t);
ulong listAction = 0; //list Action


std::string profileNameForLoading = "/media/shanty/New Volume/AM477/Project/Profiles/profile1.emu";//"C:/1/profile1.emu";
std::string profileNameForSaving = "/media/shanty/New Volume/AM477/Project/Profiles/profile1.emu";//"C:/1/profile1.emu";"C:/1/profile1.emu";

typedef EmoEngineEventHandle(*FNPTR)();
typedef int(*FNPTR1)(EmoEngineEventHandle);
typedef IEE_Event_t(*FNPTR2)(EmoEngineEventHandle);
typedef int(*FNPTR3)(EmoEngineEventHandle, unsigned int*);
typedef EmoStateHandle(*FNPTR4)();
typedef int(*FNPTR5)(EmoEngineEventHandle, EmoStateHandle);
typedef IEE_MentalCommandAction_t(*FNPTR6)(EmoStateHandle);
typedef float(*FNPTR7)(EmoStateHandle);
typedef int(*FN_FINAL)();
typedef void(*FNPTR8)(EmoStateHandle);
typedef void(*FNPTR9)(EmoEngineEventHandle);
typedef int(*FNPTR10)(unsigned int, const char*);
typedef int(*FNPTR11)(unsigned int, unsigned long*);
typedef IEE_MentalCommandEvent_t(*FNPTR12)(EmoEngineEventHandle);
typedef int(*FNPTR13)(unsigned int, IEE_MentalCommandAction_t);
typedef int(*FNPTR14)(unsigned int, IEE_MentalCommandTrainingControl_t);
typedef int(*FNPTR15)(unsigned int, unsigned long);
//typedef 
int main(int argc, char** argv) {
/*SHANTY MOD FOR OPENING DLL FILES*******************************************************************************/
	void* hinst = dlopen("/media/shanty/New Volume/community-sdk-master/bin/linux64/libedk.so.3.3.3", RTLD_NOW );
	if (!hinst)
	{
		std::cout << "Could not load dll"<<endl;
		return EXIT_FAILURE;
	}

	FNPTR4 IEE_EmoEngineEventCreate = (FNPTR4)dlsym(hinst, "IEE_EmoEngineEventCreate");
	if (!IEE_EmoEngineEventCreate)
	{
		std::cout << "IEE_EmoEngineEventCreate loading failed";
		return EXIT_FAILURE;
	}
	FNPTR IEE_EmoStateCreate = (FNPTR)dlsym(hinst, "IEE_EmoStateCreate");
	if (!IEE_EmoStateCreate)
	{
		std::cout << "IEE_EmoStateCreate loading failed";
		return EXIT_FAILURE;
	}
	FNPTR IEE_EngineConnect = (FNPTR)dlsym(hinst, "IEE_EngineConnect");
	if (!IEE_EngineConnect)
	{
		std::cout << "IEE_EngineConnect loading failed";
		return EXIT_FAILURE;
	}
	FNPTR1 IEE_EngineGetNextEvent = (FNPTR1)dlsym(hinst, "IEE_EngineGetNextEvent");
	if (!IEE_EngineGetNextEvent)
	{
		std::cout << "IEE_EngineGetNextEvent loading failed";
		return EXIT_FAILURE;
	}
	FNPTR2 IEE_EmoEngineEventGetType = (FNPTR2)dlsym(hinst, "IEE_EmoEngineEventGetType");
	if (!IEE_EmoEngineEventGetType)
	{
		std::cout << "IEE_EmoEngineEventGetType loading failed";
		return EXIT_FAILURE;
	}
	FNPTR3 IEE_EmoEngineEventGetUserId = (FNPTR3)dlsym(hinst, "IEE_EmoEngineEventGetUserId");
	if (!IEE_EmoEngineEventGetUserId)
	{
		std::cout << "IEE_EmoEngineEventGetType loading failed";
		return EXIT_FAILURE;
	}
	FNPTR5 IEE_EmoEngineEventGetEmoState = (FNPTR5)dlsym(hinst, "IEE_EmoEngineEventGetEmoState");
	if (!IEE_EmoEngineEventGetEmoState)
	{
		std::cout << "IEE_EmoEngineEventGetType loading failed";
		return EXIT_FAILURE;
	}
	FNPTR6 IS_MentalCommandGetCurrentAction = (FNPTR6)dlsym(hinst, "IS_MentalCommandGetCurrentAction");
	if (!IS_MentalCommandGetCurrentAction)
	{
		std::cout << "IEE_EmoEngineEventGetType loading failed";
		return EXIT_FAILURE;
	}
	FNPTR7 IS_MentalCommandGetCurrentActionPower = (FNPTR7)dlsym(hinst, "IS_MentalCommandGetCurrentActionPower");
	if (!IS_MentalCommandGetCurrentActionPower)
	{
		std::cout << "IEE_EmoEngineEventGetType loading failed";
		return EXIT_FAILURE;
	}
	FN_FINAL IEE_EngineDisconnect = (FN_FINAL)dlsym(hinst, "IEE_EngineDisconnect");
	if (!IEE_EngineDisconnect)
	{
		std::cout << "IEE_EmoEngineEventGetType loading failed";
		return EXIT_FAILURE;
	}
	FNPTR8 IEE_EmoStateFree = (FNPTR8)dlsym(hinst, "IEE_EmoStateFree");
	if (!IEE_EmoStateFree)
	{
		std::cout << "IEE_EmoEngineEventGetType loading failed";
		return EXIT_FAILURE;
	}
	FNPTR9 IEE_EmoEngineEventFree = (FNPTR9)dlsym(hinst, "IEE_EmoEngineEventFree");
	if (!IEE_EmoEngineEventFree)
	{
		std::cout << "IEE_EmoEngineEventGetType loading failed";
		return EXIT_FAILURE;
	}
	
	/*SHANTY MOD FOR LOAD LIBRARY STUFF ENDS***********************************************************************/
	EmoEngineEventHandle eEvent       = IEE_EmoEngineEventCreate();
	EmoStateHandle eState             = IEE_EmoStateCreate();
	unsigned int userID	              = 0;
	const unsigned short composerPort = 1726;
	int option = 0;
	int state  = 0;
	bool isUserAdded = false;
	std::string input;
	
    std::cout << "===================================================================" << "\n";
    std::cout << "   Example to use MentalCommand for training with profile functions" << "\n";
    std::cout << "===================================================================" << "\n";
    std::cout << "1. Load profile and show MentalCommand actions		             " << "\n";
    std::cout << "2. Train MentalCommand Actions						             " << "\n";
	std::cout << ">> ";

	std::getline(std::cin, input, '\n');
	option = atoi(input.c_str());

	if (IEE_EngineConnect() != 0) 
	{
		std::cout<<IEE_EngineConnect()<<endl;
		throw std::runtime_error("Emotiv Engine start up failed.");
	}
		
	while (!_kbhit()) {
		state = IEE_EngineGetNextEvent(eEvent);
		std::cout<<state<<endl;
		// New event needs to be handled
		if (state == 0) {

			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			IEE_EmoEngineEventGetUserId(eEvent, &userID);
			std::cout<<"eventType: "<<eventType<<endl;
			switch (eventType) {
				case IEE_UserAdded:
				{
                    cout << endl << "New user " << userID <<" added"  << endl;

					if (option == 1)
					{
						loadProfile(userID);
                        showTrainedActions(userID);
					}
					if (option == 2)
					{	
                        setActiveActions(userID);
                        setMentalCommandActions(userID, MC_NEUTRAL);
					}
						
					break;
				}
					
				case IEE_UserRemoved:
				{
                    std::cout << std::endl << "User " << userID << " has been removed." << std::endl;
					break;
				}
					
				case IEE_EmoStateUpdated:
				{
					std::cout<<"EmoStateUpdated"<<endl;
					IEE_EmoEngineEventGetEmoState(eEvent, eState);
					if (option == 1)
					{
						showCurrentActionPower(eState);
						IEE_MentalCommandAction_t eeAction = IS_MentalCommandGetCurrentAction(eState);
						float actionPower = IS_MentalCommandGetCurrentActionPower(eState);
						std::cout << eeAction << "\t" << actionPower << endl;
						//int ret = sprintf(data, "%f", actionPower);
						
						
					}
					break;
				}

				// Handle MentalCommand training related event
				case IEE_MentalCommandEvent:
				{
					handleMentalCommandEvent(std::cout, eEvent);
					break;
				}
				default:
					break;
			}
		}
	}
	

	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);

	return 0;
}



void loadProfile(int userID)
{			
	void* hinst = dlopen("/media/shanty/New Volume/community-sdk-master/bin/linux64/libedk.so", RTLD_LAZY );
	FNPTR10 IEE_LoadUserProfile = (FNPTR10)dlsym(hinst, "IEE_LoadUserProfile");
	if (!IEE_LoadUserProfile)
	{
		std::cout << "IEE_LoadUserProfile loading failed";
		//return EXIT_FAILURE;
	}
	if (IEE_LoadUserProfile(userID, profileNameForLoading.c_str()) == EDK_OK)
        std::cout << "Load Profile : done" << std::endl;
	else
		throw std::runtime_error("Can't load profile.");
}

const char *byte_to_binary(long x)
{    
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 8192; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

void showTrainedActions(int userID)
{
	void* hinst = dlopen("/media/shanty/New Volume/community-sdk-master/bin/linux64/libedk.so", RTLD_LAZY );
	FNPTR11 IEE_MentalCommandGetTrainedSignatureActions = (FNPTR11)dlsym(hinst, "IEE_MentalCommandGetTrainedSignatureActions");
	if (!IEE_MentalCommandGetTrainedSignatureActions)
	{
		std::cout << "IEE_MentalCommandGetTrainedSignatureActions loading failed";
		//return EXIT_FAILURE;
	}
	unsigned long pTrainedActionsOut = 0;
    IEE_MentalCommandGetTrainedSignatureActions(userID, &pTrainedActionsOut);
    
    std::cout << "Trained Actions" << " : " << byte_to_binary(pTrainedActionsOut) << "\n";
}

void showCurrentActionPower(EmoStateHandle eState)
{
	void* hinst = dlopen("/media/shanty/New Volume/community-sdk-master/bin/linux64/libedk.so", RTLD_LAZY );
	FNPTR6 IS_MentalCommandGetCurrentAction = (FNPTR6)dlsym(hinst, "IS_MentalCommandGetCurrentAction");
	FNPTR7 IS_MentalCommandGetCurrentActionPower = (FNPTR7)dlsym(hinst, "IS_MentalCommandGetCurrentActionPower");

	IEE_MentalCommandAction_t eeAction = IS_MentalCommandGetCurrentAction(eState);
	float actionPower = IS_MentalCommandGetCurrentActionPower(eState);

	switch (eeAction)
	{
        case MC_NEUTRAL: { std::cout << "Neutral" << " : " << actionPower << "; \n"; break; }
        case MC_PUSH:    { std::cout << "Push" << " : " << actionPower << "; \n"; break; }
        case MC_PULL:    { std::cout << "Pull" << " : " << actionPower << "; \n"; break; }
	}	
}

void setActiveActions(int userID)
{
	void* hinst = dlopen("/media/shanty/New Volume/community-sdk-master/bin/linux64/libedk.so", RTLD_LAZY );
	FNPTR15  IEE_MentalCommandSetActiveActions = (FNPTR15)dlsym(hinst, " IEE_MentalCommandSetActiveActions");
	ulong action1 = (ulong)1;//IEE_MentalCommandAction_t::MC_PUSH;
    ulong action2 = (ulong)2;//IEE_MentalCommandAction_t::MC_PULL;
    listAction = action1 | action2;

    int errorCode = EDK_OK;
    errorCode = IEE_MentalCommandSetActiveActions(userID, listAction);

    if (errorCode == EDK_OK)
    {
        std::cout << "Setting MentalCommand active actions (MC_PUSH | MC_PULL) for user " << userID << std::endl;
    }
    else std::cout << "Setting MentalCommand active actions error: " << errorCode;
}

void setMentalCommandActions(int headsetID,IEE_MentalCommandAction_t action)
{
	void* hinst = dlopen("/media/shanty/New Volume/community-sdk-master/bin/linux64/libedk.so", RTLD_LAZY );
	FNPTR13  IEE_MentalCommandSetTrainingAction = (FNPTR13)dlsym(hinst, " IEE_MentalCommandSetTrainingAction");
	FNPTR14 IEE_MentalCommandSetTrainingControl = (FNPTR14)dlsym(hinst, "IEE_MentalCommandSetTrainingControl");
	int errorCode = IEE_MentalCommandSetTrainingAction(headsetID, action);
    errorCode = IEE_MentalCommandSetTrainingControl(headsetID, MC_START);

	switch (action)
	{
		case MC_NEUTRAL: { std::cout << std::endl <<"TRAINING NEUTRAL" <<std::endl; break; }
		case MC_PUSH:    { std::cout << std::endl <<"TRAINING PUSH" <<std::endl; break; }
		case MC_PULL:    { std::cout << std::endl <<"TRAINING PULL" <<std::endl; break; }
	}
}

void trainMentalCommandActions(int headsetID)
{
	void* hinst = dlopen("/media/shanty/New Volume/community-sdk-master/bin/linux64/libedk.so", RTLD_LAZY );
	FNPTR10  IEE_SaveUserProfile = (FNPTR10)dlsym(hinst, "IEE_SaveUserProfile");
	
	if (listAction & MC_PUSH)
    {
        setMentalCommandActions(headsetID, MC_PUSH);
        listAction ^= MC_PUSH; //remove push action from list action
        return;
    }
    if (listAction & MC_PULL)
    {
        setMentalCommandActions(headsetID, MC_PULL);
        listAction ^= MC_PULL; //remove pull action from list action
        return;
    }

    if (IEE_SaveUserProfile(headsetID, profileNameForSaving.c_str()) == EDK_OK)
	{
		cout<<endl<<"Save Profile: done";
	}
	else cout<<endl<<"Can't save profile";
}

void handleMentalCommandEvent(std::ostream& os, EmoEngineEventHandle MentalCommandEvent) {

	void* hinst = dlopen("/media/shanty/New Volume/community-sdk-master/bin/linux64/libedk.so", RTLD_LAZY );
	FNPTR3 IEE_EmoEngineEventGetUserId = (FNPTR3)dlsym(hinst, "IEE_EmoEngineEventGetUserId");
	FNPTR12 IEE_MentalCommandEventGetType = (FNPTR12)dlsym(hinst, "IEE_MentalCommandEventGetType");
	FNPTR13  IEE_MentalCommandSetTrainingAction = (FNPTR13)dlsym(hinst, " IEE_MentalCommandSetTrainingAction");
	FNPTR14 IEE_MentalCommandSetTrainingControl = (FNPTR14)dlsym(hinst, "IEE_MentalCommandSetTrainingControl");
	unsigned int userID = 0;
	IEE_EmoEngineEventGetUserId(MentalCommandEvent, &userID);
	IEE_MentalCommandEvent_t eventType = IEE_MentalCommandEventGetType(MentalCommandEvent);


	switch (eventType) {

		case IEE_MentalCommandTrainingStarted:
		{
			os << std::endl << "MentalCommand training for user " << userID << " STARTED!" << std::endl;
			break;
		}

		case IEE_MentalCommandTrainingSucceeded:
		{
			os << std::endl << "MentalCommand training for user " << userID << " SUCCEEDED!" << std::endl;
			IEE_MentalCommandSetTrainingControl(userID, MC_ACCEPT);
#ifdef _WIN32
            Sleep(3000);
#endif
#if linux || __APPLE__
            sleep(3);
#endif
			break;
		}

		case IEE_MentalCommandTrainingFailed:
		{
			os << std::endl << "MentalCommand training for user " << userID << " FAILED!" << std::endl;
			break;
		}

		case IEE_MentalCommandTrainingCompleted:
		{
			os << std::endl << "MentalCommand training for user " << userID << " COMPLETED!" << std::endl;
			trainMentalCommandActions(userID);
			break;
		}

		case IEE_MentalCommandTrainingRejected:
		{
			os << std::endl << "MentalCommand training for user " << userID << " REJECTED!" << std::endl;
			break;
		}

		case IEE_MentalCommandTrainingReset:
		{
			os << std::endl << "MentalCommand training for user " << userID << " RESET!" << std::endl;
			break;
		}

		case IEE_MentalCommandAutoSamplingNeutralCompleted:
		{
			os << std::endl << "MentalCommand auto sampling neutral for user " << userID << " COMPLETED!" << std::endl;
			break;
		}

		case IEE_MentalCommandSignatureUpdated:
		{
			os << std::endl << "MentalCommand signature for user " << userID << " UPDATED!" << std::endl;
			break;
		}

		case IEE_MentalCommandNoEvent:
			break;

		default:
			//@@ unhandled case
			{
				assert(0);
				break;
			}
	}
}
