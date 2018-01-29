//---------------------------------------------------------------------------
#pragma hdrstop
#include "AGetTickCount.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
double __fastcall AGetTickCount()  //自訂函數，取得double型態時間
{
    LARGE_INTEGER C_time,F_time;              //宣告 LARGE_INTEGER 型態變數
    ::QueryPerformanceFrequency(&F_time);  //取得CPU頻率時間
    ::QueryPerformanceCounter(&C_time);      //取得目前時間
    return (double)C_time.QuadPart / (double)F_time.QuadPart * 1000.0;  //目前時間 / CPU頻率*1000 = (ms)
}
//---------------------------------------------------------------------------
