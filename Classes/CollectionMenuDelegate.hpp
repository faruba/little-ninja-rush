//
//  CollectionMenuDelegate.hpp
//  little-ninja-rush
//
//  Created by 郭坤 on 15/12/18.
//
//

#ifndef CollectionMenuDelegate_hpp
#define CollectionMenuDelegate_hpp
#include "ABScrollContent.h"
#include "GameRecord.h"
#include "GameData.h"

class CollectionMenuDelegate {
public:
    cocos2d::MenuItemImage *mMenuButtonRef = nullptr;
    static ABScrollContent *mScroll;
    static cocos2d::MenuItemImage *mUse;
    static cocos2d::Sprite *mItemTitle;
    static cocos2d::Label *mItemDescription;
    static cocos2d::Label *mScrollCount;
    
    virtual const GameItemBase& fetchData(int index) { throw "Nothing to get"; }
    virtual void updateUsing() { markUsing(-1); }
    virtual void equipItem() { }
    virtual void  onUse () ;
    virtual void updateItemInfo() ;
    virtual void updateScroll() = 0;

    void updateButtonImage(bool isDisabled) ;
    void activate(bool flag) ;
    void markCurrent(int i) ;
    
protected:
    std::string mImageFilename;
    
    virtual void init() = 0;
    
    static cocos2d::Sprite *mEquipedMark;
    static cocos2d::Sprite *mCurrentMark;
    int mItemCount;
    int mEquipedItem;
    int mCurrItem;
    
    void toggleShare(bool flag) { }
    void markUsing(int i) ;
    
    virtual bool isCollected(int id) { return false; }
    virtual void updateUseButtonInfo(int i) ;
    
public:
    template<typename ItemType>
    void updateList(std::vector<ItemType> &vector) {
        //clean nodes
        mScroll->contentNode->removeAllChildrenWithCleanup(true);
        
        int cc = GameRecord::sharedGameRecord()->curr_char;
        
        //create equiped mark
        mEquipedMark = cocos2d::Sprite::createWithSpriteFrameName("sc_equiped.png");
        mEquipedMark->setVisible(false);
        mScroll->contentNode->addChild(mEquipedMark, 5);
        mCurrentMark = cocos2d::Sprite::createWithSpriteFrameName("sc_xz.png");
        mCurrentMark->setVisible(false);
        mScroll->contentNode->addChild(mCurrentMark, 4);
        
        int count = 0;
        int i = 0;
        cocos2d::CCSize contentSize;//update contentSize
        cocos2d::Ref *obj;
        
        for(typename std::vector<ItemType>::iterator itr = vector.begin(); itr != vector.end(); itr++) {
            ItemType item = *itr;
            //获取对应飞镖玩家的收集数据
            int piece = GameRecord::sharedGameRecord()->collection->pieces[item.uiid];
            bool completed = GameRecord::sharedGameRecord()->collection->isItemCompleted(item.uiid);
            
            //TODO: Bug when select charactor
            if( i < GAME_CHARCOUNT ) {
                if( i != cc ) {
                    ++i;
                    continue;
                }
            }
            
            cocos2d::Sprite *itemSprite = cocos2d::Sprite::createWithSpriteFrameName("sc_fbbg2.png");
            int x = count%4;
            int y = count/4;
            itemSprite->setPosition(cocos2d::Vec2(42 + 79*x, -29.5 - 59*y));
            mScroll->contentNode->addChild(itemSprite, 1, count);
            
            //extend contentSize
            {
                float cw = itemSprite->getPosition().x + itemSprite->getContentSize().width/2;
                float ch = -itemSprite->getPosition().y + itemSprite->getContentSize().height/2;
                if( contentSize.width < cw )
                {
                    contentSize.width = cw;
                }
                if( contentSize.height < ch )
                {
                    contentSize.height = ch;
                }
            }
            
            //ITEM
            cocos2d::Sprite *icon = NULL;
            if( piece == 0 && !completed )
            {
                icon = cocos2d::Sprite::createWithSpriteFrameName("sc_fbbg.png");
            }
            else{
                icon = cocos2d::Sprite::create(item.icon.c_str());
            }
            icon->setPosition(itemSprite->getPosition());
            mScroll->contentNode->addChild(icon, 2);
            
            //MASK
            if( piece > 0 && !completed )
            {
                cocos2d::CCString *filename = cocos2d::CCString::createWithFormat("sc_sp%d.png", piece);
                cocos2d::Sprite *mask = cocos2d::Sprite::createWithSpriteFrameName(filename->getCString());
                mask->setPosition(itemSprite->getPosition());
                mScroll->contentNode->addChild(mask, 3, count);
            }
            
            //NEW?
            if( GameRecord::sharedGameRecord()->collection->isContainedInNewList(item.uiid) )
            {
                cocos2d::Sprite *newsp = cocos2d::Sprite::createWithSpriteFrameName("sc_new.png");
                newsp->setPosition(ccpAdd(itemSprite->getPosition(), Vec2(33, 24)));
                mScroll->contentNode->addChild(newsp, 6, 100+count);
            }
            count++;
            ++i;
        }
        
        mItemCount = count;
        mScroll->setContentSize(contentSize);//apply contentSize
        
        //当前配置
        int eq = GameRecord::sharedGameRecord()->char_equip_dart[cc];
        if( eq < GAME_CHARCOUNT )
        {
            eq = 0;
        }
        else {
            eq -= GAME_CHARCOUNT-1;
        }
        mEquipedItem = -1;
        mCurrItem = -1;
        markUsing(eq);
        markCurrent(-1);
    }
};

