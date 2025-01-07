#include<iostream>
#include<vector>
#include<cctype>
#include<string>
#include <algorithm>
using namespace std;

enum TokenType 
{
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    PUNCTUATION,
    UNKNOWN
};

struct Token
{
    string token;
    TokenType type;
};

bool isOperator(char c) 
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '!' || c == '<' || c == '>';
}

bool isPunctuation(char c) 
{
    return c == ';' || c == ',' || c == '.' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']';
}

bool isKeyword(string str) 
{
    std::vector<string> keywords = {"int", "return", "if", "else", "while", "for", "main"};
    
    return find(keywords.begin(), keywords.end(), str) != keywords.end();
}

vector<Token> tokenize(string input)
{
    vector<Token> tokens;

    for(int i=0; i<input.length(); i++)
    {
        char current = input[i];
        if (isspace(current)) 
        {
            i++;
            continue;
        }

        if(isalpha(current))
        {
            string lexeme;
            while (i < input.length() && (isalnum(input[i]) || input[i] == '_')) 
            {
                lexeme += input[i];
                i++;
            }
            if (isKeyword(lexeme)) 
            {
                tokens.push_back({lexeme, KEYWORD});
            } else 
            {
                tokens.push_back({lexeme, IDENTIFIER});
            }
            continue;
        }

        if (isdigit(current)) 
        {
            string number;
            while (i < input.length() && isdigit(input[i])) 
            {
                number += input[i];
                i++;
            }
            tokens.push_back({number, NUMBER});  
            continue;
        }

        if(isOperator(current))
        {
            string operatorToken(1, current);
            tokens.push_back({operatorToken, OPERATOR}); 
            i++;
            continue;
        }

        if (isPunctuation(current)) 
        {
            string punctuationToken(1, current);
            tokens.push_back({punctuationToken, PUNCTUATION}); 
            i++;
            continue;
        }

        
        string unknownToken(1, current);
        tokens.push_back({unknownToken, UNKNOWN}); 
        i++;
    }

     return tokens;
}


int main()
{
    string code = "int main() { int a = 10; return a; }";
    
    vector<Token> tokens = tokenize(code);

    for (const auto& token : tokens) 
    {
        cout << "Token: " << token.token << ", Type: ";
        switch (token.type) 
        {
            case KEYWORD: cout << "Keyword"; break;
            case IDENTIFIER: cout << "Identifier"; break;
            case NUMBER: cout << "Number"; break;
            case OPERATOR: cout << "Operator"; break;
            case PUNCTUATION: cout << "Punctuation"; break;
            case UNKNOWN: cout << "Unknown"; break;
        }
        cout << endl;
    }

    return 0;
}