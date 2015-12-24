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

#include<string>
#include<sstream>
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
namespace std
{
	template <typename T> std::string to_string(const T& n){
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}
#endif
