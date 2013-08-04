/**
Name:   Global.h
Author:  jjuiddong
Date:    8/4/2013

*/
#pragma once


struct SProcessData
{
	string cmdLine;
	PROCESS_INFORMATION pi;
	HWND hWnd;
	
	SProcessData() {hWnd = NULL; }
};

typedef vector<SProcessData> ProcessDatas;


extern int g_IntervalTime;


// Function
bool ReadConfigFile( const string &fileName, OUT ProcessDatas &out );
void LogPrint( const char *fmt, ... );

