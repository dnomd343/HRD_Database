#include <algorithm>
#include <vector>
#include <string>
#include "HRD_cal.h"

vector <unsigned long long> HRD_cal::Calculate_All(unsigned long long Code) { // 寻找整个群的全部元素
    unsigned int i;
    vector <unsigned long long> data;
    init_data(); // 初始化
    mode = 0; // 设置模式为全集计算
    if (Check_Code(Code) == false) { // 编码错误
        return data; // 返回空序列
    }
    cal(Code); // 进行广搜
    for (i = 0; i < List.size(); i++) {
        data.push_back((*List[i]).code); // 储存计算结果
    }
    return data;
}

vector <unsigned long long> HRD_cal::Calculate(unsigned long long Code, unsigned long long target) { // 寻找到target的最短路径
    vector <unsigned long long> temp;
    if (Check_Code(Code) == false || Check_Code(target) == false) { // 编码错误
        return temp; // 返回空序列
    }
    if (Code == target) { // 若输入为target
        temp.push_back(Code);
        return temp;
    }
    init_data(); // 初始化
    mode = 2; // 设置模式为寻找特定目标
    target_code = target;
    cal(Code); // 进行广搜
    if (flag == true) { // 若找到目标
        return Get_Path(result);
    } else { // 未找到目标
        return temp; // 返回空序列
    }
}

vector <unsigned long long> HRD_cal::Calculate(unsigned long long Code) { // 寻找最少步解
    vector <unsigned long long> temp;
    if (Check_Code(Code) == false) { // 编码错误
        return temp; // 返回空序列
    }
    if ((Code >> 32) == 0xD) { // 若输入已经为解
        temp.push_back(Code);
        return temp;
    }
    init_data(); // 初始化
    mode = 1; // 设置模式为寻解
    cal(Code); // 进行广搜
    if (flag == true) { // 若找到解
        return Get_Path(result);
    } else { // 无解
        return temp; // 返回空序列
    }
}

vector <unsigned long long> HRD_cal::Get_Path(unsigned int result_num) { // 找到开始到目标的最短路径
    vector <unsigned long long> Path;
    Path.clear();
    Path.push_back((*List[result_num]).code); // 加入目标布局
    while (List_source[result_num] != 0) { // 找到路径主体
        result_num = List_source[result_num];
        Path.push_back((*List[result_num]).code); // 加入记录
    }
    Path.push_back((*List[0]).code); // 加入开始布局
    reverse(Path.begin(), Path.end()); // 路径倒置
    return Path;
}

void HRD_cal::init_data() { // 初始化数据结构
    unsigned int i;
    for (i = 0; i < List.size(); i++) {
        delete List[i];
    }
    for (i = 0; i < 0x10000; i++) {
        List_hash[i].clear();
    }
    List.clear();
    List_source.clear();
}

bool HRD_cal::Check_Code(unsigned long long Code) {
    Case_cal dat;
    return Parse_Code(dat, Code);
}

void HRD_cal::cal(unsigned long long Code) { // 广搜寻找目标
    Case_cal *start = new Case_cal;
    now_move = 0; // 设置起始搜索节点编号为0
    result = 0;
    flag = false; // 设置为暂未找到
    if (Parse_Code(*start, Code) == false) { // 若输入编码错误 退出
        delete start;
        return;
    }
    List.push_back(start); // 加入根节点
    List_source.push_back(0);
    List_hash[0xffff & ((*start).code >> 16)].push_back(start);
    while (1 == 1) { // 创建死循环
        Find_Next_Case(*List[now_move]);
        if (flag == true) {break;} // 若找到则退出
        now_move++;
        if (now_move == List.size()) {break;} // 已经找到所有元素则退出
    }
}

void HRD_cal::Add_Case(Case_cal *dat) { // 将找到的布局加入队列中
    unsigned int i, x, y;
    int hash_index = 0xffff & ((*dat).code >> 16); // 取得哈希索引
    for (i = 0; i < List_hash[hash_index].size(); i++) { // 遍历索引内容
        if ((*List_hash[hash_index][i]).code == (*dat).code) { // 发现重复
            // 通过freeze表合并来屏蔽不必要的移动
            for (x = 0; x < 4; x++) { // 遍历freeze表
                for (y = 0; y < 5; y++) {
                    if ((*dat).freeze[x][y] == true) { // 将输入表合并到原先的表上
                        (*List_hash[hash_index][i]).freeze[x][y] = true;
                    }
                }
            }
            delete dat; // 释放空间
            return; // 退出
        }
    }
    List.push_back(dat); // 加入到队列中
    List_source.push_back(now_move); // 记录溯源信息
    List_hash[hash_index].push_back(dat); // 加入哈希索引
    if (mode == 1) { // 寻解模式
        if ((0xF & ((*dat).code >> 32)) == 0xD) { // 2 * 2块在出口位置
            flag = true; // 标志已找到目标
            result = List.size() - 1; // 记录找到的目标位置
        }
    } else if (mode == 2) { // 查找目标模式
        if ((*dat).code == target_code) { // 搜索到目标
            flag = true; // 标志已找到目标
            result = List.size() - 1; // 记录找到的目标位置
        }
    }

}

