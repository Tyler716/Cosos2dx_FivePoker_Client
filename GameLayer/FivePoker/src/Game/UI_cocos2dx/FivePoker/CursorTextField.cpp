#include "CursorTextField.h"
//#include "CCNotificationCenter.h"

CursorTextField::CursorTextField()
{
    CCTextFieldTTF();

    m_pCursorSprite = NULL;
    m_pCursorAction = NULL;

    m_pInputText = NULL;
    isPsw = false;
    m_limitNum = 30;
    m_disLimitNum = 20;
}

CursorTextField::~CursorTextField()
{
    //delete m_pInputText;
}

void *CursorTextField::new2d(int h, int w, int size)
{
    void **p;

    p = (void**)new char[h*sizeof(void*) + h*w*size];
    for(int i = 0; i < h; i++)
    {
        p[i] = ((char *)(p + h)) + i*w*size; 
    }

    return p;
}

int CursorTextField::calcCharCount(const char * pszText)
{
    int n = 0;
    char ch = 0;
    while ((ch = *pszText))
    {
        CC_BREAK_IF(! ch);
        
        if (0x80 != (0xC0 & ch))
        {
            ++n;
        }
        ++pszText;
    }
    return n;
}

void CursorTextField::onEnter()
{
    CCTextFieldTTF::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, false);
    this->setDelegate(this);
}

CursorTextField * CursorTextField::textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize)
{
    CursorTextField *pRet = new CursorTextField();

    if(pRet && pRet->initWithString("", fontName, fontSize))
    {
        pRet->autorelease();
        if (placeholder)
        {
            pRet->setPlaceHolder(placeholder);
        }
        pRet->initCursorSprite(fontSize);
        
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    
    return NULL;
}

void CursorTextField::initCursorSprite(int iHeight)
{
    //初始化cursor
    int iColumn = 4;

    int **pixels = (int **)new2d(iHeight, iColumn, sizeof(int));
    //[nHeight][column]

    for(int i = 0; i < iHeight; i++)
    {
        for(int j = 0; j < iColumn; j++)
        {
            pixels[i][j] = 0xffffffff;
        }
    }

    CCTexture2D *texture = new CCTexture2D();
    texture->initWithData(pixels, kCCTexture2DPixelFormat_RGB888, 1, 1, CCSizeMake(iColumn, iHeight));
    
    m_pCursorSprite = CCSprite::create(texture);
    CCSize winSize = getContentSize();
    m_cursorPos = ccp(0, winSize.height / 2);
    m_pCursorSprite->setPosition(m_cursorPos);
    this->addChild(m_pCursorSprite);

    m_pCursorAction = CCRepeatForever::create((CCActionInterval *) CCSequence::create(CCFadeOut::create(0.25f), CCFadeIn::create(0.25f), NULL));

    m_pCursorSprite->runAction(m_pCursorAction);
    m_pCursorSprite->setVisible(false);

    m_pInputText = new std::string();

    delete [] pixels;
}

bool CursorTextField::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_beginPos = pTouch->locationInView();
    m_beginPos = CCDirector::sharedDirector()->convertToGL(m_beginPos);

    return true;
}

CCRect CursorTextField::getRect()
{
    CCSize size;
    if (&m_designedSize != NULL) {
         size = m_designedSize;
    }else {
        size = getContentSize();
    }

    CCRect rect = CCRectMake(0 - size.width * getAnchorPoint().x, 0 - size.height * getAnchorPoint().y, size.width, size.height);
    return  rect;
}

//點擊彈出輸入法的範圍
void CursorTextField::setDesignedSize(cocos2d::CCSize size)
{
    m_designedSize = size;
}

CCSize CursorTextField::getDesignedSize()
{
    return m_designedSize;
}

bool CursorTextField::isInTextField(cocos2d::CCTouch *pTouch)
{
    CCPoint pToushPos = convertTouchToNodeSpaceAR(pTouch);
    return CCRect::CCRectContainsPoint(getRect(), pToushPos);
}

void CursorTextField::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint endPos = pTouch->locationInView();
    endPos = CCDirector::sharedDirector()->convertToGL(endPos);

    //判斷是否點擊
    if (::abs(endPos.x - m_beginPos.x) > DELTA || 
        ::abs(endPos.y - m_beginPos.y) > DELTA) 
    {
        //沒有點擊
        m_beginPos.x = m_beginPos.y = -1;
        
        return;
    }

    CCLOG("width: %f, height: %f.", getContentSize().width, getContentSize().height);

    //判斷是打開或關閉IME
    isInTextField(pTouch) ? openIME() : closeIME();
}

