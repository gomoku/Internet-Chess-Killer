object FormRegister: TFormRegister
  Left = 514
  Top = 298
  BorderStyle = bsDialog
  Caption = 'Registration'
  ClientHeight = 104
  ClientWidth = 262
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object lbComputerId: TLabel
    Left = 8
    Top = 16
    Width = 60
    Height = 13
    Caption = 'Computer Id.'
  end
  object lbKey: TLabel
    Left = 46
    Top = 42
    Width = 18
    Height = 13
    Caption = '&Key'
  end
  object edComputerId: TEdit
    Left = 76
    Top = 12
    Width = 177
    Height = 21
    Color = clSilver
    ReadOnly = True
    TabOrder = 0
  end
  object BitBtn1: TBitBtn
    Left = 64
    Top = 74
    Width = 69
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
    NumGlyphs = 2
  end
  object BitBtn2: TBitBtn
    Left = 142
    Top = 74
    Width = 73
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
    NumGlyphs = 2
  end
  object edKey: TEdit
    Left = 76
    Top = 40
    Width = 179
    Height = 21
    TabOrder = 3
  end
end
