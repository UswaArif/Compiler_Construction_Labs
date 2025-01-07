#include<iostream>
using namespace std;

class User{
    public:
        string name;
        int age;
        string phone;

        User(string name, int age, string phone)
        {
            this->name = name;
            this->age = age;
            this->phone = phone;

        }

        void method()
        {
            cout<< "Hello User: " << name << endl;
        }

        void method2(int age)
        {
            cout<< "Hello User: " << age << endl;
        }
};

main()
{
    // User user1;
    // user1.name = "Uswa";
    // user1.age = 22;
    // user1.phone = "03004326230";

    // user1.method();
    // user1.method2(user1.age);
    // User user2;
    // cout<< "Enter User Name:";
    // cin>> user2.name;

    User user1("Uswa",21,"030000");
    User user2("Amna",22,"03005676");

    cout << "Name: " << user1.name << endl;
    cout << "Age: " << user1.age << endl;
    cout << "Phone number: " << user1.phone << endl;

    cout << "Name: " << user2.name << endl;
    cout << "Age: " << user2.age << endl;
    cout << "Phone Number: " << user2.phone << endl;
}