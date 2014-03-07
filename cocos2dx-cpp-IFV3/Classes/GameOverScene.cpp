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

#include "GameOverScene.h"
#include "HelloWorldScene.h"
#include "StartScene.h"

using namespace cocos2d;

bool GameOverScene::init()
{
	if( Scene::init() )
	{
		this->_layer = GameOverLayer::create();
		this->_layer->retain();
		this->addChild(_layer);
		
		return true;
	}
	else
	{
		return false;
	}
}

GameOverScene::~GameOverScene()
{
	if (_layer)
	{
		_layer->release();
		_layer = NULL;
	}
}

bool GameOverLayer::init()
{
	if ( LayerColor::initWithColor( Color4B(255,255,255,255) ) )
	{
		auto winSize = Director::getInstance()->getWinSize();
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();
        auto center = Point( origin.x + visibleSize.width/2.0f, origin.y + visibleSize.height/2.0f);

		this->_label = LabelTTF::create("","Artial", 32);
		_label->retain();
		_label->setColor( Color3B(0, 0, 0) );
		_label->setPosition( Point(winSize.width/2, winSize.height/2) );
		this->addChild(_label);
		
		/***
		this->runAction( Sequence::create(
                                DelayTime::create(3),
                                CallFunc::create( CC_CALLBACK_0(GameOverLayer::gameOverDone, this)),
                                NULL));
		***/

		//bt_title
	    auto pBt_title = MenuItemImage::create(
	                                               "bt_title.png",
	                                               "bt_title.png",
	                                               CC_CALLBACK_1(GameOverLayer::bt_titleCallback,this)
	                                               );
	    pBt_title->setPosition(Point(center.x ,center.y - 110.0f) );

	    // Create a menu with the "close" menu item, it's an auto release object.
	    auto menu = Menu::create(pBt_title, NULL);
	    menu->setPosition(Point::ZERO);
	    this->addChild(menu, 1);


		
		return true;
	}
	else
	{
		return false;
	}
}

void GameOverLayer::bt_titleCallback(CCObject* pSender)
{
	CCLOG("GameOverLayer::bt_titleCallback");
	CCDirector::sharedDirector()->replaceScene( StartScene::create() );
}

void GameOverLayer::gameOverDone()
{
	Director::getInstance()->replaceScene( HelloWorld::scene() );
}

GameOverLayer::~GameOverLayer()
{
	if (_label)
	{
		_label->release();
		_label = NULL;
	}
}
