object EngineParamForm: TEngineParamForm
  Left = 399
  Top = 424
  BorderStyle = bsDialog
  Caption = 'Engine Settings'
  ClientHeight = 161
  ClientWidth = 298
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 283
    Height = 49
    Caption = 'File'
    TabOrder = 0
    object SelectEngineFile: TSpeedButton
      Left = 252
      Top = 16
      Width = 23
      Height = 22
      Caption = '...'
      OnClick = SelectEngineFileClick
    end
    object edEngineFileName: TEdit
      Left = 8
      Top = 18
      Width = 243
      Height = 21
      TabOrder = 0
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 66
    Width = 285
    Height = 57
    Caption = 'Parameters'
    TabOrder = 1
    object Label1: TLabel
      Left = 12
      Top = 22
      Width = 42
      Height = 13
      Caption = 'PV Lines'
    end
    object lbHashSize: TLabel
      Left = 132
      Top = 22
      Width = 48
      Height = 13
      Caption = 'Hash Size'
    end
    object MultiPV: TComboBox
      Left = 62
      Top = 18
      Width = 41
      Height = 21
      ItemHeight = 13
      TabOrder = 0
      Items.Strings = (
        '1'
        '2'
        '3'
        '4'
        '5')
    end
    object edHashSize: TComboBox
      Left = 188
      Top = 18
      Width = 65
      Height = 21
      ItemHeight = 13
      TabOrder = 1
      Items.Strings = (
        '64'
        '128'
        '256'
        '512'
        '1024'
        '2048'
        '4096')
    end
  end
  object BitBtn1: TBitBtn
    Left = 80
    Top = 128
    Width = 69
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 2
    NumGlyphs = 2
  end
  object BitBtn2: TBitBtn
    Left = 164
    Top = 128
    Width = 73
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
    NumGlyphs = 2
  end
  object OpenDialog: TOpenDialog
    Left = 8
    Top = 126
  end
end
