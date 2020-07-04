VERSION 5.00
Begin VB.Form Form_main 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "HRD Visual v0.2 by Dnomd343"
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
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   360
      Left            =   5820
      TabIndex        =   4
      Top             =   5925
      Width           =   840
   End
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
Dim print_now As Boolean ' 标记当前是否正处于绘制状态
Dim click_x As Integer, click_y As Integer ' 记录按下位置对应的棋盘编号 click_x -> 1 ~ 4 / click_y -> 1 ~ 5
Dim click_x_ As Integer, click_y_ As Integer ' 记录按下位置的窗体坐标
Dim mouse_x As Integer, mouse_y As Integer ' 记录当前鼠标位置合法对应的棋盘编号
Dim output As Case_size ' 输出大小
Dim case_main As Case_detail ' 布局信息
Dim exclude(-1 To 1, -1 To 1) As Boolean ' 记录按下位置周围是否能放置棋子 true -> 不难 / false -> 能

Private Sub Command_Get_Code_Click()
  Text_Code = Get_Code(case_main)
End Sub

Private Sub Command1_Click()
  MsgBox Check_2x2_seat
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

  output.start_x = 150
  output.start_y = 150

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

Private Sub Form_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single) ' 按下鼠标按键
  Dim x_ As Integer, y_ As Integer, num As Integer
  Dim raw_x As Single, raw_y As Single
  Dim addr_x As Integer, addr_y As Integer
  Dim clear As Boolean
  clear = False
  raw_x = Get_block_x(X) ' 得到点击位置在棋盘上的编号
  raw_y = Get_block_y(Y)
  If raw_x < 1 Or raw_x > 4 Or raw_y < 1 Or raw_y > 5 Then Exit Sub ' 点击在棋盘外 退出
  addr_x = Int(raw_x) - 1 ' 若点击位置为间隙则映射到其左/上方
  addr_y = Int(raw_y) - 1
  num = case_main.status(addr_x, addr_y) ' 得到编号或254
  If num <> 254 Then ' 该位置不为空
    If Int(raw_x) <> raw_x And Int(raw_y) <> raw_y Then ' 若处在两间隙相交处
      If num = 0 And Get_addr_x(num) = addr_x And Get_addr_y(num) = addr_y Then clear = True ' 判断是否点击在2 * 2块正中央
    ElseIf Int(raw_x) <> raw_x Then ' 若处在纵向间隙
      If Get_addr_x(num) = addr_x Then '判断是否点击是否在棋子上
        If case_main.kind(num) = 0 Or case_main.kind(num) = 1 Then clear = True
      End If
    ElseIf Int(raw_y) <> raw_y Then ' 若处在横向间隙
      If Get_addr_y(num) = addr_y Then '判断是否点击是否在棋子上
        If case_main.kind(num) = 0 Or case_main.kind(num) = 2 Then clear = True
      End If
    Else ' 不处在间隙
      clear = True
    End If
  End If
  If clear = True Then ' 若被标识为点击在棋子上
    Call Clear_block(num) ' 清除该棋子
    Call Output_case(Form_main, case_main, output) ' 刷新显示界面
    Exit Sub ' 退出
  End If
  If Int(raw_x) <> raw_x Or Int(raw_y) <> raw_y Then Exit Sub ' 点击在间隙上 退出
  click_x = raw_x: click_y = raw_y ' 记录有效的点击位置编号
  click_x_ = X: click_y_ = Y ' 记录点击的真实位置
  num = case_main.status(click_x - 1, click_y - 1) ' 点击位置对应的编码
  For y_ = -1 To 1
    For x_ = -1 To 1
      exclude(x_, y_) = False ' 全部初始化为可放置
    Next x_
  Next y_
  If case_main.kind(0) <> 255 Then exclude(-1, -1) = True: exclude(-1, 1) = True: exclude(1, -1) = True: exclude(1, 1) = True ' 若2 * 2块已存在 则四角都标记为不可放置
  If click_x = 1 Then exclude(-1, -1) = True: exclude(-1, 0) = True: exclude(-1, 1) = True ' 点击在最左边
  If click_x = 4 Then exclude(1, -1) = True: exclude(1, 0) = True: exclude(1, 1) = True ' 点击在最左边
  If click_y = 1 Then exclude(-1, -1) = True: exclude(0, -1) = True: exclude(1, -1) = True ' 点击在最上面
  If click_y = 5 Then exclude(-1, 1) = True: exclude(0, 1) = True: exclude(1, 1) = True ' 点击在最下面
  For y_ = -1 To 1
    For x_ = -1 To 1
      If click_x + x_ >= 1 And click_x + x_ <= 4 And click_y + y_ >= 1 And click_y + y_ <= 5 Then ' 防止越界
        If case_main.status(click_x + x_ - 1, click_y + y_ - 1) <> 254 Then exclude(x_, y_) = True ' 不为空的标记为不可放置
      End If
    Next x_
  Next y_
  print_now = True ' 进入绘制模式
  Call Form_MouseMove(Button, Shift, X, Y) ' 发起鼠标移动事件
