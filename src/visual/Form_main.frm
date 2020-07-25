VERSION 5.00
Begin VB.Form Form_main 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "HRD Visual"
   ClientHeight    =   6495
   ClientLeft      =   45
   ClientTop       =   690
   ClientWidth     =   4815
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6495
   ScaleWidth      =   4815
   StartUpPosition =   2  '屏幕中心
   Begin VB.PictureBox Picture_Print 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "微软雅黑"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   15
      Left            =   0
      ScaleHeight     =   15
      ScaleWidth      =   15
      TabIndex        =   5
      Top             =   0
      Visible         =   0   'False
      Width           =   15
   End
   Begin VB.Timer Timer_Tip 
      Enabled         =   0   'False
      Interval        =   1200
      Left            =   0
      Top             =   0
   End
   Begin VB.Timer Timer_KeyPress 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   0
      Top             =   0
   End
   Begin VB.CommandButton Command_OK 
      Caption         =   "OK"
      Height          =   465
      Left            =   3615
      TabIndex        =   3
      Top             =   5880
      Width           =   1050
   End
   Begin VB.CommandButton Command_Clear 
      Caption         =   "Clear"
      Height          =   465
      Left            =   150
      TabIndex        =   2
      Top             =   5900
      Width           =   1050
   End
   Begin VB.TextBox Text_Focus 
      Height          =   270
      Left            =   6000
      TabIndex        =   0
      Top             =   0
      Width           =   180
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
      Left            =   1400
      MaxLength       =   9
      TabIndex        =   1
      Text            =   "---"
      Top             =   5900
      Width           =   2040
   End
   Begin VB.Label Label_Print 
      AutoSize        =   -1  'True
      BeginProperty Font 
         Name            =   "微软雅黑"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   15
      Left            =   0
      TabIndex        =   6
      Top             =   0
      Visible         =   0   'False
      Width           =   15
   End
   Begin VB.Label Label_Tip 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      BeginProperty Font 
         Name            =   "微软雅黑"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   15
      Left            =   15
      TabIndex        =   4
      Top             =   0
      Visible         =   0   'False
      Width           =   15
   End
   Begin VB.Menu menu_function 
      Caption         =   "功能"
      Begin VB.Menu menu_output_pic 
         Caption         =   "导出图片"
         Begin VB.Menu ouput_pic_no_code 
            Caption         =   "无编码图片"
         End
         Begin VB.Menu output_pic_with_code 
            Caption         =   "带编码图片"
         End
      End
      Begin VB.Menu menu_analyse_code 
         Caption         =   "解析编码"
      End
      Begin VB.Menu menu_get_code 
         Caption         =   "生成编码"
      End
      Begin VB.Menu menu_clear 
         Caption         =   "清空显示"
      End
   End
   Begin VB.Menu menu_setting 
      Caption         =   "设置"
      Begin VB.Menu menu_on_top 
         Caption         =   "窗口最上"
         Checked         =   -1  'True
      End
      Begin VB.Menu menu_is_tip 
         Caption         =   "输出提示"
         Checked         =   -1  'True
      End
      Begin VB.Menu menu_skin 
         Caption         =   "外观颜色"
         Begin VB.Menu skin_white 
            Caption         =   "极简白"
         End
         Begin VB.Menu skin_blue 
            Caption         =   "胖次蓝"
         End
         Begin VB.Menu skin_yellow 
            Caption         =   "土豪金"
         End
         Begin VB.Menu skin_green 
            Caption         =   "原谅绿"
         End
         Begin VB.Menu skin_pink 
            Caption         =   "木耳粉"
         End
      End
   End
   Begin VB.Menu menu_help 
      Caption         =   "帮助"
      Begin VB.Menu usage 
         Caption         =   "使用说明"
      End
      Begin VB.Menu update 
         Caption         =   "检查更新"
      End
      Begin VB.Menu about 
         Caption         =   "关于"
      End
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
Private Sub Form_Load() ' 窗体初始化
  Form_main.Caption = Form_main.Caption & " " & version
  Call Init_case
  Call Get_settings ' 读取上一次的设置
  menu_on_top.Checked = Not on_top: Call menu_on_top_Click
  menu_is_tip.Checked = Not is_tip: Call menu_is_tip_Click
  If style_color = "white" Then
    Call skin_white_Click
  ElseIf style_color = "blue" Then
    Call skin_blue_Click
  ElseIf style_color = "yellow" Then
    Call skin_yellow_Click
  ElseIf style_color = "green" Then
    Call skin_green_Click
  ElseIf style_color = "pink" Then
    Call skin_pink_Click
  Else
    Call skin_blue_Click
  End If
  Call Save_settings ' 保存设置
  print_now = False
  output.start_x = 150
  output.start_y = 150
  output.gap = 100
  output.square_width = 1000
  Call Output_case(Form_main, case_main, output) ' 显示界面
