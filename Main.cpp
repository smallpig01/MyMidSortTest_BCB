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
  //[I]   Arr[]              : Arr�}�C��� (180105:���w��[9])
  //[I]   iArrLength         : Arr�}�C���� (180105:���w��[9])
  //[I/O] OutMidPosV         : ��X�����(��m)���G
  //[I/O] NumOfRunsProcessed : ��X���������(��m)�D��B�⦸��
  //[O]   ReturnValue        : [0]: ���� - MyMidSort�W�h�]��, ���L�k�P�_����Ȫ��A
  //                           [1]: ���\ - �����(��m)���\�D�o
  //                           [2]: ���� - �X�{���� S|E|L �ƦC���e, �ݦ^�� Arr[] ���s�˵��P�_��

  RunsCount           = 0;      //�������� MidSort �]�L�X����k (�B��q�� <= RunsCount(N-1)
  LastV               = -1;     //�����C���P�_�����᪺�ƭ�, �H�Q�U���w�P��V��
  IsNextBiggerValue   = true;   //�����W�@���P�_������, �X�z�Ȫ���V��. true:�U���ݩ��j��, false:�U���ݩ��p��
  MinValue            = -2;     //���ĭ� (�w�] -2~256)
  MaxValue            = 256;    //���ĭ� (�w�] -2~256)
  FailWhenSELClassify = false;  //�`�� S|E|L �ƶq��, ��P�O�_�ŦX�����(��m). true:��P���~, false:��P���T.


  for(int i=0;i<iArrLength;i++)
  { //�N�}�C��m 1-9 �i����

    // Step1. �w�P�����X�z��
    if(( IsNextBiggerValue && Arr[i]<LastV) || //�W�@���P�_��, �ݩ��j�Ȥ�V��, �������Ȥp��W����
       (!IsNextBiggerValue && Arr[i]>LastV) || //�W�@���P�_��, �ݩ��p�Ȥ�V��, �������Ȥj��W����
       Arr[i] <= MinValue                   || //��V���T, ���p���ĭ�
       Arr[i] >= MaxValue)                     //��V���T, ���j���ĭ�
    {  //�����P�_�z�Ȥ��X�z, �����P�_
       continue;
    }

    // Step2. S|E|L �ƶq�p��
    // �i�ѦҥH�U func ���e, ���ϥ� func �|�W�[�B��ɶ�, �G���ϥ� func
    // MySELClassfy(Arr, iArrLength, i, SmallerCount, EqualCount, LargerCount);
    RunsCount++;                                  
    SmallerCount=0; EqualCount=0; LargerCount=0;  //S|E|L �k�s
    int GoldenSampleV = Arr[i];                   //
    FailWhenSELClassify = false;
    for(int j=0;j<iArrLength;j++)
    {
      if(j==i) continue;             //��P�_��m���ۤv��, ���L
      if(Arr[j]<GoldenSampleV)       //S|E|L�ƶq�P�_ : �O�_��Smaller
      {
        if(++SmallerCount>4)          //����ȧP�_��1:���p�ƶq�L�h -> Fail
        {
          IsNextBiggerValue=false;
          MaxValue = Arr[i];
          FailWhenSELClassify = true;
          break;
        }
      }
      else if(Arr[j]>GoldenSampleV)  //S|E|L�ƶq�P�_ : �O�_��Larger
      {
        if(++LargerCount>4)           //����ȧP�_��2:���j�ƶq�L�h -> Fail
        {
          IsNextBiggerValue=true;
          MinValue = Arr[i];
          FailWhenSELClassify = true;
          break;
        }
      }
      else                           //S|E|L�ƶq�P�_ : �T�{��Equal
      {
        if(++EqualCount>3)            //����ȧP�_��3:�۵��ƶq�L�h -> Get�����(��m)
        {
          OutMidPosV = Arr[i];
          NumOfRunsProcessed = RunsCount;
          return 1;
        }
      }
    }

    //Step3. �P�_ S|E|L �ƶq�X�z��, �èM�w�O�_�������
    if(FailWhenSELClassify==true)
    {
      LastV = Arr[i];
      continue;
    }
    else if(SmallerCount==LargerCount  ||  //����ȧP�_��4: �� Equal �q�H�~, ���k�۵�, �����ȽT�w
            SmallerCount+EqualCount>=6 ||  //����ȧP�_��5: "���p+�۵�" �q�񭫹L�j, �����ȽT�w
            LargerCount +EqualCount>=6 ||  //����ȧP�_��6: "���j+�۵�" �q�񭫹L�j, �����ȽT�w
            (SmallerCount>LargerCount  && SmallerCount==LargerCount +EqualCount) || //����ȧP�_��7: �� ���p�q>���j�q, �B���p�q=�۵��q+���j�q, �����ȽT�w��۵��q��
            (LargerCount >SmallerCount && LargerCount ==SmallerCount+EqualCount))   //����ȧP�_��8: �� ���j�q>���p�q, �B���j�q=�۵��q+���p�q, �����ȽT�w��۵��q��
    {
      OutMidPosV = Arr[i];
      NumOfRunsProcessed = RunsCount;
      return 1;  //�����(��m)���\�D�o
    }
    return 2; //�X�{���� S|E|L �ƦC���e
  }
  return 0;   //MyMidSort�W�h�]��, ���L�k�P�_����Ȫ��A
}
/*/
//---------------------------------------------------------------------------
/*/
int MyMidSort(byte* MaskCenterPtr, int PtrShiftArr[], int iArrLength, byte &OutMidPosV)
{
  //[I]   Arr[]              : Arr�}�C��� (180105:���w��[9])
  //[I]   iArrLength         : Arr�}�C���� (180105:���w��[9])
  //[I/O] OutMidPosV         : ��X�����(��m)���G
  //[I/O] NumOfRunsProcessed : ��X���������(��m)�D��B�⦸��
  //[O]   ReturnValue        : [0]: ���� - MyMidSort�W�h�]��, ���L�k�P�_����Ȫ��A
  //                           [1]: ���\ - �����(��m)���\�D�o
  //                           [2]: ���� - �X�{���� S|E|L �ƦC���e, �ݦ^�� Arr[] ���s�˵��P�_��

  LastV               = -1;     //�����C���P�_�����᪺�ƭ�, �H�Q�U���w�P��V��
  IsNextBiggerValue   = true;   //�����W�@���P�_������, �X�z�Ȫ���V��. true:�U���ݩ��j��, false:�U���ݩ��p��
  MinValue            = -2;     //���ĭ� (�w�] -2~256)
  MaxValue            = 256;    //���ĭ� (�w�] -2~256)
  FailWhenSELClassify = false;  //�`�� S|E|L �ƶq��, ��P�O�_�ŦX�����(��m). true:��P���~, false:��P���T.

  for(int i=0;i<iArrLength;i++)
  { //�N�}�C��m 1-9 �i����


    // Step1. �w�P�����X�z��
    if(( IsNextBiggerValue && MaskCenterPtr[PtrShiftArr[i]]<LastV) || //�W�@���P�_��, �ݩ��j�Ȥ�V��, �������Ȥp��W����
       (!IsNextBiggerValue && MaskCenterPtr[PtrShiftArr[i]]>LastV) || //�W�@���P�_��, �ݩ��p�Ȥ�V��, �������Ȥj��W����
       MaskCenterPtr[PtrShiftArr[i]] <= MinValue                   || //��V���T, ���p���ĭ�
       MaskCenterPtr[PtrShiftArr[i]] >= MaxValue)                     //��V���T, ���j���ĭ�
    {  //�����P�_�z�Ȥ��X�z, �����P�_
       continue;
    }

    // Step2. S|E|L �ƶq�p��
    // �i�ѦҥH�U func ���e, ���ϥ� func �|�W�[�B��ɶ�, �G���ϥ� func
    // MySELClassfy(Arr, iArrLength, i, SmallerCount, EqualCount, LargerCount);
    SmallerCount=0; EqualCount=0; LargerCount=0;  //S|E|L �k�s
    FailWhenSELClassify = false;


    for(int j=0;j<iArrLength;j++)
    {
      if(j==i) continue;             //��P�_��m���ۤv��, ���L
      if(MaskCenterPtr[PtrShiftArr[j]]<MaskCenterPtr[PtrShiftArr[i]])       //S|E|L�ƶq�P�_ : �O�_��Smaller
      {
        if(++SmallerCount>4)          //����ȧP�_��1:���p�ƶq�L�h -> Fail
        {
          IsNextBiggerValue=false;
          MaxValue = MaskCenterPtr[PtrShiftArr[i]];
          FailWhenSELClassify = true;
          break;
        }
      }
      else if(MaskCenterPtr[PtrShiftArr[j]]>MaskCenterPtr[PtrShiftArr[i]])  //S|E|L�ƶq�P�_ : �O�_��Larger
      {
        if(++LargerCount>4)           //����ȧP�_��2:���j�ƶq�L�h -> Fail
        {
          IsNextBiggerValue=true;
          MinValue = MaskCenterPtr[PtrShiftArr[i]];
          FailWhenSELClassify = true;
          break;
        }
      }
      else                           //S|E|L�ƶq�P�_ : �T�{��Equal
      {
        if(++EqualCount>3)            //����ȧP�_��3:�۵��ƶq�L�h -> Get�����(��m)
        {
          OutMidPosV = MaskCenterPtr[PtrShiftArr[i]];
          return 1;
        }
      }
    }

    //Step3. �P�_ S|E|L �ƶq�X�z��, �èM�w�O�_�������
    if(FailWhenSELClassify==true)
    {
      LastV = MaskCenterPtr[PtrShiftArr[i]];
      continue;
    }
    else if(SmallerCount==LargerCount  ||  //����ȧP�_��4: �� Equal �q�H�~, ���k�۵�, �����ȽT�w
            SmallerCount+EqualCount>=6 ||  //����ȧP�_��5: "���p+�۵�" �q�񭫹L�j, �����ȽT�w
            LargerCount +EqualCount>=6 ||  //����ȧP�_��6: "���j+�۵�" �q�񭫹L�j, �����ȽT�w
            (SmallerCount>LargerCount  && SmallerCount==LargerCount +EqualCount) || //����ȧP�_��7: �� ���p�q>���j�q, �B���p�q=�۵��q+���j�q, �����ȽT�w��۵��q��
            (LargerCount >SmallerCount && LargerCount ==SmallerCount+EqualCount))   //����ȧP�_��8: �� ���j�q>���p�q, �B���j�q=�۵��q+���p�q, �����ȽT�w��۵��q��
    {
      OutMidPosV = MaskCenterPtr[PtrShiftArr[i]];
      return 1;  //�����(��m)���\�D�o
    }

    return 2; //�X�{���� S|E|L �ƦC���e
  }
  return 0;   //MyMidSort�W�h�]��, ���L�k�P�_����Ȫ��A
}
/*/

//---------------------------------------------------------------------------
char GetMaskValue(byte* MaskCenterPtr, int MaskLength, int MaskPtrShiftArr[], byte OutArr[])
{   //�Q�� MaskCenterPtr + MaskPtrShiftArr ���X MaskParentData ������
    //[I/ ] MaskCenterPtr   : �]�w Mask ���߫��Ц�m
    //[I/ ] MaskLength      : ��J Mask ��ƪ���
    //[I/ ] MaskPtrShiftArr : �]�w�Ѧ� MaskPtr ������
    //[ /O] OutArr          : ��X MaskParentData ��
    //[ /O] ReturnValue     : ���ȵ��G. [1]: ���Ȧ��\
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

