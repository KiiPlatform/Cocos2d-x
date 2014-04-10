#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

extern char kii_name[256];

HelloWorld::~HelloWorld()
{
	if (_targets)
	{
		_targets->release();
		_targets = NULL;
	}

	if (_projectiles)
	{
		_projectiles->release();
		_projectiles = NULL;
	}

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

HelloWorld::HelloWorld()
:_targets(NULL)
,_projectiles(NULL)
,_projectilesDestroyed(0)
{
}

Scene* HelloWorld::scene()
{
	Scene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = Scene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		HelloWorld *layer = HelloWorld::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! LayerColor::initWithColor( Color4B(255,255,255,255) ) );

		//////////////////////////////////////////////////////////////////////////
		// add your codes below...
		//////////////////////////////////////////////////////////////////////////

		// 1. Add a menu item with "X" image, which is clicked to quit the program.

		// Create a "close" menu item with close icon, it's an auto release object.
		auto closeItem = MenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
            CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
		CC_BREAK_IF(! closeItem);
        
		// Place the menu item bottom-right conner.
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto origin = Director::getInstance()->getVisibleOrigin();
        
		closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2,
                                    origin.y + closeItem->getContentSize().height/2));

		// Create a menu with the "close" menu item, it's an auto release object.
		auto menu = Menu::create(closeItem, NULL);
		menu->setPosition(Point::ZERO);
		CC_BREAK_IF(! menu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(menu, 1);

		/////////////////////////////
		// 2. add your codes below...
		auto player = Sprite::create("Player.png", Rect(0, 0, 27, 40) );
        
		player->setPosition( Point(origin.x + player->getContentSize().width/2,
                                 origin.y + visibleSize.height/2) );
		this->addChild(player);
        
        //_label_score
        _score = 0;
        sprintf(_score_buffer,"%05d",_score);
        auto winSize = Director::getInstance()->getWinSize();
		this->_label_score = LabelTTF::create("","Artial", 32);
		_label_score->retain();
		_label_score->setColor( Color3B(0, 0, 0) );
		_label_score->setString(_score_buffer);
		_label_score->setPosition( Point(winSize.width/2, winSize.height/2+120) );
		this->addChild(_label_score);
        
        //
        _userDefault = CCUserDefault::sharedUserDefault();
		_hiScore = _userDefault->getIntegerForKey("k_hiscore",0);	//load
		CCLOG("_hiScore  %d", _hiScore);

		this->schedule( schedule_selector(HelloWorld::gameLogic), 1.0 );

        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
        dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
		_targets = new Array();
        _targets->init();
        
		_projectiles = new Array();
        _projectiles->init();

		_userDefault = CCUserDefault::sharedUserDefault();
		_hiScore = _userDefault->getIntegerForKey("k_hiscore",0);	//load
		CCLOG("_hiScore  %d", _hiScore);
		//_pCKiiApiTest = CKiiApiTest::create();

		// use updateGame instead of update, otherwise it will conflit with SelectorProtocol::update
		// see http://www.cocos2d-x.org/boards/6/topics/1478
		this->schedule( schedule_selector(HelloWorld::updateGame) );

		//KiiRanking
		_pKiiRanking = KiiRanking::create();	//CKiiApiTest
		//_pCKiiApiTest->_name = kii_name;
		//CCLOG("_pCKiiApiTest->_name=%s", _pCKiiApiTest->_name.c_str());

		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("background-music-aac.wav", true);

		bRet = true;
	} while (0);

	return bRet;
}

void HelloWorld::saveHIScore(int score){
	CCLOG("HelloWorld::saveHIScore");
	if(_hiScore<score){
		CCLOG("save hiscore");
		_hiScore = score;
		_userDefault->setIntegerForKey("k_hiscore", _hiScore);	//save
		CCLOG("_hiScore %d ", _hiScore);
		//xx jni_ranking_post("hoge", _hiScore);
        //_pCKiiApiTest = CKiiApiTest::create();	//CKiiApiTest
		_pKiiRanking->postMyScore(_hiScore);	//ランキングをポストする
	} else {
		CCLOG("not hiscore");
		//for debug
		//_hiScore = score;
		//_userDefault->setIntegerForKey("k_hiscore", _hiScore);	//save
		CCLOG("not _hiScore %d ", _hiScore);
		//xx jni_ranking_post("hoge", _hiScore);
		_pKiiRanking->postMyScore(_hiScore);	//for debug
		//
	}
	CCLOG("HelloWorld::saveHIScore2");
}

void HelloWorld::menuCloseCallback(Object* sender)
{
	// "close" menu item clicked
	Director::getInstance()->end();
}

// cpp with cocos2d-x
void HelloWorld::addTarget()
{
	Sprite *target = Sprite::create("Target.png", Rect(0,0,27,40) );
    
	// Determine where to spawn the target along the Y axis
	Size winSize = Director::getInstance()->getVisibleSize();
	float minY = target->getContentSize().height/2;
	float maxY = winSize.height -  target->getContentSize().height/2;
	int rangeY = (int)(maxY - minY);
	// srand( TimGetTicks() );
	int actualY = ( rand() % rangeY ) + (int)minY;

	// Create the target slightly off-screen along the right edge,
	// and along a random position along the Y axis as calculated
	target->setPosition( 
		Point(winSize.width + (target->getContentSize().width/2), 
            Director::getInstance()->getVisibleOrigin().y + actualY) );
	this->addChild(target);

	// Determine speed of the target
	int minDuration = (int)2.0;
	int maxDuration = (int)4.0;
	int rangeDuration = maxDuration - minDuration;
	// srand( TimGetTicks() );
	int actualDuration = ( rand() % rangeDuration ) + minDuration;

	// Create the actions
	FiniteTimeAction* actionMove = MoveTo::create( (float)actualDuration,
                                            Point(0 - target->getContentSize().width/2, actualY) );
	FiniteTimeAction* actionMoveDone = CallFuncN::create( CC_CALLBACK_1(HelloWorld::spriteMoveFinished, this));
	target->runAction( Sequence::create(actionMove, actionMoveDone, NULL) );

	// Add to targets array
	target->setTag(1);
	_targets->addObject(target);
}

