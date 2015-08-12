object MainForm: TMainForm
  Left = 792
  Top = 132
  Width = 456
  Height = 243
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'InternetChessKiller'
  Color = clBtnFace
  Constraints.MinHeight = 50
  Constraints.MinWidth = 100
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Icon.Data = {
    0000010001002020100000000000E80200001600000028000000200000004000
    0000010004000000000080020000000000000000000000000000000000000000
    000000008000008000000080800080000000800080008080000080808000C0C0
    C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF000000
    000000000000330077000000000000000000000000003B077070000000000000
    000000000000BB807007000000000000000000000300B0007000700000000000
    00000000330070070700070000000000000000003B0700700070007000000000
    00000000BB800700000700070000000000000300B00070000000700070000000
    0000330070070000000007000700000000003B07007000000000007007000000
    0000BB800700000000000007070000000300B000700000000070000077000000
    330070070000000007000000803300003B070070000000000000000800330000
    BB8007000000000000000080BBBB0300B000700000000070000008000BB03300
    70070000000707000000803300003B070070000000707000000800330000BB80
    07000000070700000080BBBB0000B000700000000070000008000BB000007007
    0000000007000000803300000000707000007770000000080033000000008700
    0007070700000080BBBB00000000080000077777000008000BB0000000000080
    0007070700008033000000000000000800007770000800330000000000000000
    800000000080BBBB00000000000000000800000008000BB00000000000000000
    0080000080330000000000000000000000080008003300000000000000000000
    00008080BBBB00000000000000000000000008000BB00000000000000000FFFF
    33FFFFFF21FFFFFF00FFFFFB007FFFF3003FFFF2001FFFF0000FFFB00007FF30
    0003FF200003FF000003FB000003F3000000F2000000F0000010B00000393000
    000F2000000F0000010F0000039F000000FF000000FF000010FF800039FFC000
    0FFFE0000FFFF0010FFFF8039FFFFC00FFFFFE00FFFFFF10FFFFFFB9FFFF}
  KeyPreview = True
  Menu = MainMenu1
  OldCreateOrder = False
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  DesignSize = (
    448
    189)
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox2: TGroupBox
    Left = 164
    Top = 0
    Width = 111
    Height = 39
    Caption = 'Recognize State'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    object shBoardCaptured: TShape
      Left = 4
      Top = 14
      Width = 19
      Height = 21
      Hint = 'Board captured'
      Brush.Color = clRed
      ParentShowHint = False
      Shape = stCircle
      ShowHint = True
    end
    object shPosRecognized: TShape
      Left = 28
      Top = 14
      Width = 19
      Height = 21
      Hint = 'Position recognized'
      Brush.Color = clRed
      ParentShowHint = False
      Shape = stCircle
      ShowHint = True
    end
    object edLastMove: TEdit
      Left = 52
      Top = 14
      Width = 53
      Height = 21
      Hint = 'Last move'
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 0
    end
  end
  object Memo: TMemo
    Left = 2
    Top = 88
    Width = 443
    Height = 140
    Anchors = [akLeft, akTop, akRight, akBottom]
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    ReadOnly = True
    TabOrder = 2
    WordWrap = False
  end
  object GroupBox1: TGroupBox
    Left = 2
    Top = 0
    Width = 159
    Height = 39
    Caption = 'UCI Engine'
    TabOrder = 0
    object SpeedButton1: TSpeedButton
      Left = 132
      Top = 12
      Width = 23
      Height = 22
      Hint = 'Select engine'
      Caption = '...'
      ParentShowHint = False
      ShowHint = True
      OnClick = SpeedButton1Click
    end
    object lbEngineName: TLabel
      Left = 12
      Top = 16
      Width = 5
      Height = 13
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
  object GroupBox3: TGroupBox
    Left = 2
    Top = 38
    Width = 195
    Height = 49
    Caption = 'Time'
    TabOrder = 3
    object shWhite: TShape
      Left = 6
      Top = 18
      Width = 9
      Height = 13
      Shape = stCircle
    end
    object shBlack: TShape
      Left = 92
      Top = 20
      Width = 9
      Height = 11
      Brush.Color = clBlack
      Shape = stCircle
    end
    object btTimeDialog: TSpeedButton
      Left = 168
      Top = 14
      Width = 23
      Height = 22
      Caption = '...'
      OnClick = btTimeDialogClick
    end
    object edWhiteTime: TEdit
      Left = 18
      Top = 14
      Width = 53
      Height = 21
      ReadOnly = True
      TabOrder = 0
    end
    object edBlackTime: TEdit
      Left = 102
      Top = 14
      Width = 45
      Height = 21
      ReadOnly = True
      TabOrder = 1
    end
    object csWhiteTime: TCSpinButton
      Left = 70
      Top = 14
      Width = 15
      Height = 22
      Hint = 'increment - "a" decrement "z"'
      DownGlyph.Data = {
        0E010000424D0E01000000000000360000002800000009000000060000000100
        200000000000D800000000000000000000000000000000000000008080000080
        8000008080000080800000808000008080000080800000808000008080000080
        8000008080000080800000808000000000000080800000808000008080000080
        8000008080000080800000808000000000000000000000000000008080000080
        8000008080000080800000808000000000000000000000000000000000000000
        0000008080000080800000808000000000000000000000000000000000000000
        0000000000000000000000808000008080000080800000808000008080000080
        800000808000008080000080800000808000}
      TabOrder = 2
      UpGlyph.Data = {
        0E010000424D0E01000000000000360000002800000009000000060000000100
        200000000000D800000000000000000000000000000000000000008080000080
        8000008080000080800000808000008080000080800000808000008080000080
        8000000000000000000000000000000000000000000000000000000000000080
        8000008080000080800000000000000000000000000000000000000000000080
        8000008080000080800000808000008080000000000000000000000000000080
        8000008080000080800000808000008080000080800000808000000000000080
        8000008080000080800000808000008080000080800000808000008080000080
        800000808000008080000080800000808000}
      OnDownClick = csWhiteTimeDownClick
      OnUpClick = csWhiteTimeUpClick
    end
    object csBlackTime: TCSpinButton
      Left = 146
      Top = 14
      Width = 15
      Height = 23
      Hint = 'increment - "s" decrement "x"'
      DownGlyph.Data = {
        0E010000424D0E01000000000000360000002800000009000000060000000100
        200000000000D800000000000000000000000000000000000000008080000080
        8000008080000080800000808000008080000080800000808000008080000080
        8000008080000080800000808000000000000080800000808000008080000080
        8000008080000080800000808000000000000000000000000000008080000080
        8000008080000080800000808000000000000000000000000000000000000000
        0000008080000080800000808000000000000000000000000000000000000000
        0000000000000000000000808000008080000080800000808000008080000080
        800000808000008080000080800000808000}
      TabOrder = 3
      UpGlyph.Data = {
        0E010000424D0E01000000000000360000002800000009000000060000000100
        200000000000D800000000000000000000000000000000000000008080000080
        8000008080000080800000808000008080000080800000808000008080000080
        8000000000000000000000000000000000000000000000000000000000000080
        8000008080000080800000000000000000000000000000000000000000000080
        8000008080000080800000808000008080000000000000000000000000000080
        8000008080000080800000808000008080000080800000808000000000000080
        8000008080000080800000808000008080000080800000808000008080000080
        800000808000008080000080800000808000}
      OnDownClick = csBlackTimeDownClick
      OnUpClick = csBlackTimeUpClick
    end
  end
  object GroupBox4: TGroupBox
    Left = 202
    Top = 38
    Width = 89
    Height = 49
    Caption = 'AutoPlay'
    TabOrder = 4
    object chWhite: TCheckBox
      Left = 8
      Top = 12
      Width = 73
      Height = 17
      Caption = 'White (w)'
      TabOrder = 0
    end
    object chBlack: TCheckBox
      Left = 8
      Top = 30
      Width = 73
      Height = 17
      Caption = 'Black (b)'
      TabOrder = 1
    end
  end
  object MainMenu1: TMainMenu
    Left = 280
    Top = 16
    object File1: TMenuItem
      Caption = '&Settings'
      object Parameters1: TMenuItem
        Caption = 'Common...'
        OnClick = Parameters1Click
      end
    end
    object Debug1: TMenuItem
      Caption = '&Debug'
      Visible = False
      object ShowDebugWindow1: TMenuItem
        Caption = '&Position...'
        OnClick = ShowDebugWindow1Click
      end
      object SquareDebug1: TMenuItem
        Caption = '&Square...'
        OnClick = SquareDebug1Click
      end
      object ScreenDebug1: TMenuItem
        Caption = '&Screen...'
        OnClick = ScreenDebug1Click
      end
    end
    object About1: TMenuItem
      Caption = '&Help'
      object About2: TMenuItem
        Caption = 'About...'
        OnClick = About2Click
      end
    end
  end
end
