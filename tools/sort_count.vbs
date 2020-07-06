 ' 使用以下SQL语句在main.csv导入的表中提取数据到文件
 ' ------------------------------------------------------------------
 ' SELECT jiang_num,bing_num,style_num,group_num,COUNT(*) FROM main
 ' GROUP BY jiang_num,bing_num,style_num,group_num
 ' ORDER BY jiang_num,bing_num,style_num,group_num
 ' INTO OUTFILE '(Path)/./res/group_size.data';
 ' ------------------------------------------------------------------
 ' 
Option Explicit ' 强制要求声明变量
dim fso, input, output
dim range, dat, num
dim start_group_num
dim jiang_num, bing_num, style_num, group_num, group_count
dim last_jiang_num, last_bing_num, last_style_num, last_group_num, last_group_count
dim input_file_name, output_file_name

input_file_name = "../res/group_size.data" ' 数据库导出内容的文件名
output_file_name = "../docs/group_size.md" ' 整理后输出的的文件名
call init() ' 初始化数据并创建输出文件
call sort() ' 
msgbox "OK"

function sort()
    while input.atEndOfStream <> true ' 循环读到文件尾
        dat = input.ReadLine() ' 读入一行新数据
        range = split(dat, chr(9)) ' 根据tab进行切割
        jiang_num = range(0) ' 记录切割结果
        bing_num = range(1)
        style_num = range(2)
        group_num = range(3)
        group_count = range(4)
        if jiang_num = last_jiang_num and bing_num = last_bing_num and style_num = last_style_num and group_count = last_group_count then
            last_group_num = group_num ' 备份group_num
        else ' 如果另起一类
			if num <> 0 then call output_data(last_jiang_num, last_bing_num, last_style_num, start_group_num, last_group_num, last_group_count) ' 去除第一个空数据
            last_jiang_num = jiang_num ' 记录找到的新一类数据
            last_bing_num = bing_num
            last_style_num = style_num
            last_group_num = group_num
            last_group_count = group_count
            start_group_num = group_num
        end if
        num = num + 1
    wend
    call output_data(jiang_num, bing_num, style_num, start_group_num, last_group_num, group_count) ' 补全最后一类
end function

function output_data(str_1, str_2, str_3, str_4_1, str_4_2, str_5)
    dat = "|" & chr(9) & str_1 & chr(9) & "|" & chr(9) & str_2
    dat = dat & chr(9) & "|" & chr(9) & str_3 & chr(9) & "|" & chr(9)
    if str_4_1 = str_4_2 then ' 如果该类中仅有一行数据
        dat = dat & str_4_1
    else
        dat = dat & str_4_1 & " \~ " & str_4_2
    end if
    dat = dat & chr(9) & "|" & chr(9) & str_5 & chr(9) & "|"
    output.WriteLine(dat) ' 写入文件
end function

function init()
    set fso = CreateObject("Scripting.FileSystemObject")
    set input = fso.OpenTextFile(input_file_name, 1, false) ' 打开文件进行读取
    set output = fso.CreateTextFile(output_file_name, true) ' 打开文件进行写入

    output.WriteLine("| jiang_num | bing_num | style_num | group_num | group_size |") ' 写入markdown表格头
    output.WriteLine("| :-: | :-: | :-: | :-: | :-: |")
    
    last_jiang_num = "" ' 初始化
    last_bing_num = ""
    last_style_num = ""
    last_group_num = ""
    last_group_count = ""
    start_group_num = "0"
end function
