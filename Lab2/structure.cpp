#include<iostream>
using namespace std;

struct User
{
    string name;
    int age;
    string phone;
};

main()
{
    struct User user1;
    user1.name = "Uswa";
    user1.age = 22;
    user1.phone = "03004326230";

    cout << "Name: " << user1.name << endl;
    cout << "Age: " << user1.age << endl;
    cout << "Phone number: " << user1.phone << endl;

    struct User user2 = {"Amna",22,"453"};
    cout<< user2.name << user2.age << user2.phone;
}