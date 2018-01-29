//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "Main.h"
#include "MedianFilter.h"
#include "AGetTickCount.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AxAltairUDrv_OCX"
#pragma link "AxOvkBase_OCX"
#pragma link "AxOvkImage_OCX"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------

/*/
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
  int MidSortErrorCount;
/*/

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
/*/
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
  MaxValue            = 256;    //內斂值 (預設 -2~256)
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
        if(++SmallerCount>4)          //中位值判斷式1:較小數量過多 -> Fail
        {
          IsNextBiggerValue=false;
          MaxValue = Arr[i];
          FailWhenSELClassify = true;
          break;
        }
      }
      else if(Arr[j]>GoldenSampleV)  //S|E|L數量判斷 : 是否為Larger
      {
        if(++LargerCount>4)           //中位值判斷式2:較大數量過多 -> Fail
        {
          IsNextBiggerValue=true;
          MinValue = Arr[i];
          FailWhenSELClassify = true;
          break;
        }
      }
      else                           //S|E|L數量判斷 : 確認為Equal
      {
        if(++EqualCount>3)            //中位值判斷式3:相等數量過多 -> Get中位值(位置)
        {
          OutMidPosV = Arr[i];
          NumOfRunsProcessed = RunsCount;
          return 1;
        }
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
/*/
//---------------------------------------------------------------------------
/*/
int MyMidSort(byte* MaskCenterPtr, int PtrShiftArr[], int iArrLength, byte &OutMidPosV)
{
  //[I]   Arr[]              : Arr陣列資料 (180105:限定為[9])
  //[I]   iArrLength         : Arr陣列長度 (180105:限定為[9])
  //[I/O] OutMidPosV         : 輸出中位值(位置)結果
  //[I/O] NumOfRunsProcessed : 輸出本次中位值(位置)挑選運算次數
  //[O]   ReturnValue        : [0]: 失敗 - MyMidSort規則跑完, 但無法判斷中位值狀態
  //                           [1]: 成功 - 中位值(位置)成功求得
  //                           [2]: 失敗 - 出現未知 S|E|L 排列內容, 需回報 Arr[] 重新檢視判斷式

  LastV               = -1;     //紀錄每次判斷完成後的數值, 以利下次預判方向性
  IsNextBiggerValue   = true;   //紀錄上一次判斷完成後, 合理值的方向性. true:下次需往大值, false:下次需往小值
  MinValue            = -2;     //內斂值 (預設 -2~256)
  MaxValue            = 256;    //內斂值 (預設 -2~256)
  FailWhenSELClassify = false;  //蒐集 S|E|L 數量時, 初判是否符合中位值(位置). true:初判錯誤, false:初判正確.

  for(int i=0;i<iArrLength;i++)
  { //將陣列位置 1-9 進行比較


    // Step1. 預判本次合理性
    if(( IsNextBiggerValue && MaskCenterPtr[PtrShiftArr[i]]<LastV) || //上一次判斷後, 需往大值方向找, 但本次值小於上次值
       (!IsNextBiggerValue && MaskCenterPtr[PtrShiftArr[i]]>LastV) || //上一次判斷後, 需往小值方向找, 但本次值大於上次值
       MaskCenterPtr[PtrShiftArr[i]] <= MinValue                   || //方向正確, 但小於內斂值
       MaskCenterPtr[PtrShiftArr[i]] >= MaxValue)                     //方向正確, 但大於內斂值
    {  //本次判斷述值不合理, 不予判斷
       continue;
    }

    // Step2. S|E|L 數量計算
    // 可參考以下 func 內容, 但使用 func 會增加運算時間, 故不使用 func
    // MySELClassfy(Arr, iArrLength, i, SmallerCount, EqualCount, LargerCount);
    SmallerCount=0; EqualCount=0; LargerCount=0;  //S|E|L 歸零
    FailWhenSELClassify = false;


    for(int j=0;j<iArrLength;j++)
    {
      if(j==i) continue;             //當判斷位置為自己時, 跳過
      if(MaskCenterPtr[PtrShiftArr[j]]<MaskCenterPtr[PtrShiftArr[i]])       //S|E|L數量判斷 : 是否為Smaller
      {
        if(++SmallerCount>4)          //中位值判斷式1:較小數量過多 -> Fail
        {
          IsNextBiggerValue=false;
          MaxValue = MaskCenterPtr[PtrShiftArr[i]];
          FailWhenSELClassify = true;
          break;
        }
      }
      else if(MaskCenterPtr[PtrShiftArr[j]]>MaskCenterPtr[PtrShiftArr[i]])  //S|E|L數量判斷 : 是否為Larger
      {
        if(++LargerCount>4)           //中位值判斷式2:較大數量過多 -> Fail
        {
          IsNextBiggerValue=true;
          MinValue = MaskCenterPtr[PtrShiftArr[i]];
          FailWhenSELClassify = true;
          break;
        }
      }
      else                           //S|E|L數量判斷 : 確認為Equal
      {
        if(++EqualCount>3)            //中位值判斷式3:相等數量過多 -> Get中位值(位置)
        {
          OutMidPosV = MaskCenterPtr[PtrShiftArr[i]];
          return 1;
        }
      }
    }

    //Step3. 判斷 S|E|L 數量合理性, 並決定是否為中位數
    if(FailWhenSELClassify==true)
    {
      LastV = MaskCenterPtr[PtrShiftArr[i]];
      continue;
    }
    else if(SmallerCount==LargerCount  ||  //中位值判斷式4: 除 Equal 量以外, 左右相等, 中間值確定
            SmallerCount+EqualCount>=6 ||  //中位值判斷式5: "較小+相等" 量比重過大, 中間值確定
            LargerCount +EqualCount>=6 ||  //中位值判斷式6: "較大+相等" 量比重過大, 中間值確定
            (SmallerCount>LargerCount  && SmallerCount==LargerCount +EqualCount) || //中位值判斷式7: 當 較小量>較大量, 且較小量=相等量+較大量, 中間值確定於相等量內
            (LargerCount >SmallerCount && LargerCount ==SmallerCount+EqualCount))   //中位值判斷式8: 當 較大量>較小量, 且較大量=相等量+較小量, 中間值確定於相等量內
    {
      OutMidPosV = MaskCenterPtr[PtrShiftArr[i]];
      return 1;  //中位值(位置)成功求得
    }

    return 2; //出現未知 S|E|L 排列內容
  }
  return 0;   //MyMidSort規則跑完, 但無法判斷中位值狀態
}
/*/

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
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    AxAltairU1->QuickCreateChannel();
    if(AxAltairU1->ColorFormat == AXAU_COLOR_FORMAT_GREYLEVEL)
      ColorDepth = 1;
    else
      ColorDepth = 3;
    AxImageBW81->LoadFile(L"C:\\Zoo-Test2.bmp");
    AxImageBW81->DrawImage((long)Image1->Canvas->Handle,1,1,0,0);

    AxImageBW82->SetSize(AxImageBW81->ImageWidth,AxImageBW81->ImageHeight);
    AxImageBW82->SetSize(AxImageBW81->ImageWidth,AxImageBW81->ImageHeight);

    AxROIBW81->ParentHandle = AxImageBW81->VegaHandle;
    AxROIBW81->SetPlacement(100,100,100,100);
    AxROIBW81->DrawFrame((long)Image1->Canvas->Handle,1,1,0,0,255);

    Image1->Refresh();
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
  //int RandRange = 2000;