void HRD_cal::Find_Next_Case(Case_cal &dat_raw) { // 找到下一步移动的情况(一步可以为同一块多次移动) 结果聚集到Add_Case中
    int num, x, y, i, j;
    bool addr[4][5]; // 在Find_Sub_Case深搜中用于剪枝
    Case_cal dat = dat_raw;
    for (y = 0; y < 5; y++) { // 仅保留空格位置的freeze为true
        for (x = 0; x < 4; x++) {
            if (dat.status[x][y] != 0xFE && dat.freeze[x][y] == true) { // 不为空格但freeze为true
                dat.freeze[x][y] = false; // 重置为false
            }
        }
    }
    for (y = 0; y < 5; y++) { // 遍历整个棋盘
        for (x = 0; x < 4; x++) {
            if (dat_raw.freeze[x][y] == true) {continue;} // 遇到freeze为true的跳过
            num = dat.status[x][y]; // 统一修改(x, y)块 减少代码量
            dat.status[x][y] = 0xFE;
            dat.freeze[x][y] = true;
            for (i = 0; i < 4; i++) { // 初始化
                for (j = 0; j < 5; j++) {
                    addr[i][j] = false;
                }
            }
            addr[x][y] = true; // 加入当前块 防止重复查询
            switch (dat.type[num]) {
                case 0: // 2 * 2
                    dat_raw.freeze[x + 1][y]
                        = dat_raw.freeze[x][y + 1] = dat_raw.freeze[x + 1][y + 1] = true;
                    dat.status[x + 1][y] = dat.status[x][y + 1] = dat.status[x + 1][y + 1] = 0xFE;
                    dat.freeze[x + 1][y] = dat.freeze[x][y + 1] = dat.freeze[x + 1][y + 1] = true;
                    Find_Sub_Case(dat, num, x, y, addr); // 进行子步递归搜索
                    dat.status[x + 1][y] = dat.status[x][y + 1] = dat.status[x + 1][y + 1] = num;
                    dat.freeze[x + 1][y] = dat.freeze[x][y + 1] = dat.freeze[x + 1][y + 1] = false;
                    break;
                case 1: // 2 * 1
                    dat_raw.freeze[x + 1][y] = true;
                    dat.status[x + 1][y] = 0xFE;
                    dat.freeze[x + 1][y] = true;
                    Find_Sub_Case(dat, num, x, y, addr); // 进行子步递归搜索
                    dat.status[x + 1][y] = num;
                    dat.freeze[x + 1][y] = false;
                    break;
                case 2: // 1 * 2
                    dat_raw.freeze[x][y + 1] = true;
                    dat.status[x][y + 1] = 0xFE;
                    dat.freeze[x][y + 1] = true;
                    Find_Sub_Case(dat, num, x, y, addr); // 进行子步递归搜索
                    dat.status[x][y + 1] = num;
                    dat.freeze[x][y + 1] = false;
                    break;
                case 3: // 1 * 1
                    Find_Sub_Case(dat, num, x, y, addr); // 进行子步递归搜索
                    break;
            }
            dat.status[x][y] = num; // 复原统一修改的块
            dat.freeze[x][y] = false;
        }
    }
}

