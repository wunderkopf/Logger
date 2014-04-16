/*
 *  File:		Logger.hpp
 *  Version:	1.0
 *
 *  Created by Oleksandr Kurylenko on 4/9/14.
 *
 *  Copyright (c) 2014 Wunderkopf. All rights reserved.
 *
 */
#pragma once
#include "Singleton.hpp"

#define FOUR_CHAR_ID(c1_, c2_, c3_, c4_)	(std::uint32_t) (   \
((unsigned char)c1_) << 24 | ((unsigned char)c2_) << 16 |	\
((unsigned char)c3_) << 8  | (unsigned char)c4_)

void LogWarning(const char* inFormat, ...);
void LogError(const char* inFormat, ...);
void LogInfo(const char* inFormat, ...);
void LogDebug(const char* inFormat, ...);

//void LogWarningWithData(const boost::any& inData, const char* inFormat, ...);
//void LogErrorWithData(const boost::any& inData, const char* inFormat, ...);
//void LogInfoWithData(const boost::any& inData, const char* inFormat, ...);
//void LogDebugWithData(const boost::any& inData, const char* inFormat, ...);

namespace Logging
{
	struct CLogParam
	{
        std::uint32_t mFormatID;
		std::string mTrace;
		std::string mFileName;
		std::uint32_t mLine;
		//boost::any mData; // use boost any if needed
	};
	
	class CElement
	{
	public:
		CElement();
		virtual ~CElement();
		
		virtual std::string ToString(const CLogParam& inParam) = 0;
	};
	
	class CText : public CElement
	{
	public:
		CText(const std::string& inText);
		virtual ~CText();
		
		virtual std::string ToString(const CLogParam& inParam);
		
	private:
		std::string mText;
	};
	
	class CTime : public CElement
	{
	public:
		CTime();
		virtual ~CTime();
		
		virtual std::string ToString(const CLogParam& inParam);
	};
	
	class CFileName : public CElement
	{
	public:
		CFileName();
		virtual ~CFileName();
		
		virtual std::string ToString(const CLogParam& inParam);
	};
	
	class CEndOFLine : public CElement
	{
	public:
		CEndOFLine();
		virtual ~CEndOFLine();
		
		virtual std::string ToString(const CLogParam& inParam);
	};
	
	class CLine : public CElement
	{
	public:
		CLine();
		virtual ~CLine();
		
		virtual std::string ToString(const CLogParam& inParam);
	};
	
	class CTrace : public CElement
	{
	public:
		CTrace();
		virtual ~CTrace();
		
		virtual std::string ToString(const CLogParam& inParam);
	};
	
	class CFormat
	{
		friend CFormat& operator << (CFormat& outFormat, CElement* inElement);
		friend class CLogger;
		
	public:
		CFormat(const std::uint32_t inFormatID);
		~CFormat();
		inline std::uint32_t GetID() const;
		
	private:
		std::string ToString(const CLogParam& inParam);
		std::vector<std::shared_ptr<CElement> > mFormat;
        std::uint32_t mFormatID;
	};
	
	class CSink
	{
	public:
		CSink();
		virtual ~CSink();
		
		virtual void Write(const std::string& inText/*, const boost::any& inData*/) = 0;
	};
	
	class CLogger :  public CSingleton<CLogger>
	{
	public:
		CLogger();
		
		virtual ~CLogger();
		
		void AddLogger(std::shared_ptr<CFormat> inFormat, std::shared_ptr<CSink> inSink);
		void PrintLog(const CLogParam& inParam);
        void Reset();
		
	private:
		std::vector<std::pair<std::shared_ptr<CFormat>, std::shared_ptr<CSink> > > mLogger;
	};
};
