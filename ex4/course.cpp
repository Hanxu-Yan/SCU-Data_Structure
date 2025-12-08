#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

constexpr int MAX_COURSES = 100;
struct CourseInfo
{
    string id; // 编号c01
    string name; // 课程名称
    int hours; // 学时
    int fixSem; // 指定开课学期，0表示自动安排
    int inDegree; //入度：记录由于先修课维修而受阻的课程数
    bool isTaken; // 标记该课程是否已修读
    vector<int> next; //邻接表：记录修完这门课后可以解锁的课程在数组中的下标
};

CourseInfo courses[MAX_COURSES];
int courseCount = 0;  //实际课程数量
int semLimit[9];  //存储8个学期的课时限制 (下标1-8)
map<string, int> idMap; // 课程ID到数组下标的映射

vector<string> tempPre[MAX_COURSES];

//读取文件
void LoadFile()
{
    ifstream infile("course_inf.txt");
    if (!infile)
    {
        cerr << "无法打开文件 courses.txt" << endl;
        return;
    }
    string line;
    bool readLimit = false;
    courseCount = 0;
    while (getline(infile, line))
    {
        if (line.empty()) continue;
        if (line.size() >=2 && line[0] == '/' && line[1] == '/') continue; //跳过注释行
        // 读取学期课程数量限制
        if (isdigit(line[0])) 
        {
            stringstream ss(line);
            for (int i = 1; i <=8; i++) ss >> semLimit[i];
            readLimit = true;
            continue;
        }

        //处理课程行
        stringstream ss(line);
        string id, name;
        int hours, fixSem;
        ss >> id >> name >> hours >> fixSem;
        courses[courseCount].id = id;
        courses[courseCount].name = name;
        courses[courseCount].hours = hours;
        courses[courseCount].fixSem = fixSem;
        courses[courseCount].inDegree = 0;
        courses[courseCount].isTaken = false;
        idMap[id] = courseCount;

        //读取先修课程
        string pre;
        while (ss >> pre)
        {
            tempPre[courseCount].push_back(pre);
        }
        courseCount++;
    }
    infile.close();
}

//构建课程依赖图
void BuildGraph()
{
    for (int i = 0; i < courseCount; i++)
    {
        for (const string& preId : tempPre[i])
        {
            //先导课课程编号preId
            if (idMap.find(preId) != idMap.end())
            {
                int preIndex = idMap[preId];
                //邻接表：先导课->当前课程
                courses[preIndex].next.push_back(i);
                courses[i].inDegree++;
            }
        }
    }
}

//拓扑排序排课
void TopoSort()
{
    cout << "------------ 课程表排课结果 ------------" << endl;
    for (int sem = 1; sem <= 8; sem++)
    {
        int limit = semLimit[sem];
        vector<int> thisTermCourses;//本学期安排的课程下标
        //处理指定了学期的课程
        for (int i = 0; i < courseCount; i++)
        {
            if (!courses[i].isTaken && courses[i].fixSem == sem)
            {
                if (courses[i].inDegree == 0)
                {
                    thisTermCourses.push_back(i);
                    courses[i].isTaken = true;
                }
            }
        }
        //处理没有指定学期的课程
        for (int i = 0; i < courseCount; i++)
        {
            if (thisTermCourses.size() >= limit) break;
            if (!courses[i].isTaken && courses[i].inDegree == 0 && courses[i].fixSem == 0)
            {
                thisTermCourses.push_back(i);
                courses[i].isTaken = true;
            }
        }
        cout << "第 " << sem << " 学期课程：" << endl;
        if (thisTermCourses.empty()) cout << "  无课程安排" << endl;
        else 
        {
            for (int i = 0; i < thisTermCourses.size(); i++)
            {
                int idx = thisTermCourses[i];
                cout << " " << courses[idx].id << " " <<courses[idx].name << " ";
            }
        }
        cout << endl;
        //更新后续课程的入度
        for (int i = 0; i < thisTermCourses.size(); i++)
        {
            for (int j = 0; j < courses[thisTermCourses[i]].next.size(); j++)
            {
                int nextCourse = courses[thisTermCourses[i]].next[j];
                courses[nextCourse].inDegree--;
            }
        }
    }
}



int main()
{
    LoadFile();
    BuildGraph();
    TopoSort();
    return 0;
}