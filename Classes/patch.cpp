/*
 * =====================================================================================
 *
 *       Filename:  patch.cpp
 *
 *    Description:  :
 *
 *        Version:  1.0
 *        Created:  2015/12/30 22时31分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  faruba , 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "patch.h"

namespace std{
  std::string string_format(const std::string fmt_str, ...) {
    int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
      formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
      strcpy(&formatted[0], fmt_str.c_str());
      va_start(ap, fmt_str);
      final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
      va_end(ap);
      if (final_n < 0 || final_n >= n)
        n += abs(final_n - n + 1);
      else
        break;
    }
    return std::string(formatted.get());
  }
}
