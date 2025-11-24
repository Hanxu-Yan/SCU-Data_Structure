#include <iostream>
#include <string>
#include <vector>
using namespace std;

const int NUM_BOWLS = 4;
const int NUM_DAYS = 7;

// 'A' 代表 Apple, 'O' 代表 Orange
char fruit[NUM_BOWLS][NUM_DAYS] = {
    {'A', 'O', 'A', 'O', 'A', 'A', 'O'}, // 蓝
    {'O', 'O', 'O', 'A', 'A', 'O', 'A'}, // 红
    {'A', 'A', 'O', 'O', 'O', 'A', 'A'}, // 绿
    {'A', 'O', 'O', 'A', 'O', 'O', 'O'}  // 黄
};

int target[NUM_BOWLS] = {5, 4, 5, 3};
// 当前递归路径中累积的次数
int current[NUM_BOWLS] = {0, 0, 0, 0};
int choices[NUM_DAYS]; // 记录每天选择的水果：0表示吃苹果，1表示吃橙子

void dfs(int day)
{
    if (day == NUM_DAYS)
    {
        bool valid = true;
        for (int i = 0; i < NUM_BOWLS; i++)
        {
            if (current[i] != target[i])
            {
                valid = false;
                break;
            }
        }
        if (valid)
        {
            for (int d = 0; d < NUM_DAYS; d++)
            {
                cout << "星期" << (d + 1) << ": " << (choices[d] == 0 ? "苹果" : "橙子") << endl;
            }
        }
        return;
    }
    char fruittype[2] = {'A', 'O'};
    for (int type = 0; type < 2; type++)
    {
        char fruit_try = fruittype[type];
        // 记录当天吃这种水果的食盆
        vector<int> bowls_eaten;
        bool can_eat = true;
        for (int i = 0; i < NUM_BOWLS; i++)
        {
            if (fruit[i][day] == fruit_try)
            {
                //剪枝
                if (current[i] + 1 > target[i])
                {
                    can_eat = false;
                    break;
                }
                bowls_eaten.push_back(i);
            }
        }
        if (can_eat)
        {
            // 选择这种水果
            for (int idx : bowls_eaten)
            {
                current[idx]++;
            }
            choices[day] = type;
            dfs(day + 1);
            // 回溯
            for (int idx : bowls_eaten)
            {
                current[idx]--;
            }
        }
    }
}

int main()
{
    dfs(0);


    return 0;
}