End Sub
Private Sub Form_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single) ' 移动鼠标
  Dim delta_x As Integer, delta_y As Integer
  Dim print_x As Integer, print_y As Integer
  Dim print_width As Integer, print_height As Integer
  If print_now = False Then Exit Sub ' 判断是否在绘制模式
  Call Output_case(Form_main, case_main, output) ' 覆盖上一次的显示
  delta_x = signed(Get_block_x(X) - click_x) ' 记录鼠标位置与点击位置的相对方向
  delta_y = signed(Get_block_y(Y) - click_y)
  mouse_x = click_x: mouse_y = click_y ' 默认
  If Abs(delta_x) + Abs(delta_y) = 1 Then ' 如果在上下左右
    If exclude(delta_x, delta_y) = False Then ' 目标位置可放置棋子
      mouse_x = click_x + delta_x
      mouse_y = click_y + delta_y
    End If
  End If
  If Abs(delta_x) + Abs(delta_y) = 2 Then ' 如果在斜对角
    If exclude(delta_x, 0) = True And exclude(0, delta_y) = False Then mouse_y = click_y + delta_y ' 若侧边被挡到
    If exclude(delta_x, 0) = False And exclude(0, delta_y) = True Then mouse_x = click_x + delta_x
    If exclude(delta_x, delta_y) = True And exclude(delta_x, 0) = False And exclude(0, delta_y) = False Then ' 若斜角不可放置而两边是空的
      If Abs(click_x_ - X) > Abs(click_y_ - Y) Then ' x轴方向位移较大
        mouse_x = click_x + delta_x
      Else ' y轴方向位移较大
        mouse_y = click_y + delta_y
      End If
    End If
    If exclude(delta_x, delta_y) = False And exclude(delta_x, 0) = False And exclude(0, delta_y) = False Then ' 若三个位置均可放置
      mouse_x = click_x + delta_x
      mouse_y = click_y + delta_y
    End If
  End If
  print_x = (min(click_x, mouse_x) - 1) * (output.square_width + output.gap) + output.gap + output.start_x ' 计算绘制起始位置
  print_y = (min(click_y, mouse_y) - 1) * (output.square_width + output.gap) + output.gap + output.start_y
  If Abs(click_x - mouse_x) = 1 Then print_width = output.square_width * 2 + output.gap Else print_width = output.square_width ' 计算绘制宽度
  If Abs(click_y - mouse_y) = 1 Then print_height = output.square_width * 2 + output.gap Else print_height = output.square_width ' 计算绘制高度
  Call Print_Block(Form_main, print_x, print_y, print_width, print_height, style.block_line_width, style.block_color, style.block_line_color) ' 绘制
