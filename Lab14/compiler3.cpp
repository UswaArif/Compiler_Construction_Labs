#include<iostream>
#include<vector>
#include<string>
#include<cctype>
#include<map>
#include <unordered_map> 
#include <fstream>

using namespace std;

enum TokenType
{
    T_INT, T_ID, T_NUM, T_IF, T_ELSE, T_RETURN,
    T_ASSIGN, T_PLUS, T_MINUS, T_MUL, T_DIV,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE,  
    T_SEMICOLON, T_GT, T_EOF, 
    T_FOR, T_WHILE, T_EQ, T_LE, T_AND, T_FLOAT, T_STRING,
    T_AGAR, T_OR, T_NOT, T_GE, T_LT, T_CHAR
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

// Symbol Table Class using unordered_map
class SymbolTable {
    private:
        map<string, string> table;  
    public:
        void insert(const string& name, const string& type) {
            if (table.find(name) != table.end())
            {
                throw runtime_error("Semantic error: Variable '" + name + "' is already declared.");
            }
            table[name] = type;
        }

        string lookup(const string& name) {
            if (table.find(name) != table.end()) {
                return table[name];
            }
            return "";
        }

        void display() {
            cout << "\nSymbol Table:\n";
            for (const auto& entry : table) {
                cout << "Name: " << entry.first << ", Type: " << entry.second << endl;
            }
        }

        bool isDeclared(const string &name) const 
        {
            return table.find(name) != table.end();
        }
};

class IntermediateCodeGnerator {
    public:
        vector<string> instructions;
        int tempCount = 0;

        string newTemp() {
            return "t" + to_string(tempCount++);
        }

        void addInstruction(const string &instr) {
            instructions.push_back(instr);
        }

        void printInstructions() {
            cout << "Three-Address Code (TAC):" << endl;
            for (const auto &instr : instructions) {
                cout << instr << endl;
            }
        }

        // Translate Three-Address Code (TAC) to Assembly code
        void generateAssemblyCode() {
        unordered_map<string, string> tempToRegMap;  // Maps TAC variables to registers
        string nextReg = "R1";                       // Start with register R1
        int regCount = 1;                            // Register counter
        int labelCount = 1;                          // Label counter

        cout << "\nAssembly Code:\n";

        for (const auto &instr : instructions) {
            vector<string> parts = splitInstruction(instr);

            // Handle arithmetic operations (addition, subtraction, multiplication, division)
            if (parts.size() == 5 && (parts[3] == "+" || parts[3] == "-" || parts[3] == "*" || parts[3] == "/")) {
                string operation = parts[3];  // Get the operator
                string assemblyOp;

                // Map the operator to the corresponding assembly instruction
                if (operation == "+") assemblyOp = "ADD";
                else if (operation == "-") assemblyOp = "SUB";
                else if (operation == "*") assemblyOp = "MUL";
                else if (operation == "/") assemblyOp = "DIV";

                // Generate assembly for arithmetic operations
                cout << "LOAD " << parts[2] << ", " << nextReg << endl;
                cout << assemblyOp << " " << parts[4] << ", " << nextReg << endl;
                tempToRegMap[parts[0]] = nextReg;  // Store result in temp register
                cout << "STORE " << nextReg << ", " << parts[0] << endl;
            }

            // Handle assignments (e.g., a = b)
            else if (parts.size() == 3 && parts[1] == "=") {
                // TAC: a = b
                // Assembly: LOAD b, R1 -> STORE R1, a
                cout << "LOAD " << parts[2] << ", " << nextReg << endl;
                cout << "STORE " << nextReg << ", " << parts[0] << endl;
            }

            // Handle unary operations (e.g., t3 = -a)
            else if (parts.size() == 4 && parts[2] == "-") {
                // TAC: t3 = -a
                // Assembly: LOAD a, R1 -> NEG R1 -> STORE R1, t3
                cout << "LOAD " << parts[3] << ", " << nextReg << endl;
                cout << "NEG " << nextReg << endl;
                tempToRegMap[parts[0]] = nextReg;
                cout << "STORE " << nextReg << ", " << parts[0] << endl;
            }

            // Handle conditional jumps (e.g., if a < b goto L1)
            else if (parts.size() == 6 && parts[0] == "if") {
                string condOp = parts[2];  // Get comparison operator
                string assemblyCond;

                // Map the condition operator to assembly branch condition
                if (condOp == "<") assemblyCond = "BLT";
                else if (condOp == "<=") assemblyCond = "BLE";
                else if (condOp == ">") assemblyCond = "BGT";
                else if (condOp == ">=") assemblyCond = "BGE";
                else if (condOp == "==") assemblyCond = "BEQ";
                else if (condOp == "!=") assemblyCond = "BNE";

                // Assembly: LOAD a, R1 -> COMP b, R1 -> CONDITIONAL BRANCH L1
                cout << "LOAD " << parts[1] << ", " << nextReg << endl;
                cout << "COMP " << parts[3] << ", " << nextReg << endl;
                cout << assemblyCond << " " << parts[5] << endl;
            }

            // Handle unconditional jumps (e.g., goto L2)
            else if (parts.size() == 2 && parts[0] == "goto") {
                // TAC: goto L2
                // Assembly: JMP L2
                cout << "JMP " << parts[1] << endl;
            }

            // Handle labels (e.g., L1:)
            else if (parts.size() == 1 && parts[0].back() == ':') {
                // TAC: L1:
                // Assembly: L1:
                cout << parts[0] << endl;
            }

            // Update next available register
            nextReg = "R" + to_string(++regCount);
        }
    }

