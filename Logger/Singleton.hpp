/*
 *  File:		Singleton.hpp
 *  Version:	http://enki-tech.blogspot.com/2012/08/c11-generic-singleton.html
 *
 *  Created by Oleksandr Kurylenko on 4/9/14.
 *
 *  Copyright (c) 2014 Wunderkopf. All rights reserved.
 *
 */
#pragma once

// Use own implementation if needed
template <class T>
class CSingleton
{
public:
    template <typename... Args>
    static T& getInstance(Args... args)
    {
        if (!mInstance)
            mInstance = new T(std::forward<Args>(args)...);
        
        return *mInstance;
    }
    
    static void deleteInstance()
    {
        delete mInstance;
        mInstance = nullptr;
    }
    
private:
    static T* mInstance;
};

template <class T> T*  CSingleton<T>::mInstance = nullptr;
