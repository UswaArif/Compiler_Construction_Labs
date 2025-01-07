#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

enum TokenType
{
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    NUMBER,
    PUNTUATION,
    UNKNOWN
};

struct Token
{
    string lexeme;
    TokenType type;
};

unordered_set<string> keywords = {"if", "else", "for", "while", "return", "int", "float", "char", "void", "double"};
unordered_set<char> operators = {'+', '-', '*', '/', '=', '>', '<', '&', '|', '|'};
unordered_set<char> puntuation = {'{', '}', '(', ')', ';', ','};

bool isKeyword(const string &str)
{
    return keywords.find(str) != keywords.end();
}

vector<Token> tokenize(const string &input)
{
    vector<Token> tokens;
    int i = 0;

    while (i < input.length())
    {

        if (isspace(input[i]))
        {
            i++;
            continue;
        }
        if (isalpha(input[i]))
        {
            string lexeme;
            while (isalnum(input[i]))
            {
                lexeme += input[i];
                i++;
            }
            if (isKeyword(lexeme))
            {
                tokens.push_back({lexeme, KEYWORD});
            }
            else
            {
                tokens.push_back({lexeme, IDENTIFIER});
            }
        }
        else if (isdigit(input[i]))
        {
            string lexeme;
            while (isdigit(input[i]))
            {
                lexeme += input[i];
                i++;
            }
            tokens.push_back({lexeme, NUMBER});
        }
        else if (operators.find(input[i]) != operators.end())
        {
            string lexeme(1, input[i]);
            tokens.push_back({lexeme, OPERATOR});
            i++;
        }
        else if (puntuation.find(input[i]) != puntuation.end())
        {
            string lexeme(1, input[i]);
            tokens.push_back({lexeme, PUNTUATION});
            i++;
        }
        else
        {
            string lexeme(1, input[i]);
            tokens.push_back({lexeme, UNKNOWN});
            i++;
        }
    }
    return tokens;
}

void printTokens(const vector<Token> &tokens)
{
    for (const auto &token : tokens)
    {
        cout << "Token : " << token.lexeme << ", Type : ";
        switch (token.type)
        {
        case TokenType::IDENTIFIER:
            cout << "Identifier";
            break;
        case TokenType::NUMBER:
            cout << "Number";
            break;
            ;
        case TokenType::OPERATOR:
            cout << "Operator";
            break;
        case TokenType::PUNTUATION:
            cout << "Punctuation";
            break;
        case TokenType::KEYWORD:
            cout << "Keyword";
            break;
        case TokenType::UNKNOWN:
            cout << "Unknown";
            break;
        }
        cout << endl;
    }
}
int main()
{
    string input = "int main() { return 0; }";
    vector<Token> tokens = tokenize(input);
    printTokens(tokens);
    return 0;
}