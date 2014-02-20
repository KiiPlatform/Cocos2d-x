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
	if( CCScene::init() )
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
	if ( CCLayerColor::initWithColor( ccc4(255,255,255,255) ) )
	{
		this->setTouchEnabled(true);
		//center
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        _center = ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		this->_label = CCLabelTTF::create("","Artial", 32);
		_label->retain();
		_label->setColor( ccc3(0, 0, 0) );
		_label->setPosition( ccp(winSize.width/2, winSize.height/2) );
		this->addChild(_label);

		//label_result
		CCLabelTTF *label_result = CCLabelTTF::create("","Artial", 32);
		label_result->retain();
		label_result->setColor( ccc3(0, 0, 0) );
		label_result->setString("RESULT");
		label_result->setPosition( ccp(_center.x , _center.y+100) );
		this->addChild(label_result);

		//bt_title
		CCMenuItemImage *pBt_title = CCMenuItemImage::create(
			"bt_title.png",
			"bt_title.png",
			this,
			menu_selector(GameOverLayer::bt_titleCallback));	//title
		//CC_BREAK_IF(! pBt_ranking);
		pBt_title->setPosition(ccp( _center.x , _center.y-110  ) );

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pBt_title, NULL);
		pMenu->setPosition(CCPointZero);
		//CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

		return true;
	}
	else
	{
		return false;
	}
}

void GameOverLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event){
	CCLOG("GameOverLayer::ccTouchesEnded");
}

void GameOverLayer::bt_titleCallback(CCObject* pSender)
{
	CCLOG("GameOverLayer::bt_titleCallback");
	CCDirector::sharedDirector()->replaceScene( StartScene::create() );
}

void GameOverLayer::gameOverDone()
{
	CCDirector::sharedDirector()->replaceScene( StartScene::create() );
}

GameOverLayer::~GameOverLayer()
{
	if (_label)
	{
		_label->release();
		_label = NULL;
	}
}
