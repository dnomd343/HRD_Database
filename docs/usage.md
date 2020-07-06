## 命令行参数

+ show命令：根据编码输出实际布局样式
```bash
--show <code> [square_width]
    Purpose: Visualize the <code>
    exp: ./engine --show 1A9BF0C00
         ./engine --show 1A9BF0C00 4
```

+ cal命令：计算输入布局的最少步解法
```bash
--cal <code> [file_name]
    Purpose: Find the minimum step solution of <code>
    exp: ./engine --cal 1A9BF0C00
         ./engine --cal 1A9BF0C00 demo.txt
```

+ cal-target命令：计算两布局之间的最短路径
```bash
  --cal-target <code> <target> [file_name]
    Purpose: Find the shortest path from <code> to <target>
    exp: ./engine --cal-target 4FEA13400 43EA73400
         ./engine --cal-target 4FEA13400 43EA73400 demo.txt
```

+ group命令：计算起始布局的所有衍生情况，即计算其所在群的全部元素
```bash
  --group <code> [file_name]
    Purpose: Find all elements of the group where <code> located
    exp: ./engine --group 4FEA13400
         ./engine --group 4FEA13400 demo.txt
```

+ analy命令：分析布局的具体参数，包括层级结构，层间链接，解与最少步解，最远布局等
```bash
  --analy <code> [file_name]
    Purpose: Detailed analysis of the <code>
    exp: ./engine --analy 1A9BF0C00
         ./engine --analy 1A9BF0C00 demo.txt
```

+ analy-quiet命令：同analy命令，但不输出计算过程
```bash
  --analy-quiet <code> <file_name>
    Purpose: The same function as --analy, but doesn't show the specific process
    exp: ./engine --analy-quiet 1A9BF0C00 demo.txt
```

+ analy-group命令：分析`code`衍生出的群中所有元素的具体参数，并输出到`最远布局信息`和`解的信息`两张csv表格中
```bash
  --analy-group <code> <file_name_farthest> <file_name_solution>
    <file_name_farthest>: As the output file of "farthest"
    <file_name_solution>: As the output file of "solution"
    Purpose: Analyze the whole group where <code> located
    exp: ./engine --analy-group 1A9BF0C00 farthest.csv solution.csv
```

+ analy-group-integral命令：同analy-group命令，但`解的信息`中将会输出全部解的编码及步数
```bash
  --analy-group-integral <code> <file_name_farthest> <file_name_farthest>
    Purpose: The same function as --analy-group, but all solution case will be output
    exp: ./engine --analy-group-integral 1A9BF0C00 farthest.csv solution.csv
```

+ analy-multi-group命令：多群组同时分析，相当于多次执行analy-group命令，结果合并输出到`最远布局信息`和`解的信息`两张csv表格中
```bash
  --analy-multi-group <input_file_name> <file_name_farthest> <file_name_farthest>
    <input_file_name>: As the input file of seeds
    <file_name_farthest>: As the output file of "farthest"
    <file_name_farthest>: As the output file of "solution"
    Purpose: Analyze the whole group where each seed located
    exp: ./engine --analy-multi-group 5-4-0.txt farthest.csv solution.csv
```

+ analy-multi-group-integral命令：同analy-multi-group命令，但`解的信息`中将会输出全部解的编码及步数
```bash
  --analy-multi-group-integral <input_file_name> <file_name_farthest> <file_name_farthest>
    Purpose: The same function as --analy-multi-group, but all solution case will be output
    exp: ./engine --analy-multi-group-integral 5-4-0.txt farthest.csv solution.csv
```

+ all命令：找到所有可能的布局，同时得到其分类情况及群组关系，计算结果会输出到All_Case.txt（所有布局的编码），main.csv（所有布局的编码及其分类），\*-\*-\*.txt（各群组的第一个子布局集合）
```bash
  --all
    Purpose: Find all the cases of klotski with detail
    exp: ./engine --all
```

+ all-code命令：简化版的all命令，仅输出全部布局的编码
```bash
  --all-code <file_name>
    Purpose: Find all the code of klotski
    exp: ./engine --all-code All_Case.txt
```

+ help命令：显示所有命令的使用方法
```bash
  --help
    Purpose: Display instructions for use
    exp: ./engine --help
```

## 编译

+ Windows下，要求计算机有mingw环境或其他C/C++编译器

+ Linux下，要求计算机有gcc编译器

+ Android下，可以在termux中安装clang编译器

使用make进行编译
```bash
cd ./scr/engine
make
```

使用g++编译
```bash
g++ -O3 ./src/engine/*.cpp -o ./engine 
```