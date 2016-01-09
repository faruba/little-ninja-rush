#include "CollectionMenu.h"
#include "GameTool.h"
#include "GameRecord.h"
#include "GameConstants.h"
#include "TitleMenu.h"
#include "SelectMenu.h"
#include "GameData.h"
#include "UniversalFit.h"
#include "CEClipedNode.h"
#include "PublicLoad.h"
#include "SimpleAudioEngine.h"
#include "GTAnimatedEffect.h"
#include "GamePlay.h"
//#include "MidBridge.h"

using namespace CocosDenshion;
static int gNavBack = 0;

void CollectionMenu::setNavBack(int nid) {
	gNavBack = nid;
}

void CollectionMenu::setModal(const char * title, const char * desc, Ref* target, SEL_CallFunc sel)
{
	if( mModal )
		return ;
	mModal = true;
	mMask->setVisible(true);
	mModalTarget = target;
	mModalSel = sel;
	mMask->removeAllChildrenWithCleanup(true);
	//mItems->setTouchEnabled(false;//1.2.0)
	mTouchEnabled = false;
	mMenu->setTouchEnabled(false);
	mPowerupMenu->setTouchEnabled(false);
	cocos2d::Sprite *spbg = cocos2d::Sprite::createWithSpriteFrameName("sp-tc.png");
	spbg->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	mMask->addChild(spbg);
	cocos2d::Sprite *titlesp = cocos2d::Sprite::createWithSpriteFrameName(title);
	titlesp->setPosition(cocos2d::Vec2( spbg->getContentSize().width/2, 105));
	spbg->addChild(titlesp);
	cocos2d::Sprite *descsp = cocos2d::Sprite::createWithSpriteFrameName(desc);
	descsp->setPosition(cocos2d::Vec2( spbg->getContentSize().width/2, 75));
	spbg->addChild(descsp);
	cocos2d::MenuItemSprite *menuConfirm = cocos2d::MenuItemSprite::create(
			cocos2d::Sprite::createWithSpriteFrameName("pu-confirm1.png"),
			cocos2d::Sprite::createWithSpriteFrameName("pu-confirm2.png"),
			this, CC_MENU_SELECTOR(CollectionMenu::onModalConfirm));
	menuConfirm->setPosition(cocos2d::Vec2(75, 35));
	cocos2d::MenuItemSprite *menuCancel = cocos2d::MenuItemSprite::create(
			cocos2d::Sprite::createWithSpriteFrameName("pu-cancel1.png"),
			cocos2d::Sprite::createWithSpriteFrameName("pu-cancel2.png"),
			this, menu_selector(CollectionMenu::onModalCancel));

	menuCancel->setPosition(cocos2d::Vec2(200, 35));
	cocos2d::CCMenu *menu = cocos2d::CCMenu::create(menuConfirm, menuCancel, NULL);
	menu->setPosition(cocos2d::Vec2(0, 0));
	spbg->addChild(menu);
}

void CollectionMenu::onModalConfirm(cocos2d::Ref*)
{
	mModal = false;
	mMask->setVisible(false);
	//mItems->setTouchEnabled(true;//1.2.0)
	mTouchEnabled = true;
	mMenu->setTouchEnabled(true);
	mPowerupMenu->setTouchEnabled(true);
	this->runAction(cocos2d::CallFunc::create(mModalTarget, mModalSel));
	GameTool::PlaySound("charge.mp3");
}

void CollectionMenu::onModalCancel(cocos2d::Ref*)
{
	mModal = false;
	mMask->setVisible(false);
	//mItems->setTouchEnabled(true;//1.2.0)
	mTouchEnabled = true;
	mMenu->setTouchEnabled(true);
	mPowerupMenu->setTouchEnabled(true);
}

