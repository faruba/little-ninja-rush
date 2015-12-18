#ifndef ___CLASSES_COLLECTIONMENU_H_
#define ___CLASSES_COLLECTIONMENU_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ABScrollContent.h"
#include "GameRecord.h"
#include "GameData.h"

USING_NS_CC_EXT;
enum ButtonEnum {
  TypeShuriken,
  TypeKatana,
  TypeSpecial,
  TypePowerUp
};

class CollectionMenu : 
  public Layer, 
  public cocosbuilder::CCBMemberVariableAssigner,
  public cocosbuilder::CCBSelectorResolver
{
  public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CollectionMenu, create);
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::Node*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * , const char* );

    virtual bool init(){return true;};
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float delta);

    void onUseLife();
    void onUseDart();
    void updateShurikens() { this->updateList(GameData::fetchShurikens()); }
    void updateKatanas() { this->updateList(GameData::fetchKatanas()); }
    void updateSpecials() { this->updateList(GameData::fetchSpecials()); }
    void updatePowerups();
    void updateUsing();
    void updateItemInfo();
    void updatePowerUpButton();

    //-- auto callback --
    void onBack(cocos2d::Ref*);
    void onUse(cocos2d::Ref*);
    void onCharacter(int nRole);
    void onCharacter1(cocos2d::Ref*){onCharacter(0);};
    void onCharacter2(cocos2d::Ref*){onCharacter(1);};
    void onCharacter3(cocos2d::Ref*){onCharacter(2);};
    void onCharacter4(cocos2d::Ref*){onCharacter(3);};
    void onItem(ButtonEnum nItem);
    void onShuriken(cocos2d::Ref*){onItem(TypeShuriken);};
    void onKatana(cocos2d::Ref*){onItem(TypeKatana);};
    void onSpecial(cocos2d::Ref*){onItem(TypeSpecial);};
    void onPowerup(cocos2d::Ref*);

    void onFacebook(cocos2d::Ref*);
    void onTwitter(cocos2d::Ref*);

    void onSelectLife(cocos2d::Ref*);
    void onSelectDart(cocos2d::Ref*);

    void clickMethod();


    cocos2d::Point mShadowDir;
    int mEquipedItem;
    int mCurrItem;
    int mItemCount;
cocos2d::Sprite *mEquipedMark;
cocos2d::Sprite *mCurrentMark;
    //Node *mItemIcons;
    ABScrollContent *mScroll;

    bool mTouchEnabled;

    bool mPostingFacebook;
    bool mPostingTwitter;
    //-- auto assign --
cocos2d::Sprite *mShadow;
cocos2d::Sprite *mItemTitle;
cocos2d::Label *mItemDesc;
cocos2d::Label *mScrollCount;
cocos2d::CCMenuItemImage *mCharacter1;
cocos2d::CCMenuItemImage *mCharacter2;
cocos2d::CCMenuItemImage *mCharacter3;
cocos2d::CCMenuItemImage *mCharacter4;
cocos2d::CCMenuItemImage *mShuriken;
cocos2d::CCMenuItemImage *mKatana;
cocos2d::CCMenuItemImage *mSpecial;
cocos2d::CCMenuItemImage *mPowerup;
cocos2d::CCMenuItemImage *mUse;
cocos2d::CCMenu *mMenu;
    //CCMenu *mItems;
cocos2d::Node *mPowerUp;
cocos2d::Node *mLifeGuage;
cocos2d::Node *mDartGuage;
cocos2d::Sprite *mLifeMask;
cocos2d::Sprite *mDartMask;
cocos2d::Label *mLifeCount;
cocos2d::Label *mDartCount;
cocos2d::LayerColor *mMask;
cocos2d::CCMenu *mPowerupMenu;
cocos2d::CCMenuItemImage *mFacebook;
cocos2d::CCMenuItemImage *mTwitter;
cocos2d::Sprite *mShare;
cocos2d::Sprite *mFacebookAction;
cocos2d::Sprite *mTwitterAction;
cocos2d::Node *mScrollPoint;

    bool mModal;
cocos2d::Ref *mModalTarget;
    SEL_CallFunc mModalSel;

    void setModal(const char* title, const char* desc, Ref* target, SEL_CallFunc sel);
    void onModalConfirm(cocos2d::Ref*);
    void onModalCancel(cocos2d::Ref*);


    // 0-mainmenu 1-selectmenu
    static void setNavBack(int nid);
    void markUsing(int i);
    void markCurrent(int i);
cocos2d::CCMenuItemImage* character(int rid);

    //bid = 0 :no blink 1:blink hp 2:blink dart
    void updateCharacterInfo(int rid, int bid);

    void doneFacebook(cocos2d::CCInteger* res);
    void doneTwitter(cocos2d::CCInteger* res);

    void toggleShare(bool flag);
    void onItemCallback(int i);

  private:
    UISwapper mUISwapper;
    ButtonEnum mCurrentType; //TODO:Refactor this

    void setTypeButton(ButtonEnum type, bool isDisabled);

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

        //BG
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
class CollectionMenuLayerLoader : public cocosbuilder::NodeLoader {
  public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CollectionMenuLayerLoader, loader);

  protected:
CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CollectionMenu);
};
#endif
