## 库的调用

代码已封装为四个类，介绍如下

+ HRD_cal: 华容道快速计算器

+ HRD_analy: 华容道分析器

+ HRD_group: 华容道群组批量分析器

+ HRD_statistic: 华容道统计工具

以上可根据需要调用，包含对应头文件即可


### HRD_cal

描述：华容道快速计算器

+ 判断编码正确性

+ 对起始布局求解（仅返回搜索到的第一个最少步解的一个解法）

+ 找到两布局间最短路径（仅返回搜索到的第一条最短路径）

+ 找到起始布局衍生出的所有情况

使用：包含头文件"HRD_cal.h"，类名为HRD_cal；

示例：

```c++
#include <iostream>
#include <vector>
#include "HRD_cal.h"
using namespace std;

int main() {
    cout << "Klotski fast calculator by Dnomd343" << endl;
    cout << "----------------" << endl;
    vector <unsigned long long> dat;
    HRD_cal demo;

    // 将编码转为文本(参数为unsigned long long 返回string类)
    cout << "4FEA13400 is " << demo.Change_str(0x4FEA13400) << endl;
    cout << "----------------" << endl;

    // 将文本转为编码(参数为*char 返回unsigned long long)
    cout << "1a9bf0c00 is " << hex << demo.Change_int("1A9BF0C00") << dec << endl;
    cout << "----------------" << endl;

    // 检测编码的正确性
    cout << "Check the code: 123456789 -> ";
    if (demo.Check_Code(0x123456789) == false) {
        cout << "Code error!" << endl;
    } else {
        cout << "No problem" << endl;
    }
    cout << "----------------" << endl;

    // 计算最少步数(参数为unsigned long long 返回vector类)
    dat = demo.Calculate(0x1A9BF0C00);
    cout << demo.Change_str(0x1A9BF0C00) << "'s solution";
    cout << " need at least " << dat.size() - 1 << " steps" << endl;
    for (unsigned int i = 0; i < dat.size(); i++) {
        cout << i << ": " << demo.Change_str(dat[i]) << endl;
    }
    cout << "----------------" << endl;

    // 计算到某一布局的最短路径(参数均为unsigned long long 分别为起始编码和目标编码 返回vector类)
    dat = demo.Calculate(0x1A9BF0C00, 0x1ABE70C00);
    cout << demo.Change_str(0x1A9BF0C00) << " to " << demo.Change_str(0x1ABE70C00);
    cout << " need at least " << dat.size() - 1 << " steps" << endl;
    for (unsigned int i = 0; i < dat.size(); i++) {
        cout << i << ": " << demo.Change_str(dat[i]) << endl;
    }
    cout << "----------------" << endl;

    // 计算某一布局衍生出的所有布局
    dat = demo.Calculate_All(0x1A9BF0C00);
    cout << demo.Change_str(0x1A9BF0C00) << " can derive " << dat.size() << " cases" << endl;
    cout << "----------------" << endl;

    return 0;
}
```

### HRD_analy

描述：华容道分析器

+ 编码解析为具体排列

+ 检查编码是否合法

+ 检测编码是否左右对称

+ 根据起始布局分析出层级结构，包括各层间的链接关系

+ 得到布局的具体参数，包括全部最远布局、全部最少步解、全部合法解及其步数

+ 将分析结果导出到文件

使用：包含头文件"HRD_analy.h"，类名为HRD_analy；

示例：

