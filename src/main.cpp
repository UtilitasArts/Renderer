#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
	int num1 = 5;
	int num2 = 10;
	int sum = num1 + num2;
	
    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;
}