End Sub
Private Sub Form_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single) ' 释放鼠标按键
  Dim i As Integer, j As Integer, num As Integer
  Dim block_x As Integer, block_y As Integer
  Dim block_width As Integer, block_height As Integer
  If print_now = False Then Exit Sub ' 判断是否在绘制模式
  print_now = False ' 退出绘制模式
  block_x = min(click_x, mouse_x) - 1 ' 计算绘制棋子的起始位置
  block_y = min(click_y, mouse_y) - 1
  block_width = Abs(click_x - mouse_x) + 1 ' 计算绘制棋子的宽和高
  block_height = Abs(click_y - mouse_y) + 1
    If block_width = 2 And block_height = 2 Then ' 绘制的棋子是2 * 2
    If Get_empty_num >= 6 Then ' 当前至少有6个空位
      case_main.kind(0) = 0
      case_main.status(block_x, block_y) = 0
      case_main.status(block_x, block_y + 1) = 0
      case_main.status(block_x + 1, block_y) = 0
      case_main.status(block_x + 1, block_y + 1) = 0
    End If
  ElseIf block_width = 2 And block_height = 1 Then ' 绘制的棋子是2 * 1
    num = Get_empty_seat
    If num <> 0 And Get_empty_num >= 4 Then ' 当前棋子数未到上限且至少存在4个空位
      case_main.kind(num) = 1
      case_main.status(block_x, block_y) = num
      case_main.status(block_x + 1, block_y) = num
      If case_main.kind(0) <> 0 And Check_2x2_seat = False Then Call Clear_block(num)
    End If
  ElseIf block_width = 1 And block_height = 2 Then ' 绘制的棋子是1 * 2
    num = Get_empty_seat
    If num <> 0 And Get_empty_num >= 4 Then ' 当前棋子数未到上限且至少存在4个空位
      case_main.kind(num) = 2
      case_main.status(block_x, block_y) = num
      case_main.status(block_x, block_y + 1) = num
      If case_main.kind(0) <> 0 And Check_2x2_seat = False Then Call Clear_block(num)
    End If
  ElseIf block_width = 1 And block_height = 1 Then ' 绘制的棋子是1 * 1
    num = Get_empty_seat
    If num <> 0 And Get_empty_num >= 3 Then ' 当前棋子数未到上限且至少存在3个空位
      case_main.kind(num) = 3
      case_main.status(block_x, block_y) = num
      If case_main.kind(0) <> 0 And Check_2x2_seat = False Then Call Clear_block(num)
    End If
  End If
  Call Output_case(Form_main, case_main, output) ' 刷新显示界面
End Sub
Private Sub Clear_block(num As Integer) ' 根据编号清空棋子信息
  Dim addr_x As Integer, addr_y As Integer
  addr_x = Get_addr_x(num) ' 得到棋子左上角位置
  addr_y = Get_addr_y(num)
  If case_main.kind(num) = 0 Then ' 2 * 2
    case_main.kind(num) = 255
    case_main.status(addr_x, addr_y) = 254
    case_main.status(addr_x, addr_y + 1) = 254
    case_main.status(addr_x + 1, addr_y) = 254
    case_main.status(addr_x + 1, addr_y + 1) = 254
  ElseIf case_main.kind(num) = 1 Then ' 2 * 1
    case_main.kind(num) = 255
    case_main.status(addr_x, addr_y) = 254
    case_main.status(addr_x + 1, addr_y) = 254
  ElseIf case_main.kind(num) = 2 Then ' 1 * 2
    case_main.kind(num) = 255
    case_main.status(addr_x, addr_y) = 254
    case_main.status(addr_x, addr_y + 1) = 254
  ElseIf case_main.kind(num) = 3 Then ' 1 * 1
    case_main.kind(num) = 255
    case_main.status(addr_x, addr_y) = 254
  End If
