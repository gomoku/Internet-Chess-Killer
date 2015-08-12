object SquareViewForm: TSquareViewForm
  Left = 663
  Top = 365
  Width = 391
  Height = 339
  Caption = 'Square View Window'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    383
    305)
  PixelsPerInch = 96
  TextHeight = 13
  object gSquarePixels: TStringGrid
    Left = 1
    Top = 30
    Width = 382
    Height = 275
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 100
    DefaultColWidth = 8
    DefaultRowHeight = 8
    FixedCols = 0
    RowCount = 100
    FixedRows = 0
    TabOrder = 0
  end
  object edSquare: TEdit
    Left = 4
    Top = 4
    Width = 43
    Height = 21
    TabOrder = 1
  end
end
