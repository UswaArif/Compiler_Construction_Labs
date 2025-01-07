#include <iostream>
#include <stack>
#include <string>
using namespace std;

bool isBalanced(string expression) 
{
    stack<char> s;
    
    for (char ch : expression) 
    {
        if(ch == '(' || ch == '{' || ch == '[') 
        {
            s.push(ch);
        } 
        else if (ch == ')') 
        {
            if (s.empty() || s.top() != '(') 
            {
                return false;
            }
            s.pop();
        } 
        else if (ch == '}')
        {
            if (s.empty() || s.top() != '{') 
            {
                return false;
            }
            s.pop();
        } 
        else if (ch == ']') 
        {
            if (s.empty() || s.top() != '[') 
            {
                return false;
            }
            s.pop();
        }
    }
    return s.empty();
}

main() 
{
    string expression;

    cout << "Enter an expression with parentheses: ";
    cin>> expression;

    if (isBalanced(expression)) 
    {
        cout << "The expression is balanced." << endl;
    } else {
        cout << "The expression is not balanced." << endl;
    }
}
