VERSION 5.00
Begin VB.Form Form_main 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "HRD Visual v0.1 by Dnomd343"
   ClientHeight    =   6585
   ClientLeft      =   45
   ClientTop       =   390
   ClientWidth     =   9390
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6585
   ScaleWidth      =   9390
   StartUpPosition =   2  '屏幕中心
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   8280
      Top             =   5385
   End
   Begin VB.TextBox Text_debug 
      Height          =   5190
      Left            =   5505
      MultiLine       =   -1  'True
      TabIndex        =   3
      Top             =   495
      Width           =   2250
   End
   Begin VB.CommandButton Command_Get_Code 
      Caption         =   "生成编码"
      Height          =   465
      Left            =   210
      TabIndex        =   2
      Top             =   5970
      Width           =   975
   End
   Begin VB.CommandButton Command_Print 
      Caption         =   "解译编码"
      Height          =   465
      Left            =   3180
      TabIndex        =   1
      Top             =   5940
      Width           =   975
   End
   Begin VB.TextBox Text_Code 
      Alignment       =   2  'Center
      BeginProperty Font 
         Name            =   "微软雅黑"
         Size            =   15.75
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   465
      Left            =   1200
      TabIndex        =   0
      Text            =   "4FEA13400"
      Top             =   5955
      Width           =   2040
   End
End
Attribute VB_Name = "Form_main"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim print_now As Boolean
Dim click_x As Integer, click_y As Integer
Dim click_x_ As Integer, click_y_ As Integer
Dim mouse_x As Integer, mouse_y As Integer
Dim output As Case_size
Dim case_main As Case_detail
Dim exclude(-1 To 1, -1 To 1) As Boolean

Private Sub Command_Get_Code_Click()
  Text_Code = Get_Code(case_main)
End Sub

Private Sub Form_Load()
  print_now = False
  Dim i As Integer, j As Integer
  For i = 0 To 14
    case_main.kind(i) = 255
  Next i
  For i = 0 To 3
    For j = 0 To 4
      case_main.status(i, j) = 254
    Next j
  Next i
  'Call Parse_Code("4FEA13400")
  'Call Parse_Code("5000300C0")
  'case_main = Parse_data
  output.start_x = 150
  output.start_y = 150
  'output.start_x = 0
  'output.start_y = 0
  output.square_width = 1000
  output.gap = 100
  style.block_line_width = 1
  style.case_line_width = 2
  style.block_line_color = RGB(0, 158, 240)
  style.case_line_color = RGB(0, 158, 240)
  style.block_color = RGB(225, 245, 255)
  style.case_color = RGB(248, 254, 255)
'  case_main.status(1, 1) = 254
'  case_main.status(1, 2) = 254
'  case_main.status(2, 1) = 254
'  case_main.status(2, 2) = 254
'  case_main.kind(0) = 255
  Call Output_case(Form_main, case_main, output)
  'Call Get_Code(case_main)

End Sub
Private Sub Command_Print_Click()
  If Len(Text_Code) <> 9 Then MsgBox "编码有误", , "提示": Exit Sub
  If Parse_Code(Text_Code) = True Then
    case_main = Parse_data
    Call Output_case(Form_main, case_main, output)
  Else
    MsgBox "编码有误", , "提示"
  End If
End Sub

Private Sub Form_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
  Dim x_ As Integer, y_ As Integer
  Dim block_x As Single, block_y As Single
  
  block_x = Get_block_x(X)
  block_y = Get_block_y(Y)
  
  If Int(block_x) <> block_x Then Exit Sub
  If Int(block_y) <> block_y Then Exit Sub
  If block_x < 1 Or block_x > 4 Then Exit Sub
  If block_y < 1 Or block_y > 5 Then Exit Sub
  click_x = block_x
  click_y = block_y
  click_x_ = X
  click_y_ = Y
  
  If case_main.status(click_x - 1, click_y - 1) <> 254 Then Exit Sub
  
  
  For y_ = -1 To 1
    For x_ = -1 To 1
      exclude(x_, y_) = False
    Next x_
  Next y_
  If case_main.kind(0) <> 255 Then exclude(-1, -1) = True: exclude(-1, 1) = True: exclude(1, -1) = True: exclude(1, 1) = True
  
  If click_x = 1 Then exclude(-1, -1) = True: exclude(-1, 0) = True: exclude(-1, 1) = True
  If click_x = 4 Then exclude(1, -1) = True: exclude(1, 0) = True: exclude(1, 1) = True
  If click_y = 1 Then exclude(-1, -1) = True: exclude(0, -1) = True: exclude(1, -1) = True
  If click_y = 5 Then exclude(-1, 1) = True: exclude(0, 1) = True: exclude(1, 1) = True
  For y_ = -1 To 1
    For x_ = -1 To 1
      If click_x + x_ >= 1 And click_x + x_ <= 4 And click_y + y_ >= 1 And click_y + y_ <= 5 Then
        If case_main.status(click_x + x_ - 1, click_y + y_ - 1) <> 254 Then exclude(x_, y_) = True
      End If
    Next x_
  Next y_

  print_now = True
  Call Form_MouseMove(Button, Shift, X, Y)
