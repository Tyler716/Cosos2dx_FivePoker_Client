//
//  TouchableSprite.cpp
//  TouchDemo
//
//  Created by Tom on 12-5-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "TouchableSprite.h"

TouchableSprite::TouchableSprite()
{
}

TouchableSprite::~TouchableSprite()
{
}

TouchableSprite* TouchableSprite::touchSpriteWithFile(const char *pszFileName, CCObject *target, SEL_CallFuncI selector)
{
    return TouchableSprite::create(pszFileName, target, selector);
}

bool TouchableSprite::initWithFile(const char *file)
{
    CCSprite::initWithFile(file);
    
    return true;
}

bool TouchableSprite::initWithTarget(CCObject* pSelectorTarget, 
                                     SEL_CallFuncI selector)
{
    if(pSelectorTarget)
    {
        m_pSelectorTarget = pSelectorTarget;
        m_pCallFuncI = selector;
        return true;
    }

    return false;
}

TouchableSprite *TouchableSprite::create(const char *pszFileName, 
                                         CCObject *target, 
                                         SEL_CallFuncI selector)
{
    TouchableSprite *pobSprite = new TouchableSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->initWithTarget(target, selector);
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

void TouchableSprite::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void TouchableSprite::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}

CCRect TouchableSprite::rect()
{
    CCSize size = getTexture()->getContentSize();

    return CCRectMake(-size.width / 2, -size.height / 2, size.width, size.height);
}

bool TouchableSprite::containsTouchLocation(cocos2d::CCTouch *touch)
{
    return CCRect::CCRectContainsPoint(rect(), convertTouchToNodeSpaceAR(touch));
}

bool TouchableSprite::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    bool bRet = false;
    
    if (containsTouchLocation(touch))
    {
        CCLog("Touchable Sprite Began");
        
        bRet = true;

        if(m_pCallFuncI)
        {
            (m_pSelectorTarget->*m_pCallFuncI)(getTag());
        }
    }
    
    return bRet;
}

void TouchableSprite::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    CCLog("Touchable Sprite Moved");
}

void TouchableSprite::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    CCLog("Touchable Sprite Ended");
}

