#pragma once

#include <spdlog/spdlog.h>

#ifndef NDEBUG

#define LOG_INFO(...) spdlog::info(__VA_ARGS__)
#define LOG_WARN(...) spdlog::warn(__VA_ARGS__)
#define LOG_CRIT(...) spdlog::critical(__VA_ARGS__)
#define LOG_ERR(...) spdlog::error(__VA_ARGS__)

#else

#define LOG_INFO(...) 
#define LOG_WARN(...) 
#define LOG_CRIT(...) 
#define LOG_ERR(...) 

#endif