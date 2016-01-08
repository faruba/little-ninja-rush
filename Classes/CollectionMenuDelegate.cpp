//
//  CollectionMenuDelegate.cpp
//  little-ninja-rush
//
//  Created by 郭坤 on 15/12/18.
//
//

#include "CollectionMenuDelegate.hpp"
#include "CollectionMenu.h"


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
      GameTool::PlaySound("equip.mp3");

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
          GameTool::PlaySound("getscroll.mp3");
          mScroll->contentNode->removeChild(mask);
          mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equip1.png"));
          mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equip2.png"));
        } else {
          GameTool::PlaySound("getitem.mp3");
          int piece = GameRecord::sharedGameRecord()->collection->itemTotalPiece(uiid) - GameRecord::sharedGameRecord()->collection->itemLostPiece(uiid);
          std::string filename = std::string_format("sc_sp%d.png", piece);
          mask->setDisplayFrame(GameTool::getSpriteFrameByName(filename.c_str()));
          //bugfix
          if( GameRecord::sharedGameRecord()->collection->magic_piece <= 0 )
          {
            mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock2.png"));
            mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock2.png"));
          }
        }
        mScrollCount->setString(std::string_format("x%d", GameRecord::sharedGameRecord()->collection->magic_piece).c_str());
      } else {
        GameTool::PlaySound("error.mp3");
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

void PowerUpCollectionDelegate::init(CollectionMenu* collectionMenu) {
  mImageFilename = "sc_powerup";
  updateButtonImage(false);
  mCollectionMenu = collectionMenu;

}

void PowerUpCollectionDelegate::onUse() {
  int roleid = GameRecord::sharedGameRecord()->curr_char;
  bool enable = true;
  SEL_CallFunc selector = NULL;
  if( mCollectionMenu->mCurrItem == 1 )
  {
    selector = CC_CALLFUNC_SELECTOR(PowerUpCollectionDelegate::onUseLife);
    if( GameData::roleCurrHP(roleid) >= GameData::roleMaxHP(roleid) || GameRecord::sharedGameRecord()->collection->life_piece < 9 )
    {
      enable = false;
    }
  }
  else {
    selector = callfunc_selector(PowerUpCollectionDelegate::onUseDart);
    if( GameData::roleCurrDart(roleid) >= GameData::roleMaxDart(roleid) || GameRecord::sharedGameRecord()->collection->dart_piece < 9 )
    {
      enable = false;
    }
  }
  const char *desc = NULL;
  
  switch (roleid) {
    case 0:
    {
      desc = "xr_kt.png";
    }
      break;
    case 1:
    {
      desc = "xr_sr.png";
    }
      break;
    case 2:
    {
      desc = "xr_ms.png";
    }
      break;
    case 3:
    {
      desc = "xr_mr.png";
    }
      break;
  }
  if( enable )
  {
    GameTool::PlaySound("click.mp3");
    mCollectionMenu->setModal("pu-tc1.png", desc, this, selector);
  }
  else {
    GameTool::PlaySound("error.mp3");
  }
}
void PowerUpCollectionDelegate::onUseLife()
{
  int role = GameRecord::sharedGameRecord()->curr_char;
  if( GameData::roleCurrHP(role) < GameData::roleMaxHP(role) )
  {
    GameRecord::sharedGameRecord()->collection->life_piece -= 9;
    GameRecord::sharedGameRecord()->setCharacterHp(GameRecord::sharedGameRecord()->char_hp[role]+1, role);
    mCollectionMenu->updateCharacterInfo(role, 1);
    int lc = GameRecord::sharedGameRecord()->collection->life_piece/9;
    mLifeCount->setString(std::string_format("%d", lc).c_str());
    if( GameData::roleCurrHP(role) >= GameData::roleMaxHP(role) )
    {
      mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
      mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
    }
  }
}

void PowerUpCollectionDelegate::onUseDart()
{
  int role = GameRecord::sharedGameRecord()->curr_char;
  if( GameData::roleCurrDart(role) < GameData::roleMaxDart(role) )
  {
    GameRecord::sharedGameRecord()->collection->dart_piece -= 9;
    GameRecord::sharedGameRecord()->setCharacterDart(GameRecord::sharedGameRecord()->char_dart[role]+1, role);
    mCollectionMenu->updateCharacterInfo(role, 2);
    int lc = GameRecord::sharedGameRecord()->collection->dart_piece/9;
    mDartCount->setString(std::string_format("%d", lc).c_str());
    if( GameData::roleCurrDart(role) >= GameData::roleMaxDart(role) )
    {
      mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
      mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
    }
  }
}
