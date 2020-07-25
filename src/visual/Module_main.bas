Attribute VB_Name = "Module_main"
Option Explicit
Type Case_size ' 记录棋盘的大小
  start_x As Integer
  start_y As Integer
  square_width As Integer
  gap As Integer
End Type
Type Case_style ' 记录显示的颜色与边框粗细
  block_line_width As Integer
  case_line_width As Integer
  block_line_color As OLE_COLOR
  case_line_color As OLE_COLOR
  block_color As OLE_COLOR
  case_color As OLE_COLOR
End Type
Public version As String
Public white_style As Case_style
Public blue_style As Case_style
Public yellow_style As Case_style
Public green_style As Case_style
Public pink_style As Case_style
Public style As Case_style ' 通用显示样式
Public on_top As Boolean, is_tip As Boolean, style_color As String
Public Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
Private Sub init_style() ' 预置配色
  'skin_white
  white_style.block_line_width = 1
  white_style.case_line_width = 2
  white_style.block_line_color = RGB(0, 0, 0)
  white_style.case_line_color = RGB(0, 0, 0)
  white_style.block_color = RGB(250, 250, 250)
  white_style.case_color = RGB(256, 256, 256)
  'skin_blue
  blue_style.block_line_width = 1
  blue_style.case_line_width = 2
  blue_style.block_line_color = RGB(0, 158, 240)
  blue_style.case_line_color = RGB(0, 158, 240)
  blue_style.block_color = RGB(225, 245, 255)
  blue_style.case_color = RGB(248, 254, 255)
  'skin_yellow
  yellow_style.block_line_width = 1
  yellow_style.case_line_width = 2
  yellow_style.block_line_color = RGB(153, 0, 0)
  yellow_style.case_line_color = RGB(149, 149, 149)
  yellow_style.block_color = RGB(255, 215, 0)
  yellow_style.case_color = RGB(231, 231, 231)
  'skin_green
  green_style.block_line_width = 1
  green_style.case_line_width = 2
  green_style.block_line_color = RGB(61, 184, 78)
  green_style.case_line_color = RGB(46, 118, 72)
  green_style.block_color = RGB(61, 184, 78)
  green_style.case_color = RGB(233, 247, 212)
  'skin_pink
  pink_style.block_line_width = 1
  pink_style.case_line_width = 2
  pink_style.block_line_color = RGB(220, 20, 60)
  pink_style.case_line_color = RGB(255, 20, 147)
  pink_style.block_color = RGB(255, 192, 203)
  pink_style.case_color = RGB(255, 240, 245)
End Sub
Public Sub Get_settings() ' 从注册表获取上一次的设置
  On Error GoTo first_time ' 出错表明注册表键值不存在
  Dim ws
  Set ws = CreateObject("Wscript.Shell")
  If ws.RegRead("HKEY_CURRENT_USER\Software\HRD_Visual\on_top") = "False" Then on_top = False Else on_top = True
  If ws.RegRead("HKEY_CURRENT_USER\Software\HRD_Visual\is_tip") = "False" Then is_tip = False Else is_tip = True
  style_color = ws.RegRead("HKEY_CURRENT_USER\Software\HRD_Visual\skin")
  Exit Sub
first_time: ' 第一次设置
  on_top = True
  is_tip = True
  style_color = "blue"
End Sub
Public Sub Save_settings() ' 保存设置到注册表
  Dim ws
  Set ws = CreateObject("Wscript.Shell")
  ws.RegWrite "HKEY_CURRENT_USER\Software\HRD_Visual\on_top", Trim(on_top), "REG_SZ"
  ws.RegWrite "HKEY_CURRENT_USER\Software\HRD_Visual\is_tip", Trim(is_tip), "REG_SZ"
  ws.RegWrite "HKEY_CURRENT_USER\Software\HRD_Visual\skin", style_color, "REG_SZ"
End Sub
Sub Main() ' 程序入口
  version = "v1.2"
  Call init_style
  Form_main.Show
End Sub