class ShurikenCollectionDelegate: public CollectionMenuDelegate {
  public:
    void init() {
      mImageFilename = "sc_shuriken";
      updateButtonImage(false);
    }

    bool isCollected(int id) {
      Shuriken &sh = GameData::fetchShurikens()[id];
      return GameRecord::sharedGameRecord()->collection->isItemCompleted(sh.uiid);
    }

    void updateScroll() { this->updateList(GameData::fetchShurikens()); }

    void updateUsing() {
      int nrole = GameRecord::sharedGameRecord()->curr_char;
      markUsing(GameRecord::sharedGameRecord()->char_equip_dart[nrole]);
    }

    virtual const GameItemBase& fetchData(int index) { return GameData::fetchShurikens()[index]; }

    void equipItem() {
      int cc = GameRecord::sharedGameRecord()->curr_char;
      GameRecord::sharedGameRecord()->char_equip_dart[cc] = mEquipedItem;
    }
};

class KatanaCollectionDelegate: public CollectionMenuDelegate {
  public:
    void init() {
      mImageFilename = "sc_katana";
      updateButtonImage(false);
    }
    void updateScroll() { this->updateList(GameData::fetchKatanas()); }

    bool isCollected(int id) {
      int index = id + GAME_CHARCOUNT - 1;
      if( id == 0 ) {
        index = GameRecord::sharedGameRecord()->curr_char;
      }
      Katana &sh = GameData::fetchKatanas()[id];
      return GameRecord::sharedGameRecord()->collection->isItemCompleted(sh.uiid);
    }

    void updateUsing() {
      int nrole = GameRecord::sharedGameRecord()->curr_char;
      int index = GameRecord::sharedGameRecord()->char_equip_blade[nrole];
      if( index < GAME_CHARCOUNT )
      {
        index = 0;
      }
      else {
        index = index - GAME_CHARCOUNT + 1;
      }
      markUsing(index);
    }

    virtual const GameItemBase& fetchData(int index) { return GameData::fetchKatanas()[index]; }

    void equipItem() {
      int cc = GameRecord::sharedGameRecord()->curr_char;
      int index = mEquipedItem + GAME_CHARCOUNT - 1;
      if( mEquipedItem == 0 )
      {
        index = GameRecord::sharedGameRecord()->curr_char;
      }
      GameRecord::sharedGameRecord()->char_equip_blade[cc] = index;
    }
};

class SpecialCollectionDelegate: public CollectionMenuDelegate {
  public:
    void init() {
      mImageFilename = "sc_special";
      updateButtonImage(false);
    }

    bool isCollected(int id)
    {
      Special &sh = GameData::fetchSpecials()[id];
      return GameRecord::sharedGameRecord()->collection->isItemCompleted(sh.uiid);
    }

    void updateScroll() { this->updateList(GameData::fetchSpecials()); }

    void updateUsing() {
      int nrole = GameRecord::sharedGameRecord()->curr_char;
      markUsing(GameRecord::sharedGameRecord()->char_equip_spell[nrole]);
    }

    virtual const GameItemBase& fetchData(int index) { return GameData::fetchSpecials()[index]; }

    void equipItem() {
      int cc = GameRecord::sharedGameRecord()->curr_char;
      GameRecord::sharedGameRecord()->char_equip_spell[cc] = mEquipedItem;
    }
};