    private:
        // Helper function to split TAC instruction into parts
        vector<string> splitInstruction(const string &instr) {
            vector<string> parts;
            string part;
            for (char ch : instr) {
                if (isspace(ch)) {
                    if (!part.empty()) {
                        parts.push_back(part);
                        part.clear();
                    }
                } else {
                    part += ch;
                }
            }
            if (!part.empty()) parts.push_back(part);
            return parts;
        }
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

        string consumeCharLiteral() {
            pos++; 
            if (pos < src.size() && src[pos + 1] == '\'') {
                string charLiteral(1, src[pos]);
                pos += 2; 
                return charLiteral;
            }
            cout << "Error: Invalid character literal on line " << line << endl;
            exit(1);
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
                //comments code
                if(current=='/' && pos+1<src.size() && src[pos+1]=='/')
                {
                    while(pos<src.size() && src[pos]!='\n')
                    {
                        pos++;
                    }
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
                    else if (word == "agar") tokens.push_back(Token{T_AGAR, word, line, column});
                    else if (word == "else") tokens.push_back(Token{T_ELSE, word, line, column});
                    else if (word == "return") tokens.push_back(Token{T_RETURN, word, line, column});
                    else if (word == "for") tokens.push_back(Token{T_FOR, word, line, column});
                    else if (word == "while") tokens.push_back(Token{T_WHILE, word, line, column});
                    else if (word == "float") tokens.push_back(Token{T_FLOAT, word, line, column});
                    else if (word == "string") tokens.push_back(Token{T_STRING, word, line, column});
                    else if (word == "char") tokens.push_back(Token{T_CHAR, word, line, column});
                    else tokens.push_back(Token{T_ID, word, line, column});
                    continue;
                }
                if (current == '"') {
                    string str = consumeString();
                    tokens.push_back(Token{T_STRING, str, line, column});
                    continue;
                }

                if (current == '\'') {
                    string str = consumeCharLiteral();
                    tokens.push_back(Token{T_CHAR, str, line, column});
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

                if (current == '>' && pos + 1 < src.size() && src[pos + 1] == '=') 
                {
                    tokens.push_back(Token{T_GE, ">=", line, column});
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

                if (current == '|' && pos + 1 < src.size() && src[pos + 1] == '|')
                {
                    tokens.push_back(Token{T_OR, "||", line, column});
                    pos += 2; 
                    column += 2; 
                    continue;
                }

                if (current == '!' && pos + 1 < src.size() && src[pos + 1] == '=')
                {
                    tokens.push_back(Token{T_NOT, "!=", line, column});
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
                    case '<': tokens.push_back(Token{T_LT, "<", line, column}); break;
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
        Parser(const vector<Token> &tokens, SymbolTable &symTable, IntermediateCodeGnerator &icg)
        : tokens(tokens), symTable(symTable), pos(0), icg(icg)   
        {
            //Constructor 
            //here the private member of this class are being initalized with the arguments passed to this constructor
        }

        void parseProgram() {
        while (tokens[pos].type != T_EOF) 
            {
                parseStatement();
            }
            cout << "Parsing completed successfully! No Syntax Error" << endl;

            symTable.display();
        }

    private:
        vector<Token> tokens;
        size_t pos;
        SymbolTable& symTable;
        IntermediateCodeGnerator &icg;

        void parseStatement() 
        {
            if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || tokens[pos].type == T_STRING || tokens[pos].type == T_CHAR) {
                parseDeclaration();
            } else if (tokens[pos].type == T_ID) {
                parseAssignment();
            } else if (tokens[pos].type == T_IF) {
                parseIfStatement();
            } else if (tokens[pos].type == T_AGAR) {
                parseAgarStatement();
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
                cout << "Syntax error: unexpected token " << tokens[pos].value << "' at line " << tokens[pos].lineNumber << endl;
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


        

        /*
        parseDeclaration handles the parsing of variable declarations.
        It expects the token type to be `T_INT` (for declaring an integer type variable),
        followed by an identifier (variable name), and a semicolon to terminate the statement.
        It also registers the declared variable in the symbol table with type "int".
        Example:
        int x;   // This will be parsed and the symbol table will store x with type "int".
        */
        void parseDeclaration()
        {
            if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || tokens[pos].type == T_STRING || tokens[pos].type == T_CHAR)
            {
                TokenType type = tokens[pos].type;
                string dataType = tokens[pos].value;
                
                pos++; // Consume the type token (T_INT, T_FLOAT, or T_STRING, or T_BOOl)

                Token idToken = tokens[pos];
                expect(T_ID); // Expect an identifier token for the variable name

                // Check for redeclaration (if variable already exists in the symbol table)
                if (symTable.lookup(idToken.value) != "")
                {
                    cout << "Semantic error: Variable '" << idToken.value << "' is already declared at line "
                        << idToken.lineNumber << ", column " << idToken.columnNumber << endl;
                    exit(1); // Exit the program in case of semantic error
                }

                // Insert variable into the symbol table
                symTable.insert(idToken.value, dataType);

                if (tokens[pos].type == T_ASSIGN)
                {
                    pos++; // Consume the assignment token '='
                    if (tokens[pos].type == T_NUM)
                    {
                        // If assigned value is a number, check if it's a float or integer
                        string assignedValue = tokens[pos].value;
                        bool isFloat = assignedValue.find('.') != string::npos;

                        // Retrieve the variable type from the symbol table
                        string varType = symTable.lookup(idToken.value);

                        if (varType == "int" && isFloat)
                        {
                            cout << "Semantic error: Cannot assign a float value to an int variable '"
                                << idToken.value << "' at line " << idToken.lineNumber
                                << ", column " << idToken.columnNumber << endl;
                            exit(1); // Exit the program in case of semantic error
                        }
                        pos++; // Consume the number token
                    }
                    
                    else if (tokens[pos].type == T_STRING)
                    {
                        // If assigned value is a string, check if the variable is a string type
                        string varType = symTable.lookup(idToken.value);
                        if (varType != "string")
                        {
                            cout << "Semantic error: Cannot assign a string value to a non-string variable '"
                                << idToken.value << "' at line " << idToken.lineNumber
                                << ", column " << idToken.columnNumber << endl;
                            exit(1);
                        }
                        pos++; // Consume the string token
                    }

                    else if (tokens[pos].type == T_CHAR)
                    {
                        // If assigned value is a char, check if the variable is a char type
                        string varType = symTable.lookup(idToken.value);
                        if (varType != "char")
                        {
                            cout << "Semantic error: Cannot assign a string value to a non-string variable '"
                                << idToken.value << "' at line " << idToken.lineNumber
                                << ", column " << idToken.columnNumber << endl;
                            exit(1);
                        }
                        pos++; // Consume the char token
                    }
                                  
                    else
                    {
                        cout << "Syntax error: expected value after '=' but found " << tokens[pos].value << endl;
                        exit(1);
                    }
                }

                expect(T_SEMICOLON); // Expect a semicolon to end the declaration
            }
            else
            {
                cout << "Syntax error: expected int, float, or string but found " << tokens[pos].value << endl;
                exit(1);
            }
        }


        void parseAssignment() {
            string varName = expectAndReturnValue(T_ID);
            symTable.lookup(varName);    // Ensure the variable is declared in the symbol table.

            
            //expect(T_ID); // Expect an identifier for the variable name

            

            expect(T_ASSIGN);
            string expr = parseExpression();
            
            icg.addInstruction(varName + " = " + expr);  // Generate intermediate code for the assignment.
            expect(T_SEMICOLON);
        }
        


        /*
            parseIfStatement handles the parsing of `if` statements.
            It expects the keyword `if`, followed by an expression in parentheses that serves as the condition.
            If the condition evaluates to true, it executes the statement inside the block. If an `else` part is present,
            it executes the corresponding statement after the `else` keyword.
            Intermediate code for the `if` statement is generated, including labels for conditional jumps.
            Example:
            if(5 > 3) { x = 20; }  --> This will generate intermediate code for the condition check and jump instructions.
        */
        void parseIfStatement() {
            expect(T_IF);
            expect(T_LPAREN);// Expect and consume the opening parenthesis for the condition.
            string cond = parseExpression(); // Parse the condition expression inside the parentheses.
            expect(T_RPAREN);
            
            string temp = icg.newTemp();    // Generate a new temporary variable for the condition result.
            icg.addInstruction(temp + " = " + cond);        // Generate intermediate code for storing the condition result.
            icg.addInstruction("if " + temp + " goto L1");   // Jump to label L1 if condition is true.
            icg.addInstruction("goto L2");                  // Otherwise, jump to label L2.
            icg.addInstruction("L1:");                      // Otherwise, jump to label L2.

            parseStatement(); 

            if (tokens[pos].type == T_ELSE) {// If an `else` part exists, handle it.
                icg.addInstruction("goto L3");
                icg.addInstruction("L2:");
                expect(T_ELSE);
                parseStatement(); // Parse the statement inside the else block.
                icg.addInstruction("L3:");
            }
            else {
                icg.addInstruction("L2:");
            }
        }

        void parseAgarStatement() {
            expect(T_AGAR);
            expect(T_LPAREN);// Expect and consume the opening parenthesis for the condition.
            string cond = parseExpression(); // Parse the condition expression inside the parentheses.
            expect(T_RPAREN);
            
            string temp = icg.newTemp();    // Generate a new temporary variable for the condition result.
            icg.addInstruction(temp + " = " + cond);        // Generate intermediate code for storing the condition result.
            icg.addInstruction("agar " + temp + " goto L1");   // Jump to label L1 if condition is true.
            icg.addInstruction("goto L2");                  // Otherwise, jump to label L2.
            icg.addInstruction("L1:");                      // Otherwise, jump to label L2.
            parseStatement(); 
        }

        /*
            parseReturnStatement handles the parsing of `return` statements.
            It expects the keyword `return`, followed by an expression to return, and a semicolon to terminate the statement.
            It generates intermediate code to represent the return of the expression.
            Example:
            return x + 5;   -->  This will generate intermediate code like `return x + 5`.
        */
        void parseReturnStatement() {
            expect(T_RETURN);
            string expr = parseExpression();
            icg.addInstruction("return " + expr);  // Generate intermediate code for the return statement.
            expect(T_SEMICOLON);
        }

        /*
            parseBlock handles the parsing of block statements, which are enclosed in curly braces `{ }`.
            It parses the statements inside the block recursively until it reaches the closing brace.
            Example:
            { x = 10; y = 20; }   -->  This will parse each statement inside the block.
        */
        void parseBlock() 
        {
            expect(T_LBRACE);  // Expect and consume the opening brace `{`. 
            while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF) {
                parseStatement();// Parse the statements inside the block.
            }
            expect(T_RBRACE);  
        }

        /*
            parseExpression handles the parsing of expressions involving addition, subtraction, or comparison operations.
            It first parses a term, then processes addition (`+`) or subtraction (`-`) operators if present, generating
            intermediate code for the operations.
            Example:
            5 + 3 - 2;  -->  This will generate intermediate code like `t0 = 5 + 3` and `t1 = t0 - 2`.
        */
        string parseExpression() {
            string term = parseTerm();
            while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
                TokenType op = tokens[pos++].type;
                string nextTerm = parseTerm();    // Parse the next term in the expression.
                string temp = icg.newTemp();     // Generate a temporary variable for the result
                icg.addInstruction(temp + " = " + term + (op == T_PLUS ? " + " : " - ") + nextTerm); // Intermediate code for operation
                term = temp;
            }
            if (tokens[pos].type == T_GT || tokens[pos].type == T_LT || tokens[pos].type == T_LE || tokens[pos].type == T_EQ || tokens[pos].type == T_GE) {
                pos++;
                string nextExpr = parseExpression();    // Parse the next expression for the comparison.
                string temp = icg.newTemp();             // Generate a temporary variable for the result.
                icg.addInstruction(temp + " = " + term + " > " + nextExpr); // Intermediate code for the comparison.
                term = temp; 
            }
            if (tokens[pos].type == T_AND) {
                pos++;
                string nextExpr = parseExpression();  // Parse the next expression for the logical '&&'
                string temp = icg.newTemp();          // Generate a temporary variable for the result
                icg.addInstruction(temp + " = " + term + " && " + nextExpr); // Intermediate code for logical AND
                term = temp;  // Update term with the result of the logical AND operation
            }

            if (tokens[pos].type == T_OR) {
                pos++;
                string nextExpr = parseExpression();  // Parse the next expression for the logical '&&'
                string temp = icg.newTemp();          // Generate a temporary variable for the result
                icg.addInstruction(temp + " = " + term + " || " + nextExpr); // Intermediate code for logical AND
                term = temp;  // Update term with the result of the logical AND operation
            }

            if (tokens[pos].type == T_NOT) {
                pos++;
                string nextExpr = parseExpression();  // Parse the next expression for the logical '&&'
                string temp = icg.newTemp();          // Generate a temporary variable for the result
                icg.addInstruction(temp + " = " + term + " != " + nextExpr); // Intermediate code for logical AND
                term = temp;  // Update term with the result of the logical AND operation
            }
        
            return term;
        }

        /*
            parseTerm handles the parsing of terms involving multiplication or division operations.
            It first parses a factor, then processes multiplication (`*`) or division (`/`) operators if present,
            generating intermediate code for the operations.
            Example:
            5 * 3 / 2;   This will generate intermediate code like `t0 = 5 * 3` and `t1 = t0 / 2`.
        */
        string parseTerm() {
            string factor = parseFactor();
            while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV) {
                TokenType op = tokens[pos++].type;
                string nextFactor = parseFactor();
                string temp = icg.newTemp(); // Generate a temporary variable for the result.
                icg.addInstruction(temp + " = " + factor + (op == T_MUL ? " * " : " / ") + nextFactor);  // Intermediate code for operation.
                factor = temp;  // Update the factor to be the temporary result.
            }
            return factor;
        }

        /*
            parseFactor handles the parsing of factors in expressions, which can be either numeric literals, identifiers
            (variables), or expressions inside parentheses (for sub-expressions).
            Example:
            5;          -->  This will return the number "5".
            x;          -->  This will return the identifier "x".
            (5 + 3);    --> This will return the sub-expression "5 + 3".
        */
        string parseFactor() {
            if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID) {
                return tokens[pos++].value;
            } else if (tokens[pos].type == T_LPAREN) {
                expect(T_LPAREN);
                string expr = parseExpression();
                expect(T_RPAREN);
                return expr;
            } else {
                cout << "Syntax error: unexpected token '" << tokens[pos].value << "' at line " << tokens[pos].lineNumber << endl;
                exit(1);
            }
        }