End Sub
Private Sub Form_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
  If print_now = False Then Exit Sub
  Form_main.Cls
  Call Output_case(Form_main, case_main, output)

  Dim delta_x As Integer, delta_y As Integer
  Dim print_x As Integer, print_y As Integer
  Dim print_width As Integer, print_height As Integer
  
  delta_x = Get_block_x(X) - click_x
  delta_y = Get_block_y(Y) - click_y

  If Abs(delta_x) <> 0 Then delta_x = delta_x / Abs(delta_x)
  If Abs(delta_y) <> 0 Then delta_y = delta_y / Abs(delta_y)
  
  mouse_x = click_x
  mouse_y = click_y
  
  If Abs(delta_x) + Abs(delta_y) = 1 Then
    If exclude(delta_x, delta_y) = False Then
      mouse_x = click_x + delta_x
      mouse_y = click_y + delta_y
    End If
  End If
  If Abs(delta_x) + Abs(delta_y) = 2 Then
    If exclude(delta_x, 0) = True And exclude(0, delta_y) = False Then
      mouse_y = click_y + delta_y
    End If
    If exclude(delta_x, 0) = False And exclude(0, delta_y) = True Then
      mouse_x = click_x + delta_x
    End If
    If exclude(delta_x, delta_y) = True And exclude(delta_x, 0) = False And exclude(0, delta_y) = False Then
      If Abs(click_x_ - X) > Abs(click_y_ - Y) Then
        mouse_x = click_x + delta_x
      Else
        mouse_y = click_y + delta_y
      End If
    End If
    If exclude(delta_x, delta_y) = False And exclude(delta_x, 0) = False And exclude(0, delta_y) = False Then
      mouse_x = click_x + delta_x
      mouse_y = click_y + delta_y
    End If
    
  End If
  
  print_x = (Get_min(click_x, mouse_x) - 1) * (output.square_width + output.gap) + output.gap + output.start_x ' 计算起始位置
  print_y = (Get_min(click_y, mouse_y) - 1) * (output.square_width + output.gap) + output.gap + output.start_y
  
  If Abs(click_x - mouse_x) = 1 Then
    print_width = output.square_width * 2 + output.gap
  Else
    print_width = output.square_width
  End If
  If Abs(click_y - mouse_y) = 1 Then
    print_height = output.square_width * 2 + output.gap
  Else
    print_height = output.square_width
  End If
  Call Print_Block(Form_main, print_x, print_y, print_width, print_height, style.block_line_width, style.block_color, style.block_line_color)
End Sub

