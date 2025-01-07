#include<iostream>
#include<vector>
#include<string>
#include<cctype>
#include<map>

using namespace std;

enum TokenType
{
    T_INT, T_ID, T_NUM, T_IF, T_ELSE, T_RETURN,
    T_ASSIGN, T_PLUS, T_MINUS, T_MUL, T_DIV,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE,  
    T_SEMICOLON, T_GT, T_EOF, 
    T_FOR, T_WHILE, T_EQ, T_LE, T_AND, T_FLOAT, T_STRING
};

struct Token 
{
    TokenType type;
    string value;
    int lineNumber;
    int columnNumber;
    string dataType;

    Token(TokenType t, const string& val, int ln, int col, const string& dt = "")
        : type(t), value(val), lineNumber(ln), columnNumber(col), dataType(dt) {}
};

class Lexer{
    private:
        string src;   //code
        size_t pos;   //position of pointer
        int line;
        int column;

    public:
        Lexer(const string &src)
        {
            this->src = src;
            this->pos = 0;
            this->line = 1;
            this->column = 1;
        }

        string consumeNumber() {
            size_t start = pos;
            bool isFloat = false;  

            while (pos < src.size() && isdigit(src[pos])) {
                pos++;
            }

            if (pos < src.size() && src[pos] == '.') {
                isFloat = true;
                pos++;  

                while (pos < src.size() && isdigit(src[pos])) {
                    pos++;
                }
            }

            if (isFloat) {
                return src.substr(start, pos - start);  //Floating-point number
            } else {
                return src.substr(start, pos - start);  //Integer number
            }
        }

        string consumeString() {
            size_t start = pos;
            pos++;  

            while (pos < src.size() && src[pos] != '"') {
                pos++;  
            }

            if (pos < src.size() && src[pos] == '"') {
                pos++;  
            } else {
                cout << "Error: Unterminated string literal at line " << line << ", column " << column << endl;
                exit(1);
            }

            return src.substr(start + 1, pos - start - 2);  
        }

        string consumeWord()
        {
            size_t start = pos;
            while(pos < src.size() && isalnum(src[pos]))
            {
                pos++;
            }
            return src.substr(start, pos - start);
        }

        vector<Token> tokenize()
        {
            vector<Token> tokens;
            while (pos < src.size())
            {
                char current = src[pos];
                if (isspace(current)) 
                {
                    if (current == '\n') {
                        line++;           
                        column = 1;       
                    } else {
                        column++;         
                    }
                    pos++;
                    continue;
                }

                if (isdigit(current)) {
                    tokens.push_back(Token{T_NUM, consumeNumber(), line, column});
                    continue;
                }

                if (isalpha(current))
                {
                    string word = consumeWord();
                    if (word == "int") tokens.push_back(Token{T_INT, word, line, column});
                    else if (word == "if") tokens.push_back(Token{T_IF, word, line, column});
                    else if (word == "else") tokens.push_back(Token{T_ELSE, word, line, column});
                    else if (word == "return") tokens.push_back(Token{T_RETURN, word, line, column});
                    else if (word == "for") tokens.push_back(Token{T_FOR, word, line, column});
                    else if (word == "while") tokens.push_back(Token{T_WHILE, word, line, column});
                    else if (word == "float") tokens.push_back(Token{T_FLOAT, word, line, column});
                    else if (word == "string") tokens.push_back(Token{T_STRING, word, line, column});
                    else tokens.push_back(Token{T_ID, word, line, column});
                    continue;
                }
                if (current == '"') {
                    string str = consumeString();
                    tokens.push_back(Token{T_STRING, str, line, column});
                    continue;
                }

                if (current == '=' && pos + 1 < src.size() && src[pos + 1] == '=') 
                {
                    tokens.push_back(Token{T_EQ, "==", line, column});
                    pos += 2;
                    column += 2;
                    continue;
                }

                if (current == '<' && pos + 1 < src.size() && src[pos + 1] == '=') 
                {
                    tokens.push_back(Token{T_LE, "<=", line, column});
                    pos += 2;
                    column += 2;
                    continue;
                }
                                
                if (current == '&' && pos + 1 < src.size() && src[pos + 1] == '&')
                {
                    tokens.push_back(Token{T_AND, "&&", line, column});
                    pos += 2; 
                    column += 2; 
                    continue;
                }

                switch (current)
                {
                    case '=': tokens.push_back(Token{T_ASSIGN, "=", line, column}); break;
                    case '+': tokens.push_back(Token{T_PLUS, "+", line, column}); break;
                    case '-': tokens.push_back(Token{T_MINUS, "-", line, column}); break;
                    case '*': tokens.push_back(Token{T_MUL, "*", line, column}); break;
                    case '/': tokens.push_back(Token{T_DIV, "/", line, column}); break;
                    case '(': tokens.push_back(Token{T_LPAREN, "(", line, column}); break;
                    case ')': tokens.push_back(Token{T_RPAREN, ")", line, column}); break;
                    case '{': tokens.push_back(Token{T_LBRACE, "{", line, column}); break;  
                    case '}': tokens.push_back(Token{T_RBRACE, "}", line, column}); break;  
                    case ';': tokens.push_back(Token{T_SEMICOLON, ";", line, column}); break;
                    case '>': tokens.push_back(Token{T_GT, ">", line, column}); break;
                    case '<': tokens.push_back(Token{T_LE, "<", line, column}); break;
                    default: cout << "Unexpected character: " << current << " at line " << line << ", column " << column << endl; exit(1);
                }
                pos++;
            }
            tokens.push_back(Token{T_EOF,"", line, column});
            return tokens;
        }
};

