#include <iostream>
#include <string>
#include <stack>
#include <cctype>
using namespace std;

//1表示加减，2表示乘除，0表示括号
int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double applyOperation(double a, double b, char op) {
    switch (op) 
    {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                throw runtime_error("除数不能为零");
            }
            return a / b;
    }
    return 0;
}

double evaluateExpression(const string& expression)
{
    stack<double> values;
    stack<char> ops;
    for (int i = 0; i < expression.length(); i++)
    {
        if (expression[i] == ' ')
            continue;
        // 如果是数字，提取完整的数字并入栈
        if (isdigit(expression[i]) || expression[i] == '.') 
        {
            string num;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.'))
            {
                num += expression[i];
                i++;
            }
            values.push(stod(num));
            i--;
        }
        //如果是左括号，直接入栈
        else if (expression[i] == '(')
        {
            ops.push(expression[i]);
        }
        else if (expression[i] == ')')
        {
            while(!ops.empty() && ops.top() != '(')
            {
                double val1 = values.top();
                values.pop();
                double val2 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();

                values.push(applyOperation(val2, val1, op));
            }
            if (!ops.empty())
                ops.pop(); // 弹出左括号
        }
        //如果是运算符，处理优先级
        else
        {
            while (!ops.empty() && getPrecedence(ops.top()) >= getPrecedence(expression[i]))
            {
                double val1 = values.top();
                values.pop();
                double val2 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOperation(val2, val1, op));
            }
            ops.push(expression[i]);
        }
    }

    while (!ops.empty())
    {
        double val1 = values.top();
        values.pop();
        double val2 = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOperation(val2, val1, op));
    }

    return values.top();
}

int main()
{
    string expression;
    cout << "请输入一个数学表达式（支持 +, -, *, / 和括号）: ";
    getline(cin, expression);
    try 
    {
        double result = evaluateExpression(expression);
        cout << "表达式 " << expression << " 的计算结果是: " << result << endl;
    }
    catch (const exception& e) 
    {
        cerr << "错误: " << e.what() << endl;
    }

    return 0;
}