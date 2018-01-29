//---------------------------------------------------------------------------
#pragma hdrstop
#include "MedianFilter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
void DoMedianFilter(BYTE* pSrcImagePtr,int Width,int Height,int SrcRowPitch, int ColorDepth,BYTE* pDstImagePtr,int DstRowPitch, int MedianFilterMethod)
{
    switch(MedianFilterMethod)
    {
        case 0:
          DoSelectionMedianFilter(pSrcImagePtr,Width,Height,SrcRowPitch,ColorDepth,pDstImagePtr,DstRowPitch);
          break;
        case 1:
          DoQsortMedianFilter(pSrcImagePtr,Width,Height,SrcRowPitch,ColorDepth,pDstImagePtr,DstRowPitch);
          break;
    }
}
//---------------------------------------------------------------------------
void DoSelectionMedianFilter(BYTE* pSrcImagePtr,int Width,int Height,int SrcRowPitch, int ColorDepth, BYTE* pDstImagePtr,int DstRowPitch)
{
    int EndWidthIndex  = Width *ColorDepth -1;
    int EndHeightIndex = Height            -1;
    int MaskPtrShiftArr[9];
    byte DataArr[9];
    int LastV;                  //紀錄每次判斷完成後的數值, 以利下次預判方向性
    bool IsNextBiggerValue;     //紀錄上一次判斷完成後, 合理值的方向性. true:下次需往大值, false:下次需往小值
    int MinValue;               //內斂值 (預設 -2~256)
    int MaxValue;               //內斂值 (預設 -2~256)
    bool FailWhenSELClassify;   //蒐集 S|E|L 數量時, 初判是否符合中位值(位置). true:初判錯誤, false:初判正確.
    int SmallerCount,EqualCount,LargerCount;  //S|E|L 分類計數
    int MaskLength=9;           //Mask (3*3)

    //Make Ptr Shift Arr
    //MyMakeMaskPtrShiftArr(MaskPtrShiftArr,3,SrcRowPitch);
    int OrgPosPtr  = 0-1*SrcRowPitch-ColorDepth;
	for(int i=0;i<MaskLength;i++)
		MaskPtrShiftArr[i]=OrgPosPtr+(i/3)*SrcRowPitch+(i%3)*ColorDepth;

    //補邊值
    BYTE* TempSrcImagePtr;
    BYTE* TempDstImagePtr;
    //Top Line
    TempSrcImagePtr = pSrcImagePtr;
    TempDstImagePtr = pDstImagePtr;
    memcpy(TempDstImagePtr,TempSrcImagePtr,Width*ColorDepth);
    //Bot Line
    TempSrcImagePtr = pSrcImagePtr+ (EndHeightIndex)*SrcRowPitch;
    TempDstImagePtr = pDstImagePtr+ (EndHeightIndex)*DstRowPitch;
    memcpy(TempDstImagePtr,TempSrcImagePtr,Width*ColorDepth);
    //LeftRight Line
    TempSrcImagePtr = pSrcImagePtr+ SrcRowPitch;
    TempDstImagePtr = pDstImagePtr+ DstRowPitch;
    for(int h=1;h<Height-1;h++)
    {
        if(ColorDepth==1)
        {
          TempDstImagePtr[0]             = TempSrcImagePtr[0];
          TempDstImagePtr[EndWidthIndex] = TempSrcImagePtr[EndWidthIndex];
        }
        else
        {
          TempDstImagePtr[0]             = TempSrcImagePtr[0];
          TempDstImagePtr[1]             = TempSrcImagePtr[1];
          TempDstImagePtr[2]             = TempSrcImagePtr[2];
          TempDstImagePtr[EndWidthIndex-2] = TempSrcImagePtr[EndWidthIndex-2];
          TempDstImagePtr[EndWidthIndex-1] = TempSrcImagePtr[EndWidthIndex-1];
          TempDstImagePtr[EndWidthIndex  ] = TempSrcImagePtr[EndWidthIndex  ];
        }

        TempSrcImagePtr += SrcRowPitch;
        TempDstImagePtr += DstRowPitch;
    }

    //MedianFilter
    pSrcImagePtr+=SrcRowPitch+ColorDepth;
    pDstImagePtr+=DstRowPitch+ColorDepth;
    for(int h=1;h<EndHeightIndex;h++)
    {
      for(int w=1;w<EndWidthIndex;w++)
      {
        //從 pSrcImagePtr 取 DataArr[]
        //GetMaskValue(pSrcImagePtr,9,MaskPtrShiftArr,DataArr);
        DataArr[0]=pSrcImagePtr[MaskPtrShiftArr[0]];
        DataArr[1]=pSrcImagePtr[MaskPtrShiftArr[1]];
        DataArr[2]=pSrcImagePtr[MaskPtrShiftArr[2]];
        DataArr[3]=pSrcImagePtr[MaskPtrShiftArr[3]];
        DataArr[4]=pSrcImagePtr[MaskPtrShiftArr[4]];
        DataArr[5]=pSrcImagePtr[MaskPtrShiftArr[5]];
        DataArr[6]=pSrcImagePtr[MaskPtrShiftArr[6]];
        DataArr[7]=pSrcImagePtr[MaskPtrShiftArr[7]];
        DataArr[8]=pSrcImagePtr[MaskPtrShiftArr[8]];

        //MidSort
        //MyMidSort(DataArr, 9, pDstImagePtr[0], Runs);
        LastV               = -1;     //紀錄每次判斷完成後的數值, 以利下次預判方向性
        IsNextBiggerValue   = true;   //紀錄上一次判斷完成後, 合理值的方向性. true:下次需往大值, false:下次需往小值
        MinValue            = -2;     //內斂值 (預設 -2~256)
        MaxValue            = 256;    //內斂值 (預設 -2~256)
        FailWhenSELClassify = false;  //蒐集 S|E|L 數量時, 初判是否符合中位值(位置). true:初判錯誤, false:初判正確.

        for(int i=0;i<MaskLength;i++)
          { //將陣列位置 1-9 進行比較

            // Step1. 預判本次合理性
            if(( IsNextBiggerValue && DataArr[i]<LastV) || //上一次判斷後, 需往大值方向找, 但本次值小於上次值
               (!IsNextBiggerValue && DataArr[i]>LastV) || //上一次判斷後, 需往小值方向找, 但本次值大於上次值
               DataArr[i] <= MinValue                   || //方向正確, 但小於內斂值
               DataArr[i] >= MaxValue)                     //方向正確, 但大於內斂值
            {  //本次判斷述值不合理, 不予判斷
               continue;
            }

            // Step2. S|E|L 數量計算
            // 可參考以下 func 內容, 但使用 func 會增加運算時間, 故不使用 func
            // MySELClassfy(Arr, iArrLength, i, SmallerCount, EqualCount, LargerCount);
            SmallerCount=0; EqualCount=0; LargerCount=0;  //S|E|L 歸零
            FailWhenSELClassify = false;
            for(int j=0;j<MaskLength;j++)
            {
              if(j==i) continue;             //當判斷位置為自己時, 跳過
              if(DataArr[j]<DataArr[i])       //S|E|L數量判斷 : 是否為Smaller
              {
                if(++SmallerCount>4)          //中位值判斷式1:較小數量過多 -> Fail
                {
                  IsNextBiggerValue=false;
                  MaxValue = DataArr[i];
                  FailWhenSELClassify = true;
                  break;
                }
              }
              else if(DataArr[j]>DataArr[i])  //S|E|L數量判斷 : 是否為Larger
              {
                if(++LargerCount>4)           //中位值判斷式2:較大數量過多 -> Fail
                {
                  IsNextBiggerValue=true;
                  MinValue = DataArr[i];
                  FailWhenSELClassify = true;
                  break;
                }
              }
              else                           //S|E|L數量判斷 : 確認為Equal
              {
                if(++EqualCount>3)            //中位值判斷式3:相等數量過多 -> Get中位值(位置)
                {
                  pDstImagePtr[0] = DataArr[i];
                  break;
                }
              }
            }

            //Step3. 判斷 S|E|L 數量合理性, 並決定是否為中位數
            if(FailWhenSELClassify==true)
            {
              LastV = DataArr[i];
              continue;
            }
            else if(SmallerCount==LargerCount  ||  //中位值判斷式4: 除 Equal 量以外, 左右相等, 中間值確定
                    SmallerCount+EqualCount>=6 ||  //中位值判斷式5: "較小+相等" 量比重過大, 中間值確定
                    LargerCount +EqualCount>=6 ||  //中位值判斷式6: "較大+相等" 量比重過大, 中間值確定
                    (SmallerCount>LargerCount  && SmallerCount==LargerCount +EqualCount) || //中位值判斷式7: 當 較小量>較大量, 且較小量=相等量+較大量, 中間值確定於相等量內
                    (LargerCount >SmallerCount && LargerCount ==SmallerCount+EqualCount))   //中位值判斷式8: 當 較大量>較小量, 且較大量=相等量+較小量, 中間值確定於相等量內
            {
              pDstImagePtr[0] = DataArr[i]; //中位值(位置)成功求得
              break;
            }
            break;
          }

        //移動 pSrcImagePtr/pDstImagePtr 位置
        pSrcImagePtr++;
        pDstImagePtr++;
      }
      //移動 pSrcImagePtr/pDstImagePtr 位置
      pSrcImagePtr+=SrcRowPitch-EndWidthIndex+1;
      pDstImagePtr+=DstRowPitch-EndWidthIndex+1;
    }
}
//---------------------------------------------------------------------------
void DoQsortMedianFilter(BYTE* pSrcImagePtr,int Width,int Height,int SrcRowPitch, int ColorDepth,BYTE* pDstImagePtr,int DstRowPitch)
{
    int EndWidthIndex  = Width *ColorDepth -1;
    int EndHeightIndex = Height            -1;
    int MaskPtrShiftArr[9];
    byte DataArr[9];
    int MaskLength=9;           //Mask (3*3)

    //Make Ptr Shift Arr
    //MyMakeMaskPtrShiftArr(MaskPtrShiftArr,3,SrcRowPitch); //測試func
    int OrgPosPtr  = 0-1*SrcRowPitch-ColorDepth;
	for(int i=0;i<MaskLength;i++)
        MaskPtrShiftArr[i]=OrgPosPtr + (i/3)*SrcRowPitch + (i%3)*ColorDepth;

    //補邊值
    BYTE* TempSrcImagePtr;
    BYTE* TempDstImagePtr;
    //Top Line
    TempSrcImagePtr = pSrcImagePtr;
    TempDstImagePtr = pDstImagePtr;
    memcpy(TempDstImagePtr,TempSrcImagePtr,Width);
    //Bot Line
    TempSrcImagePtr = pSrcImagePtr+ (EndHeightIndex)*SrcRowPitch;
    TempDstImagePtr = pDstImagePtr+ (EndHeightIndex)*DstRowPitch;
    memcpy(TempDstImagePtr,TempSrcImagePtr,Width);
    //LeftRight Line
    TempSrcImagePtr = pSrcImagePtr+ SrcRowPitch;
    TempDstImagePtr = pDstImagePtr+ DstRowPitch;
    for(int h=1;h<Height-1;h++)
    {
        TempDstImagePtr[0]             = TempSrcImagePtr[0];
        TempDstImagePtr[EndWidthIndex] = TempSrcImagePtr[EndWidthIndex];
        TempSrcImagePtr += SrcRowPitch;
        TempDstImagePtr += DstRowPitch;
    }

    //MedianFilter(Qsort)
    pSrcImagePtr+=SrcRowPitch+1;
    pDstImagePtr+=DstRowPitch+1;
    for(int h=1;h<EndHeightIndex;h++)
    {
      for(int w=1;w<EndWidthIndex;w++)
      {
        //從 pSrcImagePtr 取 DataArr[]
        //GetMaskValue(pSrcImagePtr,9,MaskPtrShiftArr,DataArr);
        DataArr[0]=pSrcImagePtr[MaskPtrShiftArr[0]];
        DataArr[1]=pSrcImagePtr[MaskPtrShiftArr[1]];
        DataArr[2]=pSrcImagePtr[MaskPtrShiftArr[2]];
        DataArr[3]=pSrcImagePtr[MaskPtrShiftArr[3]];
        DataArr[4]=pSrcImagePtr[MaskPtrShiftArr[4]];
        DataArr[5]=pSrcImagePtr[MaskPtrShiftArr[5]];
        DataArr[6]=pSrcImagePtr[MaskPtrShiftArr[6]];
        DataArr[7]=pSrcImagePtr[MaskPtrShiftArr[7]];
        DataArr[8]=pSrcImagePtr[MaskPtrShiftArr[8]];

        //QSort
        quickSort(DataArr,0,8);
        pDstImagePtr[0] = DataArr[4];

        //移動 pSrcImagePtr/pDstImagePtr 位置
        pSrcImagePtr++;
        pDstImagePtr++;
      }
      //移動 pSrcImagePtr/pDstImagePtr 位置
      pSrcImagePtr+=SrcRowPitch-EndWidthIndex+1;
      pDstImagePtr+=DstRowPitch-EndWidthIndex+1;
    }
}
//---------------------------------------------------------------------------
int MyMakeMaskPtrShiftArr(int MaskPtrShiftArr[], int MaskWH, int ImageRowPitch)
{   //製作 MaskPtr 偏移表, 供 Mask 撈值用.
    //[I/O] MaskShiftArr  : 提供一陣列, 存放 MaskPtrShift 值
    //[I/ ] MaskWH        : MaskPtrShift Table 寬高
    //[I/ ] ImageRowPitch : Mask Parent Data ROW 寬度
    //[ /O] ReturnValue   : MyMakeMaskPtrShiftArr 製作結果
    //                      [1]: 製作成功
    //                      [2]: Error-Mask寬度不為奇數
    //                      [3]: Error-影像寬度Pitch小於Mask寬度

    //資料防呆
	if(MaskWH%2==0)
	    return 2; // ErrorCode[2]: Mask寬度不為奇數
    if(ImageRowPitch<MaskWH)
	    return 3; // ErrorCode[3]: 影像寬度Pitch小於Mask寬度

	int HalfMaskWH = (MaskWH-1)/2;
	int Length     = MaskWH*MaskWH;
    int OrgPosPtr  = 0-HalfMaskWH*ImageRowPitch-HalfMaskWH;

	for(int i=0;i<Length;i++)
	{
		MaskPtrShiftArr[i]=OrgPosPtr+(i/MaskWH)*ImageRowPitch+(i%MaskWH);
	}

 	return 1; //製作成功
}
//---------------------------------------------------------------------------
void quickSort(byte number[], int left, int right)
{
    if(left < right) {
        int s = number[(left+right)/2];
        int i = left - 1;
        int j = right + 1;

        while(1) {
            while(number[++i] < s);
            while(number[--j] > s);
            if(i >= j)
                break;
            SWAP(number[i], number[j]);
        }
        quickSort(number, left, i-1);
        quickSort(number, j+1, right);
    }
}
//---------------------------------------------------------------------------
