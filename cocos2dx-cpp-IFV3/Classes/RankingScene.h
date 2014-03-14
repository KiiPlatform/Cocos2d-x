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

#ifndef _RANKING_SCENE_H_
#define _RANKING_SCENE_H_

#include "PName.h"
#include "cocos2d.h"
//#include "test/CKiiApiTest.h"
#include "kiiRanking/KiiRanking.h"

class RankingLayer : public cocos2d::LayerColor
{
public:
    RankingLayer():_label(NULL) {};
    virtual ~RankingLayer();
    bool init();
    CREATE_FUNC(RankingLayer);
	//void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

    virtual void bt_titleCallback(cocos2d::CCObject* pSender);
    void gameOverDone();
	void update(float dt);

	//CC_SYNTHESIZE_READONLY(cocos2d::LabelTTF*, _label_score, Label);
    CC_SYNTHESIZE_READONLY(cocos2d::LabelTTF*, _label, Label);
    //CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label_name, LabelName);
    CCPoint _center;
	//static char label_buff[1024];
    PName *_pName;
    KiiRanking *_pKiiRanking;
};

class RankingScene : public cocos2d::Scene
{
public:
    RankingScene():_layer(NULL) {};
    ~RankingScene();
    bool init();
    CREATE_FUNC(RankingScene);
  
    CC_SYNTHESIZE_READONLY(RankingLayer*, _layer, Layer);
};

#endif // _RANKING_SCENE_H_
