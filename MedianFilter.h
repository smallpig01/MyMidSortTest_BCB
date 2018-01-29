//---------------------------------------------------------------------------
#ifndef MedianFilterH
#define MedianFilterH
#define SWAP(x,y) {int t; t = x; x = y; y = t;}
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
void DoMedianFilter(BYTE* pSrcImagePtr,int Width,int Height,int SrcRowPitch, int ColorDepth, BYTE* pDstImagePtr,int DstRowPitch, int MedianFilterMethod);
void DoSelectionMedianFilter(BYTE* pSrcImagePtr,int Width,int Height,int SrcRowPitch, int ColorDepth, BYTE* pDstImagePtr,int DstRowPitch);
void DoQsortMedianFilter(BYTE* pSrcImagePtr,int Width,int Height,int SrcRowPitch, int ColorDepth,BYTE* pDstImagePtr,int DstRowPitch);
int MyMakeMaskPtrShiftArr(int MaskPtrShiftArr[], int MaskWH, int ImageRowPitch);
void quickSort(byte number[], int left, int right);
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------

