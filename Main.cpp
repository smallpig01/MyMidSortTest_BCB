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
  MaxValue            = 2560;    //���ĭ� (�w�] -2~256)
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
        if(SmallerCount>=4)          //����ȧP�_��1:���p�ƶq�L�h -> Fail
        {
          IsNextBiggerValue=false;
          MaxValue = Arr[i];
          FailWhenSELClassify = true;
          break;
        }
        else
          SmallerCount++;
      }
      else if(Arr[j]>GoldenSampleV)  //S|E|L�ƶq�P�_ : �O�_��Larger
      {
        if(LargerCount>=4)           //����ȧP�_��2:���j�ƶq�L�h -> Fail
        {
          IsNextBiggerValue=true;
          MinValue = Arr[i];
          FailWhenSELClassify = true;
          break;
        }
        else
          LargerCount++;
      }
      else                           //S|E|L�ƶq�P�_ : �T�{��Equal
      {
        if(EqualCount>=3)            //����ȧP�_��3:�۵��ƶq�L�h -> Get�����(��m)
        {
          OutMidPosV = Arr[i];
          NumOfRunsProcessed = RunsCount;
          return 1;
        }
        else
          EqualCount++;
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
//---------------------------------------------------------------------------
double AGetTickCount()  //�ۭq��ơA���odouble���A�ɶ�
{
    LARGE_INTEGER C_time,F_time;              //�ŧi LARGE_INTEGER ���A�ܼ�
    ::QueryPerformanceFrequency(&F_time);  //���oCPU�W�v�ɶ�
    ::QueryPerformanceCounter(&C_time);      //���o�ثe�ɶ�
    return (double)C_time.QuadPart / (double)F_time.QuadPart * 1000.0;  //�ثe�ɶ� / CPU�W�v*1000 = (ms)
}
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
int MyMakeMaskPtrShiftArr(int MaskPtrShiftArr[], int MaskWH, int ImageRowPitch)
{   //�s�@ MaskPtr ������, �� Mask ���ȥ�.
    //[I/O] MaskShiftArr  : ���Ѥ@�}�C, �s�� MaskPtrShift ��
    //[I/ ] MaskWH        : MaskPtrShift Table �e��
    //[I/ ] ImageRowPitch : Mask Parent Data ROW �e��
    //[ /O] ReturnValue   : MyMakeMaskPtrShiftArr �s�@���G
    //                      [1]: �s�@���\
    //                      [2]: Error-Mask�e�פ����_��
    //                      [3]: Error-�v���e��Pitch�p��Mask�e��
    
    //��ƨ��b
	if(MaskWH%2==0)
	    return 2; // ErrorCode[2]: Mask�e�פ����_��  
    if(ImageRowPitch<MaskWH)
	    return 3; // ErrorCode[3]: �v���e��Pitch�p��Mask�e��
		
	int HalfMaskWH = (MaskWH-1)/2;
	int Length     = MaskWH*MaskWH;
    int OrgPosPtr  = 0-HalfMaskWH*ImageRowPitch-HalfMaskWH;
	
	for(int i=0;i<Length;i++)
	{
		MaskPtrShiftArr[i]=OrgPosPtr+(i/MaskWH)*ImageRowPitch+(i%MaskWH);
	}
		
 	return 1; //�s�@���\
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

