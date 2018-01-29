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
    int LastV;                  //�����C���P�_�����᪺�ƭ�, �H�Q�U���w�P��V��
    bool IsNextBiggerValue;     //�����W�@���P�_������, �X�z�Ȫ���V��. true:�U���ݩ��j��, false:�U���ݩ��p��
    int MinValue;               //���ĭ� (�w�] -2~256)
    int MaxValue;               //���ĭ� (�w�] -2~256)
    bool FailWhenSELClassify;   //�`�� S|E|L �ƶq��, ��P�O�_�ŦX�����(��m). true:��P���~, false:��P���T.
    int SmallerCount,EqualCount,LargerCount;  //S|E|L �����p��
    int MaskLength=9;           //Mask (3*3)

    //Make Ptr Shift Arr
    //MyMakeMaskPtrShiftArr(MaskPtrShiftArr,3,SrcRowPitch);
    int OrgPosPtr  = 0-1*SrcRowPitch-ColorDepth;
	for(int i=0;i<MaskLength;i++)
		MaskPtrShiftArr[i]=OrgPosPtr+(i/3)*SrcRowPitch+(i%3)*ColorDepth;

    //�����
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
        //�q pSrcImagePtr �� DataArr[]
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
        LastV               = -1;     //�����C���P�_�����᪺�ƭ�, �H�Q�U���w�P��V��
        IsNextBiggerValue   = true;   //�����W�@���P�_������, �X�z�Ȫ���V��. true:�U���ݩ��j��, false:�U���ݩ��p��
        MinValue            = -2;     //���ĭ� (�w�] -2~256)
        MaxValue            = 256;    //���ĭ� (�w�] -2~256)
        FailWhenSELClassify = false;  //�`�� S|E|L �ƶq��, ��P�O�_�ŦX�����(��m). true:��P���~, false:��P���T.

        for(int i=0;i<MaskLength;i++)
          { //�N�}�C��m 1-9 �i����

            // Step1. �w�P�����X�z��
            if(( IsNextBiggerValue && DataArr[i]<LastV) || //�W�@���P�_��, �ݩ��j�Ȥ�V��, �������Ȥp��W����
               (!IsNextBiggerValue && DataArr[i]>LastV) || //�W�@���P�_��, �ݩ��p�Ȥ�V��, �������Ȥj��W����
               DataArr[i] <= MinValue                   || //��V���T, ���p���ĭ�
               DataArr[i] >= MaxValue)                     //��V���T, ���j���ĭ�
            {  //�����P�_�z�Ȥ��X�z, �����P�_
               continue;
            }

            // Step2. S|E|L �ƶq�p��
            // �i�ѦҥH�U func ���e, ���ϥ� func �|�W�[�B��ɶ�, �G���ϥ� func
            // MySELClassfy(Arr, iArrLength, i, SmallerCount, EqualCount, LargerCount);
            SmallerCount=0; EqualCount=0; LargerCount=0;  //S|E|L �k�s
            FailWhenSELClassify = false;
            for(int j=0;j<MaskLength;j++)
            {
              if(j==i) continue;             //��P�_��m���ۤv��, ���L
              if(DataArr[j]<DataArr[i])       //S|E|L�ƶq�P�_ : �O�_��Smaller
              {
                if(++SmallerCount>4)          //����ȧP�_��1:���p�ƶq�L�h -> Fail
                {
                  IsNextBiggerValue=false;
                  MaxValue = DataArr[i];
                  FailWhenSELClassify = true;
                  break;
                }
              }
              else if(DataArr[j]>DataArr[i])  //S|E|L�ƶq�P�_ : �O�_��Larger
              {
                if(++LargerCount>4)           //����ȧP�_��2:���j�ƶq�L�h -> Fail
                {
                  IsNextBiggerValue=true;
                  MinValue = DataArr[i];
                  FailWhenSELClassify = true;
                  break;
                }
              }
              else                           //S|E|L�ƶq�P�_ : �T�{��Equal
              {
                if(++EqualCount>3)            //����ȧP�_��3:�۵��ƶq�L�h -> Get�����(��m)
                {
                  pDstImagePtr[0] = DataArr[i];
                  break;
                }
              }
            }

            //Step3. �P�_ S|E|L �ƶq�X�z��, �èM�w�O�_�������
            if(FailWhenSELClassify==true)
            {
              LastV = DataArr[i];
              continue;
            }
            else if(SmallerCount==LargerCount  ||  //����ȧP�_��4: �� Equal �q�H�~, ���k�۵�, �����ȽT�w
                    SmallerCount+EqualCount>=6 ||  //����ȧP�_��5: "���p+�۵�" �q�񭫹L�j, �����ȽT�w
                    LargerCount +EqualCount>=6 ||  //����ȧP�_��6: "���j+�۵�" �q�񭫹L�j, �����ȽT�w
                    (SmallerCount>LargerCount  && SmallerCount==LargerCount +EqualCount) || //����ȧP�_��7: �� ���p�q>���j�q, �B���p�q=�۵��q+���j�q, �����ȽT�w��۵��q��
                    (LargerCount >SmallerCount && LargerCount ==SmallerCount+EqualCount))   //����ȧP�_��8: �� ���j�q>���p�q, �B���j�q=�۵��q+���p�q, �����ȽT�w��۵��q��
            {
              pDstImagePtr[0] = DataArr[i]; //�����(��m)���\�D�o
              break;
            }
            break;
          }

        //���� pSrcImagePtr/pDstImagePtr ��m
        pSrcImagePtr++;
        pDstImagePtr++;
      }
      //���� pSrcImagePtr/pDstImagePtr ��m
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
    //MyMakeMaskPtrShiftArr(MaskPtrShiftArr,3,SrcRowPitch); //����func
    int OrgPosPtr  = 0-1*SrcRowPitch-ColorDepth;
	for(int i=0;i<MaskLength;i++)
        MaskPtrShiftArr[i]=OrgPosPtr + (i/3)*SrcRowPitch + (i%3)*ColorDepth;

    //�����
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
        //�q pSrcImagePtr �� DataArr[]
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

        //���� pSrcImagePtr/pDstImagePtr ��m
        pSrcImagePtr++;
        pDstImagePtr++;
      }
      //���� pSrcImagePtr/pDstImagePtr ��m
      pSrcImagePtr+=SrcRowPitch-EndWidthIndex+1;
      pDstImagePtr+=DstRowPitch-EndWidthIndex+1;
    }
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
