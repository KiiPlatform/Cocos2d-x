//
//  PStaff.cpp
//  SimpleGame
//
//  Created by 椋梨 健太郎 on 14/01/08.
//
//

//#include <string.h>

#include "PName.h"
#include "StartScene.h"

extern void jni_ranking_query_all();
extern void jni_ranking_post(const char *name, int score);
extern void jni_save_display_name(const char *name);
extern char kii_display_name[256];

PName* PName::layer()
{
    CCLOG("PName::layer");
    
    PName *layer = PName::create();
    return layer;
}

bool PName::init()
{
    CCLOG("PName::init");
    
    // super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    this->schedule( schedule_selector(PName::update) );
    //setTouchEnabled(true);
    //setTouchMode(kCCTouchesOneByOne);
    
    //
    //center
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint center = ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);

    //
    CCRect rect = CCRectMake(0,0,visibleSize.width, visibleSize.height);
    float h = rect.size.height;
    float w = rect.size.height/2;
    CCPoint rect1[4];
    rect1[0] = ccp(-w, -h);
    rect1[1] = ccp(-w,  h);
    rect1[2] = ccp( w,  h);
    rect1[3] = ccp( w, -h);
    const ccColor4B ccGRAY4 ={192,192,192, 255};
    CCDrawNode* node_ = CCDrawNode::create();
    node_->setPosition( CCPointMake( rect.getMidX(), rect.getMidY() ) );
    node_->drawPolygon(rect1, 4, ccc4FFromccc4B(ccGRAY4), 1, ccc4FFromccc4B(ccGRAY4));
    this->addChild(node_);
    
    //
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    this->_label = CCLabelTTF::create("","Artial", 32);
    _label->setString("Please Input Name");
    _label->retain();
    _label->setColor( ccc3(0, 0, 0) );
    _label->setPosition(ccp( center.x , center.y+110  ) );
    this->addChild(_label);
    //

    CCSize editBoxSize = CCSizeMake(400, 50);
    CCEditBox* pEditBox = CCEditBox::create(editBoxSize, CCScale9Sprite::create("sprite9.png"));
    pEditBox->setPosition(ccp(center.x , center.y));
    pEditBox->setFontColor(ccBLUE);
    pEditBox->setPlaceHolder("Name:");
    pEditBox->setMaxLength(32);
    pEditBox->setReturnType(kKeyboardReturnTypeDone);
    pEditBox->setDelegate(this);    //CCEditBoxDelegateが必要
    this->addChild(pEditBox);
    
    //ボタン
    CCMenuItemImage *button_name = CCMenuItemImage::create(
                                                        "bt_name.png",
                                                        "bt_name.png",
                                                        this,
                                                        menu_selector(PName::bt_nameCallback));	//ranking
    button_name->setPosition(ccp( center.x , center.y-100  ) );


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

void PName::editBoxEditingDidBegin(CCEditBox* editBox) {
    CCLOG("did begin");
}

void PName::editBoxEditingDidEnd(CCEditBox* editBox) {
    CCLOG("did end : %s", editBox->getText());
    char buff[256];
    strcpy( buff, editBox->getText() );
    if(buff[0]!=0){
    	strcpy( kii_display_name, buff );
    	jni_save_display_name(kii_display_name);
    } else {
    	CCLOG("text null");
    }
}

void PName::editBoxTextChanged(CCEditBox* editBox, const std::string& text) {
    CCLOG("change %s", text.c_str());
}

void PName::editBoxReturn(CCEditBox* editBox) {
    CCLOG("return");
}

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
    CCLOG("kii_display_name %s", kii_display_name);

    this->setTouchEnabled(false);
    this->setVisible(false);
    
    _slayer->setButtonEnabled(true);
}
