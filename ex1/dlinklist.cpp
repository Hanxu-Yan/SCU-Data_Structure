#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

struct Node
{
    string data;
    Node* prev;
    Node* next;

    Node(const string& val) : data(val), prev(nullptr), next(nullptr) {}
};
// 双向链表类
class DLinkList
{
public:
    Node* head;
    Node* tail;
    int length;
public:
    DLinkList();
    ~DLinkList();

    void push_back(const string& val);
    void clear();

    void insert_after(Node* prev_node, const string& val);
    void insert_before(Node* next_node, const string& val);

    void delete_node(Node* del_node);
};

DLinkList::DLinkList() : head(nullptr), tail(nullptr), length(0) {}
DLinkList::~DLinkList()
{
    clear();
}

void DLinkList::clear()
{
    Node* current = head;
    while (current)
    {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = tail = nullptr;
    length = 0;
}
void DLinkList::push_back(const string& val)
{
    Node* newNode = new Node(val);
    if (!head)
    {
        head = tail = newNode;
    }
    else
    {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    length++;
}
void DLinkList::insert_after(Node* prev_node, const string& val)
{
    if (!prev_node)
    {
        cout << "不能插入在空节点之后" << endl;

        return;
    }
    if (prev_node == tail)
    {
        push_back(val);
    }
    else
    {
        Node* newNode = new Node(val);

        newNode->next = prev_node->next;
        newNode->prev = prev_node;
        prev_node->next->prev = newNode;
        prev_node->next = newNode;

        length++;
    }
}
void DLinkList::insert_before(Node* next_node, const string& val)
{
    if (!next_node)
    {
        cout << "不能插入在空节点之前" << endl;

        return;
    }
    if (next_node == head)
    {
        Node* newNode = new Node(val);
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
        length++;
    }
    else 
    {
        insert_after(next_node->prev, val);
    }
}
void DLinkList::delete_node(Node* del_node)
{
    if (!del_node)
    {
        cout << "不能删除空节点" << endl;
        return;
    }
    if (del_node == head)
    {
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr; //变成空链表了
    }
    else if (del_node == tail)
    {
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr; //变成空链表了
    }
    else
    {
        del_node->prev->next = del_node->next;
        del_node->next->prev = del_node->prev;
    }

    delete del_node;
    length--;
}


//文本编辑类
class TextEditor
{
private:
    DLinkList buffer;
    Node* cursor; // 当前指针位置
    int cursor_lines; // 当前行号
    string input_filename;
    string output_filename;
public:
    TextEditor(const string& in_file, const string& out_file);
    void run();
private:
    void show_help(); // 显示帮助信息
    void display_all(); // 显示所有文件内容
    void move_to_first_line(); // 移动到第一行
    void move_to_last_line(); // 移动到最后一行
    void move_to_prev_line(); // 移动到前一行
    void move_to_next_line(); // 移动到下一行
    void move_to_line(int line_num); // 移动到指定行号
    void insert_line_after(); // 在当前行后插入新行
    void delete_current_line(); // 删除当前行
    void replace_current_line(); // 替换当前行内容
    void replace_substring_in_current_line(); // 替换当前行中的指定子串
    void find_substring(); // 查找指定子串
    void show_stats(); // 统计文件行数和字符数 
    void reload_file(); // 重新从输入文件读入，放弃所有修改
    void save_to_file(); // 保存当前内容到输出文件
    void exit_editor(); // 退出编辑器

    bool load_file(const string& filename);  // 从文件加载内容到缓冲区
    void print_current_line_info(); // 打印当前行信息
};
TextEditor::TextEditor(const string& in_file, const string& out_file):input_filename(in_file), output_filename(out_file), cursor(nullptr), cursor_lines(0) 
{
    if (!load_file(input_filename))
    {
        cout << "输入文件为空或无法打开" << input_filename << endl;
    }
    if (buffer.head)
    {
        cursor = buffer.head;
        cursor_lines = 1;
    }
}
bool TextEditor::load_file(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    buffer.clear();
    string line;
    while (getline(file, line))
    {
        buffer.push_back(line);
    }

    file.close();
    // 初始化光标位置
    if (buffer.length > 0)
    {
        cursor = buffer.head;
        cursor_lines = 1;
    }
    else
    {
        cursor = nullptr;
        cursor_lines = 0;
    }

    return buffer.length > 0;
}
void TextEditor::run()
{
    char command;
    cout << "简易文本编辑器已启动。输入 'h' 获取帮助。" << endl;

    while (true)
    {
        print_current_line_info();
        cout << "> ";
        cin >> command;

        // 清除输入缓冲区中留下的换行符
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (tolower(command))
        {
        case 'h': show_help(); break;
        case 'd': display_all(); break;
        case 't': move_to_first_line(); break;
        case 'b': move_to_last_line(); break;
        case 'p': move_to_prev_line(); break;
        case 'n': move_to_next_line(); break;
        case 'g': 
        {
            int line_num;
            cout << "输入行号: ";
            cin >> line_num;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            move_to_line(line_num);
        } break;
        case 'i': insert_line_after(); break;
        case 'x': delete_current_line(); break;
        case 'c': replace_current_line(); break;
        case 'r': replace_substring_in_current_line(); break;
        case 'f': find_substring(); break;
        case 's': show_stats(); break;
        case 'l': reload_file(); break;
        case 'w': save_to_file(); break;
        case 'q': 
        {
            cout << "退出编辑器。" << endl;
            return;
        }

        default:
            cout << "未知命令 '" << command << "'。输入 'h' 获取帮助。" << endl;
        }
    }
    
}
void TextEditor::print_current_line_info()
{
    if (cursor)
    {
        cout << "[" << cursor_lines << "/" << buffer.length << "] " << cursor->data << endl;
    }
    else
    {
        cout << "空白行" << endl;
    }
}
void TextEditor::show_help()
{
    cout << "\n--- 命令帮助 ---\n"
         << "h: 显示此帮助信息\n"
         << "d: 显示当前文件所有内容\n"
         << "t: 跳转到第一行\n"
         << "b: 跳转到最后一行\n"
         << "p: 跳转到前一行\n"
         << "n: 跳转到下一行\n"
         << "g: 跳转到指定行号\n"
         << "i: 在当前行后插入新行\n"
         << "x: 删除当前行\n"
         << "c: 替换当前行内容\n"
         << "r: 替换当前行中的指定子串\n"
         << "f: 查找指定子串\n"
         << "s: 统计文件行数和字符数\n"
         << "l: 重新从输入文件读入，放弃所有修改\n"
         << "w: 保存当前内容到输出文件\n"
         << "q: 退出系统\n"
         << "-----------------\n";
}
void TextEditor::display_all()
{
    if (!buffer.head)
    {
        cout << "[文件为空]" << endl;
        return;
    }
    Node* current = buffer.head;
    int line_num = 1;
    while (current)
    {
        if(current == cursor)
        {
            cout << "->" << line_num << ": " << current->data << endl;
        }
        else
        {
            cout << "  " << line_num << ": " << current->data << endl;
        }
        current = current->next;
        line_num++;
    }

}
void TextEditor::move_to_first_line()
{
    if (buffer.head)
    {
        cursor = buffer.head;
        cursor_lines = 1;
        cout << "已移动到第一行。" << endl;
    }
    else
    {
        cout << "文件为空。" << endl;
    }
}
void TextEditor::move_to_last_line()
{
    if (buffer.tail)
    {
        cursor = buffer.tail;
        cursor_lines = buffer.length;
        cout << "已移动到最后一行。" << endl;
    }
    else
    {
        cout << "文件为空。" << endl;
    }
}
void TextEditor::move_to_prev_line()
{
    if (cursor && cursor->prev)
    {
        cursor = cursor->prev;
        cursor_lines--;
    }
    else
    {
        cout << "已经是第一行或文件为空。" << endl;
    }
}
void TextEditor::move_to_next_line()
{
    if (cursor && cursor->next)
    {
        cursor = cursor->next;
        cursor_lines++;
    }
    else
    {
        cout << "已经是最后一行或文件为空。" << endl;
    }
}
void TextEditor::move_to_line(int line_num)
{
    if (line_num < 1 || line_num > buffer.length)
    {
        cout << "无效的行号！" << endl;
        return;
    }
    while (cursor_lines < line_num) move_to_next_line();
    while (cursor_lines > line_num) move_to_prev_line();

    cout << "已移动到第 " << line_num << " 行。" << endl;
}
void TextEditor::insert_line_after()
{
    string new_line;
    cout << "输入新行内容: ";
    getline(cin, new_line);
    if (!cursor)
    {
        buffer.push_back(new_line);
        cursor = buffer.head;
        cursor_lines = 1;
    }
    else
    {
        buffer.insert_after(cursor, new_line);
        cursor = cursor->next;
        cursor_lines++;
    }
    cout << "已插入新行。" << endl;
}
void TextEditor::delete_current_line()
{
    if (!cursor)
    {
        cout << "文件为空，无法删除。" << endl;
        return;
    }
    Node* node_to_delete = cursor;
    if (cursor->next)
    {
        cursor = cursor->next;
    }
    else if (cursor->prev)
    {
        cursor = cursor->prev;
        cursor_lines--;
    }
    else
    {
        cursor = nullptr;
        cursor_lines = 0;
    }
    buffer.delete_node(node_to_delete);
    cout << "已删除当前行。" << endl;
}
void TextEditor::replace_current_line()
{
    if (!cursor)
    {
        cout << "文件为空，无法替换。" << endl;
        return;
    }
    string new_line;
    cout << "输入新行内容: ";
    getline(cin, new_line);
    cursor->data = new_line;
    cout << "已替换当前行内容。" << endl;
}
void TextEditor::replace_substring_in_current_line()
{
    if (!cursor)
    {
        cout << "文件为空，无法替换子串。" << endl;
        return;
    }
    string old_substr, new_substr;
    cout << "输入要替换的子串: ";
    getline(cin, old_substr);
    cout << "输入新的子串: ";
    getline(cin, new_substr);
    size_t pos = cursor->data.find(old_substr);
    if (pos == string::npos)
    {
        cout << "未找到要替换的子串。" << endl;
    }
    else
    {
        int count = 0;
        while (pos != string::npos)
        {
            cursor->data.replace(pos, old_substr.length(), new_substr);
            pos = cursor->data.find(old_substr, pos + new_substr.length());
            count++;
        }
        cout << "已在当前行完成 " << count << " 次替换。" << endl;
    }
}
void TextEditor::find_substring()
{
    if (!buffer.head)
    {
        cout << "文件为空，无法查找子串。" << endl;
        return;
    }
    string substr;
    cout << "输入要查找的子串: ";
    getline(cin, substr);
    Node* search = cursor;
    int line_num = cursor_lines;
    do
    {
        size_t pos = search->data.find(substr);
        if (pos != string::npos)
        {
            cout << "在第 " << line_num << " 行找到子串 '" << substr << "'。" << endl;
            cursor = search;
            cursor_lines = line_num;
            return;
        }
        search = (search->next) ? search->next : buffer.head;
        line_num = (search == buffer.head) ? 1 : line_num + 1;
    } while (search != cursor);

    cout << "在整个文件里未找到子串 '" << substr << "'。" << endl;

}
void TextEditor::show_stats()
{
    long long char_count = 0;
    Node* current = buffer.head;
    while (current)
    {
        char_count += current->data.length();
        current = current->next;
    }
    cout << "--- 文件统计 ---\n"
         << "总行数: " << buffer.length << "\n"
         << "总字符数: " << char_count << "\n"
         << "-----------------\n";
}
void TextEditor::reload_file()
{
    cout << "重新从 '" << input_filename << "' 加载文件... 所有未保存的修改将丢失。" << endl;
    if (load_file(input_filename)) 
    {
        cout << "文件重新加载成功。" << endl;
    } else 
    {
        cout << "警告：无法重新加载输入文件，缓冲区可能为空。" << endl;
    }
}
void TextEditor::save_to_file()
{
    ofstream file(output_filename);
    if (!file.is_open())
    {
        cout << "无法打开输出文件 '" << output_filename << "' 进行写入。" << endl;
        return;
    }
    Node* current = buffer.head;
    while (current)
    {
        file << current->data << endl;
        current = current->next;
    }
    file.close();
    cout << "已将当前内容保存到 '" << output_filename << "'。" << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "用法: " << argv[0] << " <输入文件> <输出文件>" << endl;
        return 1;
    }


    TextEditor editor(argv[1], argv[2]);
    editor.run();

    return 0;
}
