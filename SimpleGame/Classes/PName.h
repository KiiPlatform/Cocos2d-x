//
//  PStaff.h
//  SimpleGame
//
//  Created by 椋梨 健太郎 on 14/01/08.
//
//

#ifndef SimpleGame_PStaff_h
#define SimpleGame_PStaff_h

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

class StartLayer;

class PName :public CCLayer
{
public:
    
    virtual bool init();
    virtual bool update();
    
    static PName* layer();
    CREATE_FUNC(PName);
    bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void bt_nameCallback(cocos2d::CCObject* pSender);
    
    void display();
    
    
    StartLayer *_slayer;

};

#endif
