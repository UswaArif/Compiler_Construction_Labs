#include<iostream>
#include<deque>
#include <string>
using namespace std;

class TaskScheduling
{
    public:
        deque<string> tasks;

        void displayTasks()
        {   if(tasks.size() != 0)
            {
                for(int i=0; i<tasks.size(); i++)
                {   
                    cout<< i+1 << ":" << tasks[i] << endl;
                }
            }
            else
            {
                cout<< "No task to show";
            }
        }

        void add_FrontTasks(string task)
        {
            tasks.push_front(task);
        }

        void add_EndTasks(string task)
        {
            tasks.push_back(task);
        }

        void remove_FrontTask()
        {
            if(tasks.empty())
            {
                cout<< "No Task to remove.";
            }
            else
            {
                tasks.pop_front();
            }
        }

        void remove_EndTask()
        {
            if(tasks.empty())
            {
                cout<< "No Task to remove.";
            }
            else
            {
                tasks.pop_back();
            }
        }

};

main()
{
    TaskScheduling t;
    t.add_FrontTasks("learn JS");
    t.add_FrontTasks("learn Java");
    t.add_EndTasks("Practice C++");
    t.add_EndTasks("Practice C#");
    t.displayTasks();

    cout<< endl;
    t.remove_FrontTask();
    t.remove_EndTask();
    t.displayTasks();
}