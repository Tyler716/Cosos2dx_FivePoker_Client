//
//  TouchableSprite.h
//  TouchDemo
//
//  Created by Tom on 12-5-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef TouchDemo_TouchableSprite_h
#define TouchDemo_TouchableSprite_h

#include "cocos2d.h"

USING_NS_CC;

typedef void (CCObject::*SEL_CallFuncI)(int iTag);
#define callfuncI_selector(_SELECTOR) (SEL_CallFuncI)(&_SELECTOR)

// 可接收触摸事件的精灵
class TouchableSprite: public CCSprite, public CCTargetedTouchDelegate
{
public:
    TouchableSprite();
    virtual ~TouchableSprite();
    
    static TouchableSprite *touchSpriteWithFile(const char *pszFileName, CCObject *target, SEL_CallFuncI selector);
    static TouchableSprite *create(const char *pszFileName, CCObject *target, SEL_CallFuncI selector);
    
    bool initWithFile(const char *file);
    
    virtual void onEnter();
    virtual void onExit();
    
    CCRect rect();
    bool containsTouchLocation(CCTouch *touch);
    
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchMoved(CCTouch *touch, CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);

protected:
    bool initWithTarget(CCObject* pSelectorTarget, SEL_CallFuncI selector);

private:
    //static CCAction *m_pAction;
    SEL_CallFuncI   m_pCallFuncI;
    CCObject        *m_pSelectorTarget;
};

#endif
