#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "RankingScene.h"

#include "KRanking.h"

#include "SimpleAudioEngine.h"


#include "picojson.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

using namespace cocos2d;

#define CLASS_NAME "org/cocos2dx/simplegame/CallFromCPP"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
	//JNIEXPORT void JNICALL Java_org_cocos2dx_simplegame_SimpleGame_nativeEnd(JNIEnv* env, jobject thiz)
	JNIEXPORT void JNICALL Java_org_cocos2dx_simplegame_CallCPP_nativeEnd
	(JNIEnv* env, jobject thiz)
	{
	    CCLOG("SimpleGame_nativeEnd");
		//CCDirector::sharedDirector()->end();
	}

//package org.cocos2dx.simplegame;
//	JNIEXPORT jstring JNICALL Java_SampleJava4_concat
	JNIEXPORT jstring JNICALL Java_org_cocos2dx_simplegame_CallCPP_concat
	(JNIEnv *env, jobject obj, jstring str1, jstring str2) {
		char buf[256];

	    CCLOG("Java_org_cocos2dx_simplegame_concat");

		const char *c1 = env->GetStringUTFChars(str1, 0);
		const char *c2 = env->GetStringUTFChars(str2, 0);

		CCLOG("c1 = %s",c1);
		CCLOG("c2 = %s",c2);

		// ï∂éöóÒÇòAåãÇ∑ÇÈ
		strcpy(buf, c1);
		strcat(buf, c2);

		CCLOG("buf = %s", buf);

		env->ReleaseStringUTFChars(str1, c1);
		env->ReleaseStringUTFChars(str2, c2);

		// òAåãÇµÇΩï∂éöóÒÇï‘Ç∑
		return env->NewStringUTF(buf);
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_simplegame_CallCPP_rankingResponse
	(JNIEnv *env, jobject obj, jstring str1) {
		char buf[256];

	    CCLOG("Java_org_cocos2dx_simplegame_CallCPP_rankingResponse");

		const char *json = env->GetStringUTFChars(str1, 0);

		CCLOG("json = %s",json);
		//json
		std::string err;

		picojson::value v;
		//picojson::parse( c1, c1+strlen(c1), err );
		picojson::parse(v, json, json + strlen(json), &err);
		//picojson::object& o = v.get<picojson::object>();


	    // arrayílÇÃéÊìæ
	    //picojson::array& a1 = o["array1"].get<picojson::array>();
		picojson::array& a1 = v.get<picojson::array>();

		/***
		picojson::array::iterator it;
	    for(it = records.begin(); it != records.end(); it++){
	    	picojson::object& record = it->get<picojson::object>();

	    }
		 ****/

		// arrayÇÃíÜÇÃílÇÉãÅ[ÉvÇ≈éÊìæ
		int index = 0;
		strcpy(HelloWorld::label_buff, "");	//èâä˙âª
		char buff[256];
		HelloWorld::vScore.clear();	//ÉNÉäÉA
		for (picojson::array::iterator it = a1.begin(); it != a1.end(); it++) {
			picojson::object& o1 = it->get<picojson::object>();
			std::string& s1 = o1["name"].get<std::string>();
			std::string& s2 = o1["score"].get<std::string>();
			//CCLOG("s1 = %s",s1.c_str() );
			//CCLOG("s2 = %s",s2.c_str() );

			HelloWorld::vScore.push_back(ScoreData(s1,s2));
			sprintf(buff,"%s : %s \n", s1.c_str(), s2.c_str() );
			strcat(HelloWorld::label_buff, buff);//í«â¡
			index++;
			if(index>=7){
				break;
			}
		}
		CCLOG("label_buff  %s",HelloWorld::label_buff);	//ÉâÉìÉLÉìÉOópÇÃï∂éöóÒÇ™èoóàÇΩ
		//HelloWorld::rankingScene->getLayer()->getLabel()->setString(HelloWorld::label_buff);

		int i;
		for(i=0; i<HelloWorld::vScore.size(); i++){
			//CCLOG("vScore  %d  %s %s", i, HelloWorld::vScore[i].name.c_str(),  HelloWorld::vScore[i].score.c_str());
		}

		//rankingâÊñ Ç…îΩâfÇ∑ÇÈ
		//gameOverScene->getLayer()->getLabel()->setString("You Lose :[");


		//return env->NewStringUTF(buf);
	}


}

