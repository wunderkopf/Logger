/*
 *  File:		main.cpp
 *  Version:	1.0
 *
 *  Created by Oleksandr Kurylenko on 4/9/14.
 *
 *  Copyright (c) 2014 Wunderkopf. All rights reserved.
 *
 */
#include "Precompile.hpp"
#include "Logger.hpp"

class CCoutSink : public Logging::CSink
{
public:
	CCoutSink()
	{
	}
    
	virtual ~CCoutSink()
	{
	}
    
	virtual void Write(const std::string& inText/*, const boost::any& inData*/)
	{
        std::cout << inText << std::endl;
	}
};

void CreateLog()
{
	Logging::CLogger& logger = Logging::CLogger::getInstance();
    logger.Reset();
    
	std::shared_ptr<Logging::CFormat> warningFormat(new Logging::CFormat(FOUR_CHAR_ID('w', 'a', 'r', 'n')));
	*warningFormat.get() << new Logging::CTime() << new Logging::CText(", ") << new Logging::CText("[WARNING] - ") << new Logging::CTrace();
    
	std::shared_ptr<Logging::CFormat> errorFormat(new Logging::CFormat(FOUR_CHAR_ID('e', 'r', 'r', 'o')));
	*errorFormat.get() << new Logging::CTime() << new Logging::CText(", ") << new Logging::CText("[ERROR]   - ") << new Logging::CTrace();
    
    
	std::shared_ptr<Logging::CFormat> infoFormat(new Logging::CFormat(FOUR_CHAR_ID('i', 'n', 'f', 'o')));
	*infoFormat.get() << new Logging::CTime() << new Logging::CText(", ") << new Logging::CText("[INFO]    - ") << new Logging::CTrace();
    
#ifdef DEBUG
	std::shared_ptr<Logging::CFormat> debugFormat(new Logging::CFormat(FOUR_CHAR_ID('d', 'e', 'b', 'u')));
	*debugFormat.get() << new Logging::CTime() << new Logging::CText(", ") << new Logging::CText("[DEBUG]   - ") << new Logging::CTrace();
#endif
    
	std::shared_ptr<CCoutSink> sink(new CCoutSink());
    //std::shared_ptr<CFileSink> fileSink(new CFileSink());
    
	logger.AddLogger(warningFormat, sink);
	logger.AddLogger(errorFormat, sink);
	logger.AddLogger(infoFormat, sink);
	
	//logger.AddLogger(warningFormat, fileSink);
	//logger.AddLogger(errorFormat, fileSink);
	//logger.AddLogger(infoFormat, fileSink);
    
#ifdef DEBUG
	logger.AddLogger(debugFormat, sink);
#endif
}

int main(int argc, const char * argv[])
{
    CreateLog();
    
    LogWarning("Warning message example with number = %d", 1);
    LogError("Error message example with number = %d", 2);
    LogInfo("Info message example with number = %d", 3);
    LogDebug("Debug message example with number = %d", 4);
    
    return 0;
}