bool CursorTextField::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
    if (m_pInputText->empty()) {
        return false;
    }

    m_pCursorSprite->setPositionX(getContentSize().width);

    return false;
}

bool CursorTextField::onTextFieldInsertText(cocos2d::CCTextFieldTTF *pSender, const char *text, int nLen)
{
    CCLOG("Width: %f", pSender->getContentSize().width);
    CCLOG("Text: %s", text);
    CCLOG("Length: %d", nLen);

    std::string tempStr = m_pInputText->substr();
    tempStr.append(text);
    if (tempStr.length() > m_limitNum) {
        return true;
    }

    m_pInputText->append(text);

    if (isPsw)
    {
        std::string tempStr;
        std::string sText(*m_pInputText);
        
        for(int i = 0; i < m_pInputText->size(); i++)
        {
            tempStr.append("*");
        }
        setString(tempStr.c_str(), sText.c_str());
    }
    else
    {
        std::string tempStr(*m_pInputText);
        std::string sText(*m_pInputText);

        int iInputLen = m_pInputText->length();

        if(iInputLen > m_disLimitNum)
        {
            tempStr = m_pInputText->substr(iInputLen - m_disLimitNum, m_disLimitNum);
        }
        setString(tempStr.c_str(), sText.c_str());
    }

    m_pCursorSprite->setPositionX(getContentSize().width);

    return true;
}


bool CursorTextField::onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *pSender, const char *delText, int nLen)
{
    m_pInputText->resize(m_pInputText->size() - nLen);
    CCLog(m_pInputText->c_str());

    if (isPsw)
    {
        std::string tempStr;
        std::string sText(*m_pInputText);
        for (int i = 0; i < m_pInputText->size(); i++)
        {
            tempStr.append("*");
        }
        setString(tempStr.c_str(), sText.c_str());
    }
    else
    {
        std::string tempStr(*m_pInputText);
        std::string sText(*m_pInputText);
        int iInputLen = m_pInputText->length();

        if(iInputLen > m_disLimitNum)
        {
            tempStr = m_pInputText->substr(iInputLen - m_disLimitNum, m_disLimitNum);
        }
        setString(tempStr.c_str(), sText.c_str());
    }

    m_pCursorSprite->setPositionX(getContentSize().width);

    if (m_pInputText->empty()) {
        m_pCursorSprite->setPositionX(0);
    }

    return true;
}

bool CursorTextField::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
    return false;
}

void CursorTextField::openIME()
{
    m_pCursorSprite->setVisible(true);
    this->attachWithIME();
}

void CursorTextField::closeIME()
{
    m_pCursorSprite->setVisible(false);
    this->detachWithIME();
}

void CursorTextField::onExit()
{
    this->detachWithIME();
    CCTextFieldTTF::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool CursorTextField::getIsPsw()
{
    return isPsw;
}

//set password * or real words
void CursorTextField::setIsPsw( bool bFlag)
{
    isPsw = bFlag;
}

int CursorTextField::getLimitNum()
{
    return m_limitNum;
}

//set string limit
void CursorTextField::setLimitNum(int limitNum)
{
    m_limitNum = limitNum;
}

int CursorTextField::getDiaplayLimitNum()
{
    return m_disLimitNum;
}

//set string limit
void CursorTextField::setDiaplayLimitNum(int disLimitNum)
{
    m_disLimitNum = disLimitNum;
}

void CursorTextField::setString(const char *displayTx, const char* inputTx)
{
    CC_SAFE_DELETE(m_pInputText);

    if (inputTx)
    {
        m_pInputText = new std::string(inputTx);
    }
    else
    {
        m_pInputText = new std::string;
    }

    // if there is no input text, display placeholder instead
    if (! m_pInputText->length())
    {
        CCLabelTTF::setString(m_pPlaceHolder->c_str());
    }
    else
    {
        CCLabelTTF::setString(displayTx);
    }
    m_nCharCount = calcCharCount(m_pInputText->c_str());
}

void CursorTextField::setColor(const ccColor3B& color3)
{
    m_sColor = m_sColorUnmodified = color3;

    if (m_bOpacityModifyRGB)
    {
        m_sColor.r = color3.r * m_nOpacity/255.0f;
        m_sColor.g = color3.g * m_nOpacity/255.0f;
        m_sColor.b = color3.b * m_nOpacity/255.0f;
    }

    updateColor();
    m_pCursorSprite->setColor(color3);
}