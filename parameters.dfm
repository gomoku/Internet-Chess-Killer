object CommonSettingsForm: TCommonSettingsForm
  Left = 450
  Top = 456
  BorderStyle = bsDialog
  Caption = 'Common Settings'
  ClientHeight = 132
  ClientWidth = 417
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
    Left = 142
    Top = 102
    Width = 69
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    NumGlyphs = 2
  end
  object Panel: TPanel
    Left = 2
    Top = 4
    Width = 411
    Height = 91
    TabOrder = 1
    object Label4: TLabel
      Left = 12
      Top = 14
      Width = 66
      Height = 13
      Caption = 'Program Type'
    end
    object edProgramType: TComboBox
      Left = 84
      Top = 10
      Width = 321
      Height = 21
      ItemHeight = 13
      TabOrder = 0
      Items.Strings = (
        'www.playchess.com'
        'www.chessplanet.ru'
        'www.bereg.ru'
        'winboard'
        'www.kurnik.org')
    end
    object chDebug: TCheckBox
      Left = 352
      Top = 66
      Width = 51
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Debug'
      TabOrder = 1
    end
    object chThinkMode: TCheckBox
      Left = 174
      Top = 40
      Width = 229
      Height = 17
      Alignment = taLeftJustify
      BiDiMode = bdLeftToRight
      Caption = 'Switch off Engine, When Opponent Move'
      ParentBiDiMode = False
      TabOrder = 2
    end
  end
  object BitBtn2: TBitBtn
    Left = 222
    Top = 102
    Width = 73
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
    NumGlyphs = 2
  end
end