End Sub
Private Sub ouput_pic_no_code_Click() ' 导出无编码图片
  Call output_pic(False)
  MsgBox "图片已保存"
End Sub
Private Sub ouput_pic_with_code_Click() ' 导出带编码图片
  Call output_pic(True)
  MsgBox "图片已保存"
End Sub
Private Sub menu_analyse_code_Click() ' 解析编码
  If Try_parse_code(Text_Code) = False Then
    MsgBox "编码错误"
  Else
    Text_Code = UCase(Text_Code) & String(9 - Len(Text_Code), "0") ' 修改为大写并补0
    MsgBox "解析完成"
  End If
End Sub
Private Sub menu_get_code_Click() ' 生成编码
  Call Try_get_code
  If case_main.kind(0) <> 0 Then
    Text_Focus.SetFocus ' 防止文本框光标闪烁
    MsgBox "2 * 2块未确定"
    Text_Code.ForeColor = vbBlack
    Text_Code = "---"
  Else
    Text_Code = Text_Code & String(9 - Len(Text_Code), "0") ' 补0
    Text_Code.ForeColor = vbBlack
    MsgBox "编码：" & Text_Code
  End If
End Sub
Private Sub menu_clear_Click() ' 清空显示
  Call Command_Clear_Click
  Text_Code = "---"
End Sub
Private Sub menu_on_top_Click() ' 是否出口最上
  menu_on_top.Checked = Not menu_on_top.Checked
  on_top = menu_on_top.Checked
  If on_top = True Then
    SetWindowPos Me.hwnd, -1, 0, 0, 0, 0, 1 Or 2
  Else
    SetWindowPos Me.hwnd, -2, 0, 0, 0, 0, 1 Or 2
  End If
  Call Save_settings
End Sub
Private Sub menu_is_tip_Click() ' 是否显示提示
  menu_is_tip.Checked = Not menu_is_tip.Checked
  is_tip = menu_is_tip.Checked
  Call Save_settings
End Sub
Private Sub skin_white_Click() ' 白色皮肤
  Call skin_empty
  skin_white.Checked = True
  style_color = "white"
  Call load_skin(white_style)
End Sub
Private Sub skin_blue_Click() ' 蓝色皮肤
  Call skin_empty
  skin_blue.Checked = True
  style_color = "blue"
  Call load_skin(blue_style)
End Sub
Private Sub skin_yellow_Click() ' 黄色皮肤
  Call skin_empty
  skin_yellow.Checked = True
  style_color = "yellow"
  Call load_skin(yellow_style)
End Sub
Private Sub skin_green_Click() ' 绿色皮肤
  Call skin_empty
  skin_green.Checked = True
  style_color = "green"
  Call load_skin(green_style)
End Sub
Private Sub skin_pink_Click() ' 粉色皮肤
  Call skin_empty
  skin_pink.Checked = True
  style_color = "pink"
  Call load_skin(pink_style)
End Sub
Private Sub skin_empty() ' 清除所有皮肤项勾选
  skin_white.Checked = False
  skin_blue.Checked = False
  skin_yellow.Checked = False
  skin_green.Checked = False
  skin_pink.Checked = False
End Sub
Private Sub load_skin(new_style As Case_style) ' 载入皮肤
  style.block_line_width = new_style.block_line_width
  style.case_line_width = new_style.case_line_width
  style.block_line_color = new_style.block_line_color
  style.case_line_color = new_style.case_line_color
  style.block_color = new_style.block_color
  style.case_color = new_style.case_color
  Call Output_case(Form_main, case_main, output)
  Call Save_settings
