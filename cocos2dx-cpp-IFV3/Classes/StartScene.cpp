/****************************************************************************
 Copyright (c) 2010-2011 cocos2d-x.org
 Copyright (c) 2010      Ray Wenderlich
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "StartScene.h"
#include "HelloWorldScene.h"
#include "RankingScene.h"

#include <android_native_app_glue.h>

using namespace cocos2d;

extern char kii_name[256];
extern char kii_display_name[256];
extern void jni_ranking_query_all();
extern void jni_ranking_post(const char *name, int score);
extern void jni_save_display_name(const char *name);

bool StartScene::init()
{
	if( Scene::init() )
	{
		this->_layer = StartLayer::create();
		this->_layer->retain();
		this->addChild(_layer);
		
		return true;
	}
	else
	{
		return false;
	}
}

StartScene::~StartScene()
{
	if (_layer)
	{
		_layer->release();
		_layer = NULL;
	}
}


bool StartLayer::init()
{
	if ( LayerColor::initWithColor( Color4B(255,255,255,255) ) )
	{
		CCLOG("StartLayer::init");

		auto winSize = Director::getInstance()->getWinSize();
		this->_label = LabelTTF::create("","Artial", 32);
        _label->setString("TITLE kii sample");
		_label->retain();
		_label->setColor( Color3B(0, 0, 0) );
		_label->setPosition( Point(winSize.width/2, winSize.height/2 + 110 ) );
		this->addChild(_label);

        char buff[256];
		//_label_name
        this->_label_name = LabelTTF::create("","Artial", 10);
        //sprintf(buff,"name:%s",kii_display_name);
        sprintf(buff,"name:%s","name");
		_label_name->setString(buff);   //kii_display_name
		_label_name->retain();
		_label_name->setColor( Color3B(0, 0, 0) );
		_label_name->setPosition( Point(winSize.width/2, winSize.height/2 +60+17) );
		this->addChild(_label_name);

		//_label_dname
        this->_label_dname = LabelTTF::create("","Artial", 24);
        //sprintf(buff,"name:%s",kii_display_name);
        sprintf(buff,"name:%s","dname");
        _label_dname->setString(buff);   //kii_display_name
        _label_dname->retain();
        _label_dname->setColor( Color3B(0, 0, 0) );
        _label_dname->setPosition( Point(winSize.width/2, winSize.height/2 +60) );
		this->addChild(_label_dname);

        // Place the menu item bottom-right conner.
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();
        auto center = Point( origin.x + visibleSize.width/2.0f, origin.y + visibleSize.height/2.0f);

		CCLOG("center %f,%f", center.x, center.y);
        //ボタン
        //closeItem
        auto closeItem = MenuItemImage::create(
                                               "CloseNormal.png",
                                               "CloseSelected.png",
                                               CC_CALLBACK_1(StartLayer::menuCloseCallback,this)
                                               );
        closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2,
                                     origin.y + closeItem->getContentSize().height/2));
        
        //pBt_start
        pBt_start = MenuItemImage::create(
                                               "bt_start.png",
                                               "bt_start.png",
                                               CC_CALLBACK_1(StartLayer::bt_startCallback,this)
                                               );
        pBt_start->setPosition(Point(center.x ,center.y + 10.0f) );
        
        //pBt_ranking
        pBt_ranking = MenuItemImage::create(
                                               "bt_ranking.png",
                                               "bt_ranking.png",
                                               CC_CALLBACK_1(StartLayer::bt_rankingCallback,this)
                                               );
        pBt_ranking->setPosition(Point(center.x ,center.y - 50.0f) );
        
        //pBt_name
        pBt_name = MenuItemImage::create(
                                               "bt_name.png",
                                               "bt_name.png",
                                               CC_CALLBACK_1(StartLayer::bt_nameCallback,this)
                                               );
        pBt_name->setPosition(Point(center.x ,center.y - 110.0f) );
        

        
        // Create a menu with the "close" menu item, it's an auto release object.
        auto menu = Menu::create(closeItem, pBt_start, pBt_ranking, pBt_name, NULL);
        menu->setPosition(Point::ZERO);
        this->addChild(menu, 1);
        
        //PName
        _pname = PName::layer();
        _pname->setVisible(false);
        _pname->setTouchEnabled(false);
        this->addChild(_pname, 1);
        _pname->_slayer = this;

        //_pCKiiApiTest = CKiiApiTest::create();	//CKiiApiTest
        //_pCKiiApiTest->createApplicationScopeBucketTest();
        //_pCKiiApiTest->clause4_Test();

		this->schedule( schedule_selector(StartLayer::update) );

		CCLOG("StartLayer::init end ---");
		return true;
	}
	else
	{
		return false;
	}
}

void StartLayer::update(float dt){
	//CCLOG("StartLayer::update");
	//CCLOG("StartLayer::update %s", kii_display_name);
    char buff[256];
    sprintf(buff,"name:%s",kii_name);
    _label_name->setString(buff);   //kii_display_name

    sprintf(buff,"display_name:%s",kii_display_name);
    _label_dname->setString(buff);   //kii_display_name
}

void StartLayer::setButtonEnabled(bool b){
    if(b){
        pBt_start->setEnabled(true);
        pBt_ranking->setEnabled(true);
        pBt_name->setEnabled(true);
        //_label_name->setString(kii_display_name);
    } else {
        pBt_start->setEnabled(false);
        pBt_ranking->setEnabled(false);
        pBt_name->setEnabled(false);
    }
}

void StartLayer::gameOverDone()
{
	Director::getInstance()->replaceScene( HelloWorld::scene() );
}

/***
void StartLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
	CCLOG("StartLayer::ccTouchesEnded");
	//CCDirector::sharedDirector()->replaceScene( HelloWorld::scene() );
}
***/

void StartLayer::menuCloseCallback(Object* sender)
{
	// "close" menu item clicked
	Director::getInstance()->end();
}

void StartLayer::bt_startCallback(Object* sender)
{
	Director::getInstance()->replaceScene( HelloWorld::scene() );
}

extern void jni_ranking_query_all();

void StartLayer::bt_rankingCallback(Object* sender)
{
    //auto scene = StartScene::create();	//Start
	auto scene = RankingScene::create();
	Director::getInstance()->replaceScene( scene );
	//jni_ranking_query_all();
}

extern void jniTest(struct android_app* state);
extern struct android_app* g_state;

void StartLayer::bt_nameCallback(Object* sender)
{
	CCLOG("StartLayer::bt_nameCallback");
    _pname ->display();
    //jniTest(g_state);
	//_pCKiiApiTest->clause3_Test();
}

StartLayer::~StartLayer()
{
	if (_label)
	{
		_label->release();
		_label = NULL;
	}
}
