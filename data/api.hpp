/// @author Владимир Керимов

#pragma once

#if !defined(WIN32) || defined(DOZEN_STATIC)
#define DOZEN_API 
#elif defined(DOZEN_EXPORTS)
#define DOZEN_API __declspec(dllexport)
#else
#define DOZEN_API __declspec(dllimport)
#endif
