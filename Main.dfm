object Form1: TForm1
  Left = 351
  Top = 168
  Width = 1546
  Height = 853
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 572
    Top = 8
    Width = 376
    Height = 240
  end
  object Image2: TImage
    Left = 8
    Top = 324
    Width = 752
    Height = 480
  end
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 32
    Height = 13
    Caption = 'Label1'
  end
  object Label2: TLabel
    Left = 8
    Top = 24
    Width = 32
    Height = 13
    Caption = 'Label1'
  end
  object Label3: TLabel
    Left = 8
    Top = 40
    Width = 32
    Height = 13
    Caption = 'Label1'
  end
  object Image3: TImage
    Left = 768
    Top = 324
    Width = 752
    Height = 480
  end
  object Label4: TLabel
    Left = 730
    Top = 256
    Width = 70
    Height = 13
    Alignment = taCenter
    Caption = 'NumOfCapture'
  end
  object Label5: TLabel
    Left = 742
    Top = 272
    Width = 43
    Height = 13
    Alignment = taCenter
    Caption = 'ThisTime'
  end
  object Label6: TLabel
    Left = 744
    Top = 288
    Width = 42
    Height = 13
    Alignment = taCenter
    Caption = 'AvgTime'
  end
  object Label7: TLabel
    Left = 656
    Top = 256
    Width = 60
    Height = 13
    Alignment = taCenter
    Caption = 'Label7'
  end
  object Label8: TLabel
    Left = 656
    Top = 272
    Width = 60
    Height = 13
    Alignment = taCenter
    Caption = 'Label8'
  end
  object Label9: TLabel
    Left = 656
    Top = 288
    Width = 60
    Height = 13
    Alignment = taCenter
    Caption = 'Label9'
  end
  object Label10: TLabel
    Left = 816
    Top = 256
    Width = 60
    Height = 13
    Alignment = taCenter
    Caption = 'Label7'
  end
  object Label11: TLabel
    Left = 816
    Top = 272
    Width = 60
    Height = 13
    Alignment = taCenter
    Caption = 'Label8'
  end
  object Label12: TLabel
    Left = 816
    Top = 288
    Width = 60
    Height = 13
    Alignment = taCenter
    Caption = 'Label9'
  end
  object Label13: TLabel
    Left = 360
    Top = 288
    Width = 50
    Height = 13
    Caption = 'MyMidSort'
  end
  object Label14: TLabel
    Left = 1128
    Top = 288
    Width = 47
    Height = 13
    Caption = 'QuickSort'
  end
  object Label15: TLabel
    Left = 739
    Top = 304
    Width = 48
    Height = 13
    Alignment = taCenter
    Caption = 'FastCount'
  end
  object Label16: TLabel
    Left = 670
    Top = 304
    Width = 32
    Height = 13
    Alignment = taCenter
    Caption = 'Label9'
  end
  object Label17: TLabel
    Left = 831
    Top = 304
    Width = 32
    Height = 13
    Alignment = taCenter
    Caption = 'Label9'
  end
  object AxAltairU1: TAxAltairU
    Left = 352
    Top = 8
    Width = 30
    Height = 30
    TabOrder = 0
    OnSurfaceFilled = AxAltairU1SurfaceFilled
    ControlData = {
      545046300B5456636C416C7461697255025F31044C65667403600103546F7002
      08055769647468021E06486569676874021E124E756D4F664472697665724275
      66666572730203094672616D6552617465020009576F6953746172745802FF09
      576F6953746172745902FF07576F69456E645802FF07576F69456E645902FF07
      564F66667365740200104461634368314D617843757272656E7403E8030E4461
      634368314D617856616C756503FF031A426C61636B4C6576656C436F6D70656E
      736174696F6E4D6F64650711415841555F424C435F4D4F44455F4F4646185368
      6F77436F6C6F7243616C6962726174696F6E5061676508055742526564050000
      000000000080FF3F075742477265656E050000000000000080FF3F065742426C
      7565050000000000000080FF3F195761746368446F6754696D657254696D656F
      75745469636B7302001453757266616365517565756543617061636974790201
      0000}
  end
  object AxImageBW81: TAxImageBW8
    Left = 384
    Top = 8
    Width = 30
    Height = 30
    TabOrder = 1
    ControlData = {
      545046300C5456636C496D616765425738025F31044C65667403800103546F70
      0208055769647468021E06486569676874021E0A496D61676557696474680200
      0B496D61676548656967687402000000}
  end
  object Button1: TButton
    Left = 8
    Top = 56
    Width = 75
    Height = 25
    Caption = 'Test'
    TabOrder = 2
    OnClick = Button1Click
  end
  object AxImageBW82: TAxImageBW8
    Left = 1136
    Top = 256
    Width = 30
    Height = 30
    TabOrder = 3
    ControlData = {
      545046300C5456636C496D616765425738025F31044C65667403700403546F70
      030001055769647468021E06486569676874021E0A496D616765576964746802
      000B496D61676548656967687402000000}
  end
  object Button2: TButton
    Left = 8
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Live'
    TabOrder = 4
    OnClick = Button2Click
  end
  object AxImageBW83: TAxImageBW8
    Left = 368
    Top = 256
    Width = 30
    Height = 30
    TabOrder = 5
    ControlData = {
      545046300C5456636C496D61676542573800044C65667403700103546F700300
      01055769647468021E06486569676874021E0A496D616765576964746802000B
      496D61676548656967687402000000}
  end
end
