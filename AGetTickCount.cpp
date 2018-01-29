//---------------------------------------------------------------------------
#pragma hdrstop
#include "AGetTickCount.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
double __fastcall AGetTickCount()  //�ۭq��ơA���odouble���A�ɶ�
{
    LARGE_INTEGER C_time,F_time;              //�ŧi LARGE_INTEGER ���A�ܼ�
    ::QueryPerformanceFrequency(&F_time);  //���oCPU�W�v�ɶ�
    ::QueryPerformanceCounter(&C_time);      //���o�ثe�ɶ�
    return (double)C_time.QuadPart / (double)F_time.QuadPart * 1000.0;  //�ثe�ɶ� / CPU�W�v*1000 = (ms)
}
//---------------------------------------------------------------------------
