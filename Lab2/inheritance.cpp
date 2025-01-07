#include <iostream>
#include <string> 
using namespace std;

class Username
{ 
    public:
        string name;
        int age;
        string phone;


        Username(string name = "", int age = 0, string phone = "") {
            this->name = name;
            this->age = age;
            this->phone = phone;
        }
    };

class Email : public Username 
{
    public:
        string email;
        Email(string name, int age, string phone, string email)
            : Username(name, age, phone) 
        {
            this->email = email;
        }
};

main() {
    Email user1("Uswa", 22, "34223", "uswa@gmail.com");
    cout << "Name: " << user1.name << endl;
    cout << "Age: " << user1.age << endl;
    cout << "Phone: " << user1.phone << endl;
    cout << "Email: " << user1.email << endl;
}