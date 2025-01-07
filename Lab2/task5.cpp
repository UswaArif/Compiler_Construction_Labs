#include<iostream>
#include<queue>
#include <string>
using namespace std;

class Cinema
{
    public:
        queue<string> line;

        void addQueue(string customerName)
        {
            line.push(customerName);
        }

        void displayQueue()
        {
            if(line.empty())
            {
                cout<< "No customer in the line.";
            }
            else
            {
                int count = 1;
                while(!line.empty())
                {
                    cout<< count << ":" << line.front() << endl;
                    count++;
                    line.pop();
                }
            }
        }

        void joinVipQueue(string customerName) 
        {
            queue<string> tempQueue;

            if(!line.empty()) 
            {
                tempQueue.push(line.front());
                line.pop();
            }

            tempQueue.push(customerName);

            while (!line.empty()) {
                tempQueue.push(line.front());
                line.pop();
            }
            line = tempQueue;
        }

        void processTicket() 
        {
        if(!line.empty()) 
        {
            string nextCustomer = line.front();
            line.pop();
            cout << "Serving customer: " << nextCustomer << endl;
        } 
        else 
        {
            cout << "No customers in the queue." << endl;
        }
    }
};

main()
{
    Cinema c;
    c.addQueue("Uswa");
    c.addQueue("Amna");
    c.joinVipQueue("zainab");
    c.joinVipQueue("fatima");
    c.displayQueue();

    c.processTicket();
    c.displayQueue();
}