///////////////////////////////

    int StatrW = 1;
    int StartH = 1;
    int EndW   = AxImageBW81->ImageWidth  -1;
    int EndH   = AxImageBW81->ImageHeight -1;
    int MaskPtrShiftArr[9];
    byte DataArr[9];

    MyMakeMaskPtrShiftArr(MaskPtrShiftArr,3,AxImageBW81->RowPitch);

    byte* ImagePtr;

    //long I1I2Diff=AxImageBW82->GetSurfacePtr(0,0) -AxImageBW81->GetSurfacePtr(0,0);

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
    // Initialize variable
    AxImageBW81->SetSurfaceObj(SurfaceHandle);
    SrcPtr      = (byte*)AxImageBW81->GetSurfacePtr(0,0);
    SrcWidth    = AxImageBW81->ImageWidth;
    SrcHeight   = AxImageBW81->ImageHeight;
    SrcRowPitch = AxImageBW81->RowPitch;
    if(SrcWidth!=AxImageBW82->ImageWidth || SrcHeight!=AxImageBW82->ImageHeight)
      AxImageBW82->SetSize(AxImageBW81->ImageWidth, AxImageBW81->ImageHeight);
    if(SrcWidth!=AxImageBW83->ImageWidth || SrcHeight!=AxImageBW83->ImageHeight)
      AxImageBW83->SetSize(AxImageBW81->ImageWidth, AxImageBW81->ImageHeight);

    // Make some Noise
