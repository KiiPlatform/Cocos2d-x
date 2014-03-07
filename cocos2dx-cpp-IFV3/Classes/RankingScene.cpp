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

#include "RankingScene.h"
#include "HelloWorldScene.h"
#include "HelloWorldScene.h"
#include "StartScene.h"
#include "PName.h"

extern char kii_label_buff[1024];
extern void jni_ranking_query_all();

using namespace cocos2d;

bool RankingScene::init()
{
	if( Scene::init() )
	{
		this->_layer = RankingLayer::create();
		this->_layer->retain();
		this->addChild(_layer);
		
		return true;
	}
	else
	{
		return false;
	}
}

RankingScene::~RankingScene()
{
	if (_layer)
	{
		_layer->release();
		_layer = NULL;
	}
}

bool RankingLayer::init()
{
	if ( LayerColor::initWithColor( Color4B(255,255,255,255) ) )
	{
		auto winSize = Director::getInstance()->getWinSize();
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();
        auto center = Point( origin.x + visibleSize.width/2.0f, origin.y + visibleSize.height/2.0f);

        //_lavel
		this->_label = LabelTTF::create("","Artial", 12);
        _label->setString("no data");
		_label->retain();
		_label->setColor( Color3B(0, 0, 0) );
		_label->setPosition( Point(center.x, center.y) );
		this->addChild(_label);

		LabelTTF *label_ranking = LabelTTF::create("","Artial", 32);
		label_ranking->setString("RANKING");
		label_ranking->retain();
		label_ranking->setColor( Color3B(0, 0, 0) );
		label_ranking->setPosition( Point(center.x, center.y+100) );
		this->addChild(label_ranking);

        //ボタン
        //bt_title
        auto pBt_title = MenuItemImage::create(
                                               "bt_title.png",
                                               "bt_title.png",
                                               CC_CALLBACK_1(RankingLayer::bt_titleCallback,this)
                                               );
        pBt_title->setPosition(Point(center.x ,center.y + -110.0f) );
        
         // Create a menu with the "close" menu item, it's an auto release object.
         auto menu = Menu::create(pBt_title, NULL);
         menu->setPosition(Point::ZERO);
         this->addChild(menu, 1);

         //CC_BREAK_IF(! menu);
        strcpy(kii_label_buff, "no data");
 		//jni_ranking_query_all();

        _pCKiiApiTest = CKiiApiTest::create();	//CKiiApiTest
        _pCKiiApiTest->createApplicationScopeBucketTest();
        //_pCKiiApiTest->clause4_Test();

 		this->schedule( schedule_selector(RankingLayer::update) );
		return true;
	}
	else
	{
		return false;
	}
}

void RankingLayer::update(float dt)
{
	//CCLOG("RankingLayer::update %s ", kii_label_buff);

	//_label->setString(HelloWorld::label_buff);
    //_label->setString(KRanking::ranking_buff);
    _label->setString(kii_label_buff);
}


void RankingLayer::gameOverDone()
{
	Director::getInstance()->replaceScene( HelloWorld::scene() );
}

void RankingLayer::bt_titleCallback(CCObject* pSender)
{
	CCLOG("RankingLayer::bt_titleCallback");
	CCDirector::sharedDirector()->replaceScene( StartScene::create() );
}

RankingLayer::~RankingLayer()
{
	if (_label)
	{
		_label->release();
		_label = NULL;
	}
}