void HelloWorld::spriteMoveFinished(Node* sender)
{
	Sprite *sprite = (Sprite *)sender;
	this->removeChild(sprite, true);

	if (sprite->getTag() == 1)  // target
	{
		_targets->removeObject(sprite);
        
		auto gameOverScene = GameOverScene::create();
		//gameOverScene->getLayer()->getLabel()->setString("You Lose :[");
        char buff[256];
        //スコア文字列
        //sprintf(buff,"HISCORE: %05d\n  SCORE: %05d",_hiScore, _score);
        saveHIScore(_score);
        sprintf(buff,"RESULT\nHISCORE: %05d\n  SCORE: %05d",_hiScore, _score);
        gameOverScene->getLayer()->getLabel()->setString(buff);
        
		Director::getInstance()->replaceScene(gameOverScene);

	}
	else if (sprite->getTag() == 2) // projectile
	{
		_projectiles->removeObject(sprite);
	}
}

void HelloWorld::gameLogic(float dt)
{
	this->addTarget();
}

// cpp with cocos2d-x
void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
	// Choose one of the touches to work with
	Touch* touch = touches[0];
	Point location = touch->getLocation();
    
	log("++++++++after  x:%f, y:%f", location.x, location.y);

	// Set up initial location of projectile
	Size winSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
	Sprite *projectile = Sprite::create("Projectile.png", Rect(0, 0, 20, 20));
	projectile->setPosition( Point(origin.x+20, origin.y+winSize.height/2) );

	// Determinie offset of location to projectile
	float offX = location.x - projectile->getPosition().x;
	float offY = location.y - projectile->getPosition().y;

	// Bail out if we are shooting down or backwards
	if (offX <= 0) return;

	// Ok to add now - we've double checked position
	this->addChild(projectile);

	// Determine where we wish to shoot the projectile to
	float realX = origin.x+winSize.width + (projectile->getContentSize().width/2);
	float ratio = offY / offX;
	float realY = (realX * ratio) + projectile->getPosition().y;
	Point realDest = Point(realX, realY);

	// Determine the length of how far we're shooting
	float offRealX = realX - projectile->getPosition().x;
	float offRealY = realY - projectile->getPosition().y;
	float length = sqrtf((offRealX * offRealX) + (offRealY*offRealY));
	float velocity = 480/1; // 480pixels/1sec
	float realMoveDuration = length/velocity;

	// Move projectile to actual endpoint
	projectile->runAction( Sequence::create(
		MoveTo::create(realMoveDuration, realDest),
		CallFuncN::create(CC_CALLBACK_1(HelloWorld::spriteMoveFinished, this)),
                                            NULL) );

	// Add to projectiles array
	projectile->setTag(2);
	_projectiles->addObject(projectile);

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pew-pew-lei.wav");
}

void HelloWorld::updateGame(float dt)
{
	Array *projectilesToDelete = new Array();
    projectilesToDelete->init();
    
    Object* it = NULL;
    Object* jt = NULL;

	// for (it = _projectiles->begin(); it != _projectiles->end(); it++)
    CCARRAY_FOREACH(_projectiles, it)
	{
		auto projectile = dynamic_cast<Sprite*>(it);
		auto projectileRect = Rect(
			projectile->getPosition().x - (projectile->getContentSize().width/2),
			projectile->getPosition().y - (projectile->getContentSize().height/2),
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		auto targetsToDelete = new Array();
        targetsToDelete->init();

		// for (jt = _targets->begin(); jt != _targets->end(); jt++)
        CCARRAY_FOREACH(_targets, jt)
		{
			auto target = dynamic_cast<Sprite*>(jt);
			auto targetRect = Rect(
				target->getPosition().x - (target->getContentSize().width/2),
				target->getPosition().y - (target->getContentSize().height/2),
				target->getContentSize().width,
				target->getContentSize().height);

			// if (Rect::RectIntersectsRect(projectileRect, targetRect))
            if (projectileRect.intersectsRect(targetRect))
			{
                //スコア計算
                _score += 7 * target->getPosition().x;
                sprintf(_score_buffer,"%05d",_score);
                _label_score->setString(_score_buffer);
                CCLOG("_score = %d", _score);
                
				targetsToDelete->addObject(target);
			}
		}

		// for (jt = targetsToDelete->begin(); jt != targetsToDelete->end(); jt++)
        CCARRAY_FOREACH(targetsToDelete, jt)
		{
			auto target = dynamic_cast<Sprite*>(jt);
			_targets->removeObject(target);
			this->removeChild(target, true);

			_projectilesDestroyed++;
			if (_projectilesDestroyed >= 5)
			{
				/***
				auto gameOverScene = GameOverScene::create();
				gameOverScene->getLayer()->getLabel()->setString("You Win!");
				Director::getInstance()->replaceScene(gameOverScene);
				***/
			}
		}

		if (targetsToDelete->count() > 0)
		{
			projectilesToDelete->addObject(projectile);
		}
		targetsToDelete->release();
	}

	// for (it = projectilesToDelete->begin(); it != projectilesToDelete->end(); it++)
    CCARRAY_FOREACH(projectilesToDelete, it)
	{
		auto projectile = dynamic_cast<Sprite*>(it);
		_projectiles->removeObject(projectile);
		this->removeChild(projectile, true);
	}
	projectilesToDelete->release();
}