class PowerUpCollectionDelegate: public CollectionMenuDelegate {
  private:
    cocos2d::Sprite *mLifeMask;
    cocos2d::Sprite *mDartMask;
    cocos2d::Label *mLifeCount;
    cocos2d::Label *mDartCount;
  public:
    void init() {
      mImageFilename = "sc_powerup";
      updateButtonImage(false);
    }
    void updateScroll() {
      mScroll->contentNode->removeAllChildren();

      mMenuButtonRef->removeChildByTag(999, false);
      mCurrentMark = cocos2d::Sprite::createWithSpriteFrameName("sc_xz.png");
      mCurrentMark->setVisible(false);
      mMenuButtonRef->addChild(mCurrentMark, 1, 999);
      mUse->setVisible(false);

      //update mask
      //TODO:Refactor this
      cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
      int lm = GameRecord::sharedGameRecord()->collection->life_piece%9;
      int lc = GameRecord::sharedGameRecord()->collection->life_piece/9;
      cocos2d::CCString *lfn = cocos2d::CCString::createWithFormat("sc_sp%d.png", lm);
      mLifeMask->setDisplayFrame(cache->spriteFrameByName(lfn->getCString()));
      mLifeCount->setString(cocos2d::CCString::createWithFormat("%d", lc)->getCString());

      int dm = GameRecord::sharedGameRecord()->collection->dart_piece%9;
      int dc = GameRecord::sharedGameRecord()->collection->dart_piece/9;
      cocos2d::CCString *dfn = cocos2d::CCString::createWithFormat("sc_sp%d.png", dm);
      mDartMask->setDisplayFrame(cache->spriteFrameByName(dfn->getCString()));
      mDartCount->setString(cocos2d::CCString::createWithFormat("%d", dc)->getCString());
      mCurrItem = 0;

      //TODO:updateCharacterInfo(GameRecord::sharedGameRecord()->curr_char, 0);
    }

    void onUse() {
      /*
      int roleid = GameRecord::sharedGameRecord()->curr_char;
      bool enable = true;
      SEL_CallFunc selector = NULL;
      if( mCurrItem == 1 )
      {
        selector = callfunc_selector(CollectionMenu::onUseLife);
        if( GameData::roleCurrHP(roleid) >= GameData::roleMaxHP(roleid) || GameRecord::sharedGameRecord()->collection->life_piece < 9 )
        {
          enable = false;
        }
      }
      else {
        selector = callfunc_selector(CollectionMenu::onUseDart);
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
        GameTool::PlaySound("sound/click.mp3");
        setModal("pu-tc1.png", desc, this, selector);
      }
      else {
        GameTool::PlaySound("sound/error.mp3");
      }
      */
    }

/* TODO:Refactor this two: onUseLife(), onUseDart
void onUseLife()
{
  int role = GameRecord::sharedGameRecord()->curr_char;
  if( GameData::roleCurrHP(role) < GameData::roleMaxHP(role) )
  {
    GameRecord::sharedGameRecord()->collection->life_piece -= 9;
    GameRecord::sharedGameRecord()->setCharacterHp(GameRecord::sharedGameRecord()->char_hp[role]+1, role);
    updateCharacterInfo(role, 1);
    int lc = GameRecord::sharedGameRecord()->collection->life_piece/9;
    mLifeCount->setString(cocos2d::CCString::createWithFormat("%d", lc)->getCString());
    if( GameData::roleCurrHP(role) >= GameData::roleMaxHP(role) )
    {
      mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
      mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
    }
  }
}

void onUseDart()
{
  int role = GameRecord::sharedGameRecord()->curr_char;
  if( GameData::roleCurrDart(role) < GameData::roleMaxDart(role) )
  {
    GameRecord::sharedGameRecord()->collection->dart_piece -= 9;
    GameRecord::sharedGameRecord()->setCharacterDart(GameRecord::sharedGameRecord()->char_dart[role]+1, role);
    updateCharacterInfo(role, 2);
    int lc = GameRecord::sharedGameRecord()->collection->dart_piece/9;
    mDartCount->setString(cocos2d::CCString::createWithFormat("%d", lc)->getCString());
    if( GameData::roleCurrDart(role) >= GameData::roleMaxDart(role) )
    {
      mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
      mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
    }
  }
} 
*/
};
#endif /* CollectionMenuDelegate_hpp */