End Sub
Private Sub usage_click() ' 使用说明
  Dim usage_data As String
  usage_data = usage_data & "· 鼠标拖拽画出棋子" & vbCrLf
  usage_data = usage_data & "· 单击删除棋子" & vbCrLf
  usage_data = usage_data & "· 输入编码自动生成布局" & vbCrLf
  usage_data = usage_data & "· 双击文本框可清空内容" & vbCrLf
  usage_data = usage_data & "· 清除按钮可清空上方内容" & vbCrLf
  usage_data = usage_data & "· 完成按钮可补0生成九位编码" & vbCrLf
  usage_data = usage_data & "· 空白处右键可导出无编码图片" & vbCrLf
  usage_data = usage_data & "· 空白处点击滚轮可导出带编码图片" & vbCrLf
  MsgBox usage_data, , "使用说明"
End Sub
Private Sub update_Click() ' 检查更新
  Call Check_update
End Sub
Private Sub about_Click() ' 关于
  MsgBox "HRD Visual：华容道编码可视化工具" & vbCrLf & "    版本：" & version & "  (by Dnomd343)", , "关于"
End Sub
Private Sub Command_Clear_Click() ' 清除当前显示
  Call Init_case
  Call Output_case(Form_main, case_main, output) ' 刷新显示界面
End Sub
Private Sub Command_OK_Click() ' 完成按钮
  If Try_parse_code(Text_Code) = False Then
    MsgBox "编码错误"
  Else
    Text_Code = UCase(Text_Code) & String(9 - Len(Text_Code), "0") ' 修改为大写并补0
    Text_Focus.SetFocus ' 切走焦点
  End If
End Sub
Private Sub Text_Code_DblClick() ' 双击文本框 清空内容
  Text_Code = ""
  Text_Code.ForeColor = vbBlack
End Sub
Private Sub Text_Code_GotFocus() ' 文本框得到焦点
  If Text_Code = "---" Then Text_Code = "" ' 若还未输入内容则清空
End Sub
Private Sub Text_Code_LostFocus() ' 文本框失去焦点
  If Text_Code = "" Then ' 未填写内容
    Text_Code = "---"
    Call Try_get_code
  Else
    If Text_Code.ForeColor = vbBlack Then Text_Code = UCase(Text_Code) ' 全部改为大写
  End If
End Sub
Private Sub Text_Code_KeyPress(KeyAscii As Integer) ' 文本框输入内容
  Dim code As String
  If KeyAscii = 13 Then ' 回车键
    Call Command_OK_Click ' 模拟点击OK按钮
  Else
    Timer_KeyPress.Enabled = True
  End If
End Sub
Private Sub output_pic(with_code As Boolean)  ' 导出图片 可选择带不带编码
  Picture_Print.Width = output.start_x * 2 + output.square_width * 4 + output.gap * 5  ' 设置图片大小
  Picture_Print.Height = output.start_y * 2 + output.square_width * 5 + output.gap * 6
  If with_code = True Then Picture_Print.Height = Picture_Print.Height + 1000
  Picture_Print.Cls
  Picture_Print.BackColor = vbWhite
  If with_code = True Then
    Picture_Print.FontSize = 36
    Picture_Print.CurrentX = (Picture_Print.Width - Get_str_width(Text_Code)) / 2  ' 设置输出编码文字的位置
    Picture_Print.CurrentY = output.start_y * 2 + output.square_width * 5 + output.gap * 6
    Picture_Print.Print Text_Code
  End If
  Call Output_case(Picture_Print, case_main, output)  ' 显示布局
  SavePicture Picture_Print.Image, Text_Code & ".bmp"  ' 保存图片
End Sub
Private Function Get_str_width(str As String) As Integer ' 获取字符串显示时的宽度
  Label_Print.FontSize = Picture_Print.FontSize
  Label_Print = str ' 载入到Label中 利用自动大小计算
  Get_str_width = Label_Print.Width