#else

void rankingResponse(char buff){

}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//Android
//call Java

void jni_test(){
    CCLOG("Jni_test");
    JniMethodInfo methodInfo;

    if (JniHelper::getStaticMethodInfo(methodInfo
                                       , CLASS_NAME
                                       , "test"
                                       , "()V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

void jni_ranking_query_all(){
    CCLOG("Jni_ranking_query_all");
    JniMethodInfo methodInfo;

    if (JniHelper::getStaticMethodInfo(methodInfo
                                       , CLASS_NAME
                                       , "ranking_query_all"
                                       , "()V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

#else
//iPhone
//call objective-c
void jni_test(){
    CCLOG("Jni_test iPhone");
}

extern void ranking_query_all2();

void jni_ranking_query_all(){
    CCLOG("Jni_ranking_query_all iPhone");
    //ranking_query_all2();
    KRanking::ranking_query_all();
}

#endif

std::vector<ScoreData> HelloWorld::vScore;
char HelloWorld::label_buff[1024];
RankingScene *HelloWorld::rankingScene;

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

CCScene* HelloWorld::scene()
{
	CCLOG("HelloWorld::scene");

	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
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
	CCLOG("HelloWorld::init");

	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayerColor::initWithColor( ccc4(255,255,255,255) ) );

		//////////////////////////////////////////////////////////////////////////
		// add your codes below...
		//////////////////////////////////////////////////////////////////////////

		// 1. Add a menu item with "X" image, which is clicked to quit the program.

		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(HelloWorld::menuCloseCallback));
		CC_BREAK_IF(! pCloseItem);
        
		// Place the menu item bottom-right conner.
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
		pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
                                    origin.y + pCloseItem->getContentSize().height/2));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

		/////////////////////////////
		// 2. add your codes below...
		CCSprite *player = CCSprite::create("Player.png", CCRectMake(0, 0, 27, 40) );
        
		player->setPosition( ccp(origin.x + player->getContentSize().width/2,
                                 origin.y + visibleSize.height/2) );
		this->addChild(player);

		this->schedule( schedule_selector(HelloWorld::gameLogic), 1.0 );

		this->setTouchEnabled(true);

		_targets = new CCArray;
		_projectiles = new CCArray;

		// use updateGame instead of update, otherwise it will conflit with SelectorProtocol::update
		// see http://www.cocos2d-x.org/boards/6/topics/1478
		this->schedule( schedule_selector(HelloWorld::updateGame) );

		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

		bRet = true;
	} while (0);

	jni_test();

	return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCLOG("HelloWorld::menuCloseCallback");

	// "close" menu item clicked
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    CCDirector::sharedDirector()->end();
#endif
}

// cpp with cocos2d-x
void HelloWorld::addTarget()
{
	CCSprite *target = CCSprite::create("Target.png", CCRectMake(0,0,27,40) );
    
	// Determine where to spawn the target along the Y axis
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	float minY = target->getContentSize().height/2;
	float maxY = winSize.height -  target->getContentSize().height/2;
	int rangeY = (int)(maxY - minY);
	// srand( TimGetTicks() );
	int actualY = ( rand() % rangeY ) + (int)minY;

	// Create the target slightly off-screen along the right edge,
	// and along a random position along the Y axis as calculated
	target->setPosition( 
		ccp(winSize.width + (target->getContentSize().width/2), 
            CCDirector::sharedDirector()->getVisibleOrigin().y + actualY) );
	this->addChild(target);

	// Determine speed of the target
	int minDuration = (int)2.0;
	int maxDuration = (int)4.0;
	int rangeDuration = maxDuration - minDuration;
	// srand( TimGetTicks() );
	int actualDuration = ( rand() % rangeDuration ) + minDuration;

	// Create the actions
	CCFiniteTimeAction* actionMove = CCMoveTo::create( (float)actualDuration,
                                            ccp(0 - target->getContentSize().width/2, actualY) );
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this, 
                                            callfuncN_selector(HelloWorld::spriteMoveFinished));
	target->runAction( CCSequence::create(actionMove, actionMoveDone, NULL) );

	// Add to targets array
	target->setTag(1);
	_targets->addObject(target);
}

