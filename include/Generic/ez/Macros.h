#pragma once

#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define UNUSED(x) (void)x

#define THROW_EXCEPTION_PARAMS(STREAM, DATE, TIME, FILENAME, FUNCTION, LINE)                                           \
  {                                                                                                                    \
    std::ostringstream oss;                                                                                            \
    oss << "[" << DATE << " - " << TIME << " - " << FILENAME << "::" << FUNCTION << "::" << LINE                       \
        << "]: EXCEPTION: " << STREAM;                                                                                 \
    throw std::runtime_error(oss.str());                                                                               \
  }

#define THROW_EXCEPTION(STREAM) THROW_EXCEPTION_PARAMS(STREAM, __DATE__, __TIME__, __FILENAME__, __FUNCTION__, __LINE__)

#define PRINT(EXPR) std::cout << std::boolalpha << EXPR << std::endl;
#define PEEK(EXPR) std::cout << std::boolalpha << #EXPR << ": " << EXPR << std::endl;

#define _CONCAT_(x,y) x ## y
#define CONCAT(x,y) _CONCAT_(x,y)
#define ANONYMOUS_VARIABLE_NAME() CONCAT(_anonymous_variable, __LINE__)

#define EXPECTS(EXPR) assert(EXPR)
#define ENSURES(EXPR) assert(EXPR)