#ifndef ___CLASSES_PUBLICLOAD_H_
#define ___CLASSES_PUBLICLOAD_H_
#include "GTLoadList.h"

class PublicLoad:
  public Ref
{
  public:
    static GTLoadList* commonLoadingList();
    static GTLoadList* menuLoadingList();
    static GTLoadList* gameLoadingList();

    static GTLoadList* menuTitle();
    static GTLoadList* menuCollection();
    static GTLoadList* menuSelect();
    static GTLoadList* menuShop();
    static GTLoadList* menuExtra();
};
#endif
