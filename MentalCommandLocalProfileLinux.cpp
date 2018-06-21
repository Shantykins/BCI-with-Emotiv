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
//#include <libusb.h>
#undef UNICODE

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif

#define DLLEXPORT extern "C" __declspec(dllexport)
#if __linux__ || __APPLE__
#include <unistd.h>
int _kbhit(void);
#endif

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


std::string profileNameForLoading = "E:\\community-sdk-master\\examples_basic\\C++\\Debug";//"C:/1/profile1.emu";
std::string profileNameForSaving = "E:\\community-sdk-master\\examples_basic\\C++\\Debug";//"C:/1/profile1.emu";"C:/1/profile1.emu";


EmoEngineEventHandle eEvent;
EmoStateHandle eState;
BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	return TRUE;
}

DLLEXPORT int initEngine()
{
	
	 eEvent = IEE_EmoEngineEventCreate();
	eState = IEE_EmoStateCreate();
	
	const unsigned short composerPort = 1726;
	//int option = 0;
	int state = 0;
	bool isUserAdded = false;
	std::string input;
	
	if (IEE_EngineRemoteConnect("127.0.0.1", 1726) != 0) {
		throw std::runtime_error("Emotiv Driver start up failed.");
		return 1;
	}
	else std::cout << "Emotiv Engine Connected!" << endl;
	return 0;
}

DLLEXPORT void closeEngine()
{
	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);
}

DLLEXPORT int getCommands(int option)
{
	unsigned int userID = 0;

		int state = IEE_EngineGetNextEvent(eEvent);

		if (state == 0) {

			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			IEE_EmoEngineEventGetUserId(eEvent, &userID);

			switch (eventType) {
				case IEE_UserAdded:
				{

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
					IEE_EmoEngineEventGetEmoState(eEvent, eState);
					if (option == 1)
					{
						showCurrentActionPower(eState);
						IEE_MentalCommandAction_t eeAction = IS_MentalCommandGetCurrentAction(eState);
						float actionPower = IS_MentalCommandGetCurrentActionPower(eState);
						if (eeAction!= 1) {
							std::cout << "Clench: " << actionPower << endl;
							return 1;
						}
						else
							return 0;
											
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
	//}

	

	return 0;
}



void loadProfile(int userID)
{
	if (!IEE_LoadUserProfile)
	{
		std::cout << "IEE_LoadUserProfile loading failed";
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
	if (!IEE_MentalCommandGetTrainedSignatureActions)
	{
		std::cout << "IEE_MentalCommandGetTrainedSignatureActions loading failed";
	}
	unsigned long pTrainedActionsOut = 0;
    IEE_MentalCommandGetTrainedSignatureActions(userID, &pTrainedActionsOut);
    
    std::cout << "Trained Actions" << " : " << byte_to_binary(pTrainedActionsOut) << "\n";
}

void showCurrentActionPower(EmoStateHandle eState)
{
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

	ulong action1 = (ulong)IEE_MentalCommandAction_t::MC_PUSH;
    ulong action2 = (ulong)IEE_MentalCommandAction_t::MC_PULL;
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
