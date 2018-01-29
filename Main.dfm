object Form1: TForm1
  Left = 374
  Top = 8
  Width = 1546
  Height = 1056
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 388
    Top = 5
    Width = 752
    Height = 480
    OnMouseDown = Image1MouseDown
    OnMouseMove = Image1MouseMove
    OnMouseUp = Image1MouseUp
  end
  object Image2: TImage
    Left = 8
    Top = 564
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
    Top = 564
    Width = 752
    Height = 480
  end
  object Label4: TLabel
    Left = 730
    Top = 496
    Width = 70
    Height = 13
    Alignment = taCenter
    Caption = 'NumOfCapture'
  end
  object Label5: TLabel
    Left = 742
    Top = 512
    Width = 43
    Height = 13
    Alignment = taCenter
    Caption = 'ThisTime'
  end
  object Label6: TLabel
    Left = 744
    Top = 528
    Width = 42
    Height = 13
    Alignment = taCenter
    Caption = 'AvgTime'
  end
  object Label7: TLabel
    Left = 656
    Top = 496
    Width = 32
    Height = 13
    Alignment = taCenter
    Caption = 'Label7'
  end
  object Label8: TLabel
    Left = 656
    Top = 512
    Width = 32
    Height = 13
    Alignment = taCenter
    Caption = 'Label8'
  end
  object Label9: TLabel
    Left = 656
    Top = 528
    Width = 32
    Height = 13
    Alignment = taCenter
    Caption = 'Label9'
  end
  object Label10: TLabel
    Left = 816
    Top = 496
    Width = 32
    Height = 13
    Alignment = taCenter
    Caption = 'Label7'
  end
  object Label11: TLabel
    Left = 816
    Top = 512
    Width = 32
    Height = 13
    Alignment = taCenter
    Caption = 'Label8'
  end
  object Label12: TLabel
    Left = 816
    Top = 528
    Width = 32
    Height = 13
    Alignment = taCenter
    Caption = 'Label9'
  end
  object Label13: TLabel
    Left = 328
    Top = 544
    Width = 50
    Height = 13
    Caption = 'MyMidSort'
  end
  object Label14: TLabel
    Left = 1144
    Top = 544
    Width = 47
    Height = 13
    Caption = 'QuickSort'
  end
  object Label15: TLabel
    Left = 739
    Top = 544
    Width = 48
    Height = 13
    Alignment = taCenter
    Caption = 'FastCount'
  end
  object Label16: TLabel
    Left = 654
    Top = 544
    Width = 32
    Height = 13
    Alignment = taCenter
    Caption = 'Label9'
  end
  object Label17: TLabel
    Left = 816
    Top = 544
    Width = 32
    Height = 13
    Alignment = taCenter
    Caption = 'Label9'
  end
  object Label18: TLabel
    Left = 67
    Top = 288
    Width = 48
    Height = 13
    Alignment = taCenter
    Caption = 'FastCount'
  end
  object Label19: TLabel
    Left = 6
    Top = 288
    Width = 53
    Height = 13
    Alignment = taCenter
    Caption = 'Error Count'
  end
  object Label20: TLabel
    Left = 200
    Top = 216
    Width = 38
    Height = 13
    Caption = 'Label20'
  end
  object AxAltairU1: TAxAltairU
    Left = 8
    Top = 120
    Width = 30
    Height = 30
    TabOrder = 0
    OnSurfaceFilled = AxAltairU1SurfaceFilled
    ControlData = {
      545046300B5456636C416C7461697255025F31044C656674020803546F700278
      055769647468021E06486569676874021E124E756D4F66447269766572427566
      666572730203094672616D6552617465020009576F6953746172745802FF0957
      6F6953746172745902FF07576F69456E645802FF07576F69456E645902FF0756
      4F66667365740200104461634368314D617843757272656E7403E8030E446163
      4368314D617856616C756503FF031A426C61636B4C6576656C436F6D70656E73
      6174696F6E4D6F64650711415841555F424C435F4D4F44455F4F46461853686F
      77436F6C6F7243616C6962726174696F6E506167650805574252656405000000
      0000000080FF3F075742477265656E050000000000000080FF3F065742426C75
      65050000000000000080FF3F195761746368446F6754696D657254696D656F75
      745469636B730200145375726661636551756575654361706163697479020100
      00}
  end
  object AxImageBW81: TAxImageBW8
    Left = 40
    Top = 120
    Width = 30
    Height = 30
    TabOrder = 1
    ControlData = {
      545046300C5456636C496D616765425738025F31044C656674022803546F7002
      78055769647468021E06486569676874021E0A496D616765576964746802000B
      496D61676548656967687402000000}
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
    Left = 1152
    Top = 512
    Width = 30
    Height = 30
    TabOrder = 3
    ControlData = {
      545046300C5456636C496D616765425738025F31044C65667403800403546F70
      030002055769647468021E06486569676874021E0A496D616765576964746802
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
    Left = 336
    Top = 512
    Width = 30
    Height = 30
    TabOrder = 5
    ControlData = {
      545046300C5456636C496D616765425738025F31044C65667403500103546F70
      030002055769647468021E06486569676874021E0A496D616765576964746802
      000B496D61676548656967687402000000}
  end
  object AxImageBW84: TAxImageBW8
    Left = 8
    Top = 256
    Width = 30
    Height = 30
    TabOrder = 6
    ControlData = {
      545046300C5456636C496D616765425738025F31044C656674020803546F7003
      0001055769647468021E06486569676874021E0A496D61676557696474680200
      0B496D61676548656967687402000000}
  end
  object AxImageIntegral1: TAxImageIntegral
    Left = 8
    Top = 224
    Width = 30
    Height = 30
    TabOrder = 7
    ControlData = {
      54504630115456636C496D616765496E74656772616C025F31044C6566740208
      03546F7003E000055769647468021E06486569676874021E0000}
  end
  object AxImageALops1: TAxImageALops
    Left = 40
    Top = 224
    Width = 30
    Height = 30
    TabOrder = 8
    ControlData = {
      545046300E5456636C496D616765414C6F7073025F31044C656674022803546F
      7003E000055769647468021E06486569676874021E0000}
  end
  object Button3: TButton
    Left = 200
    Top = 152
    Width = 81
    Height = 57
    Caption = 'Button3'
    TabOrder = 9
    OnClick = Button3Click
  end
  object AxImageC241: TAxImageC24
    Left = 8
    Top = 376
    Width = 30
    Height = 30
    TabOrder = 10
    ControlData = {
      545046300C5456636C496D61676543323400044C656674020803546F70037801
      055769647468021E06486569676874021E0A496D616765576964746802000B49
      6D61676548656967687402000000}
  end
  object AxImageC242: TAxImageC24
    Left = 40
    Top = 376
    Width = 30
    Height = 30
    TabOrder = 11
    ControlData = {
      545046300C5456636C496D61676543323400044C656674020803546F70037801
      055769647468021E06486569676874021E0A496D616765576964746802000B49
      6D61676548656967687402000000}
  end
  object AxImageC243: TAxImageC24
    Left = 72
    Top = 376
    Width = 30
    Height = 30
    TabOrder = 12
    ControlData = {
      545046300C5456636C496D61676543323400044C656674020803546F70037801
      055769647468021E06486569676874021E0A496D616765576964746802000B49
      6D61676548656967687402000000}
  end
  object AxImageC244: TAxImageC24
    Left = 104
    Top = 376
    Width = 30
    Height = 30
    TabOrder = 13
    ControlData = {
      545046300C5456636C496D61676543323400044C656674020803546F70037801
      055769647468021E06486569676874021E0A496D616765576964746802000B49
      6D61676548656967687402000000}
  end
  object Button4: TButton
    Left = 1272
    Top = 248
    Width = 145
    Height = 81
    Caption = 'Button4'
    TabOrder = 14
    OnClick = Button4Click
  end
  object AxROIC241: TAxROIC24
    Left = 248
    Top = 336
    Width = 30
    Height = 30
    TabOrder = 15
    ControlData = {
      545046300A5456636C524F4943323400044C65667403F80003546F7003500105
      5769647468021E06486569676874021E0000}
  end
  object AxROIBW81: TAxROIBW8
    Left = 248
    Top = 368
    Width = 30
    Height = 30
    TabOrder = 16
    ControlData = {
      545046300A5456636C524F4942573800044C65667403F80003546F7003700105
      5769647468021E06486569676874021E0000}
  end
end