void CollectionMenu::onEnter() {
	mUISwapper.onEnter();

	PublicLoad::menuCollection()->loadAll();
	mModal = false;

	cocos2d::Node *taskcomplete = cocos2d::Node::create();
	taskcomplete->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT));
	this->addChild(taskcomplete, 5);
	GamePlay::sharedGamePlay()->setTaskCompleteNode(taskcomplete);

	cocos2d::Node * node = createUIByCCBI("menu-collection", "CollectionMenu", CollectionMenuLayerLoader::loader(), this);
	if(node != NULL) {
		this->addChild(node, 5);
	}

	//1.2.0 SinaWeibo
	if( UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo() )
	{
		mTwitter->setDisabledSpriteFrame(GameTool::getSpriteFrameByName("go_weibo.png"));
		mTwitter->setNormalSpriteFrame(GameTool::getSpriteFrameByName("go_weibo.png"));
		mTwitter->setSelectedSpriteFrame(GameTool::getSpriteFrameByName("go_weibo.png"));
		mTwitterAction->setDisplayFrame(GameTool::getSpriteFrameByName("go_weibo2.png"));
	}

	mScroll = ABScrollContent::create();
	cocos2d::CCRect clip = cocos2d::Rect(mScrollPoint->getPosition().x, mScrollPoint->getPosition().y, 322, 177.5f);
	mScroll->clickTarget = this;
	mScroll->clickMethod = callfunc_selector(CollectionMenu::clickMethod);
	mScroll->setClipRect(clip);
	mScrollPoint->addChild(mScroll, -1);

	CollectionMenuDelegate::mScroll = mScroll;
	CollectionMenuDelegate::mItemTitle = mItemTitle;
	CollectionMenuDelegate::mItemDescription = mItemDesc;
	CollectionMenuDelegate::mUse = mUse;

	mShadowDir = ccpNormalize(cocos2d::Vec2(-12, 17));
	this->scheduleUpdate();
	this->setTouchEnabled(true);

	mMask = cocos2d::LayerColor::create(Color4B(0, 0, 0, 128));
	mMask->setOpacity(128);
	mMask->setVisible(false);
	this->addChild(mMask, 10);

	//初始化人物按钮
	int currRole = GameRecord::sharedGameRecord()->curr_char;
	for(int i=0; i<GAME_CHARCOUNT; ++i) {
		cocos2d::MenuItemImage *btn = character(i);
		btn->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName(std::string_format("sc_role%d%d.png", i, i==currRole?0:1).c_str()));
		btn->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName(std::string_format("sc_role%d%d.png", i, i==currRole?1:0).c_str()));
	}

	mShurikenDelegate.init(this);
	mKatanaDelegate.init(this);
	mSpecialDelegate.init(this);
	mPowerUpDelegate.init(this);

	switch (gNavBack) {
		case 2:  mCurrentDelegate = &mKatanaDelegate;   break;
		case 3:  mCurrentDelegate = &mSpecialDelegate;  break;
		default: mCurrentDelegate = &mShurikenDelegate; break;
	}
	mCurrentDelegate->activate(true);

	mScrollCount->setString(std::string_format("x%d", GameRecord::sharedGameRecord()->collection->magic_piece).c_str());

	mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
	mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));

	mUISwapper.setSceneIntro(this);

	GameTool::PlayBackgroundMusic("menu.mp3");

	mPostingFacebook = false;
	mPostingTwitter = false;


	//MidBridge::sharedMidbridge()->setFacebookCallback(this, menu_selector(CollectionMenu::doneFacebook));
	//MidBridge::sharedMidbridge()->setTwitterCallback(this, menu_selector(CollectionMenu::doneTwitter:));
	//toggleShare(true);
	cocos2d::Layer::onEnter();
}

void CollectionMenu::onExit()
{
	GameRecord::sharedGameRecord()->collection->cleanNewList();
	GamePlay::sharedGamePlay()->setTaskCompleteNode(NULL);
	PublicLoad::menuCollection()->unloadAll();

	//MidBridge::sharedMidbridge()->setFacebookCallback(NULL, menu_selector(CollectionMenu::doneFacebook));
	//MidBridge::sharedMidbridge()->setTwitterCallback(NULL, menu_selector(CollectionMenu::doneTwitter));
	cocos2d::Layer::onExit();
}