void HelloWorld::spriteMoveFinished(CCNode* sender)
{
	CCSprite *sprite = (CCSprite *)sender;
	this->removeChild(sprite, true);

	if (sprite->getTag() == 1)  // target
	{
		_targets->removeObject(sprite);
        
		GameOverScene *gameOverScene = GameOverScene::create();
		gameOverScene->getLayer()->getLabel()->setString("You Lose :[");
		//gameOverScene->getLayer()->getLabel()->setString(label_buff);
		CCDirector::sharedDirector()->replaceScene(gameOverScene);

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
void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	jni_test();

	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();
    
	CCLog("++++++++after  x:%f, y:%f", location.x, location.y);

	// Set up initial location of projectile
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSprite *projectile = CCSprite::create("Projectile.png", CCRectMake(0, 0, 20, 20));
	projectile->setPosition( ccp(origin.x+20, origin.y+winSize.height/2) );

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
	CCPoint realDest = ccp(realX, realY);

	// Determine the length of how far we're shooting
	float offRealX = realX - projectile->getPosition().x;
	float offRealY = realY - projectile->getPosition().y;
	float length = sqrtf((offRealX * offRealX) + (offRealY*offRealY));
	float velocity = 480/1; // 480pixels/1sec
	float realMoveDuration = length/velocity;

	// Move projectile to actual endpoint
	projectile->runAction( CCSequence::create(
		CCMoveTo::create(realMoveDuration, realDest),
		CCCallFuncN::create(this, 
                            callfuncN_selector(HelloWorld::spriteMoveFinished)), 
        NULL) );

	// Add to projectiles array
	projectile->setTag(2);
	_projectiles->addObject(projectile);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
}

void HelloWorld::updateGame(float dt)
{
	CCArray *projectilesToDelete = new CCArray;
    CCObject* it = NULL;
    CCObject* jt = NULL;

	// for (it = _projectiles->begin(); it != _projectiles->end(); it++)
    CCARRAY_FOREACH(_projectiles, it)
	{
		CCSprite *projectile = dynamic_cast<CCSprite*>(it);
		CCRect projectileRect = CCRectMake(
			projectile->getPosition().x - (projectile->getContentSize().width/2),
			projectile->getPosition().y - (projectile->getContentSize().height/2),
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		CCArray* targetsToDelete =new CCArray;

		// for (jt = _targets->begin(); jt != _targets->end(); jt++)
        CCARRAY_FOREACH(_targets, jt)
		{
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
			CCRect targetRect = CCRectMake(
				target->getPosition().x - (target->getContentSize().width/2),
				target->getPosition().y - (target->getContentSize().height/2),
				target->getContentSize().width,
				target->getContentSize().height);

			// if (CCRect::CCRectIntersectsRect(projectileRect, targetRect))
            if (projectileRect.intersectsRect(targetRect))
			{
				targetsToDelete->addObject(target);
			}
		}

		// for (jt = targetsToDelete->begin(); jt != targetsToDelete->end(); jt++)
        CCARRAY_FOREACH(targetsToDelete, jt)
		{
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
			_targets->removeObject(target);
			this->removeChild(target, true);

			_projectilesDestroyed++;
			if (_projectilesDestroyed >= 5)
			{
				GameOverScene *gameOverScene = GameOverScene::create();
				gameOverScene->getLayer()->getLabel()->setString("You Win!");
				CCDirector::sharedDirector()->replaceScene(gameOverScene);
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
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		_projectiles->removeObject(projectile);
		this->removeChild(projectile, true);
	}
	projectilesToDelete->release();
}

void HelloWorld::registerWithTouchDispatcher()
{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}
