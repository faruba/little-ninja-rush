//
//  CollectionMenuDelegate.cpp
//  little-ninja-rush
//
//  Created by 郭坤 on 15/12/18.
//
//

#include "CollectionMenuDelegate.hpp"


cocos2d::Sprite *CollectionMenuDelegate::mEquipedMark;
cocos2d::Sprite *CollectionMenuDelegate::mCurrentMark;
ABScrollContent *CollectionMenuDelegate::mScroll;
cocos2d::Sprite *CollectionMenuDelegate::mItemTitle;
cocos2d::Label  *CollectionMenuDelegate::mItemDescription;
cocos2d::Label  *CollectionMenuDelegate::mScrollCount;
cocos2d::MenuItemImage *CollectionMenuDelegate::mUse;

void CollectionMenuDelegate::activate(bool flag) {
  updateButtonImage(flag);
  if (flag) {
    updateScroll();
    updateItemInfo();
  }
}

void  CollectionMenuDelegate::onUse () {
  if( mCurrItem != mEquipedItem && mCurrItem>=0 ) {
    int uiid = fetchData(mCurrItem).uiid;
    bool collected = isCollected(mCurrItem);

    if ( collected ) {
      markUsing(mCurrItem);
      //保存数据
      equipItem();
      GameTool::PlaySound("sound/equip.mp3");

      mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
      mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));

      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_CHANGEEQUIPMENT, 1);
    } else {
      //如果没有解锁就使用万能卷轴
      if ( GameRecord::sharedGameRecord()->collection->magic_piece > 0 ) {
        GameRecord::sharedGameRecord()->collection->magic_piece--;
        GameRecord::sharedGameRecord()->collection->gainItemPiece(uiid);
        //update info
        cocos2d::Sprite *mask = (cocos2d::Sprite*)(mScroll->contentNode->getChildByTag(mCurrItem));

        if( GameRecord::sharedGameRecord()->collection->isItemCompleted(uiid) ) {
          GameTool::PlaySound("sound/getscroll.mp3");
          mScroll->contentNode->removeChild(mask);
          mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equip1.png"));
          mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equip2.png"));
        } else {
          GameTool::PlaySound("sound/getitem.mp3");
          int piece = GameRecord::sharedGameRecord()->collection->itemTotalPiece(uiid) - GameRecord::sharedGameRecord()->collection->itemLostPiece(uiid);
          cocos2d::CCString *filename = cocos2d::CCString::createWithFormat("sc_sp%d.png", piece);
          mask->setDisplayFrame(GameTool::getSpriteFrameByName(filename->getCString()));
          //bugfix
          if( GameRecord::sharedGameRecord()->collection->magic_piece <= 0 )
          {
            mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock2.png"));
            mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock2.png"));
          }
        }
        mScrollCount->setString(cocos2d::CCString::createWithFormat("x%d", GameRecord::sharedGameRecord()->collection->magic_piece)->getCString());
      } else {
        GameTool::PlaySound("sound/error.mp3");
      }
    }
  }
}

void CollectionMenuDelegate::updateItemInfo() {
  int uid = mCurrItem;
  if ( mCurrItem < 0 ) {
    uid = mEquipedItem;
  }
  const GameItemBase &item = fetchData(uid);
  mItemTitle->setDisplayFrame(cocos2d::Sprite::create(item.name.c_str())->displayFrame());
  mItemDescription->setString(item.desc.c_str());
}

void CollectionMenuDelegate::updateButtonImage(bool isDisabled) {
  mMenuButtonRef->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName(mImageFilename + (isDisabled?"1.png":"2.png")));
  mMenuButtonRef->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName(mImageFilename + (isDisabled?"2.png":"1.png")));
}

void CollectionMenuDelegate::markUsing(int i) {
  if( mEquipedItem < 0 )
  {
    mEquipedMark->setVisible(true);
  }
  if( i >= 0 )
  {
    cocos2d::Sprite *item = (cocos2d::Sprite*)(mScroll->contentNode->getChildByTag(i));
    mEquipedMark->setPosition(item->getPosition());
  }
  else {
    mEquipedMark->setVisible(false);
  }
  mEquipedItem = i;
}

void CollectionMenuDelegate::updateUseButtonInfo(int i) {
  bool equiped =  (i == mEquipedItem);
  bool collected = isCollected(mCurrItem);
  if(!equiped) {
    if ( !collected ) {
      if ( GameRecord::sharedGameRecord()->collection->magic_piece > 0 ) {
        mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock1.png"));
        mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock2.png"));
      } else {
        mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock2.png"));
        mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock2.png"));
      }
      toggleShare(false);
    } else {
      mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equip1.png"));
      mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equip2.png"));
      toggleShare(false);
    }
  } else {
    mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
    mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
    toggleShare(false);
  }
}

void CollectionMenuDelegate::markCurrent(int i) {
  if( mCurrItem < 0 ) {
    mCurrentMark->setVisible(true);
  }

  if ( i >= 0 ) {
    cocos2d::Sprite *item = (cocos2d::Sprite*)(mScroll->contentNode->getChildByTag(i));
    mCurrentMark->setPosition(item->getPosition());
  } else {
    mCurrentMark->setVisible(false);
  }

  mCurrItem = i;
  //获取当前道具的信息
  updateUseButtonInfo(i);
}
