/**
Name:   Global.h
Author:  jjuiddong
Date:    8/4/2013

*/
#pragma once

enum PROC_STATE { ALIVE, DEAD };

struct SProcessData
{
	string cmdLine;
	PROCESS_INFORMATION pi;
	HWND hWnd;
	PROC_STATE state;
	bool checkProcess; // default : true
	int data; // 여러 용도로 쓰이는 데이타.
	
	SProcessData() : checkProcess(true), state(DEAD) {
		hWnd = NULL; 
		 pi.hProcess = NULL;
		 pi.hThread = NULL; 
	}
	bool operator== (const SProcessData &rhs) { 
		return (state == rhs.state) && (data == rhs.data) && (cmdLine == rhs.cmdLine);
	}
};

typedef vector<SProcessData> ProcessDatas;


extern int g_IntervalTime;


// Function
bool ReadConfigFile( const string &fileName, OUT ProcessDatas &out );
void LogPrint( const char *fmt, ... );

