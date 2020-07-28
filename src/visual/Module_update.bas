Attribute VB_Name = "Module_update"
Option Explicit
Public Declare Function URLDownloadToFile Lib "urlmon" Alias "URLDownloadToFileA" (ByVal pCaller As Long, ByVal szURL As String, ByVal szFileName As String, ByVal dwReserved As Long, ByVal lpfnCB As Long) As Long
Public Declare Function DeleteUrlCacheEntry Lib "wininet" Alias "DeleteUrlCacheEntryA" (ByVal lpszUrlName As String) As Long
Dim update_version As String
Dim update_file_name As String
Dim update_url As String
Dim update_detail As String
Dim my_file_name As String
Private Sub Get_update_information() ' 从服务器返回数据中提取更新信息
  Dim str As String
  update_detail = ""
  Open "update.txt" For Input As #1
    Line Input #1, str ' version
    Line Input #1, str
    update_version = str
    Line Input #1, str ' file_name
    Line Input #1, str
    update_file_name = str
    Line Input #1, str ' url
    Line Input #1, str
    update_url = str
    Line Input #1, str ' detail
    Do While Not EOF(1)
      Line Input #1, str
      If Left(str, 1) = "[" Then GoTo out
      update_detail = update_detail & str & vbCrLf
    Loop
out:
  Close #1
End Sub
Private Sub Create_update_shell() ' 创建用于更新的vbs脚本
  my_file_name = App.EXEName & ".exe"
  Open "update.vbs" For Output As #1
    Print #1, "Set ws = CreateObject(""Wscript.Shell"")"
    Print #1, "Set fso = CreateObject(""Scripting.FileSystemObject"")"
    Print #1, "ws.Run ""taskkill /f /im " & my_file_name & """, 0"
    Print #1, "Wscript.sleep 2000"
    Print #1, "fso.GetFile(""" & my_file_name & """).Attributes = 0"
    Print #1, "fso.DeleteFile (""" & my_file_name & """)"
    Print #1, "fso.MoveFile """ & update_file_name & """, """ & my_file_name & """"
    Print #1, "msgbox ""更新完成"",, ""提示" & """"
    Print #1, "Wscript.sleep 2000"
    Print #1, "ws.Run """ & my_file_name & """, 9"
    Print #1, "fso.DeleteFile(""update.vbs"")"
    Print #1, "fso.DeleteFile (""" & my_file_name & "-bak" & """)"
  Close #1
  SetAttr "update.vbs", vbHidden
End Sub
Public Sub Delete_file(file_name As String) ' 删除文件 自动规避错误
  If Check_file(file_name) = True Then
    SetAttr file_name, 0
    Kill file_name
  End If
End Sub
Public Function Download_file(url As String, file_name As String) As Boolean ' 下载文件 成功返回true 错误返回false
  Call DeleteUrlCacheEntry(url)
  If URLDownloadToFile(0, url, file_name, 0, 0) = 0 Then
    Download_file = True
  Else
    Download_file = False
  End If
End Function
Public Function Check_file(file_name As String) As Boolean ' 判断文件是否存在
  If Dir(file_name, vbNormal Or vbHidden Or vbReadOnly Or vbSystem) <> "" Then ' 如果文件存在
    Check_file = True
  Else
    Check_file = False
  End If
End Function
Public Sub Check_update() ' 检查更新
  Call Delete_file("update.txt")
  Call Delete_file("update.vbs")
  If Not Download_file("https://hrd.dnomd343.top/update/HRD_Visual", "update.txt") Then MsgBox "连接服务器失败", , "检查更新": Exit Sub
  If Check_file("update.txt") = False Then MsgBox "更新失败", , "检查更新": Exit Sub
  Call Get_update_information
  Call Delete_file("update.txt")
  If update_version = version Then MsgBox "当前已是最新版本", , "检查更新": Exit Sub
  If MsgBox("检查到新版本：" & update_version & vbCrLf & vbCrLf & "更新特性" & vbCrLf & update_detail & vbCrLf & "是否更新？", vbYesNo, "检查更新") = vbNo Then Exit Sub
  If Not Download_file(update_url, update_file_name) Then MsgBox "下载更新文件失败", , "检查更新": Exit Sub
  If Check_file(update_file_name) = False Then MsgBox "下载发生错误", , "检查更新": Exit Sub
  Call Create_update_shell
  If Check_file("update.vbs") = False Then MsgBox "发生未知错误", , "检查更新": Exit Sub
  Call Delete_file(App.EXEName & ".exe-bak")
  FileCopy App.EXEName & ".exe", App.EXEName & ".exe-bak"
  SetAttr App.EXEName & ".exe-bak", vbHidden
  Shell "cmd.exe /c ""update.vbs""", vbHide
End Sub