End Function
Private Function Try_parse_code(code As String) As Boolean ' 尝试解码
  Dim i As Integer, dat As String, flag As Boolean
  If Len(code) > 9 Then Try_parse_code = False: Exit Function ' 输入超9位 退出
  For i = 1 To Len(code) ' 遍历字符串
    dat = Mid(code, i, 1)
    flag = False
    If Asc(dat) >= 48 And Asc(dat) <= 57 Then flag = True ' 0 - 9
    If Asc(dat) >= 65 And Asc(dat) <= 70 Then flag = True ' A - F
    If Asc(dat) >= 97 And Asc(dat) <= 102 Then flag = True ' a - f
    If flag = False Then Try_parse_code = False: Exit Function ' 如果存在不合法字符 退出
  Next i
  If Len(code) < 9 Then code = code & String(9 - Len(code), "0") ' 补0到9位
  Try_parse_code = False
  If Parse_Code(code) = True Then ' 解码正确
    case_main = Parse_data
    Call Output_case(Form_main, case_main, output) ' 刷新显示界面
    Try_parse_code = True
  End If
End Function
Private Sub Try_get_code() ' 尝试获取编码
  Dim code As String
  If case_main.kind(0) <> 0 Then Exit Sub ' 2 * 2块还未确定 退出
  code = Get_Code(case_main)
  While Right(code, 1) = "0" ' 去除后方的0
    code = Left(code, Len(code) - 1)
    If Len(code) = 0 Then Text_Code = "0": Exit Sub ' 若全为0 保留一个退出
  Wend
  Text_Code = code
End Sub
Private Sub Timer_KeyPress_Timer()
  If Text_Code = "" Then ' 已经被删空
    Call Init_case
    Call Output_case(Form_main, case_main, output)
  Else
    If Try_parse_code(Text_Code) = False Then ' 根据输入正确性显示不同颜色
      Text_Code.ForeColor = vbRed
    Else
      Text_Code.ForeColor = vbBlack
    End If
  End If
  Timer_KeyPress.Enabled = False
End Sub
Private Sub Show_tip(tip_data As String) ' 显示提示 1.2s后消失
  If is_tip = False Then Exit Sub
  Timer_Tip.Enabled = False ' 防止闪烁
  Label_Tip = tip_data
  Label_Tip.ForeColor = style.block_line_color
  Label_Tip.Top = (Form_main.Height - Label_Tip.Height) / 2 - 480
  Label_Tip.Left = (Form_main.Width - Label_Tip.Width) / 2 - 60
  Label_Tip.Visible = True
  Timer_Tip = True ' 定时关闭
End Sub
Private Sub Timer_Tip_Timer()
  Label_Tip.Visible = False
  Timer_Tip.Enabled = False ' 开启后停止运转
End Sub
Private Sub Form_MouseDown(Button As Integer, Shift As Integer, x As Single, y As Single) ' 按下鼠标按键
  Text_Focus.SetFocus
  If Button <> 1 Then Exit Sub
  Dim x_ As Integer, y_ As Integer, num As Integer
  Dim raw_x As Single, raw_y As Single
  Dim addr_x As Integer, addr_y As Integer
  Dim clear As Boolean
  clear = False
  raw_x = Get_block_x(x) ' 得到点击位置在棋盘上的编号
  raw_y = Get_block_y(y)
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
    Text_Code.ForeColor = vbBlack
    Text_Code = "---"
    Call Try_get_code
    Exit Sub ' 退出
  End If
  If Int(raw_x) <> raw_x Or Int(raw_y) <> raw_y Then Exit Sub ' 点击在间隙上 退出
  click_x = raw_x: click_y = raw_y ' 记录有效的点击位置编号
  click_x_ = x: click_y_ = y ' 记录点击的真实位置
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
  Call Form_MouseMove(Button, Shift, x, y) ' 发起鼠标移动事件