cocos2d::MenuItemImage* CollectionMenu::character(int rid)
{
	switch (rid) {
		case 0:
			return mCharacter1;
		case 1:
			return mCharacter2;
		case 2:
			return mCharacter3;
		case 3:
			return mCharacter4;
	}
	return NULL;
}

void CollectionMenu::update(float delta)
{
	//update shadow
	cocos2d::Point np = ccpAdd(mShadow->getPosition(), ccpMult(mShadowDir, delta*7));
	if( np.x < -10 )
	{
		np.x = -10;
		mShadowDir.x *= -1;
	}
	if( np.x > 0 )
	{
		np.x = 0;
		mShadowDir.x *= -1;
	}
	if( np.y < 320 )
	{
		np.y = 320;
		mShadowDir.y *= -1;
	}
	if( np.y > 330 )
	{
		np.y = 330;
		mShadowDir.y *= -1;
	}
	mShadow->setPosition(np);
}
void CollectionMenu::onBack(cocos2d::Ref*)
{
	if(mUISwapper.isDone())
	{
		GameTool::PlaySound("menu-change.mp3");
		if( gNavBack == 0 ) {
			mUISwapper.setSceneOutro(GameTool::scene<TitleMenu>(), this);
		} else {
			mUISwapper.setSceneOutro(GameTool::scene<SelectMenu>(), this);
		}
	}
}

void CollectionMenu::onUse(cocos2d::Ref*) {
	mCurrentDelegate->onUse();
}

void CollectionMenu::onCharacter(int nRole)
{
	int orole = GameRecord::sharedGameRecord()->curr_char;
	if( orole == nRole )
		return;
	GameTool::PlaySound("click.mp3");
	cocos2d::MenuItemImage *orl = character(orole);
	orl->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName(std::string_format("sc_role%d%d.png", orole, 1).c_str()));
	orl->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName(std::string_format("sc_role%d%d.png", orole, 0).c_str()));

	int nrole = nRole;
	cocos2d::MenuItemImage *nr = character(nrole);
	nr->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName(std::string_format("sc_role%d%d.png", nrole, 0).c_str()));
	nr->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName(std::string_format("sc_role%d%d.png", nrole, 1).c_str()));
	GameRecord::sharedGameRecord()->curr_char = nrole;

	if( mCurrentDelegate != &mPowerUpDelegate )
	{
		mCurrentDelegate->markCurrent(-1);
		mCurrentDelegate->updateUsing();
		mCurrentDelegate->updateItemInfo();
	} else {
		updateCharacterInfo(nRole, mCurrItem);
		updatePowerUpButton();
	}

	mCurrentDelegate->updateScroll();
}



void CollectionMenu::onItem(CollectionMenuDelegate *newDelegate)
{
	if( newDelegate == mCurrentDelegate )
		return ;

	GameTool::PlaySound("click.mp3");
	mCurrentDelegate->activate(false);
	newDelegate->activate(true);
	mCurrentDelegate = newDelegate;

	mItemDesc->setVisible(true);

	mPowerUp->setVisible(false);
	mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
	mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
	mUse->setVisible(true);
	mScroll->resetContentPosition();

	toggleShare(true);
}

void CollectionMenu::onPowerup(cocos2d::Ref*)
{
	CollectionMenuDelegate *newDelegate = &mPowerUpDelegate;
	if( newDelegate == mCurrentDelegate )
		return ;

	GameTool::PlaySound("click.mp3");
	newDelegate->updateButtonImage(true);
	mCurrentDelegate->updateButtonImage(false);


	mCurrentDelegate = newDelegate;

	mCurrentDelegate->updateScroll();
	updateCharacterInfo(GameRecord::sharedGameRecord()->curr_char, 0);

	mPowerUp->setVisible(true);
	mItemDesc->setVisible(false);

	this->onSelectLife(nullptr);

	toggleShare(false);
}

