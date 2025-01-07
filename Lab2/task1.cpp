#include<iostream>
#include<vector>
using namespace std;

class Grades
{
    public:
        vector<int> grades;

        void insertGrades(int grade)
        {
            grades.push_back(grade);
        }

        void displayGrades()
        {
            //cout<< grades.size();
            for(int i=0; i<grades.size(); i++)
            {
                cout<< grades[i] << " ";
            }
        }

        void deleteGrade()
        {
            if(grades.size() > 0)
            {
                grades.pop_back();
            }
            else
            {
                cout<< "Index out of range.";
            }           
        }

        int getGrade(int index) 
        {
        if (index < 0 || index >= grades.size()) 
        {
            cout << "Index out of range";
            return -1; 
        }
        return grades[index];
    }
};

main()
{
    Grades g;
    g.insertGrades(23);
    g.insertGrades(12);
    
    g.displayGrades();

    cout<< endl;
    g.deleteGrade();
    g.displayGrades();

    cout<< endl;
    g.insertGrades(34);
    g.insertGrades(54);
    g.displayGrades();

    cout<< endl;
    cout<< g.getGrade(2);
}