void HRD_cal::Find_Sub_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]) { // 找到下一个单格移动的情况
    switch (dat.type[num]) {
        case 0: // 2 * 2
            if (y != 0) { // 不在最上面
                if (dat.status[x][y - 1] == 0xFE && dat.status[x + 1][y - 1] == 0xFE) { // 上面为空
                    Build_Case(dat, num, x, y - 1, addr);
                }
            }
            if (y != 3) { // 不在最下面
                if (dat.status[x][y + 2] == 0xFE && dat.status[x + 1][y + 2] == 0xFE) { // 下面为空
                    Build_Case(dat, num, x, y + 1, addr);
                }
            }
            if (x != 0) { // 不在最左边
                if (dat.status[x - 1][y] == 0xFE && dat.status[x - 1][y + 1] == 0xFE) { // 左边为空
                    Build_Case(dat, num, x - 1, y, addr);
                }
            }
            if (x != 2) { // 不在最右边
                if (dat.status[x + 2][y] == 0xFE && dat.status[x + 2][y + 1] == 0xFE) { // 右边为空
                    Build_Case(dat, num, x + 1, y, addr);
                }
            }
            break;
        case 1: // 2 * 1
            if (y != 0) { // 不在最上面
                if (dat.status[x][y - 1] == 0xFE && dat.status[x + 1][y - 1] == 0xFE) { // 上面为空
                    Build_Case(dat, num, x, y - 1, addr);
                }
            }
            if (y != 4) { // 不在最下面
                if (dat.status[x][y + 1] == 0xFE && dat.status[x + 1][y + 1] == 0xFE) { // 下面为空
                    Build_Case(dat, num, x, y + 1, addr);
                }
            }
            if (x != 0) { // 不在最左边
                if (dat.status[x - 1][y] == 0xFE) { // 左边为空
                    Build_Case(dat, num, x - 1, y, addr);
                }
            }
            if (x != 2) { // 不在最右边
                if (dat.status[x + 2][y] == 0xFE) { // 右边为空
                    Build_Case(dat, num, x + 1, y, addr);
                }
            }
            break;
        case 2: // 1 * 2
            if (y != 0) { // 不在最上面
                if (dat.status[x][y - 1] == 0xFE) { // 上面为空
                    Build_Case(dat, num, x, y - 1, addr);
                }
            }
            if (y != 3) { // 不在最下面
                if (dat.status[x][y + 2] == 0xFE) { // 下面为空
                    Build_Case(dat, num, x, y + 1, addr);
                }
            }
            if (x != 0) { // 不在最左边
                if (dat.status[x - 1][y] == 0xFE && dat.status[x - 1][y + 1] == 0xFE) { // 左边为空
                    Build_Case(dat, num, x - 1, y, addr);
                }
            }
            if (x != 3) { // 不在最右边
                if (dat.status[x + 1][y] == 0xFE && dat.status[x + 1][y + 1] == 0xFE) { // 右边为空
                    Build_Case(dat, num, x + 1, y, addr);
                }
            }
            break;
        case 3: // 1 * 1
            if (y != 0) { // 不在最上面
                if (dat.status[x][y - 1] == 0xFE) { // 上面为空
                    Build_Case(dat, num, x, y - 1, addr);
                }
            }
            if (y != 4) { // 不在最下面
                if (dat.status[x][y + 1] == 0xFE) { // 下面为空
                    Build_Case(dat, num, x, y + 1, addr);
                }
            }
            if (x != 0) { // 不在最左边
                if (dat.status[x - 1][y] == 0xFE) { // 左边为空
                    Build_Case(dat, num, x - 1, y, addr);
                }
            }
            if (x != 3) { // 不在最右边
                if (dat.status[x + 1][y] == 0xFE) { // 右边为空
                    Build_Case(dat, num, x + 1, y, addr);
                }
            }
            break;
    }
}

void HRD_cal::Build_Case(Case_cal &dat, int &num, int x, int y, bool addr[4][5]) { // 实现移动并将结果发送到Add_Case
    if (addr[x][y] == true) { // 重复
        return; // 退出
    } else {
        addr[x][y] = true; // 加入位置数据
    }
    Case_cal *dat_mod = new Case_cal; // 新建对象 在Add_Case中加入层中或被释放
    *dat_mod = dat;
    switch ((*dat_mod).type[num]) { // 注入移动后的信息
        case 0: // 2 * 2
            (*dat_mod).status[x][y] = (*dat_mod).status[x][y + 1]
                = (*dat_mod).status[x + 1][y] = (*dat_mod).status[x + 1][y + 1] = num;
            break;
        case 1: // 2 * 1
            (*dat_mod).status[x][y] = (*dat_mod).status[x + 1][y] = num;
            break;
        case 2: // 1 * 2
            (*dat_mod).status[x][y] = (*dat_mod).status[x][y + 1] = num;
            break;
        case 3: // 1 * 1
            (*dat_mod).status[x][y] = num;
            break;
    }
    Get_Code(*dat_mod); // 更新移动后的编码
    Add_Case(dat_mod); // 发送给Add_Case
    Find_Sub_Case(dat, num, x, y, addr); // 递归搜索
}

