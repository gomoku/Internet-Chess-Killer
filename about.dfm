object AboutForm: TAboutForm
  Left = 656
  Top = 293
  BorderStyle = bsToolWindow
  Caption = 'About'
  ClientHeight = 363
  ClientWidth = 431
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object RichEdit3: TRichEdit
    Left = 2
    Top = 2
    Width = 423
    Height = 357
    Lines.Strings = (
      'Program name: InternetChessKiller'
      
        'Target: automatic use computer chess program help for playing on' +
        ' chess servers'
      'Short description: '
      '  program periodically capture screen, '
      '  if chess board has been found with start position, '
      '     when chess program has been started'
      '  if board has been changed,'
      '    when analyze has been changed accordingly'
      'Advices for use:'
      
        '  1. Don'#39't touch internetchesskiller window when you are playing' +
        ' !!!'
      
        '  2. Use new login, if you intend get higher rating, than you ha' +
        've now'
      '  3. Don'#39't copy engine moves which you don'#39't understand'
      
        'Bug:  For small board and not standart pieces/board doesn'#39't work' +
        '.'
      'Author: Dmitry Morozov'
      'Contacts: '
      '     email: dm-chess@yandex.ru'
      '     please, fill up the letter subject: internetchesskiller')
    ReadOnly = True
    TabOrder = 0
  end
end
