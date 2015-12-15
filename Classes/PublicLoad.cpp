#include "PublicLoad.h"
#include "AnimationRes.h"
#include "GameRecord.h"

GTLoadList* PublicLoad::commonLoadingList() 
{
    GTLoadList *list = GTLoadList::create();
    list->addSpriteSheet("common.plist");;
    list->addSfx("sound/menu-change.mp3");;
    list->addSfx("sound/menu-start.mp3");;
    list->addSfx("sound/logo.mp3");;
    list->addSfx("sound/open.mp3");;
    list->addSfx("sound/close.mp3");;
    list->addSfx("sound/click.mp3");;
    list->addSfx("sound/error.mp3");;
    return list;
}

GTLoadList* PublicLoad::menuLoadingList() 
{
    GTLoadList *list = GTLoadList::create();
    list->addMusic("sound/menu.mp3");;
    return list;
}

GTLoadList* PublicLoad::menuTitle() 
{
    GTLoadList *list = GTLoadList::create();
    list->addSpriteSheet("menu-title.plist");;
    list->addSpriteSheet("menu-title2.plist");;
    list->addSpriteSheet("enemy1.plist");;
    list->addLoadFunc(assetEnemy);;
    list->addLoadFunc(assetMninja);;
    list->addLoadFunc(assetHninja);;
    list->addSfx("sound/getcoin.mp3");;
    list->addSfx("sound/flip.mp3");;
    return list;
}

GTLoadList* PublicLoad::menuCollection() 
{
    GTLoadList *list = GTLoadList::create();
    list->addSpriteSheet("items.plist");;
    list->addSpriteSheet("menu-collection.plist");;
    list->addSpriteSheet("menu-effect.plist");;
    list->addSpriteSheet("protrait.plist");;
    list->addSpriteSheet("menu-extra.plist");;
    list->addLoadFunc(assetUI);;
    list->addSfx("sound/equip.mp3");;
    list->addSfx("sound/getitem.mp3");;
    list->addSfx("sound/getscroll.mp3");;
    list->addSfx("sound/charge.mp3");;
    return list;
}

GTLoadList* PublicLoad::menuSelect() 
{
    GTLoadList *list = GTLoadList::create();
    list->addSpriteSheet("zchar1.plist");;
    list->addSpriteSheet("zchar2.plist");;
    list->addSpriteSheet("zchar3.plist");;
    list->addSpriteSheet("zchar4.plist");;
    list->addSpriteSheet("items.plist");;
    list->addSpriteSheet("menu-effect.plist");;
    list->addSpriteSheet("menu-select.plist");;
    list->addSpriteSheet("protrait.plist");;
    list->addLoadFunc(assetMainRole);;
    list->addLoadFunc(assetMainGirl);;
    list->addLoadFunc(assetMainMusashi);;
    list->addLoadFunc(assetMainPussy);;
    list->addLoadFunc(assetUI);;
    list->addSfx("sound/flip.mp3");;
    return list;
}

GTLoadList* PublicLoad::menuShop() 
{
    GTLoadList *list = GTLoadList::create();
    list->addSpriteSheet("items.plist");;
    list->addSpriteSheet("menu-shop.plist");;
    list->addSpriteSheet("menu-extra.plist");;
    list->addSfx("sound/getscroll.mp3");
    return list;
}

GTLoadList* PublicLoad::menuExtra() 
{
    return PublicLoad::menuShop();
}

