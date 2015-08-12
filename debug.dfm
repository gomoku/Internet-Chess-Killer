object BoardScanerWindow: TBoardScanerWindow
  Left = 1020
  Top = 564
  BorderStyle = bsDialog
  Caption = 'Board Scaner Window'
  ClientHeight = 131
  ClientWidth = 242
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object lbScreenPos: TLabel
    Left = 4
    Top = 6
    Width = 34
    Height = 13
    Caption = 'Screen'
  end
  object lbBoardPos: TLabel
    Left = 122
    Top = 6
    Width = 28
    Height = 13
    Caption = 'Board'
  end
  object gScreen: TDrawGrid
    Left = 4
    Top = 22
    Width = 109
    Height = 95
    ColCount = 8
    DefaultColWidth = 12
    DefaultRowHeight = 10
    FixedCols = 0
    RowCount = 8
    FixedRows = 0
    TabOrder = 0
  end
  object gBoard: TDrawGrid
    Left = 122
    Top = 22
    Width = 109
    Height = 95
    ColCount = 8
    DefaultColWidth = 12
    DefaultRowHeight = 10
    FixedCols = 0
    RowCount = 8
    FixedRows = 0
    TabOrder = 1
  end
end
