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
//#include "CallFromCPP.h"

using namespace cocos2d;

extern void jni_ranking_query_all();
extern void jni_ranking_post(const char *name, int score);

bool StartScene::init()
{
	if( CCScene::init() )
	{
		CCLOG("StartScene::init");

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
	if ( CCLayerColor::initWithColor( ccc4(255,255,255,255) ) )
	{
		CCLOG("StartLayer::init");
		this->setTouchEnabled(true);
		//center
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        _center = ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		this->_label = CCLabelTTF::create("","Artial", 32);
		_label->setString("TITLE kii sample");
		_label->retain();
		_label->setColor( ccc3(0, 0, 0) );
		_label->setPosition(ccp( _center.x , _center.y+110  ) );
		this->addChild(_label);

        this->_label_name = CCLabelTTF::create("","Artial", 32);
		_label_name->setString("PlayerName");
		_label_name->retain();
		_label_name->setColor( ccc3(0, 0, 0) );
		_label_name->setPosition(ccp( _center.x , _center.y+80  ) );
		this->addChild(_label_name);

        CCLOG("center %f,%f", _center.x, _center.y);

		//
        /***
		CCSprite *player = CCSprite::create("Player.png", CCRectMake(0, 0, 27, 40) );
		//player->setPosition( ccp(origin.x + player->getContentSize().width/2, origin.y + visibleSize.height/2) );
		player->setPosition( _center );
		this->addChild(player);
		***/

/***
		CCPoint ccp1;
		float dx,dy;
		dx = 120.0f;
		dy = 120.0f;

		CCSprite *player1 = CCSprite::create("Player.png", CCRectMake(0, 0, 27, 40) );
		ccp1 = ccp( _center.x-dx , _center.y-dy  );
		player1->setPosition( ccp1 );
		this->addChild(player1);

		CCSprite *player4 = CCSprite::create("Player.png", CCRectMake(0, 0, 27, 40) );
		ccp1 = ccp( _center.x+dx , _center.y+dy  );
		player4->setPosition( ccp1 );
		this->addChild(player4);

		CCSprite *player7 = CCSprite::create("Player.png", CCRectMake(0, 0, 27, 40) );
		ccp1 = ccp( _center.x-dx , _center.y+dy  );
		player7->setPosition( ccp1 );
		this->addChild(player7);

		CCSprite *player8 = CCSprite::create("Player.png", CCRectMake(0, 0, 27, 40) );
		ccp1 = ccp( _center.x+dx , _center.y-dy  );
		player8->setPosition( ccp1 );
		this->addChild(player8);
***/
		//É{É^Éì
		//bt_start
		pBt_start = CCMenuItemImage::create(
			"bt_start.png",
			"bt_start.png",
			this,
			menu_selector(StartLayer::bt_startCallback));	//start
		//CC_BREAK_IF(! pBt_start);
		pBt_start->setPosition(ccp( _center.x , _center.y+10  ) );

		//bt_ranking
		pBt_ranking = CCMenuItemImage::create(
			"bt_ranking.png",
			"bt_ranking.png",
			this,
			menu_selector(StartLayer::bt_rankingCallback));	//ranking
		//CC_BREAK_IF(! pBt_ranking);
		pBt_ranking->setPosition(ccp( _center.x , _center.y-50  ) );
        
        //bt_ranking
		pBt_name = CCMenuItemImage::create(
            "bt_ranking.png",
            "bt_ranking.png",
            this,
            menu_selector(StartLayer::bt_nameCallback));	//ranking
		//CC_BREAK_IF(! pBt_ranking);
		pBt_name->setPosition(ccp( _center.x , _center.y-110  ) );

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pBt_start, pBt_ranking, pBt_name, NULL);
		pMenu->setPosition(CCPointZero);
		//CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);


        _pname = PName::layer();
        _pname->setVisible(false);
        _pname->setTouchEnabled(false);
        this->addChild(_pname, 1);
        _pname->_slayer = this;
		
		return true;
	}
	else
	{
		return false;
	}
}

void StartLayer::setButtonEnabled(bool b){
    if(b){
        pBt_start->setEnabled(true);
        pBt_ranking->setEnabled(true);
        pBt_name->setEnabled(true);
    } else {
        pBt_start->setEnabled(false);
        pBt_ranking->setEnabled(false);
        pBt_name->setEnabled(false);
    }
}

void StartLayer::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event){
	CCLOG("StartLayer::ccTouchesEnded");
	//CCDirector::sharedDirector()->replaceScene( HelloWorld::scene() );
}

void StartLayer::bt_startCallback(CCObject* pSender)
{
	CCLOG("StartLayer::bt_startCallback");
	CCDirector::sharedDirector()->replaceScene( HelloWorld::scene() );
}

void StartLayer::bt_rankingCallback(CCObject* pSender)
{
	CCLOG("StartLayer::bt_rankingCallback");
	CCDirector::sharedDirector()->replaceScene( RankingScene::create() );
    
    //CallFromCpp::showAstWall();   //for test
    //CallFromCpp::ranking_query_all();
    //CallFromCpp::ranking_post("muku", 123456+2);    //for test
	//jni_ranking_post("muku-3", 123456+3);

	//jni_ranking_query_all();
}

void StartLayer::bt_nameCallback(CCObject* pSender)
{
	CCLOG("StartLayer::bt_nameCallback");
    _pname ->display();
}

void StartLayer::gameOverDone()
{
	CCDirector::sharedDirector()->replaceScene( RankingScene::create() );
}

StartLayer::~StartLayer()
{
	if (_label)
	{
		_label->release();
		_label = NULL;
	}
}
