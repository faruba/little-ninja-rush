/*
 * =====================================================================================
 *
 *       Filename:  patch.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  24/12/15 12:18:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef ___PATCH_H_
#define ___PATCH_H_

#include<string>
#include<sstream>
#include<stdarg.h>  // For va_start, etc.
#include<memory>    // For std::unique_ptr

namespace std
{
  #if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	template <typename T> std::string to_string(const T& n){
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
  #endif
  
  std::string string_format(const std::string fmt_str, ...);
}
#define MUTABLE_ARRAY_FOREACH(__array__, __object__)                                                                         \
if ((__array__).size() > 0)                                                                     \
for(Ref** __arr__ = (__array__)->data->arr, **__end__ = (__array__)->data->arr + (__array__)->data->num-1;    \
__arr__ <= __end__ && (((__object__) = *__arr__) != NULL/* || true*/);                                             \
__arr__++)

#endif