void HRD_cal::Get_Code(Case_cal &dat) { // 获取编码并存储在dat.code 输入数据必须无误
    bool temp[4][5]; // 用于临时标记
    int x, y, num;
    dat.code = 0;
    for (x = 0; x < 4; x++) { // 初始化temp
        for (y = 0; y < 5; y++) {
            temp[x][y] = false;
        }
    }
    num = 0;
    for (y = 0; y < 5; y++) { // 遍历20个格
        for (x = 0; x < 4; x++) {
            if (temp[x][y] == true) {continue;} // 该格已被占用
            if (dat.status[x][y] == 0xFE) { // space
                num++;
                dat.code <<= 2;
                continue;
            }
            switch (dat.type[dat.status[x][y]]) { // type -> 0 / 1 / 2 / 3
                case 0: // 2 * 2
                    dat.code |= (x + y * 4) << (num * 2); // 写入2 * 2块位置
                    temp[x][y + 1] = temp[x + 1][y] = temp[x + 1][y + 1] = true; // 标记占用
                    break;
                case 1: // 2 * 1
                    num++;
                    dat.code <<= 2;
                    dat.code |= 1; // 01
                    temp[x + 1][y] = true; // 标记占用
                    break;
                case 2: // 1 * 2
                    num++;
                    dat.code <<= 2;
                    dat.code |= 2; // 10
                    temp[x][y + 1] = true; // 标记占用
                    break;
                case 3: // 1 * 1
                    num++;
                    dat.code <<= 2;
                    dat.code |= 3; // 11
                    break;
            }
        }
    }
    dat.code <<= (16 - num) * 2; // 左移使编码占满低36位
    dat.code &= 0xFFFFFFFFF; // 清除高28位内容
}

bool HRD_cal::Parse_Code(Case_cal &dat, unsigned long long Code) { // 解析编码 返回false表示编码有误
    unsigned char range[16]; // dat低32位分16组
    int i, x, y, num, space_num = 0;
    dat.code = Code;
    for (x = 0; x < 4; x++) { // 初始化status和freeze
        for (y = 0; y < 5; y++) {
            dat.status[x][y] = 0xFF;
            dat.freeze[x][y] = false;
        }
    }
    for (i = 0; i < 15; i++) { // 初始化type
        dat.type[i] = 0xFF;
    }
    num = 0;
    for (i = 15; i >= 0; i--) { // 载入排列到range
        range[i] = Code & 0x3  ;
        if (range[i] == 0) {num++;}
        Code >>= 2;
    }
    if (num < 2) {return false;} // 0的个数低于两个出错
    if (Code > 14) {return false;} // 排除越界情况
    if (Code % 4 == 3) {return false;}
    dat.type[0] = 0; // 载入2 * 2方块
    x = Code % 4;
    y = Code / 4;
    dat.status[x][y] = dat.status[x + 1][y] = dat.status[x][y + 1] = dat.status[x + 1][y + 1] = 0;

    num = x = y = 0;
    for (i = 0; i < 16; i++) {
        while (dat.status[x][y] != 0xFF) { // 找到下一个未填入的位置
            if (++x == 4) {
                x = 0;
                if (++y == 5) { // 已填满20个空位 越界
                    if (space_num < 2) {return false;} // 空格低于两个 出错
                    for (num = i; num < 16; num++) { // 检查余下编码是否为0
                        if (range[num] != 0) {return false;} // 出现非0 编码错误
                    }
                    return true; // 全为0 编码正确
                }
            }
        }
        switch (range[i]) { // 分别处理四种情况
            case 0: // space
                space_num++;
                dat.status[x][y] = 0xFE;
                dat.freeze[x][y] = true; // 空格标记为不可移动
                break;
            case 1: // 2 * 1
                if (x == 3) {return false;} // 越界出错
                if (dat.status[x + 1][y] != 0xFF) {return false;} // 方块重叠
                num++;
                dat.type[num] = 1;
                dat.status[x][y] = dat.status[x + 1][y] = num;
                break;
            case 2: // 1 * 2
                if (y == 4) {return false;} // 越界出错
                if (dat.status[x][y + 1] != 0xFF) {return false;} // 方块重叠
                num++;
                dat.type[num] = 2;
                dat.status[x][y] = dat.status[x][y + 1] = num;
                break;
            case 3: // 1 * 1
                num++;
                dat.type[num] = 3;
                dat.status[x][y] = num;
                break;
        }
    }
    return true; // 20格恰好被填满
}

string HRD_cal::Change_str(unsigned long long dat) { // 将数字转化为文本编码
    string str;
    str.resize(9); // 修改其长度为9位
    for (int i = 8; i >= 0; i--) { // 将每一位从数值转为ASCII码
        if ((dat & 0xF) <= 9) { // 0 ~ 9
            str[i] = (dat & 0xF) + 48;
        } else { // A ~ F
            str[i] = (dat & 0xF) + 55;
        }
        dat >>= 4;
    }
    return str;
}

unsigned long long HRD_cal::Change_int (char *str) { // 将文本编码转化为数字(传入9位字符串)
    unsigned long long dat = 0;
    for (int i = 0; i < 9; i++) { // 将每一位从ASCII码转为数值
        dat <<= 4;
        if (str[i] >= 48 && str[i] <= 57) { // 0 ~ 9
            dat |= str[i] - 48;
        } else if (str[i] >= 65 && str[i] <= 70) { // A ~ F
            dat |= str[i] - 55;
        } else if (str[i] >= 97 && str[i] <= 102) { // a ~ f
            dat |= str[i] - 87;
        }
	}
    return dat;
}
