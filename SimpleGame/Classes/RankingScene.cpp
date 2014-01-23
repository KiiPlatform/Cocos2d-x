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
#include "StartScene.h"

//#include "KRanking.h"

extern char kii_label_buff[1024];
extern void jni_ranking_query_all();

using namespace cocos2d;
bool RankingScene::init()
{
	if( CCScene::init() )
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

//char RankingLayer::label_buff[1024];

bool RankingLayer::init()
{
	if ( CCLayerColor::initWithColor( ccc4(255,255,255,255) ) )
	{
		CCLOG("RankingLayer::init");
		this->setTouchEnabled(true);
		//center
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        _center = ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);

        //
        /***
		CCSprite *player = CCSprite::create("Player.png", CCRectMake(0, 0, 27, 40) );
		player->setPosition( _center );
		this->addChild(player);
		***/

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		//this->_label = CCLabelTTF::create("","Artial", 32);
		this->_label = CCLabelTTF::create("","Artial", 12);
		//_label->setString("RANKING");
		_label->retain();
		_label->setColor( ccc3(0, 0, 0) );
		_label->setPosition( ccp( _center.x , _center.y  ) );
		this->addChild(_label);

		//label_ranking
		CCLabelTTF *label_ranking = CCLabelTTF::create("","Artial", 32);
		label_ranking->retain();
		label_ranking->setColor( ccc3(0, 0, 0) );
		label_ranking->setString("RANKING");
		label_ranking->setPosition( ccp(_center.x , _center.y+100) );
		this->addChild(label_ranking);

		//bt_title
		CCMenuItemImage *pBt_title = CCMenuItemImage::create(
			"bt_title.png",
			"bt_title.png",
			this,
			menu_selector(RankingLayer::bt_titleCallback));	//title
		//CC_BREAK_IF(! pBt_ranking);
		pBt_title->setPosition(ccp( _center.x , _center.y-110  ) );

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pBt_title, NULL);
		pMenu->setPosition(CCPointZero);
		//CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

/***
		this->runAction( CCSequence::create(
                                CCDelayTime::create(5),
                                CCCallFunc::create(this, 
                                callfunc_selector(GameOverLayer::gameOverDone)),
                                NULL));
***/
        strcpy(kii_label_buff, "no data");
		jni_ranking_query_all();
		this->schedule( schedule_selector(RankingLayer::update) );

        //strcpy(kii_label_buff, "no data");  //iPhoneの場合、ここだとno dataのまま
        
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

void RankingLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event){
	CCLOG("RankingLayer::ccTouchesEnded");
	//CCDirector::sharedDirector()->replaceScene( StartScene::create() );
}

void RankingLayer::bt_titleCallback(CCObject* pSender)
{
	CCLOG("RankingLayer::bt_titleCallback");
	CCDirector::sharedDirector()->replaceScene( StartScene::create() );
}

void RankingLayer::gameOverDone()
{
	CCDirector::sharedDirector()->replaceScene( HelloWorld::scene() );
}

RankingLayer::~RankingLayer()
{
	if (_label)
	{
		_label->release();
		_label = NULL;
	}
}
