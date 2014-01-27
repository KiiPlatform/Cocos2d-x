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

#ifndef _START_SCENE_H_
#define _START_SCENE_H_

#include "PName.h"
#include "cocos2d.h"
using namespace cocos2d;

class StartLayer : public cocos2d::CCLayerColor
{
public:
	StartLayer():_label(NULL) {};
    virtual ~StartLayer();
    bool init();
    CREATE_FUNC(StartLayer);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	virtual void bt_startCallback(cocos2d::CCObject* pSender);
	virtual void bt_rankingCallback(cocos2d::CCObject* pSender);
    virtual void bt_nameCallback(cocos2d::CCObject* pSender);
    void gameOverDone();
    
    CCMenuItemImage *pBt_start, *pBt_ranking, *pBt_name;

    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label, Label);
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label_name, LabelName);
    CCPoint _center;
    void setButtonEnabled(bool b);
    
    PName *_pname;
};

class StartScene : public cocos2d::CCScene
{
public:
	StartScene():_layer(NULL) {};
    ~StartScene();
    bool init();
    CREATE_FUNC(StartScene);
  
    CC_SYNTHESIZE_READONLY(StartLayer*, _layer, Layer);
};

#endif // _GAME_OVER_SCENE_H_