/*
    byte* NoiseValue;
    int   NoisePosition;
    int   NoiseCount = 1000;
    int   ImageLength = SrcWidth*SrcHeight;
    for(int i=0;i<NoiseCount;i++)
    {
      NoiseValue = rand() % 256;
      NoisePosition = rand() % 256;
    }
*/

    // DoMedianFilter (SelectionMedianFilter)
    DstImagePtr = (byte*)AxImageBW82->GetSurfacePtr(0,0);
    DstRowPitch = AxImageBW82->RowPitch;
    Method1_T=AGetTickCount();
    DoMedianFilter(SrcPtr,SrcWidth,SrcHeight,SrcRowPitch,ColorDepth,DstImagePtr,DstRowPitch,0);
    Method1_T=AGetTickCount()-Method1_T;
    Method1_AvgT+=Method1_T;

    // DoMedianFilter (QsortMedianFilter)
    DstImagePtr = (byte*)AxImageBW83->GetSurfacePtr(0,0);
    DstRowPitch = AxImageBW83->RowPitch;
    Method2_T=AGetTickCount();
    DoMedianFilter(SrcPtr,SrcWidth,SrcHeight,SrcRowPitch,ColorDepth,DstImagePtr,DstRowPitch,1);
    Method2_T=AGetTickCount()-Method2_T;
    Method2_AvgT+=Method2_T;

    // Calculate and Display Resault
    CaptureCount++;
    Label7->Caption = CaptureCount;
    Label8->Caption = Caption.sprintf("%.3f",Method1_T);
    Label9->Caption = Caption.sprintf("%.3f",Method1_AvgT/(double)CaptureCount);
    Label10->Caption = CaptureCount;
    Label11->Caption = Caption.sprintf("%.3f",Method2_T);
    Label12->Caption = Caption.sprintf("%.3f",Method2_AvgT/(double)CaptureCount);

    if(Method1_T<Method2_T) MyMidQuickCount++;
    else                    QuickSortQuickCount++;
    Label16->Caption = MyMidQuickCount;
    Label17->Caption = QuickSortQuickCount;


    // Verification two SelectionMedianFilter
    AxImageALops1->SubtractAbs(AxImageBW82->VegaHandle,
                               AxImageBW83->VegaHandle,
                               AxImageBW84->VegaHandle);
    AxImageIntegral1->SrcImageHandle = AxImageBW84->VegaHandle;
    AxImageIntegral1->Integrate();
    if(AxImageIntegral1->GreenSum>0)
        MidSortErrorCount++;
    Label18->Caption = MidSortErrorCount;

    AxImageBW81->DrawImage((long)Image1->Canvas->Handle,1,1,0,0);
    Image1->Refresh();
    AxImageBW82->DrawImage((long)Image2->Canvas->Handle,1,1,0,0);
    Image2->Refresh();
    AxImageBW83->DrawImage((long)Image3->Canvas->Handle,1,1,0,0);
    Image3->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{

    //
    Method1_T=0;
    Method1_AvgT=0;
    Method2_T=0;
    Method2_AvgT=0;
    CaptureCount=0;
    //
    MyMidQuickCount=0;
    QuickSortQuickCount=0;
    MidSortErrorCount=0;
    //
    AxAltairU1->Live();
    AxAltairU1->ShowControlPanel = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
    AxAltairU1->ShowControlPanel = true;
    byte* SrcPtr;
    long SrcWidth;
    long SrcHeight;
    long SrcRowPitch;
    byte* DstImagePtr;
    long DstRowPitch;

    AxAltairU1->SnapAndWait();

    if(ColorDepth==1)
    {
        AxImageBW81->SetSurfaceObj(AxAltairU1->ActiveSurfaceHandle);
        SrcPtr      = (byte*)AxImageBW81->GetSurfacePtr(0,0);
        SrcWidth    = AxImageBW81->ImageWidth ;
        SrcHeight   = AxImageBW81->ImageHeight;
        SrcRowPitch = AxImageBW81->RowPitch;
        AxImageBW82->SetSize(SrcWidth,SrcHeight);
        DstImagePtr = (byte*)AxImageBW82->GetSurfacePtr(0,0);
        DstRowPitch = AxImageBW82->RowPitch;
    }
    else
    {
        //AxImageC241->SetSurfaceObj(AxAltairU1->ActiveSurfaceHandle);
        AxImageC241->SetSize(AxAltairU1->ImageWidth ,AxAltairU1->ImageHeight);
        memcpy((void*)AxImageC241->GetSurfacePtr(0,0), (void*)AxAltairU1->GetImagePtr(AxAltairU1->ActiveSurfaceHandle,0,0),AxAltairU1->ImageWidth*AxAltairU1->ImageHeight*this->ColorDepth);
        SrcPtr      = (byte*)AxImageC241->GetSurfacePtr(0,0);
        SrcWidth    = AxImageC241->ImageWidth ;
        SrcHeight   = AxImageC241->ImageHeight;
        SrcRowPitch = AxImageC241->RowPitch;
        AxImageC242->SetSize(SrcWidth,SrcHeight);
        DstImagePtr = (byte*)AxImageC242->GetSurfacePtr(0,0);
        DstRowPitch = AxImageC242->RowPitch;
    }



    double T=AGetTickCount();
    DoMedianFilter(SrcPtr,SrcWidth,SrcHeight,SrcRowPitch,ColorDepth,DstImagePtr,DstRowPitch,0);
    T=AGetTickCount()-T;
    this->Caption = T;

    if(ColorDepth==1)
    {
        AxImageBW81->DrawImage((long)Image1->Canvas->Handle,0.5f,0.5f,0,0);
        Image1->Refresh();
        AxImageBW82->DrawImage((long)Image2->Canvas->Handle,1,1,0,0);
        Image2->Refresh();
    }
    else
    {
        AxImageC241->DrawImage((long)Image1->Canvas->Handle,1,1,0,0);
        Image1->Refresh();
        AxImageC242->DrawImage((long)Image2->Canvas->Handle,1,1,0,0);
        Image2->Refresh();
    }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
    this->AxAltairU1->Freeze();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button4Click(TObject *Sender)
{
    AxImageC241->SaveFile(L"OrgImage.bmp",AX_IMAGE_FILE_TYPE_FULLCOLOR_BMP);
    AxImageC242->SaveFile(L"DstImage.bmp",AX_IMAGE_FILE_TYPE_FULLCOLOR_BMP);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    IsMouseDownFlag = true;
    if(ColorDepth==1)
        eHT = AxROIBW81->HitTest(X,Y,1,1,0,0);
    else
        eHT = AxROIC241->HitTest(X,Y,1,1,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{


    if(IsMouseDownFlag==false)
        return;


    if(ColorDepth==1)
    {
        AxROIBW81->DragROI(eHT,X,Y,1,1,0,0);
        AxROIBW81->ParentHandle = AxImageBW81->VegaHandle;
        SrcPtr      = (byte*)AxROIBW81->GetSurfacePtr(0,0);
        SrcWidth    = AxROIBW81->ROIWidth;
        SrcHeight   = AxROIBW81->ROIHeight;
        SrcRowPitch = AxROIBW81->RowPitch;
        if(SrcWidth!=AxROIBW81->ROIWidth || SrcHeight!=AxROIBW81->ROIHeight)
            AxImageBW82->SetSize(AxROIBW81->ROIWidth, AxROIBW81->ROIHeight);
        DstImagePtr = (byte*)AxImageBW82->GetSurfacePtr(0,0);
        DstRowPitch = AxImageBW82->RowPitch;

        DoMedianFilter(SrcPtr,SrcWidth,SrcHeight,SrcRowPitch,ColorDepth,DstImagePtr,DstRowPitch,0);

        AxImageBW81->DrawImage((long)Image1->Canvas->Handle,1,1,0,0);
        AxROIBW81->DrawFrame((long)Image1->Canvas->Handle,1,1,0,0,255);
        Image1->Refresh();
        AxImageBW82->DrawImage((long)Image2->Canvas->Handle,1,1,0,0);
        Image2->Refresh();
    }
    else
    {
        AxROIC241->DragROI(eHT,X,Y,1,1,0,0);
        AxROIC241->ParentHandle = AxImageC241->VegaHandle;
        SrcPtr      = (byte*)AxROIC241->GetSurfacePtr(0,0);
        SrcWidth    = AxROIC241->ROIWidth;
        SrcHeight   = AxROIC241->ROIHeight;
        SrcRowPitch = AxROIC241->RowPitch;
        if(SrcWidth!=AxROIC241->ROIWidth || SrcHeight!=AxROIC241->ROIHeight)
            AxImageC242->SetSize(AxROIC241->ROIWidth, AxROIC241->ROIHeight);
        DstImagePtr = (byte*)AxImageC242->GetSurfacePtr(0,0);
        DstRowPitch = AxImageC242->RowPitch;

        DoMedianFilter(SrcPtr,SrcWidth,SrcHeight,SrcRowPitch,ColorDepth,DstImagePtr,DstRowPitch,0);

        AxImageC241->DrawImage((long)Image1->Canvas->Handle,1,1,0,0);
        AxROIC241->DrawFrame((long)Image1->Canvas->Handle,1,1,0,0,255);
        Image1->Refresh();
        Image2->Canvas->Rectangle(TRect(0,0,Image1->Width,Image1->Height));
        AxImageC242->DrawImage((long)Image2->Canvas->Handle,1,1,0,0);
        Image2->Refresh();

    }



}
//---------------------------------------------------------------------------


void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
    IsMouseDownFlag = false;
    eHT = 0;
}
//---------------------------------------------------------------------------

