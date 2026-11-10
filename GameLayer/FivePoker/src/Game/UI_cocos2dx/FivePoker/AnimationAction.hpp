#ifndef _ANIMATIONACTION_HPP_
#define _ANIMATIONACTION_HPP_

#include "cocos2d.h"

struct PicInfo
{
    int iWidth;     //圖片寬度
    int iHeight;    //圖片高度
    int Hor_Num;    //橫的張數
    int Ver_Num;    //直的張數
    int Total_Num;  //總張數
    int x;          //x座標
    int y;          //y座標
};

USING_NS_CC;

class AnimationAction : public CCObject
{
public:
    AnimationAction(const char *szFileName, PicInfo info, float fDelayUnit = 0.05, int iLoop = 0)
    {
        m_texture = CCTextureCache::sharedTextureCache()->addImage(szFileName); 

        memcpy((void *)&m_picinfo, (void *)&info, sizeof(m_picinfo));
        m_strFileName = szFileName;
        m_fDelayUnit = fDelayUnit;
        m_iLoop = iLoop;

        m_pAnimationSprite = CCSprite::create(CCSpriteFrame::frameWithTexture(m_texture, CCRectMake(0, 0, info.iWidth, info.iHeight)));
        m_pAnimationSprite->setPosition(ccp(info.x, info.y));
        
        m_Action = NULL;
    }

    void runAction()
    {
        m_Animation = CCAnimation::create();    //會autorelease

        for(int ver = 0; ver < m_picinfo.Ver_Num; ver++)
        {
            for(int hor = 0; hor < m_picinfo.Hor_Num; hor++)
            {
                if((ver+1)*(hor+1) <= m_picinfo.Total_Num)
                {
                    m_Animation->addSpriteFrame(CCSpriteFrame::frameWithTexture(m_texture, CCRectMake(m_picinfo.iWidth*hor, m_picinfo.iHeight*ver, 
                        m_picinfo.iWidth,m_picinfo.iHeight)));
                }
            }
        }

        m_Animation->setDelayPerUnit(m_fDelayUnit);

        if(m_iLoop != 0)
        {
            m_Animation->setLoops(m_iLoop);
            //m_Action = CCAnimate::create(m_Animation);
            m_Action = CCSequence::actions(
                            CCAnimate::create(m_Animation),
                            CCCallFuncN::actionWithTarget(this, 
                                callfuncN_selector(AnimationAction::onActionComplete)), 
                        NULL);
            m_pAnimationSprite->runAction(m_Action);
        }
        else
        {
            m_ActionForever = CCRepeatForever::create(CCAnimate::create(m_Animation));
            m_pAnimationSprite->runAction(m_ActionForever);
        }
    }

    void runActionSequence(CCAction *action)
    {
    }

    void onActionComplete(CCNode *sender)
    {
        if(m_Action)
        {
            m_pAnimationSprite->stopAction(m_Action);
            m_Action = NULL;
        }
    }

    //動畫跑完後維持在全景的哪一張
    void setFullFrame(int H_Pos, int V_Pos)
    {
        m_Animation = CCAnimation::create();

        m_Animation->addSpriteFrame(
            CCSpriteFrame::frameWithTexture(
                m_texture, CCRectMake(
                    m_picinfo.iWidth*(H_Pos-1),
                    m_picinfo.iHeight*(V_Pos-1),
                    m_picinfo.iWidth,m_picinfo.iHeight)));

        m_Animation->setDelayPerUnit(m_fDelayUnit);

        m_Animation->setLoops(1);

        m_Action = CCAnimate::create(m_Animation);
        m_pAnimationSprite->runAction(m_Action);
    }

    void stopAction()
    {
        if(m_pAnimationSprite->isRunning())
        {
            if(m_iLoop == 0)
            {
                if(m_ActionForever)
                {
                    if(!m_ActionForever->isDone())
                    {
                        m_pAnimationSprite->stopAction(m_ActionForever);
                        m_ActionForever = NULL;
                    }
                }
            }
            else
            {
                if(m_Action)
                {
                    if(!m_Action->isDone())
                    {
                        m_pAnimationSprite->stopAction(m_Action);
                        m_Action = NULL;
                    }
                }
            }
        }
    }

    void moveAnimationPos(int x, int y)
    {
        CCMoveTo*  action = CCMoveTo::create( 0.1, CCPointMake(x, y));
  
        m_pAnimationSprite->runAction(action);
    }

    CCNode *getAnimation()
    {
        return (CCNode *)m_pAnimationSprite;
    }

    void setOpacity(int iValue)
    {
        m_pAnimationSprite->setOpacity(iValue);
    }

protected:
    CCTexture2D *m_texture;
    CCSprite *m_pAnimationSprite;
    CCAnimation *m_Animation;
    CCAction *m_Action;
    CCRepeatForever *m_ActionForever;
    PicInfo m_picinfo;
    std::string m_strFileName;
    float m_fDelayUnit;
    int m_iLoop;
};

#endif //_ANIMATIONACTION_HPP_