/*
 *  File:		Logger.cpp
 *  Version:	1.0
 *
 *  Created by Oleksandr Kurylenko on 4/9/14.
 *
 *  Copyright (c) 2014 Wunderkopf. All rights reserved.
 *
 */
#include "Precompile.hpp"
#include "Logger.hpp"

void Log(std::uint32_t inFormatID, const std::string& inFileName, std::uint32_t inLine, /*const boost::any& inData,*/
	const char* inFormat, va_list inArgs);

namespace Logging
{
	//CElement class
	CElement::CElement()
	{
	}
	
	CElement::~CElement()
	{
	}
	
	//CText class
	CText::CText(const std::string& inText)
    : mText(inText)
	{
	}
	
	CText::~CText()
	{
	}
	
	std::string CText::ToString(const CLogParam& /*inParam*/)
	{
		return mText;
	}
	
	//CTime class
	CTime::CTime()
	{
	}
	
	CTime::~CTime()
	{
	}
	
	std::string CTime::ToString(const CLogParam& /*inParam*/)
	{
		time_t currTime = time(NULL);
		std::string timeStr(ctime(&currTime));
        std::size_t pos = timeStr.find('\n');
		if (pos != std::string::npos)
			timeStr.erase(pos);
        
		return timeStr;
	}
	
	// CFileName class
	CFileName::CFileName()
	{
	}
	
	CFileName::~CFileName()
	{
	}
	
	std::string CFileName::ToString(const CLogParam& inParam)
	{
		return inParam.mFileName;
	}
	
	//CEndOfLine class
	CEndOFLine::CEndOFLine()
	{
	}
	
	CEndOFLine::~CEndOFLine()
	{
	}
	
	std::string CEndOFLine::ToString(const CLogParam& /*inParam*/)
	{
		return std::string("\n");
	}
	
	//CLine class
	CLine::CLine()
	{
	}
	
	CLine::~CLine()
	{
	}
	
	std::string CLine::ToString(const CLogParam& inParam)
	{
		//return boost::lexical_cast<std::string>(inParam.mLine);
        std::stringstream stream;
        stream << inParam.mLine;
        std::string result(stream.str());
        return result;
	}
	
	//CTrace class
	CTrace::CTrace()
	{
	}
	
	CTrace::~CTrace()
	{
	}
	
	std::string CTrace::ToString(const CLogParam& inParam)
	{
		return inParam.mTrace;
	}
	
	//CFormat class
	CFormat::CFormat(const std::uint32_t inFormatID)
    : mFormatID(inFormatID)
	{
	}
	
	CFormat::~CFormat()
	{
	}
	
	CFormat& operator << (CFormat& outFormat, CElement* inElement)
	{
		assert(inElement != nullptr);
        
		std::shared_ptr<CElement> element(inElement);
		outFormat.mFormat.push_back(element);
		return outFormat;
	}
	
    std::uint32_t CFormat::GetID() const
	{
		return mFormatID;
	}
	
	std::string CFormat::ToString(const CLogParam& inParam)
	{
		std::string result;
		std::vector<std::shared_ptr<CElement> >::iterator it = mFormat.begin();
		for (; it != mFormat.end(); ++it)
			result += (*it)->ToString(inParam);
		
		return result;
	}
	
	//CSink class
	CSink::CSink()
	{
	}
	
	CSink::~CSink()
	{
	}
	
	//CLogger class
	CLogger::CLogger()
	{
	}
	
	
	CLogger::~CLogger()
	{
	}
	
	void CLogger::AddLogger(std::shared_ptr<CFormat> inFormat, std::shared_ptr<CSink> inSink)
	{
		std::shared_ptr<CFormat> format = inFormat;
		std::shared_ptr<CSink> sink = inSink;
		mLogger.push_back(std::make_pair(format, sink));
	}
	
	void CLogger::PrintLog(const CLogParam& inParam)
	{
		std::vector<std::pair<std::shared_ptr<CFormat>, std::shared_ptr<CSink> > >::iterator it = mLogger.begin();
		for (; it != mLogger.end(); ++it)
		{
			if (it->first->GetID() == inParam.mFormatID)
				it->second->Write(it->first->ToString(inParam)/*, inParam.m_Data*/);
		}
	}
    
    void CLogger::Reset()
    {
        mLogger.clear();
    }
}

void Log(std::uint32_t inFormatID, const std::string& inFileName, std::uint32_t inLine, /*const boost::any& inData,*/
	const char* inFormat, va_list inArgs)
{
	char buff[4096 + 1] = { 0 };

	// vsnprintf behave differently on win and mac in case if output buffer is too small
#ifdef WIN32
	::_vsnprintf(buff, 4096, inFormat, inArgs);
#else
	::vsnprintf(buff, 4096 + 1, inFormat, inArgs);
#endif

	Logging::CLogger& logger = Logging::CLogger::getInstance();
	Logging::CLogParam param;
	param.mFormatID = inFormatID;
	param.mTrace = std::string(buff, ::strlen(buff));
	param.mFileName = inFileName;
	param.mLine = inLine;
	//param.mData = inData;
	logger.PrintLog(param);
}

void LogWarning(const char* inFormat, ...)
{
	va_list valist;
	va_start(valist, inFormat);
	Log(FOUR_CHAR_ID('w', 'a', 'r', 'n'), std::string(""), 0, /*boost::any(),*/ inFormat, valist);
	va_end(valist);
}

void LogError(const char* inFormat, ...)
{
	va_list valist;
	va_start(valist, inFormat);
	Log(FOUR_CHAR_ID('e', 'r', 'r', 'o'), std::string(""), 0, /*boost::any(),*/ inFormat, valist);
	va_end(valist);
}

void LogInfo(const char* inFormat, ...)
{
	va_list valist;
	va_start(valist, inFormat);
	Log(FOUR_CHAR_ID('i', 'n', 'f', 'o'), std::string(""), 0, /*boost::any(),*/ inFormat, valist);
	va_end(valist);
}

void LogDebug(const char* inFormat, ...)
{
#ifdef DEBUG
	va_list valist;
	va_start(valist, inFormat);
	Log(FOUR_CHAR_ID('d', 'e', 'b', 'u'), std::string(""), 0, /*boost::any(),*/ inFormat, valist);
	va_end(valist);
#endif
}

/*void LogWarningWithData(const boost::any& inData, const char* inFormat, ...)
{
	va_list valist;
	va_start(valist, inFormat);
	Log(FOUR_CHAR_ID('w', 'a', 'r', 'n'), std::string(""), 0, inData, inFormat, valist); 
	va_end(valist);
}

void LogErrorWithData(const boost::any& inData, const char* inFormat, ...)
{
	va_list valist;
	va_start(valist, inFormat);
	Log(FOUR_CHAR_ID('e', 'r', 'r', 'o'), std::string(""), 0, inData, inFormat, valist); 
	va_end(valist);
}

void LogInfoWithData(const boost::any& inData, const char* inFormat, ...)
{
	va_list valist;
	va_start(valist, inFormat);
	Log(FOUR_CHAR_ID('i', 'n', 'f', 'o'), std::string(""), 0, inData, inFormat, valist); 
	va_end(valist);
}

void LogDebugWithData(const boost::any& inData, const char* inFormat, ...)
{
#ifdef DEBUG
	va_list valist;
	va_start(valist, inFormat);
	Log(FOUR_CHAR_ID('d', 'e', 'b', 'u'), std::string(""), 0, inData, inFormat, valist); 
	va_end(valist);
#endif
}*/