```c++
#include <iostream>
#include "HRD_analy.h"
using namespace std;

int main() {
    cout << "Klotski Analyser by Dnomd343" << endl;
    cout << "---------------------------" << endl;
    HRD_analy demo;

    // 判断编码是否左右对称
    cout << "Check Mirror(code: 1A9BF0C00) -> ";
    if (demo.Is_Mirror(0x1A9BF0C00)) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }
    cout << "Check Mirror(code: 4FEA13400) -> ";
    if (demo.Is_Mirror(0x4FEA13400)) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }
    cout << "---------------------------" << endl;

    // 显示编码的实际布局样式
    cout << "Show 1A9BF0C00: " << endl;
    demo.Output_Graph(0x1A9BF0C00, 4, 1, "&%");
    cout << "---------------------------" << endl;

    // 解译编码
    demo.Parse_Code(0x1A9BF0C00);
    cout << "code: " << demo.Change_str(demo.Parse_dat.code) << endl;
    cout << "status" << endl;
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 4; x++) {
            if (demo.Parse_dat.status[x][y] <= 9) { // 0 ~ 9
                cout << int(demo.Parse_dat.status[x][y]) << " ";
            } else if (demo.Parse_dat.status[x][y] <= 0xE) { // A ~ E
                cout << char(demo.Parse_dat.status[x][y] + 55) << " ";
            } else if (demo.Parse_dat.status[x][y] == 0xFE) { // space
                cout << ". ";
            } else if (demo.Parse_dat.status[x][y] == 0xFF) { // undefined
                cout << "* ";
            } else { // error
                cout << "! ";
            }
        }
        cout << endl;
    }
    cout << "type" << endl;
    for (int i = 0; i < 15; i++) {
        if (i < 10) {
            cout << i;
        } else {
            cout << char(i + 55);
        }
        cout << " -> ";
        switch (demo.Parse_dat.type[i]) {
            case 0:
                cout << "2 * 2" << endl;
                break;
            case 1:
                cout << "2 * 1" << endl;
                break;
            case 2:
                cout << "1 * 2" << endl;
                break;
            case 3:
                cout << "1 * 1" << endl;
                break;
            default:
                cout << "undefined" << endl;
                break;
        }
    }
    cout << "---------------------------" << endl;

    // 分析布局
    demo.quiet = false; // 输出分析状态
    demo.Analyse_Case(0x1A9BF0C00);
    demo.Output_Detail("demo-1A9BF0C00.txt"); // 输出分析结果到文件
    cout << "---------------------------" << endl;

    // 查看某布局的前后情况
    int num = 29, index = 190;
    cout << "Case near (num = " << num << ", index = " << index << ")" << endl;

    // 得到全部父节点数据
    for (int k = 0; k < demo.Layer[num][index]->source_case->size(); k++) {
        cout << "(" << demo.Layer[num][index]->source_case->at(k)->layer_num;
        cout << "," << demo.Layer[num][index]->source_case->at(k)->layer_index << ") = ";
        cout << demo.Change_str(demo.Layer[num][index]->source_case->at(k)->code);
        if (k + 1 != demo.Layer[num][index]->source_case->size()) {
            cout << " / ";
        }
    }

    // 得到本节点数据
    cout << " -> ";
    cout << "(" << num << "," << index << ") = " << demo.Change_str(demo.Layer[num][index]->code);
    cout << " -> ";
    
    // 得到全部子节点数据
    for (int k = 0; k < demo.Layer[num][index]->next_case->size(); k++) {
        cout << "(" << demo.Layer[num][index]->next_case->at(k)->layer_num;
        cout << "," << demo.Layer[num][index]->next_case->at(k)->layer_index << ") = ";
        cout << demo.Change_str(demo.Layer[num][index]->next_case->at(k)->code);
        if (k + 1 != demo.Layer[num][index]->next_case->size()) {
            cout << " / ";
        }
    }
    cout << endl;
    cout << "---------------------------" << endl;

    return 0;
}
```

### HRD_group

描述：华容道群组批量分析器

+ 搜索某一群组全部元素的具体参数（输入群组中任一布局编码）

+ 排列分析多个群组（输入储存种子编码的文件名）

使用：包含头文件"HRD_group.h"，类名为HRD_group（依赖HRD_cal）；

示例：

```c++
#include <iostream>
#include "HRD_group.h"
using namespace std;

int main() {
    cout << "Klotski batch analyser by Dnomd343" << endl;
    cout << "---------------------------" << endl;
    HRD_group demo;
    
    // 计算群组中所有元素的具体参数并输出到文件（此处即计算1A9BF0C00所在群的数据）
    demo.Batch_Analyse(0x1A9BF0C00, "farthest.csv", "solution.csv", true);
    cout << "---------------------------" << endl;

    // 计算多个群组的分析信息，同时合并输出到文件（编码储存于5-4-0.txt中，最后一行切不可为空）
    demo.Multi_Analyse("5-4-0.txt", "farthest_5-4-0.csv", "solution_5-4-0.csv", true);
    cout << "---------------------------" << endl;

    return 0;
}
```

### HRD_statistic

描述：华容道统计工具

+ 搜索全部合法的华容道布局编码

+ 计算搜索到编码分类，即各布局的jiang_num-bing_num-style_num-group_num-group_index信息

+ 取各群组的种子编码输出到文件

使用：包含头文件"HRD_statistic.h"，类名为HRD_statistic（依赖HRD_cal与HRD_analy）；

示例：

```c++
#include <iostream>
#include "HRD_statistic.h"
using namespace std;

int main() {
    cout << "Klotski statistician by Dnomd343" << endl;
    cout << "---------------------------" << endl;
    HRD_statistic demo;
    
    // 找到全部合法的编码
    demo.Find_All_Case("Output-All_Case.txt");
    cout << "---------------------------" << endl;

    // 找到全部合法的编码并进行分类输出
    demo.All_Statistic();
    cout << "---------------------------" << endl;

    return 0;
}
```