Private Sub Form_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
  If print_now = False Then Exit Sub
  print_now = False
  Dim i As Integer, j As Integer, num As Integer
  Dim empty_num As Integer
  Dim block_x As Integer, block_y As Integer
  Dim block_width As Integer, block_height As Integer
  block_x = Get_min(click_x, mouse_x) - 1
  block_y = Get_min(click_y, mouse_y) - 1
  block_width = Abs(click_x - mouse_x) + 1
  block_height = Abs(click_y - mouse_y) + 1
  empty_num = 0
  If block_width = 2 And block_height = 2 Then
    For i = 0 To 3
      For j = 0 To 4
        If case_main.status(i, j) = 254 Then empty_num = empty_num + 1
      Next j
    Next i
    If empty_num >= 6 Then
      case_main.kind(0) = 0
      case_main.status(block_x, block_y) = 0
      case_main.status(block_x, block_y + 1) = 0
      case_main.status(block_x + 1, block_y) = 0
      case_main.status(block_x + 1, block_y + 1) = 0
    End If
  ElseIf block_width = 2 And block_height = 1 Then
    num = Get_empty_seat
    For i = 0 To 3
      For j = 0 To 4
        If case_main.status(i, j) = 254 Then empty_num = empty_num + 1
      Next j
    Next i
    If num <> 0 And empty_num >= 4 Then
      case_main.kind(num) = 1
      case_main.status(block_x, block_y) = num
      case_main.status(block_x + 1, block_y) = num
    End If
  ElseIf block_width = 1 And block_height = 2 Then
    num = Get_empty_seat
    For i = 0 To 3
      For j = 0 To 4
        If case_main.status(i, j) = 254 Then empty_num = empty_num + 1
      Next j
    Next i
    If num <> 0 And empty_num >= 4 Then
      case_main.kind(num) = 2
      case_main.status(block_x, block_y) = num
      case_main.status(block_x, block_y + 1) = num
    End If
  ElseIf block_width = 1 And block_height = 1 Then
    num = Get_empty_seat
    For i = 0 To 3
      For j = 0 To 4
        If case_main.status(i, j) = 254 Then empty_num = empty_num + 1
      Next j
    Next i
    If num <> 0 And empty_num >= 3 Then
      case_main.kind(num) = 3
      case_main.status(block_x, block_y) = num
    End If
  End If
  Call Output_case(Form_main, case_main, output)
End Sub
Private Function Get_empty_seat() As Integer
  Dim i As Integer
  Get_empty_seat = 0
  For i = 1 To 14
    If case_main.kind(i) = 255 Then
      Get_empty_seat = i
      Exit For
    End If
  Next i
End Function
Private Function Get_min(dat_1 As Integer, dat_2 As Integer) As Integer
  If dat_1 > dat_2 Then
    Get_min = dat_2
  Else
    Get_min = dat_1
  End If
End Function
Private Function Get_block_x(dat As Single) As Single ' 间隔上返回*.5 左越界返回0 右越界返回5
  dat = dat - output.start_x
  Get_block_x = -1
  Dim i As Integer
  For i = 1 To 4
    If dat > output.gap * i + output.square_width * (i - 1) And dat < (output.gap + output.square_width) * i Then Get_block_x = i
    If dat >= (output.gap + output.square_width) * i And dat <= output.gap * (i + 1) + output.square_width * i Then Get_block_x = i + 0.5
  Next i
  If dat > (output.gap + output.square_width) * 4 Then Get_block_x = 5
  If dat < output.gap Then Get_block_x = 0
  dat = dat + output.start_x
End Function
Private Function Get_block_y(dat As Single) As Single ' 间隔上返回*.5 上越界返回0 下越界返回6
  dat = dat - output.start_y
  Get_block_y = -1
  Dim i As Integer
  For i = 1 To 5
    If dat > output.gap * i + output.square_width * (i - 1) And dat < (output.gap + output.square_width) * i Then Get_block_y = i
    If dat >= (output.gap + output.square_width) * i And dat <= output.gap * (i + 1) + output.square_width * i Then Get_block_y = i + 0.5
  Next i
  If dat > (output.gap + output.square_width) * 5 Then Get_block_y = 6
  If dat < output.gap Then Get_block_y = 0
  dat = dat + output.start_y
End Function

Private Sub Text_Code_KeyPress(KeyAscii As Integer)
  If KeyAscii = 13 Then Call Command_Print_Click
End Sub

Private Sub Timer1_Timer()
  Dim i As Integer, j As Integer
  Dim debug_dat As String
  debug_dat = ""
  For j = 0 To 4
    For i = 0 To 3
      If case_main.status(i, j) = 254 Then
        debug_dat = debug_dat & "- "
      Else
        debug_dat = debug_dat & case_main.status(i, j) & " "
      End If
    Next i
    debug_dat = debug_dat & vbCrLf
  Next j
  
  For i = 0 To 14
    debug_dat = debug_dat & Trim(i) & ": " & case_main.kind(i) & vbCrLf
  Next i
  Text_debug = debug_dat
End Sub
