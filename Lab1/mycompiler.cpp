#include <iostream>
#include <fstream>
#include <string>
using namespace std; 

main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Write code as: mycompiler.exe mycode.txt" << endl;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << argv[1] << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}
