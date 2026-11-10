#ifndef CURSORTEXTFIELD_H_
#define CURSORTEXTFIELD_H_

#include "cocos2d.h"

USING_NS_CC;

static const float DELTA = 5.0f;

class CursorTextField : public CCTextFieldTTF, public CCTextFieldDelegate, public CCTouchDelegate
{
public:
    CursorTextField();
    virtual ~CursorTextField();

    /** creates a CCLabelTTF from a fontname and font size */
    static CursorTextField * textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);

    virtual void onEnter();
    virtual void onExit();

    void initCursorSprite(int iHeight);
    void setColor(const ccColor3B& color3);

    // CCTextFieldDelegate
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF *pSender);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);

    // CCLayer Touch
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    //判斷是否點到Text
    bool isInTextField(CCTouch *pTouch);

    CCRect getRect();

    void openIME();

    void closeIME();

    bool getIsPsw();

    void setIsPsw(bool bFlag);

    //設定長度限制
    void setLimitNum(int limitNum);
    int getLimitNum();

    void setDiaplayLimitNum(int disLimitNum);
    int getDiaplayLimitNum();

    void setString(const char* displayTx, const char* inputTx);

    //點擊彈出輸入法的尺寸範圍
    void setDesignedSize(CCSize size);
    CCSize getDesignedSize();

    int calcCharCount(const char * pszText);

    //建置動態二維陣列
    void *new2d(int h, int w, int size);

private:
    CCPoint m_beginPos;             //點擊開始位置

    CCSprite *m_pCursorSprite;      //游標精靈

    CCAction *m_pCursorAction;      //游標動畫

    CCPoint m_cursorPos;            //游標位置

    bool isPsw;                     //是否顯示密碼
    int m_limitNum, m_disLimitNum;

    CCSize m_designedSize;
};

#endif  //CURSORTEXTFIELD_H_