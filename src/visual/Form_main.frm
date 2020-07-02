VERSION 5.00
Begin VB.Form Form_main 
   AutoRedraw      =   -1  'True
   BorderStyle     =   1  'Fixed Single
   Caption         =   "HRD Visual v0.0 by Dnomd343"
   ClientHeight    =   6585
   ClientLeft      =   45
   ClientTop       =   390
   ClientWidth     =   4830
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6585
   ScaleWidth      =   4830
   StartUpPosition =   2  '屏幕中心
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
Dim output As Case_size
Dim case_main As Case_detail
Private Sub Form_Load()
  Call Parse_Code("4FEA13400")
  case_main = Parse_data
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
  Call Output_case(Form_main, case_main, output)
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
Private Sub Text_Code_KeyPress(KeyAscii As Integer)
  If KeyAscii = 13 Then Call Command_Print_Click
End Sub
