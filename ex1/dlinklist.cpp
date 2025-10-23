#include <iostream>
#include <string>
using namespace std;

struct Node
{
    string data;
    Node* prev;
    Node* next;

    Node(const string& val) : data(val), prev(nullptr), next(nullptr) {}
};

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
        
    }
}
int main()
{
    DLinkList dlist;
    dlist.push_back("Node1");
    dlist.push_back("Node2");

    // Insert after the first node
    Node* firstNode = dlist.head; // Assuming head is accessible for demonstration
    dlist.insert_after(firstNode, "Node1.5");
    // Print the list to verify insertion
    for (Node* curr = dlist.head; curr != nullptr; curr = curr->next)
    {
        cout << curr->data << " ";
    }
    cout << endl;

    return 0;
}