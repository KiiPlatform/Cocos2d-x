//
//  PStaff.cpp
//  SimpleGame
//
//  Created by 椋梨 健太郎 on 14/01/08.
//
//

#include "PName.h"
#include "StartScene.h"



PName* PName::layer()
{
    CCLOG("PName::layer");
    
    PName *layer = PName::create();
    return layer;
}

bool PName::init()
{
    CCLOG("PStaff::init");
    
    // super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    this->schedule( schedule_selector(PName::update) );
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    //
    //center
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint center = ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);

    //

     CCRect rect = CCRectMake(0,0,visibleSize.width, visibleSize.height);
     float h = rect.size.height/2;
     float w = rect.size.height/2;
     CCPoint rect1[4];
     rect1[0] = ccp(-w, -h);
     rect1[1] = ccp(-w,  h);
     rect1[2] = ccp( w,  h);
     rect1[3] = ccp( w, -h);
     const ccColor4B ccGRAY4 ={166,166,166, 192};
     CCDrawNode* node_ = CCDrawNode::create();
     node_->setPosition( CCPointMake( rect.getMidX(), rect.getMidY() ) );
     node_->drawPolygon(rect1, 4, ccc4FFromccc4B(ccGRAY4), 1, ccc4FFromccc4B(ccGRAY4));
     this->addChild(node_);

    //
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCSize editBoxSize = CCSizeMake(200, 50);
    CCEditBox* pEditBox = CCEditBox::create(editBoxSize,
                                            CCScale9Sprite::create("sprite9.png"));
    pEditBox->setPosition(ccp(size.width/2, size.height/2));
    pEditBox->setFontColor(ccBLUE);
    pEditBox->setPlaceHolder("Name:");
    pEditBox->setMaxLength(8);
    pEditBox->setReturnType(kKeyboardReturnTypeDone);
    pEditBox->setDelegate(this);
    this->addChild(pEditBox);
    
    //ボタン
    CCMenuItemImage *button_name = CCMenuItemImage::create(
                                                        "bt_start.png",
                                                        "bt_start.png",
                                                        this,
                                                        menu_selector(PName::bt_nameCallback));	//ranking
    button_name->setPosition(ccp( center.x , center.y  ) );


    //メニューの登録
    CCMenu* menu_p =CCMenu::create(button_name, NULL); //ボタンを追加
    menu_p->setPosition(CCPointZero);
    this->addChild(menu_p);
    
    return true;
}

bool PName::update()
{
    //CCLOG("LayerColWindow::start");
    
    return true;
}

//ボタン


bool PName::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCLOG("PStaff::ccTouchBegan");
    
    return true;
}

void PName::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCLOG("PStaff::ccTouchMoved");
}

void PName::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCLOG("PStaff::ccTouchEnded");
    /***
    this->setTouchEnabled(false);
    this->setVisible(false);
    
    _slayer->setButtonEnabled(true);
     ***/
}

void PName::display(){
    CCLOG("PStaff::display");
    
    this->setTouchEnabled(true);
    this->setVisible(true);
    
    _slayer->setButtonEnabled(false);
    
}

void PName::bt_nameCallback(CCObject* pSender)
{
	CCLOG("StartLayer::bt_nameCallback");

    this->setTouchEnabled(false);
    this->setVisible(false);
    
    _slayer->setButtonEnabled(true);
}
