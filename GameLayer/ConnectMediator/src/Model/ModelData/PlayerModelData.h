#ifndef _PLAYERMODELDATA_H_
#define _PLAYERMODELDATA_H_

#include "Type.h"
#include "IData.h"
#include <wchar.h>

using BaseLayer::Common::IData;

struct PlayerModelData : public IData
{
    PlayerModelData()
    {

    }

    /**
     * 更新點數。
     * 輸入小於零的數值表示該欄位不更新
     * @param   fpoint       玩家免費點數。
     * @param   vpoint       玩家有價點數
     * @param   wpoint       玩家戰點
     */
    void UpdatePoint(int fpoint = -1, int vpoint = -1, int wpoint = -1)
    {
        if(fpoint > 0)
        {
            nFPoints = fpoint;
        }

        if(vpoint > 0)
        {
            nVPoints = vpoint;
        }

        if(wpoint > 0)
        {
            nWarPoints = wpoint;
        }

        //TODO:更新點數的事件
    }

    /**
     * 更新經驗值。
     * @param   exp         玩家經驗值
     * @param   bonus       玩家累積紅利
     * @param   level       玩家等級
     * @param   levelName   等級名稱
     */
    void updateExperience(int exp, int bonus, int level, wchar_t *LevelName)
    {
        nExps = exp;
        nBonus = bonus;
        Level = level;

        if(wcslen(LevelName) > 0 && wcslen(LevelName) <= MAX_LEVELNAME)
        {
#if defined(_WIN32) || defined(_WIN64)
            wcscpy_s(m_szLevelName, wcslen(LevelName), LevelName);
#else
            wcsncpy(m_szLevelName, LevelName, wcslen(LevelName));
#endif  //defined(_WIN32) || defined(_WIN64)
        }
        //TODO:更新經驗的事件
    }

    Dword       PLUniID;                            //< 玩家的唯一識別碼
    int         nExps;                              //< 玩家的經驗值
    int         nVPoints;                           //< 玩家的有價點數
    int         nFPoints;                           //< 玩家的免費點數
    int         nWarPoints;                         //< 玩家的戰點
    int         nBonus;                             //< 玩家的累積紅利
    int         Level;                              //< 玩家的等級
    int         PointRatio;                         //< 點數比值
    Dword       GameTypeFlags;                      //< 可執行的遊戲類型
    Dword       GameModeFlasg;                      //< 可執行的遊戲模式
    wchar_t     m_szAccount[MAX_ACCOUNT];           //< 玩家帳號
    wchar_t     m_szNickName[MAX_NICKNAME];         //< 玩家暱稱
    wchar_t     m_szLevelName[MAX_LEVELNAME];       //< 等級名稱
    wchar_t     m_szPicURL[MAX_PIC_LENGTH];         //< 照片連結網址
};

#endif //_PLAYERMODELDATA_H_
