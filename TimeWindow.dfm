object TimeForm: TTimeForm
  Left = 641
  Top = 403
  BorderStyle = bsDialog
  Caption = 'Time Control'
  ClientHeight = 154
  ClientWidth = 268
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object BitBtn1: TBitBtn
    Left = 58
    Top = 124
    Width = 69
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    NumGlyphs = 2
  end
  object BitBtn2: TBitBtn
    Left = 136
    Top = 124
    Width = 73
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
    NumGlyphs = 2
  end
  object Panel: TPanel
    Left = 4
    Top = 4
    Width = 261
    Height = 109
    TabOrder = 2
    object lbTime: TLabel
      Left = 38
      Top = 16
      Width = 57
      Height = 13
      Caption = 'Time      min'
    end
    object lbIncrement: TLabel
      Left = 22
      Top = 48
      Width = 73
      Height = 13
      Caption = 'Increment (sec)'
    end
    object lbLagTime: TLabel
      Left = 30
      Top = 84
      Width = 66
      Height = 13
      Caption = 'Lag Time (ms)'
    end
    object lbSec: TLabel
      Left = 164
      Top = 16
      Width = 17
      Height = 13
      Caption = 'sec'
    end
    object edTime: TCSpinEdit
      Left = 102
      Top = 12
      Width = 53
      Height = 22
      MaxValue = 1000
      TabOrder = 0
      Value = 1
    end
    object edIncTime: TCSpinEdit
      Left = 102
      Top = 44
      Width = 53
      Height = 22
      MaxValue = 1000
      TabOrder = 1
    end
    object edLagTime: TCSpinEdit
      Left = 102
      Top = 78
      Width = 53
      Height = 22
      Increment = 100
      MaxValue = 100000
      TabOrder = 2
      Value = 100
    end
    object edTimeSec: TCSpinEdit
      Left = 184
      Top = 12
      Width = 51
      Height = 22
      TabOrder = 3
    end
  end
end