class Parser
{
    public:
        Parser(const vector<Token> &tokens)
        {
            this->tokens = tokens;
            this->pos = 0;
        }

        void parseProgram() {
        while (tokens[pos].type != T_EOF) 
            {
                parseStatement();
            }
            cout << "Parsing completed successfully! No Syntax Error" << endl;
        }

    private:
        vector<Token> tokens;
        size_t pos;

        void parseStatement() 
        {
            if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || tokens[pos].type == T_STRING) {
                parseDeclaration();
            } else if (tokens[pos].type == T_ID) {
                parseAssignment();
            } else if (tokens[pos].type == T_IF) {
                parseIfStatement();
            } else if (tokens[pos].type == T_RETURN) {
                parseReturnStatement();
            } else if (tokens[pos].type == T_FOR) {
                parseForLoop();  
            }
            else if (tokens[pos].type == T_WHILE) {
                parseWhileStatement();
            }
             else if (tokens[pos].type == T_LBRACE) {  
                parseBlock();
            } else {
                cout << "Syntax error: unexpected token " << tokens[pos].value << endl;
                exit(1);
            }
        }

        void parseForLoop() 
        {
            expect(T_FOR);          // Expect 'for'
            expect(T_LPAREN);       // Expect '('

            // Parse initialization
            expect(T_INT);
            expect(T_ID);
            expect(T_ASSIGN);
            parseExpression();      // Initialize (e.g., int i = 0)
            expect(T_SEMICOLON);    // Expect ';' after initialization

            // Parse condition
            parseExpression();      // Condition (e.g., i < 5)
            expect(T_SEMICOLON);    // Expect ';' after condition

            // Parse increment
            expect(T_ID);          // Increment variable (e.g., i)
            expect(T_ASSIGN);      // Expect '=' for increment
            parseExpression();      // Expression for increment (e.g., i + 1)
            expect(T_RPAREN);       // Expect ')' after increment

            parseStatement();       // Parse the body of the for loop
        }

        void parseWhileStatement() 
        {
            expect(T_WHILE);
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
            parseStatement();
        }


        void parseBlock() 
        {
            expect(T_LBRACE);  
            while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF) {
                parseStatement();
            }
            expect(T_RBRACE);  
        }
        void parseDeclaration() {
            if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || tokens[pos].type == T_STRING) {  
                pos++;  //Consume the type token (T_INT, T_FLOAT, or T_STRING)
                
                expect(T_ID);  
                
                if (tokens[pos].type == T_ASSIGN) {
                    pos++;  
                    if (tokens[pos].type == T_NUM || tokens[pos].type == T_STRING) {
                        pos++;  
                    } else {
                        cout << "Syntax error: expected value after '=' but found " << tokens[pos].value << endl;
                        exit(1);
                    }
                }

                expect(T_SEMICOLON);  
            } else {
                cout << "Syntax error: expected int, float, or string but found " << tokens[pos].value << endl;
                exit(1);
            }
        }

        void parseAssignment() {
            expect(T_ID);
            expect(T_ASSIGN);
            parseExpression();
            expect(T_SEMICOLON);
        }

        void parseIfStatement() {
            expect(T_IF);
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
            parseStatement();  
            if (tokens[pos].type == T_ELSE) {
                expect(T_ELSE);
                parseStatement();  
            }
        }

        void parseReturnStatement() {
            expect(T_RETURN);
            parseExpression();
            expect(T_SEMICOLON);
        }

        void parseExpression() {
            parseTerm();
            while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
                pos++;
                parseTerm();
            }
            if (tokens[pos].type == T_GT || tokens[pos].type == T_LE || tokens[pos].type == T_EQ) {
                pos++;
                parseExpression();  // After relational operator, parse the next expression
            }
            if (tokens[pos].type == T_AND) {
                pos++;
                parseExpression();  // Handle && logical operator
            }
        }

        void parseTerm() {
            parseFactor();
            while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV) {
                pos++;
                parseFactor();
            }
        }

        void parseFactor() {
            if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID) {
                pos++;
            } else if (tokens[pos].type == T_LPAREN) {
                expect(T_LPAREN);
                parseExpression();
                expect(T_RPAREN);
            } else {
                cout << "Syntax error: unexpected token " << tokens[pos].value << endl;
                exit(1);
            }
        }

        void expect(TokenType type) {
            if (tokens[pos].type == type) {
                pos++;
            } else {
                cout << "Syntax error: expected " << type << " but found " << tokens[pos].value << endl;
                exit(1);
            }
        }

};

int main() {
    string input = R"(
    int a;
    a = 5;
    int b;
    b = a + 10;
    float c;      
    c = 2.5; 
    string name = "uswa";

    for (int i = 0; i < b; i = i + 1) {
        if (b > 10) {
            return b;
        } else {
            return 0;
        }
    }

    int x;
    x = 10;
    while (x > 0) {
        x = x - 1;
    }
    return x;

)";


    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        cout << "Token: " << token.value << ", Type: " << token.type << ", Line: " << token.lineNumber << ", Col: " << token.columnNumber << endl;
    }
    
    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}