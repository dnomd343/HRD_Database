dim jiang_num, bing_num, style_num
dim fso, file
dim cmd, dat
set fso = CreateObject("Scripting.FileSystemObject")
fso.CreateFolder("./cal_sh")

for jiang_num = 0 to 7
  for bing_num = 0 to 14 - jiang_num * 2
    set file = fso.CreateTextFile("./cal_sh/" & jiang_num & "-" & bing_num & ".sh", true)
    cmd = ""
    for style_num = 0 to jiang_num
      if jiang_num = 7 and bing_num = 0 and style_num = 0 then style_num = 1
      dat = jiang_num & "-" & bing_num & "-" & style_num
      cmd = cmd & "../engine --analy-multi-group ../seed/" & dat & ".txt "
      cmd = cmd & "../farthest/farthest_" & dat & ".csv ../solution/solution_" & dat & ".csv" & chr(10)
    next
    cmd = cmd & "touch " & jiang_num & "-" & bing_num & "_OK"
    file.Write(cmd)
    file.Close()
  next
next
msgbox "OK"