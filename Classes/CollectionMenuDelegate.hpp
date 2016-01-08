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

class CollectionMenu;
class CollectionMenuDelegate :public cocos2d::Ref{
public:
    cocos2d::MenuItemImage *mMenuButtonRef = nullptr;
    static ABScrollContent *mScroll;
    static cocos2d::MenuItemImage *mUse;
    static cocos2d::Sprite *mItemTitle;
    static cocos2d::Label *mItemDescription;
    static cocos2d::Label *mScrollCount;
    static cocos2d::Sprite *mEquipedMark;
    static cocos2d::Sprite *mCurrentMark;
    
    virtual const GameItemBase& fetchData(int index) { throw "Nothing to get"; }
    virtual void updateUsing() { markUsing(-1); }
    virtual void equipItem() { }
  virtual int getCurrentEquipment(){ return 0;}
    virtual void  onUse () ;
    virtual void updateItemInfo() ;
    virtual void updateScroll() = 0;

    void updateButtonImage(bool isDisabled) ;
    void activate(bool flag) ;
    void markCurrent(int i) ;
  int getItemCount(){return mItemCount;}
  virtual bool isCollected(int id) { return false; }
    
protected:
    std::string mImageFilename;
  std::string backgroundImageFileName;
  
    virtual void init(CollectionMenu* collectionMenu) = 0;
    
    int mItemCount;
    int mEquipedItem;
    int mCurrItem;
    
    void toggleShare(bool flag) { }
    void markUsing(int i) ;
    
    virtual void updateUseButtonInfo(int i) ;
    virtual bool isMineItem(int itemIdx, int currentCharater) { return true;}
    
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
        
        for(typename std::vector<ItemType>::iterator iterator = vector.begin(); iterator != vector.end(); iterator++) {
            ItemType &item = *iterator;
          //CCLOG("name %s, pic %s id %d", item.name.c_str(), item.icon.c_str(),item.uiid);
            //获取对应飞镖玩家的收集数据
            int piece = GameRecord::sharedGameRecord()->collection->pieces[item.uiid];
            bool completed = GameRecord::sharedGameRecord()->collection->isItemCompleted(item.uiid);
            
          if( !isMineItem(i,cc)){
            ++i;
            continue;
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
                icon = cocos2d::Sprite::createWithSpriteFrameName(backgroundImageFileName);
            }
            else{
                icon = cocos2d::Sprite::create(item.icon.c_str());
            }
            icon->setPosition(itemSprite->getPosition());
            mScroll->contentNode->addChild(icon, 2);
            
            //MASK
          //CCLOG("%s %s, %s [%d, %d] (%d) <%d>", item.titl.c_str(),  item.name.c_str(), item.icon.c_str() , x, y , piece, completed);
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
        int eq = getCurrentEquipment();
        mEquipedItem = -1;
        mCurrItem = -1;
        markUsing(eq);
        markCurrent(-1);
    }
};

class ShurikenCollectionDelegate: public CollectionMenuDelegate {
  public:
    void init(CollectionMenu* collectionMenu) {
      mImageFilename = "sc_shuriken";
      backgroundImageFileName = "sc_fbbg.png";
      updateButtonImage(false);
    }

    bool isCollected(int id) {
      if(id == -1){
        return true;
      }
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
  virtual int getCurrentEquipment(){
      int cc = GameRecord::sharedGameRecord()->curr_char;
      return GameRecord::sharedGameRecord()->char_equip_dart[cc];
  }
};

class KatanaCollectionDelegate: public CollectionMenuDelegate {
  public:
    void init(CollectionMenu* collectionMenu) {
      mImageFilename = "sc_katana";
      backgroundImageFileName = "sc_ktnbg.png";
      updateButtonImage(false);
    }
    void updateScroll() { this->updateList(GameData::fetchKatanas()); }

    bool isCollected(int id) {
      if( id == -1){ return true;}
      int index = id + GAME_CHARCOUNT - 1;
      if( id == 0 ) {
        index = GameRecord::sharedGameRecord()->curr_char;
      }
      auto &sh = fetchData(id); //GameData::fetchKatanas()[id];
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

    virtual const GameItemBase& fetchData(int index) { return GameData::fetchKatanas()[GAME_CHARCOUNT - 1 + index]; }

    void equipItem() {
      int cc = GameRecord::sharedGameRecord()->curr_char;
      int index = mEquipedItem + GAME_CHARCOUNT - 1;
      if( mEquipedItem == 0 )
      {
        index = GameRecord::sharedGameRecord()->curr_char;
      }
      GameRecord::sharedGameRecord()->char_equip_blade[cc] = index;
    }
  
  virtual int getCurrentEquipment(){
    int cc = GameRecord::sharedGameRecord()->curr_char;
    int eq = GameRecord::sharedGameRecord()->char_equip_blade[cc];
    if( eq < GAME_CHARCOUNT )
    {
      eq = 0;
    }
    else {
      eq -= GAME_CHARCOUNT-1;
    }
    return eq;
  }
protected:
  virtual bool isMineItem(int itemIdx, int currentCharater){
    if( itemIdx < GAME_CHARCOUNT ) {
      return itemIdx == currentCharater ;
    }
    return true;
  }
};

class SpecialCollectionDelegate: public CollectionMenuDelegate {
  public:
    void init(CollectionMenu* collectionMenu) {
      mImageFilename = "sc_special";
      backgroundImageFileName = "sc_jnbg.png";
      updateButtonImage(false);
    }

    bool isCollected(int id)
    {
      //TODO ? what's the meaning of -1?
      if(id == -1 ){
        return true;
      }
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
  
  virtual int getCurrentEquipment(){
    int cc = GameRecord::sharedGameRecord()->curr_char;
    return GameRecord::sharedGameRecord()->char_equip_spell[cc];
  }
};

class PowerUpCollectionDelegate: public CollectionMenuDelegate {
  private:
    CollectionMenu* mCollectionMenu;
  public:
    cocos2d::Sprite *mLifeMask;
    cocos2d::Sprite *mDartMask;
    cocos2d::Label *mLifeCount;
    cocos2d::Label *mDartCount;

    void init(CollectionMenu* collectionMenu);
    void updateScroll() {
      mScroll->contentNode->removeAllChildren();

      mMenuButtonRef->removeChildByTag(999, false);
      mCurrentMark = cocos2d::Sprite::createWithSpriteFrameName("sc_xz.png");
      mCurrentMark->setVisible(false);
      mMenuButtonRef->addChild(mCurrentMark, 1, 999);
      mUse->setVisible(true);

      //update mask
      //TODO:Refactor this
      int lm = GameRecord::sharedGameRecord()->collection->life_piece%9;
      int lc = GameRecord::sharedGameRecord()->collection->life_piece/9;
      cocos2d::CCString *lfn = cocos2d::CCString::createWithFormat("sc_sp%d.png", lm);
      mLifeMask->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(lfn->getCString()));
      mLifeCount->setString(cocos2d::CCString::createWithFormat("%d", lc)->getCString());

      int dm = GameRecord::sharedGameRecord()->collection->dart_piece%9;
      int dc = GameRecord::sharedGameRecord()->collection->dart_piece/9;
      cocos2d::CCString *dfn = cocos2d::CCString::createWithFormat("sc_sp%d.png", dm);
      mDartMask->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(dfn->getCString()));
      mDartCount->setString(cocos2d::CCString::createWithFormat("%d", dc)->getCString());
      mCurrItem = 0;
    }

    void onUse() ;
    void onUseDart();
    void onUseLife();
    };
#endif /* CollectionMenuDelegate_hpp */
