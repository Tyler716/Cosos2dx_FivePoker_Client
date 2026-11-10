#ifndef _NGSCENE_HPP_
#define _NGSCENE_HPP_

#include "NGLayer.h"
#include "DGLayer.h"
#include "LoginLayer.h"
#include "SimpleAudioEngine.h"

class GameScene : public CCScene
{
public:
    GameScene() 
        : m_pNGLayer(NULL)
        , m_pDGLayer(NULL)
        , m_pLoginLayer(NULL)
    {}

    ~GameScene();

    bool init();

    SCENE_CREATE_FUNC(GameScene);

    //宣告protected readonly的pointer及函式
    CC_SYNTHESIZE_READONLY(NGLayer*, m_pNGLayer, NGLayer);
    CC_SYNTHESIZE_READONLY(DGLayer*, m_pDGLayer, DGLayer);
    CC_SYNTHESIZE_READONLY(LoginLayer*, m_pLoginLayer, LoginLayer);

private:
    virtual void update(float fDelta);

    IFacade *m_pFacade;
    FivePokerModelData *m_ModelData;
};
#endif  //_NGSCENE_HPP_