void CollectionMenu::updateCharacterInfo(int rid, int bid)
{
	mLifeGuage->removeAllChildrenWithCleanup(false);
	for( int i=0; i<GameData::roleMaxHP(rid); ++i)
	{
		if( i<GameData::roleCurrHP(rid) )
		{
			cocos2d::Sprite *sp = cocos2d::Sprite::createWithSpriteFrameName("heart.png");
			sp->setPosition(cocos2d::Vec2(i*18, 0));
			mLifeGuage->addChild(sp, 0, i);
		}
		else {
			if( bid == 1 && i == GameData::roleCurrHP(rid) )
			{
				GTAnimatedEffect *sp = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("ui"), 0, true);
				sp->setPosition(cocos2d::Vec2(i*18, 0));
				mLifeGuage->addChild(sp);
			}
			else {
				cocos2d::Sprite *sp = cocos2d::Sprite::createWithSpriteFrameName("pulife2.png");
				sp->setPosition(cocos2d::Vec2(i*18, 0));
				mLifeGuage->addChild(sp);
			}
		}
	}

	mDartGuage->removeAllChildrenWithCleanup(false);
	for( int i=0; i<GameData::roleMaxDart(rid); ++i)
	{
		if( i<GameData::roleCurrDart(rid) )
		{
			cocos2d::Sprite *sp = cocos2d::Sprite::createWithSpriteFrameName("ui-dart.png");
			sp->setPosition(cocos2d::Vec2(i*12, 0));
			mDartGuage->addChild(sp);
		}
		else {
			if( bid == 2 && i == GameData::roleCurrDart(rid) )
			{
				GTAnimatedEffect *sp = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("ui"), 1, true);
				sp->setPosition(cocos2d::Vec2(i*12, 0));
				mDartGuage->addChild(sp);
			}
			else {
				cocos2d::Sprite *sp = cocos2d::Sprite::createWithSpriteFrameName("pushuriken2.png");
				sp->setPosition(cocos2d::Vec2(i*12, 0));
				mDartGuage->addChild(sp);
			}
		}
	}
}

void CollectionMenu::onItemCallback(int i)
{
	//获取当前道具的信息
	bool collected = false;
	auto data = mCurrentDelegate->fetchData(i);
	if(! mCurrentDelegate->isCollected(i) && GameRecord::sharedGameRecord()->collection->itemLostPiece(data.uiid) == GameRecord::sharedGameRecord()->collection->itemTotalPiece(data.uiid) ){
		GameTool::PlaySound("error.mp3");
		return;
	}
	GameTool::PlaySound("click.mp3");
	mCurrentDelegate->markCurrent(i);
	mCurrentDelegate->updateItemInfo();
}

void CollectionMenu::updatePowerUpButton()
{
	bool flag = false;
	int rid = GameRecord::sharedGameRecord()->curr_char;
	if( mCurrItem == 1 )
	{
		if( GameRecord::sharedGameRecord()->collection->life_piece >= 9 &&
				GameData::roleCurrHP(rid) < GameData::roleMaxHP(rid) )
		{
			flag = true;
		}
		mUse->setVisible(true);
	}
	else if( mCurrItem == 2 )
	{
		if( GameRecord::sharedGameRecord()->collection->dart_piece >= 9 &&
				GameData::roleCurrDart(rid) < GameData::roleMaxDart(rid) )
		{
			flag = true;
		}
		mUse->setVisible(true);
	}
	else {
		mUse->setVisible(false);
	}
	if( flag )
	{
		mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use1.png"));
		mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
	}
	else {
		mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
		mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
	}
}

void CollectionMenu::onSelectLife(cocos2d::Ref*)
{
	if( mCurrItem != 1 )
	{
		GameTool::PlaySound("click.mp3");
		CollectionMenuDelegate::mCurrentMark->setPosition(cocos2d::Vec2(385, 145));
		CollectionMenuDelegate::mCurrentMark->setVisible(true);
		mCurrItem = 1;
		updateCharacterInfo(GameRecord::sharedGameRecord()->curr_char, mCurrItem);

		//buttons & title
		mItemTitle->setDisplayFrame(cocos2d::Sprite::create("lifeplus.png")->displayFrame());
		this->updatePowerUpButton();
	}
}

