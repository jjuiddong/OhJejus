
#include "stdafx.h"
#include "Global.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


/**
 @brief config ���� ����.

  {
	"process" : [
		"logprinter.exe 1",
		"logprinter.exe 2",
	 ]
 }

 */
bool ReadConfigFile( const string &fileName, OUT ProcessDatas &out )
{
	try
	{
		using boost::property_tree::ptree;
		using std::string;
		ptree props;
		boost::property_tree::read_json(fileName.c_str(), props);

		ptree::assoc_iterator visit_it = props.find("process");
		if (props.not_found() != visit_it)
		{
			ptree &child_field = props.get_child("process");
			BOOST_FOREACH(auto &v, child_field)
			{
				SProcessData info;
				info.cmdLine =v.second.data();
				out.push_back( info );
			}
		}
	}
	catch (std::exception &e)
	{
		LogPrint( "LoadConfigFile Error msg = [%s]", e.what() );
		return false;
	}

	return true;
}


/**
 @brief ���Ͽ� msg�� �����Ѵ�.
 */
void LogFile( const char *msg )
{
	std::ofstream ofs("log_ohjejus.txt", std::ios_base::app);

	SYSTEMTIME t;
	GetLocalTime(&t);
	ofs << "(" << t.wYear << "-" << t.wMonth << "-" << t.wDay << ") " << t.wHour << "��" << t.wMinute << "��";
	ofs << t.wSecond << "��";
	ofs << "  ";
	ofs << msg << std::endl;
}


/**
 @brief �α׷� �����.
 */
void LogPrint( const char *fmt, ... )
{
	char textString[ 256] = {'\0'};
	va_list args;
	va_start ( args, fmt );
	vsnprintf_s( textString, sizeof(textString), _TRUNCATE, fmt, args );
	va_end ( args );
	LogFile( textString );
}