GTLoadList* PublicLoad::gameLoadingList() 
{
    GTLoadList *list = GTLoadList::create();
    //-------- 有条件地载入一部分资源 ----------
    int rid = GameRecord::sharedGameRecord()->curr_char;
    int dart = GameRecord::sharedGameRecord()->char_equip_dart[rid];
    int blade = GameRecord::sharedGameRecord()->char_equip_blade[rid];
    int special = GameRecord::sharedGameRecord()->char_equip_spell[rid];
    //主角
    {
        CCString *filename = CCString::createWithFormat("zchar%d.plist", rid+1);
        list->addSpriteSheet(filename->getCString());
    }
    //刀刃
    {
        CCString *filename = CCString::createWithFormat("zblade%d.plist", blade+1);
        list->addSpriteSheet(filename->getCString());
        if( blade != 0 )
        {//hack: always load the first sprite frame
            list->addSpriteSheet("zblade1.plist");;
        }
    }
    //特殊
    {
        if( dart == 6 )
        {
            list->addSpriteSheet("zsp2.plist");;
        }
        if( dart == 7 )
        {
            list->addSpriteSheet("zsp3.plist");;
        }
        if( dart == 9 )
        {
            list->addSpriteSheet("zsp4.plist");;
        }
        if( special == 7 )
        {
            list->addSpriteSheet("zsp1.plist");;
        }
    }
    //---------------------------------------
    list->addSpriteSheet("ui.plist");;
    list->addSpriteSheet("ui2.plist");;
    list->addSpriteSheet("effect.plist");;
    list->addSpriteSheet("effect2.plist");;
    list->addSpriteSheet("enemy1.plist");;
    list->addSpriteSheet("enemy2.plist");;
    list->addLoadFunc(assetEnemy);;
    list->addLoadFunc(assetMninja);;
    list->addLoadFunc(assetHninja);;
    list->addLoadFunc(assetSamurai);;
    list->addLoadFunc(assetMerchant);;
    list->addLoadFunc(assetMechanic);;
    list->addLoadFunc(assetPumpkin);;
    list->addLoadFunc(assetSanta);;
    list->addLoadFunc(assetEffect);;
    list->addLoadFunc(assetBlade);;
    list->addLoadFunc(assetMsg);;
    list->addLoadFunc(assetBullets);;
    list->addLoadFunc(assetArcher);;
    list->addLoadFunc(assetItem);;
    list->addLoadFunc(assetMisc);;
    list->addLoadFunc(assetMainRole);;
    list->addLoadFunc(assetMainGirl);;
    list->addLoadFunc(assetMainMusashi);;
    list->addLoadFunc(assetMainPussy);;
    list->addSfx("sound/duofa.mp3");;
    list->addSfx("sound/zhenkong.mp3");;
    list->addSfx("sound/throw.mp3");;
    list->addSfx("sound/hit.mp3");;
    list->addSfx("sound/ahh1.mp3");;
    list->addSfx("sound/ahh2.mp3");;
    list->addSfx("sound/ahh3.mp3");;
    list->addSfx("sound/reflect.mp3");;
    list->addSfx("sound/blade.mp3");;
    list->addSfx("sound/samurai_hit.mp3");;
    list->addSfx("sound/samurai_die.mp3");;
    list->addSfx("sound/blade_ready.mp3");;
    list->addMusic("sound/music-menu.mp3");;
    list->addSfx("sound/feverbgm.mp3");;
    list->addSfx("sound/feverbgmend.mp3");;
    list->addSfx("sound/fever.mp3");;
    list->addSfx("sound/combo1.mp3");;
    list->addSfx("sound/combo2.mp3");;
    list->addSfx("sound/combo3.mp3");;
    list->addSfx("sound/combo4.mp3");;
    list->addSfx("sound/combo5.mp3");;
    list->addSfx("sound/combo6.mp3");;
    list->addSfx("sound/combo7.mp3");;
    list->addSfx("sound/combo8.mp3");;
    list->addSfx("sound/combo9.mp3");;
    list->addSfx("sound/fever1.mp3");;
    list->addSfx("sound/fever2.mp3");;
    list->addSfx("sound/fever3.mp3");;
    list->addSfx("sound/fever4.mp3");;
    list->addSfx("sound/fever5.mp3");;
    list->addSfx("sound/getcoin.mp3");;
    list->addSfx("sound/bomb1.mp3");;
    list->addSfx("sound/bomb2.mp3");;
    list->addSfx("sound/freeze_fly.mp3");;
    list->addSfx("sound/freeze_up.mp3");;
    list->addSfx("sound/freeze_break.mp3");;
    list->addSfx("sound/samurai_warning.mp3");;
    list->addSfx("sound/samurai_attack.mp3");;
    list->addSfx("sound/laugh1.mp3");;
    list->addSfx("sound/laugh2.mp3");;
    list->addSfx("sound/buy.mp3");;
    list->addSfx("sound/lock on.mp3");;
    list->addSfx("sound/shoot ready.mp3");;
    list->addSfx("sound/shooting.mp3");;
    list->addSfx("sound/ice-blade.mp3");;
    list->addSfx("sound/ice-hurt.mp3");;
    list->addSfx("sound/starfall.mp3");;
    list->addSfx("sound/hit-01.mp3");;
    list->addSfx("sound/hit-02.mp3");;
    list->addSfx("sound/die-0.mp3");;
    list->addSfx("sound/hit-21.mp3");;
    list->addSfx("sound/hit-22.mp3");;
    list->addSfx("sound/die-2.mp3");;
    list->addSfx("sound/hit-11.mp3");;
    list->addSfx("sound/hit-12.mp3");;
    list->addSfx("sound/die-1.mp3");;
    list->addSfx("sound/hit-31.mp3");;
    list->addSfx("sound/hit-32.mp3");;
    list->addSfx("sound/die-3.mp3");;
    list->addSfx("sound/getitem.mp3");;
    list->addSfx("sound/getscroll.mp3");;
    list->addSfx("sound/blade-clay.mp3");;
    list->addSfx("sound/blade-fire.mp3");;
    list->addSfx("sound/blade-light.mp3");;
    list->addSfx("sound/blade-thunder.mp3");;
    list->addSfx("sound/hit-fire.mp3");;
    list->addSfx("sound/hit-thunder.mp3");;
    list->addSfx("sound/openscroll.mp3");;
    list->addSfx("sound/shuriken-laser.mp3");;
    list->addSfx("sound/smoke.mp3");;
    list->addSfx("sound/spell.mp3");;
    list->addSfx("sound/ui-cancel.mp3");;
    list->addSfx("sound/objective-complete.mp3");;
    list->addSfx("sound/getcrown.mp3");;
    list->addSfx("sound/flip.mp3");;
    list->addSfx("sound/moonblade.mp3");;
    list->addSfx("sound/moonblade2.mp3");;
    list->addSfx("sound/charge.mp3");;
    list->addSfx("sound/bloodfangs.mp3");;
    list->addSfx("sound/golden.mp3");;
    list->addSfx("sound/rainbow.mp3");;
    list->addSfx("sound/totsuka.mp3");;
    list->addSfx("sound/dash.mp3");;
    list->addSfx("sound/pumpkinstart.mp3");;
    list->addSfx("sound/pumpkinfly.mp3");;
    list->addSfx("sound/pumpkinhurt.mp3");;
    list->addSfx("sound/pumpkindie.mp3");;
    return list;
}


