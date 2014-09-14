#pragma once

#include <boost/noncopyable.hpp>

class IProcess : boost::noncopyable
{
public:
    virtual void VUpdate() = 0;
};