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
//#include "extensions/assets-manager/AssetsManager.h"
#include "extensions/cocos-ext.h"
//#include "cocos-ext.h"

USING_NS_CC_EXT;
USING_NS_CC;

class StartLayer;

class PName :public CCLayer, public EditBoxDelegate
{
public:
    
    virtual bool init();
	void update(float dt);
    static PName* layer();
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label, Label);
    
    CREATE_FUNC(PName);
    //bool ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    //void ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
/***
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
***/

    void editBoxEditingDidBegin(EditBox* editBox);
    void editBoxEditingDidEnd(EditBox* editBox);
    void editBoxTextChanged(EditBox* editBox, const std::string& text);
    void editBoxReturn(EditBox* editBox);
    
    virtual void bt_nameCallback(cocos2d::CCObject* pSender);
    
    void display();
    
    
    StartLayer *_slayer;

};

#endif
