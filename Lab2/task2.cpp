#include <iostream>
#include <string>
using namespace std;

class Node 
{
public:
    string data;
    Node* next;
    Node* prev;

    Node(const string& data) 
    {
        this->data = data;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

class BrowserHistory 
{
    private:
        Node* head;
        Node* tail;
        Node* current;

    public:
        BrowserHistory() 
        {
            this->head = nullptr;
            this->tail = nullptr;
            this->current = nullptr;
        }

        void addPage(const string& page) 
        {
            Node* newNode = new Node(page);
            if (!head) 
            {    
                head = tail = current = newNode;
            } 
            else 
            {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
                current = tail; 
            }
            cout << "Added page: " << page << endl;
        }

        void moveForward() 
        {
            if (current && current->next) 
            {
                current = current->next;
                cout << "Moved forward to page: " << current->data << endl;
            } 
            else 
            {
                cout << "No more pages to move forward to." << endl;
            }
        }

        void moveBackward() 
        {
            if (current && current->prev) 
            {
                current = current->prev;
                cout << "Moved backward to page: " << current->data << endl;
            } 
            else 
            {
                cout << "No more pages to move backward to." << endl;
            }
        }

        void deletePage() 
        {
            if (!current) 
            {
                cout << "No page to delete." << endl;
                return;
            }

            if (current->prev) 
            {
                current->prev->next = current->next;
            } 
            else 
            {
                head = current->next; 
            }

            if (current->next) 
            {
                current->next->prev = current->prev;
            } 
            else 
            {
                tail = current->prev; 
            }

            Node* temp = current;
            current = current->prev ? current->prev : current->next; 

            delete temp;
            cout << "Deleted current page." << endl;
        }
};

main() 
{
    BrowserHistory history;
    history.addPage("home.html");
    history.addPage("about.html");
    history.addPage("contact.html");

    history.moveBackward();
    history.moveBackward();
    history.moveForward();

    history.deletePage();
    history.deletePage(); 
}
