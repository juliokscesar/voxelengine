#pragma once

#include <griffinLog/griffinLog.hpp>

void _checkGLError(const char* file, int line, const std::string& msg);
#define GLCHECK(msg) _checkGLError(__FILE__, __LINE__, msg)
