//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AxAltairUDrv_OCX"
#pragma link "AxOvkBase_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
  int SmallerCount;
  int EqualCount;
  int LargerCount;
  int RunsCount;
  int LastV;
  bool IsNextBiggerValue;
  int MaxValue;
  int MinValue;
  int GoldenSampleV;
  bool FailWhenSELClassify;
  byte Mid;
  int Runs;
  double MyMidSortT;
  double MyMidSortAvgT;
  double QuickSortT;
  double QuickSortAvgT;
  int StatrW;
  int StartH;
  int EndW;
  int EndH;
  int CaptureCount;
  int MyMidQuickCount;
  int QuickSortQuickCount;
//---------------------------------------------------------------------------
void quickSort(byte number[], int left, int right) {
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
int MyMidSort(byte Arr[], int iArrLength, byte &OutMidPosV, int &NumOfRunsProcessed)
{
  //[I]   Arr[]              : Arr陣列資料 (180105:限定為[9])
  //[I]   iArrLength         : Arr陣列長度 (180105:限定為[9])
  //[I/O] OutMidPosV         : 輸出中位值(位置)結果
  //[I/O] NumOfRunsProcessed : 輸出本次中位值(位置)挑選運算次數
  //[O]   ReturnValue        : [0]: 失敗 - MyMidSort規則跑完, 但無法判斷中位值狀態
  //                           [1]: 成功 - 中位值(位置)成功求得
  //                           [2]: 失敗 - 出現未知 S|E|L 排列內容, 需回報 Arr[] 重新檢視判斷式

  RunsCount           = 0;      //紀錄本次 MidSort 跑過幾次算法 (運算量略 <= RunsCount(N-1)
  LastV               = -1;     //紀錄每次判斷完成後的數值, 以利下次預判方向性
  IsNextBiggerValue   = true;   //紀錄上一次判斷完成後, 合理值的方向性. true:下次需往大值, false:下次需往小值
  MinValue            = -2;     //內斂值 (預設 -2~256)
  MaxValue            = 2560;    //內斂值 (預設 -2~256)
  FailWhenSELClassify = false;  //蒐集 S|E|L 數量時, 初判是否符合中位值(位置). true:初判錯誤, false:初判正確.


  for(int i=0;i<iArrLength;i++)
  { //將陣列位置 1-9 進行比較

    // Step1. 預判本次合理性
    if(( IsNextBiggerValue && Arr[i]<LastV) || //上一次判斷後, 需往大值方向找, 但本次值小於上次值
       (!IsNextBiggerValue && Arr[i]>LastV) || //上一次判斷後, 需往小值方向找, 但本次值大於上次值
       Arr[i] <= MinValue                   || //方向正確, 但小於內斂值
       Arr[i] >= MaxValue)                     //方向正確, 但大於內斂值
    {  //本次判斷述值不合理, 不予判斷
       continue;
    }

    // Step2. S|E|L 數量計算
    // 可參考以下 func 內容, 但使用 func 會增加運算時間, 故不使用 func
    // MySELClassfy(Arr, iArrLength, i, SmallerCount, EqualCount, LargerCount);
    RunsCount++;                                  
    SmallerCount=0; EqualCount=0; LargerCount=0;  //S|E|L 歸零
    int GoldenSampleV = Arr[i];                   //
    FailWhenSELClassify = false;
    for(int j=0;j<iArrLength;j++)
    {
      if(j==i) continue;             //當判斷位置為自己時, 跳過
      if(Arr[j]<GoldenSampleV)       //S|E|L數量判斷 : 是否為Smaller
      {
        if(SmallerCount>=4)          //中位值判斷式1:較小數量過多 -> Fail
        {
          IsNextBiggerValue=false;
          MaxValue = Arr[i];
          FailWhenSELClassify = true;
          break;
        }
        else
          SmallerCount++;
      }
      else if(Arr[j]>GoldenSampleV)  //S|E|L數量判斷 : 是否為Larger
      {
        if(LargerCount>=4)           //中位值判斷式2:較大數量過多 -> Fail
        {
          IsNextBiggerValue=true;
          MinValue = Arr[i];
          FailWhenSELClassify = true;
          break;
        }
        else
          LargerCount++;
      }
      else                           //S|E|L數量判斷 : 確認為Equal
      {
        if(EqualCount>=3)            //中位值判斷式3:相等數量過多 -> Get中位值(位置)
        {
          OutMidPosV = Arr[i];
          NumOfRunsProcessed = RunsCount;
          return 1;
        }
        else
          EqualCount++;
      }
    }

    //Step3. 判斷 S|E|L 數量合理性, 並決定是否為中位數
    if(FailWhenSELClassify==true)
    {
      LastV = Arr[i];
      continue;
    }
    else if(SmallerCount==LargerCount  ||  //中位值判斷式4: 除 Equal 量以外, 左右相等, 中間值確定
            SmallerCount+EqualCount>=6 ||  //中位值判斷式5: "較小+相等" 量比重過大, 中間值確定
            LargerCount +EqualCount>=6 ||  //中位值判斷式6: "較大+相等" 量比重過大, 中間值確定
            (SmallerCount>LargerCount  && SmallerCount==LargerCount +EqualCount) || //中位值判斷式7: 當 較小量>較大量, 且較小量=相等量+較大量, 中間值確定於相等量內
            (LargerCount >SmallerCount && LargerCount ==SmallerCount+EqualCount))   //中位值判斷式8: 當 較大量>較小量, 且較大量=相等量+較小量, 中間值確定於相等量內
    {
      OutMidPosV = Arr[i];
      NumOfRunsProcessed = RunsCount;
      return 1;  //中位值(位置)成功求得
    }
    return 2; //出現未知 S|E|L 排列內容
  }
  return 0;   //MyMidSort規則跑完, 但無法判斷中位值狀態
}
//---------------------------------------------------------------------------
double AGetTickCount()  //自訂函數，取得double型態時間
{
    LARGE_INTEGER C_time,F_time;              //宣告 LARGE_INTEGER 型態變數
    ::QueryPerformanceFrequency(&F_time);  //取得CPU頻率時間
    ::QueryPerformanceCounter(&C_time);      //取得目前時間
    return (double)C_time.QuadPart / (double)F_time.QuadPart * 1000.0;  //目前時間 / CPU頻率*1000 = (ms)
}
//---------------------------------------------------------------------------
char GetMaskValue(byte* MaskCenterPtr, int MaskLength, int MaskPtrShiftArr[], byte OutArr[])
{   //利用 MaskCenterPtr + MaskPtrShiftArr 取出 MaskParentData 中的值
    //[I/ ] MaskCenterPtr   : 設定 Mask 中心指標位置 
    //[I/ ] MaskLength      : 輸入 Mask 資料長度
    //[I/ ] MaskPtrShiftArr : 設定參考 MaskPtr 偏移表
    //[ /O] OutArr          : 輸出 MaskParentData 值
    //[ /O] ReturnValue     : 取值結果. [1]: 取值成功
    for(int i=0;i<MaskLength;i++)
        OutArr[i]=MaskCenterPtr[MaskPtrShiftArr[i]];
    return 1;
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
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    AxAltairU1->QuickCreateChannel();
    AxImageBW81->LoadFile(L"C:\\Zoo-Test2.bmp");
    AxImageBW81->DrawImage((long)Image1->Canvas->Handle,1,1,0,0);
    Image1->Refresh();
    AxImageBW82->SetSize(AxImageBW81->ImageWidth,AxImageBW81->ImageHeight);
    AxImageBW82->SetSize(AxImageBW81->ImageWidth,AxImageBW81->ImageHeight);
}
//---------------------------------------------------------------------------
void TForm1::PrintArr(byte Arr[])
{
   Label1->Caption = (String)Arr[0] + ", " + (String)Arr[1] + ", " + (String)Arr[2];
   Label2->Caption = (String)Arr[3] + ", " + (String)Arr[4] + ", " + (String)Arr[5];
   Label3->Caption = (String)Arr[6] + ", " + (String)Arr[7] + ", " + (String)Arr[8];
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{

  int Times =640*480;
  int RandArrTimes = 1000;
  double TotalRuns=0;
  int MidValidCount=0;
  int MidFailCount=0;
  int MyMidFastCount=0;
  int MyMidSlowCount=0;
  String sValidState="X";
  int RandBase = 0;
  int RandRange = 2000;
///////////////////////////////

    int StatrW = 1;
    int StartH = 1;
    int EndW   = AxImageBW81->ImageWidth  -1;
    int EndH   = AxImageBW81->ImageHeight -1;
    int MaskPtrShiftArr[9];
    byte DataArr[9];

    MyMakeMaskPtrShiftArr(MaskPtrShiftArr,3,AxImageBW81->RowPitch);

    byte* ImagePtr;

    long I1I2Diff=AxImageBW82->GetSurfacePtr(0,0) -AxImageBW81->GetSurfacePtr(0,0);

    double T=AGetTickCount();

    for(int i=StartH; i<EndH; i++)
    {
        ImagePtr = (byte*)AxImageBW81->GetSurfacePtr(1,i);
        for(int j=StatrW; j<EndW; j++)
        {
            GetMaskValue(ImagePtr,9,MaskPtrShiftArr,DataArr);

            //MyMidSort(DataArr, 9, Mid, Runs);
            //ImagePtr[I1I2Diff] = Mid;

            //quickSort(DataArr,0,8);
            //ImagePtr[I1I2Diff] = DataArr[4];

            ImagePtr++;


        }
    }



    T=AGetTickCount()-T;
    this->Caption = (String)T;


    AxImageBW82->DrawImage((long)Image2->Canvas->Handle,1,1,0,0);
    Image2->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AxAltairU1SurfaceFilled(TObject *Sender,long SurfaceHandle)
{
    AxImageBW81->SetSurfaceObj(SurfaceHandle);

    //
    int MaskPtrShiftArr[9];
    byte DataArr[9];
    //
    MyMakeMaskPtrShiftArr(MaskPtrShiftArr,3,AxImageBW81->RowPitch);
    byte* ImagePtr;
    long PtrDiff;

    ///
    PtrDiff = AxImageBW82->GetSurfacePtr(0,0) - AxImageBW81->GetSurfacePtr(0,0);
    MyMidSortT=AGetTickCount();
    for(int i=StartH; i<EndH; i++)
    {
        ImagePtr = (byte*)AxImageBW81->GetSurfacePtr(1,i);
        for(int j=StatrW; j<EndW; j++)
        {
            GetMaskValue(ImagePtr,9,MaskPtrShiftArr,DataArr);
            MyMidSort(DataArr, 9, Mid, Runs);
            ImagePtr[PtrDiff] = Mid;
            ImagePtr++;
        }
    }
    MyMidSortT=AGetTickCount()-MyMidSortT;
    MyMidSortAvgT+=MyMidSortT;

    ///
    PtrDiff = AxImageBW83->GetSurfacePtr(0,0) - AxImageBW81->GetSurfacePtr(0,0);
    QuickSortT=AGetTickCount();
    for(int i=StartH; i<EndH; i++)
    {
        ImagePtr = (byte*)AxImageBW81->GetSurfacePtr(1,i);
        for(int j=StatrW; j<EndW; j++)
        {
            GetMaskValue(ImagePtr,9,MaskPtrShiftArr,DataArr);
            quickSort(DataArr,0,8);
            ImagePtr[PtrDiff] = DataArr[4];
            ImagePtr++;
        }
    }
    QuickSortT=AGetTickCount()-QuickSortT;
    QuickSortAvgT+=QuickSortT;

    AxImageBW81->DrawImage((long)Image1->Canvas->Handle,0.5f,0.5f,0,0);
    Image1->Refresh();
    AxImageBW82->DrawImage((long)Image2->Canvas->Handle,1,1,0,0);
    Image2->Refresh();
    AxImageBW83->DrawImage((long)Image3->Canvas->Handle,1,1,0,0);
    Image3->Refresh();

    //
    CaptureCount++;
    Label7->Caption = CaptureCount;
    Label8->Caption = Caption.sprintf("%.3f",MyMidSortT);
    Label9->Caption = Caption.sprintf("%.3f",MyMidSortAvgT/(double)CaptureCount);
    Label10->Caption = CaptureCount;
    Label11->Caption = Caption.sprintf("%.3f",QuickSortT);
    Label12->Caption = Caption.sprintf("%.3f",QuickSortAvgT/(double)CaptureCount);

    if(MyMidSortT<QuickSortT)
     MyMidQuickCount++;
    else
     QuickSortQuickCount++;
    Label16->Caption = MyMidQuickCount;
    Label17->Caption = QuickSortQuickCount;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
    AxImageBW82->SetSize(AxAltairU1->ImageWidth,AxAltairU1->ImageHeight);
    AxImageBW83->SetSize(AxAltairU1->ImageWidth,AxAltairU1->ImageHeight);
    //
    StatrW = 1;
    StartH = 1;
    EndW   = AxAltairU1->ImageWidth  -1;
    EndH   = AxAltairU1->ImageHeight -1;
    //
    MyMidSortT=0;
    MyMidSortAvgT=0;
    QuickSortT=0;
    QuickSortAvgT=0;
    CaptureCount=0;
    //
    MyMidQuickCount=0;
    QuickSortQuickCount=0;
    //
    AxAltairU1->Live();
}
//---------------------------------------------------------------------------