End Sub
Private Sub Form_MouseMove(Button As Integer, Shift As Integer, x As Single, y As Single) ' 移动鼠标
  Dim delta_x As Integer, delta_y As Integer
  Dim print_x As Integer, print_y As Integer
  Dim print_width As Integer, print_height As Integer
  If print_now = False Then Exit Sub ' 判断是否在绘制模式
  Call Output_case(Form_main, case_main, output) ' 覆盖上一次的显示
  delta_x = signed(Get_block_x(x) - click_x) ' 记录鼠标位置与点击位置的相对方向
  delta_y = signed(Get_block_y(y) - click_y)
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
      If Abs(click_x_ - x) > Abs(click_y_ - y) Then ' x轴方向位移较大
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
Private Sub Form_MouseUp(Button As Integer, Shift As Integer, x As Single, y As Single) ' 释放鼠标按键
  If Button = 2 Or Button = 4 Then ' 如果按下滚轮或右键
    If Text_Code.ForeColor = vbRed Then Show_tip ("编码有问题哦"): Exit Sub
    If Button = 2 Then Call output_pic(False) ' 按下右键 不带编码
    If Button = 4 Then Call output_pic(True) ' 按下滚轮 带编码
    Call Show_tip("图片已保存")
    Exit Sub
  End If
  Dim i As Integer, j As Integer, num As Integer
  Dim block_x As Integer, block_y As Integer
  Dim block_width As Integer, block_height As Integer
  If print_now = False Then Exit Sub ' 判断是否在绘制模式
  print_now = False ' 退出绘制模式
  Text_Code.ForeColor = vbBlack
  If case_main.kind(0) <> 0 Then Text_Code = "---"
  block_x = min(click_x, mouse_x) - 1 ' 计算绘制棋子的起始位置
  block_y = min(click_y, mouse_y) - 1
  block_width = Abs(click_x - mouse_x) + 1 ' 计算绘制棋子的宽和高
  block_height = Abs(click_y - mouse_y) + 1
  num = Get_empty_seat ' 取得下一个未使用的棋子编号
  If block_width = 2 And block_height = 2 Then ' 绘制的棋子是2 * 2
    If Get_empty_num < 6 Then Call Show_tip("至少要留两个空格噢"): GoTo out
    case_main.kind(0) = 0
    case_main.status(block_x, block_y) = 0
    case_main.status(block_x, block_y + 1) = 0
    case_main.status(block_x + 1, block_y) = 0
    case_main.status(block_x + 1, block_y + 1) = 0
  ElseIf block_width = 2 And block_height = 1 Then ' 绘制的棋子是2 * 1
    If num = 0 Then Call Show_tip("棋子数量超过限制啦"): GoTo out
    If Get_empty_num < 4 Then Call Show_tip("至少要留两个空格噢"): GoTo out
    case_main.kind(num) = 1
    case_main.status(block_x, block_y) = num
    case_main.status(block_x + 1, block_y) = num
    If case_main.kind(0) <> 0 And Check_2x2_seat = False Then
      Call Clear_block(num)
      Call Show_tip("要留位置给曹操哦")
      GoTo out
    End If
  ElseIf block_width = 1 And block_height = 2 Then ' 绘制的棋子是1 * 2
    If num = 0 Then Call Show_tip("棋子数量超过限制啦"): GoTo out
    If Get_empty_num < 4 Then Call Show_tip("至少要留两个空格噢"): GoTo out
    case_main.kind(num) = 2
    case_main.status(block_x, block_y) = num
    case_main.status(block_x, block_y + 1) = num
    If case_main.kind(0) <> 0 And Check_2x2_seat = False Then
      Call Clear_block(num)
      Call Show_tip("要留位置给曹操哦")
      GoTo out
    End If
  ElseIf block_width = 1 And block_height = 1 Then ' 绘制的棋子是1 * 1
    If num = 0 Then Call Show_tip("棋子数量超过限制啦"): GoTo out
    If Get_empty_num < 3 Then Call Show_tip("至少要留两个空格噢"): GoTo out
    case_main.kind(num) = 3
    case_main.status(block_x, block_y) = num
    If case_main.kind(0) <> 0 And Check_2x2_seat = False Then
      Call Clear_block(num)
      Call Show_tip("要留位置给曹操哦")
      GoTo out
    End If
  End If
  Call Try_get_code
out:
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
Private Function Check_2x2_seat() As Boolean ' 查询是否还有空位放置2 x 2棋子
  Dim i As Integer, j As Integer
  Check_2x2_seat = False
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
Private Sub Init_case() ' 初始化布局
  Dim i As Integer, j As Integer
  For i = 0 To 14
    case_main.kind(i) = 255
  Next i
  For i = 0 To 3
    For j = 0 To 4
      case_main.status(i, j) = 254
    Next j
  Next i
End Sub
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