void CollectionMenu::onSelectDart(cocos2d::Ref*)
{
	if( mCurrItem != 2 )
	{
		GameTool::PlaySound("click.mp3");
		CollectionMenuDelegate::mCurrentMark->setPosition(cocos2d::Vec2(385, 65));
		CollectionMenuDelegate::mCurrentMark->setVisible(true);
		mCurrItem = 2;
		updateCharacterInfo(GameRecord::sharedGameRecord()->curr_char, mCurrItem);

		//buttons & title
		mItemTitle->setDisplayFrame(cocos2d::Sprite::create("dartplus.png")->displayFrame());
		this->updatePowerUpButton();
	}
}

void CollectionMenu::clickMethod()
{
	cocos2d::Point click = mScroll->clickPoint;
	int x = click.x / 79;
	int y = click.y / 59;
	int cid = x + y*4;
	if( cid < mCurrentDelegate->getItemCount() && cid >= 0 )
	{
		onItemCallback(cid);
	}
}

void CollectionMenu::toggleShare(bool flag) { }

void CollectionMenu::onFacebook(cocos2d::Ref*) { }

void CollectionMenu::doneFacebook(cocos2d::CCInteger* res) { }

void CollectionMenu::onTwitter(cocos2d::Ref*) { }

void CollectionMenu::doneTwitter(cocos2d::CCInteger* res) { }

bool CollectionMenu::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShadow",         Sprite*       , mShadow)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mScrollCount",    Label*  , mScrollCount)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mItemDesc",       Label*  , mItemDesc)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPowerUp",        Node*         , mPowerUp)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLifeGuage",      Node*         , mLifeGuage)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDartGuage",      Node*         , mDartGuage)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLifeMask",       Sprite*       , mPowerUpDelegate.mLifeMask)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDartMask",       Sprite*       , mPowerUpDelegate.mDartMask)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPowerupMenu",    CCMenu*         , mPowerupMenu)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLifeCount",      Label*  , mPowerUpDelegate.mLifeCount)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDartCount",      Label*  , mPowerUpDelegate.mDartCount)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mItemTitle",      Sprite*       , mItemTitle)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShare",          Sprite*       , mShare)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMenu",           CCMenu*         , mMenu)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCharacter1",     MenuItemImage*, mCharacter1)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCharacter2",     MenuItemImage*, mCharacter2)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCharacter3",     MenuItemImage*, mCharacter3)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCharacter4",     MenuItemImage*, mCharacter4)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShuriken",       MenuItemImage*, mShurikenDelegate.mMenuButtonRef)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mKatana",         MenuItemImage*, mKatanaDelegate.mMenuButtonRef)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSpecial",        MenuItemImage*, mSpecialDelegate.mMenuButtonRef)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPowerup",        MenuItemImage*, mPowerUpDelegate.mMenuButtonRef)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mUse",            MenuItemImage*, mUse)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFacebook",       MenuItemImage*, mFacebook)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitter",        MenuItemImage*, mTwitter)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitterAction",  Sprite*       , mTwitterAction)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFacebookAction", Sprite*       , mFacebookAction)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mScrollPoint",    Node*         , mScrollPoint)

		return false;
}

SEL_MenuHandler CollectionMenu::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSelectLife", CollectionMenu::onSelectLife)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSelectDart", CollectionMenu::onSelectDart)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", CollectionMenu::onBack)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCharacter1", CollectionMenu::onCharacter1)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCharacter2", CollectionMenu::onCharacter2)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCharacter3", CollectionMenu::onCharacter3)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCharacter4", CollectionMenu::onCharacter4)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onShuriken", CollectionMenu::onShuriken)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onKatana", CollectionMenu::onKatana)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSpecial", CollectionMenu::onSpecial)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPowerup", CollectionMenu::onPowerup)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onUse", CollectionMenu::onUse)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onFacebook", CollectionMenu::onFacebook)
		CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTwitter", CollectionMenu::onTwitter)
		return NULL;
}

cocos2d::extension::Control::Handler   CollectionMenu::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
	return NULL;
}