        /*
            expect function:
            This functin is used to check whether the current token matches the expected type.
            If the token type does not match the expected type, an error message is displayed
            and the program exits. If the token type matches, it advances the position to the next token.
        */
        void expect(TokenType type) {
            if (tokens[pos].type == type) {
                pos++;
            } else {
                cout << "Syntax error: expected '" << type << "' at line " << tokens[pos].lineNumber << endl;
                exit(1);
            }
        }

        /* 
        Explanation:
        - The `expect` function ensures that the parser encounters the correct tokens in the expected order. 
        - It's mainly used for non-value-based tokens, such as keywords, operators, and delimiters (e.g., semicolons).
        - If the parser encounters an unexpected token, it halts the process by printing an error message, indicating where the error occurred (line number) and what was expected.
        - The `pos++` advances to the next token after confirming the expected token is present.

        Use Case:
        - This function is helpful when checking for the correct syntax or structure in a language's grammar, ensuring the parser processes the tokens in the correct order.
        */
        string expectAndReturnValue(TokenType type) {
            string value = tokens[pos].value;
            expect(type);
            return value;
        }

/*
    Why both functions are needed:
    - The `expect` function is useful when you are only concerned with ensuring the correct token type without needing its value.
    - For example, ensuring a semicolon `;` or a keyword `if` is present in the source code.
    - The `expectAndReturnValue` function is needed when the parser not only needs to check for a specific token but also needs to use the value of that token in the next stages of compilation or interpretation.
    - For example, extracting the name of a variable (`T_ID`) or the value of a constant (`T_NUMBER`) to process it in a symbol table or during expression evaluation.
*/
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <source file>" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cout << "Failed to open file: " << argv[1] << endl;
        return 1;
    }

    string input((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    


    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        cout << "Token: " << token.value << ", Type: " << token.type << ", Line: " << token.lineNumber << ", Col: " << token.columnNumber << endl;
    }
    
    SymbolTable symTable;
    IntermediateCodeGnerator icg;
    Parser parser(tokens, symTable, icg);
    parser.parseProgram();
    cout<< endl;
    cout<< "Three Addres Code: " << endl;
    icg.printInstructions();
    icg.generateAssemblyCode();
    return 0;
}