End Sub
Private Function Check_2x2_seat() As Boolean
  Dim i As Integer, j As Integer
  Check_2x2_seat = False
  'If case_main.kind(0) = 0 Then Exit Function
  For j = 0 To 3
    For i = 0 To 2
      If case_main.status(i, j) = 254 And case_main.status(i, j + 1) = 254 And case_main.status(i + 1, j) = 254 And case_main.status(i + 1, j + 1) = 254 Then Check_2x2_seat = True
    Next i
  Next j
End Function
Private Function Get_empty_num() As Integer ' 统计当前空位数量
  Dim i As Integer, j As Integer
  Get_empty_num = 0
  For i = 0 To 3
    For j = 0 To 4
      If case_main.status(i, j) = 254 Then Get_empty_num = Get_empty_num + 1
    Next j
  Next i
End Function
Private Function Get_empty_seat() As Integer ' 在case_main.kind中找到空位
  Dim i As Integer
  Get_empty_seat = 0
  For i = 1 To 14
    If case_main.kind(i) = 255 Then ' 该位置暂未定义
      Get_empty_seat = i
      Exit For
    End If
  Next i
End Function
Private Function signed(dat As Single) ' 得到符号 返回-1 / 0 / 1
  If Abs(dat) <> 0 Then signed = dat / Abs(dat) Else signed = 0
End Function
Private Function min(dat_1 As Integer, dat_2 As Integer) As Integer ' 返回较小的值
  If dat_1 > dat_2 Then min = dat_2 Else min = dat_1
End Function
Private Function Get_addr_x(num As Integer) As Integer ' 找到编号的左上角x坐标 编号为不存在则返回255
  Dim i As Integer, j As Integer
  Get_addr_x = 255
  For j = 0 To 4
    For i = 0 To 3
      If case_main.status(i, j) = num Then
        Get_addr_x = i
        Exit Function
      End If
    Next i
  Next j
End Function
Private Function Get_addr_y(num As Integer) As Integer ' 找到编号的左上角y坐标 编号为不存在则返回255
  Dim i As Integer, j As Integer
  Get_addr_y = 255
  For j = 0 To 4
    For i = 0 To 3
      If case_main.status(i, j) = num Then
        Get_addr_y = j
        Exit Function
      End If
    Next i
  Next j
End Function
Private Function Get_block_x(dat As Single) As Single ' 计算鼠标位置所在对应棋盘上的横坐标 间隔上返回*.5 左越界返回0 右越界返回5
  dat = dat - output.start_x ' 去除起始偏移
  Dim i As Integer
  For i = 1 To 4
    If dat > output.gap * i + output.square_width * (i - 1) And dat < (output.gap + output.square_width) * i Then Get_block_x = i ' 点击在棋子上
    If dat >= (output.gap + output.square_width) * i And dat <= output.gap * (i + 1) + output.square_width * i Then Get_block_x = i + 0.5 ' 点击在棋子间隔
  Next i
  If dat > (output.gap + output.square_width) * 4 Then Get_block_x = 5 ' 右越界
  If dat < output.gap Then Get_block_x = 0 ' 左越界
  dat = dat + output.start_x
End Function
Private Function Get_block_y(dat As Single) As Single ' 计算鼠标位置所在对应棋盘上的纵坐标 间隔上返回*.5 上越界返回0 下越界返回6
  dat = dat - output.start_y ' 去除起始偏移
  Dim i As Integer
  For i = 1 To 5
    If dat > output.gap * i + output.square_width * (i - 1) And dat < (output.gap + output.square_width) * i Then Get_block_y = i ' 点击在棋子上
    If dat >= (output.gap + output.square_width) * i And dat <= output.gap * (i + 1) + output.square_width * i Then Get_block_y = i + 0.5 ' 点击在棋子间隔
  Next i
  If dat > (output.gap + output.square_width) * 5 Then Get_block_y = 6 ' 下越界
  If dat < output.gap Then Get_block_y = 0 ' 上越界
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
