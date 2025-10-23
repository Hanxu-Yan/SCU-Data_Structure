#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v; //一个存放int元素的向量，一开始里面没有元素
	v.push_back(1);	
	v.push_back(2); 
	v.push_back(3);  
	v.push_back(4);
	for(vector<int>::const_iterator i = v.begin();i != v.end();i ++ ) 
		cout << * i << ",";
	cout << endl;





    return 0;
}


