//
//  UserSetting.h
//  little-ninja-rush
//
//  Created by tringame on 16/1/20.
//
//

#ifndef __little_ninja_rush__UserSetting__
#define __little_ninja_rush__UserSetting__

#include <stdio.h>

class UserSetting{
public:
  static  UserSetting* instance(){
    if(_self == NULL){
      _self = new UserSetting();
    }
    return _self;
  }
  
  template<typename T>
  T getData(const std::string& key){
    return (T)data[key];
  }
  void setData(const std::string& key, float value){
    data[key] = value;
  }
private:
  std::map<std::string, float> data;
  static UserSetting* _self;
};
#endif /* defined(__little_ninja_rush__UserSetting__) */
