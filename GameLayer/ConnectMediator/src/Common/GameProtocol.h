#ifndef _GAMEPROTOCOL_H_
#define _GAMEPROTOCOL_H_

/// 卡的種類
enum CARD_TYPE
{
    CARD_TYPE_MEMBER,                           //< 會員. 唯一識別碼
    CARD_TYPE_MAX,
};

/// 遊戲名稱
enum GAME_TYPE
{
    GAME_TYPE_LOBBY,                            //< 大廳
    GAME_TYPE_ALL = GAME_TYPE_LOBBY,            //< 指所有遊戲
    GAME_TYPE_HOLDEM,                           //< 德州撲克
    GAME_TYPE_BLACKJACK,                        //< 二十一點
    GAME_TYPE_BACARRA,                          //< 百家樂
    GAME_TYPE_DONDEN,                           //< 滿貫大亨
    GAME_TYPE_SEVEN_PK,                         //< 7PK
    GAME_TYPE_SICBO,                            //< 骰寶
    GAME_TYPE_BINGO_PLANET,                     //< 賓果行星
    GAME_TYPE_BEAST_BAR,                        //< 動物星球
    GAME_TYPE_BIG2,                             //< 大老二
    GAME_TYPE_LANDOWNER,                        //< 鬥地主
    GAME_TYPE_TAKE_SEVEN,                       //< 排七
    GAME_TYPE_THIRTEEN,                         //< 十三支
    GAME_TYPE_TW4MJ16,                          //< 台灣四人十六張麻將
    GAME_TYPE_TW2MJ16,                          //< 台灣二人十六張麻將
    GAME_TYPE_FRUIT_BAR,                        //< 水果盤
    GAME_TYPE_SMARIO,                           //< 小瑪莉
    GAME_TYPE_FIVE_PK,                          //< 5PK
    GAME_TYPE_MAX,
};

/// 遊戲模式
enum GAME_GRP_MODE
{
    GAME_GRP_MODE_AUTO_LV1,                                                         ///< LV1, GAME_GRP_MODE_TYPE_AUTO
    GAME_GRP_MODE_AUTO_LV2,                                                     ///< LV2, GAME_GRP_MODE_TYPE_AUTO
    GAME_GRP_MODE_AUTO_LV3,                                               ///< LV3, GAME_GRP_MODE_TYPE_AUTO
    GAME_GRP_MODE_AUTO_LV4,                                                         ///< LV4, GAME_GRP_MODE_TYPE_AUTO
    GAME_GRP_MODE_CUSTOM,                                                               ///< GAME_GRP_MODE_CUSTOM, GAME_GRP_MODE_TYPE_CUSTOM
    GAME_GRP_MODE_MAX,
    GAME_GRP_MODE_AUTO_NUM = GAME_GRP_MODE_CUSTOM - GAME_GRP_MODE_AUTO_LV1, ///< The number of auto group
};

/// 貨幣的種類
enum COIN_TYPE
{
    COIN_TYPE_V_COIN,                                                                       ///< 有價幣
    COIN_TYPE_F_COIN,                                                                       ///< 免費幣
    COIN_TYPE_MAX,
};

#endif //_GAMEPROTOCOL_H_
