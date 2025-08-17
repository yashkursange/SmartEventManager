#include <bits/stdc++.h>
using namespace std;
#include <ctime> // for current date

struct Event
{
    int id;
    string name;
    string date; // "DD-MM-YYYY"
    string time; // "HH:MM"
    string type;
    string location;

    string key() const
    {
        // Convert date DD-MM-YYYY to YYYYMMDD
        string y = date.substr(6, 4);
        string m = date.substr(3, 2);
        string d = date.substr(0, 2);
        return y + m + d + " " + time; // YYYYMMDD HH:MM
    }
    
};

struct adminacc
{
public:
    string adm = "Yash_kursange";
    string pass = "@Yash123";
};

struct Node
{
    Event event;
    Node *left;
    Node *right;

    Node(Event e) : event(e), left(NULL), right(NULL) {}
};

class EventBST
{
private:
    Node *root;

    Node *insert(Node *node, Event e)
    {
        if (!node)
            return new Node(e);

        if (e.key() < node->event.key())
            node->left = insert(node->left, e);
        else if (e.key() > node->event.key())
            node->right = insert(node->right, e);
        else
            cout << "Conflict: Event already exists at " << e.date << " " << e.time << endl;

        return node;
    }

    void inorder(Node *node)
    {
        if (!node)
            return;
        inorder(node->left);
        cout << node->event.id << ". " << node->event.date << " " << node->event.time
             << " | " << node->event.name
             << " (" << node->event.type << ") at "
             << node->event.location << endl;
        inorder(node->right);
    }

    string toLower(string s)
    {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    void searchByField(Node *node, string key, string field)
    {
        if (!node)
            return;
        searchByField(node->left, key, field);

        string target = toLower(key);
        string cmp;
        if (field == "name")
            cmp = toLower(node->event.name);
        else
            cmp = toLower(node->event.type);

        if (cmp.find(target) != string::npos)
        {
            cout << node->event.id << ". " << node->event.date << " " << node->event.time
                 << " | " << node->event.name
                 << " (" << node->event.type << ") at "
                 << node->event.location << endl;
        }

        searchByField(node->right, key, field);
    }

    Node *findMin(Node *node)
    {
        while (node && node->left)
            node = node->left;
        return node;
    }

    Node *remove(Node *node, string key)
    {
        if (!node)
            return node;

        if (key < node->event.key())
            node->left = remove(node->left, key);
        else if (key > node->event.key())
            node->right = remove(node->right, key);
        else
        {
            if (!node->left)
            {
                Node *temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right)
            {
                Node *temp = node->left;
                delete node;
                return temp;
            }
            Node *temp = findMin(node->right);
            node->event = temp->event;
            node->right = remove(node->right, temp->event.key());
        }
        return node;
    }

    void inorderByDate(Node *node, string date)
    {
        if (!node)
            return;
        inorderByDate(node->left, date);
        if (node->event.date == date)
        {
            cout << node->event.id << ". " << node->event.date << " " << node->event.time
                 << " | " << node->event.name
                 << " (" << node->event.type << ") at "
                 << node->event.location << endl;
        }
        inorderByDate(node->right, date);
    }

    Node *search(Node *node, string key)
    {
        if (!node)
            return nullptr;
        if (node->event.key() == key)
            return node;
        if (key < node->event.key())
            return search(node->left, key);
        return search(node->right, key);
    }

    string makeKey(string date, string time)
    {
        string y = date.substr(6, 4);
        string m = date.substr(3, 2);
        string d = date.substr(0, 2);
        return y + m + d + " " + time;
    }
     void saveNode(ofstream &out, Node *node)
    {
        if (!node) return;
        saveNode(out, node->left);
        out << node->event.id << "|"
            << node->event.name << "|"
            << node->event.date << "|"
            << node->event.time << "|"
            << node->event.type << "|"
            << node->event.location << "\n";
        saveNode(out, node->right);
    }

public:
    EventBST() : root(NULL) {}

    void addEvent(Event e) { root = insert(root, e); }

    void viewEvents()
    {
        if (!root)
        {
            cout << "No events scheduled.\n";
            return;
        }
        inorder(root);
    }

    void deleteEvent(string date, string time)
    {
        string k = makeKey(date, time);
        root = remove(root, k);
    }

    void searchEvent(string key, string field)
    {
        cout << "Search results for \"" << key << "\" in " << field << ":\n";
        searchByField(root, key, field);
    }

    void viewEventsByDate(string date)
    {
        cout << "Events on " << date << " (chronological order):\n";
        inorderByDate(root, date);
    }

    void viewTodaysEvents()
    {
        time_t t = time(nullptr);
        tm *now = localtime(&t);
        char buf[11];
        sprintf(buf, "%02d-%02d-%04d", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
        string today(buf);
        cout << "Today's Events (" << today << "):\n";
        inorderByDate(root, today);
    }

    void editEvent(string date, string time)
    {
        string k = makeKey(date, time);
        Node *node = search(root, k);
        if (!node)
        {
            cout << "Event not found.\n";
            return;
        }

        cout << "Editing Event: " << node->event.name << " on " << node->event.date << " " << node->event.time << endl;

        cout << "Enter new name (leave empty to keep current): ";
        string newName;
        cin.ignore();
        getline(cin, newName);
        if (!newName.empty())
            node->event.name = newName;

        cout << "Enter new date (DD-MM-YYYY, leave empty to keep current): ";
        string newDate;
        getline(cin, newDate);
        if (!newDate.empty())
            node->event.date = newDate;

        cout << "Enter new time (HH:MM, leave empty to keep current): ";
        string newTime;
        getline(cin, newTime);
        if (!newTime.empty())
            node->event.time = newTime;

        cout << "Enter new type (leave empty to keep current): ";
        string newType;
        getline(cin, newType);
        if (!newType.empty())
            node->event.type = newType;

        cout << "Enter new location (leave empty to keep current): ";
        string newLocation;
        getline(cin, newLocation);
        if (!newLocation.empty())
            node->event.location = newLocation;

        cout << "Event updated successfully!\n";
    }

     void saveToFile()
    {
        ofstream out("events.txt");
        saveNode(out, root);
        out.close();
    }

    void loadFromFile(int &idCounter)
    {
        ifstream in("events.txt");
        if (!in.is_open()) return;
        string line;
        while (getline(in, line))
        {
            stringstream ss(line);
            string idStr, name, date, time, type, location;
            getline(ss, idStr, '|'); getline(ss, name, '|'); getline(ss, date, '|');
            getline(ss, time, '|'); getline(ss, type, '|'); getline(ss, location, '|');
            Event e;
            e.id = stoi(idStr);
            e.name = name;
            e.date = date;
            e.time = time;
            e.type = type;
            e.location = location;
            addEvent(e);
            if (e.id >= idCounter) idCounter = e.id + 1;
        }
        in.close();
    }
};

void adminrMenu()
{
    cout << "1. View All Events\n";
    cout << "2. Search Event\n";
    cout << "3. Day View (Specific Date)\n";
    cout << "4. Today's Events\n";
    cout << "5. Add Event\n";
    cout << "6. Edit Event\n";
    cout << "7. Delete Event\n";
    cout << "0. Exit\n";
    cout << endl;
    cout << "Enter choice: ";
}
void usermenu()
{
    cout << "1. View All Events\n";
    cout << "2. Search Event\n";
    cout << "3. Day View (Specific Date)\n";
    cout << "4. Today's Events\n";
    cout << "0. Exit\n";
    cout << endl;
    cout << "Enter choice: ";
}

int main()
{
    EventBST bst;
    int choice, log;
    int idCounter = 1;
    bst.loadFromFile(idCounter);

    cout << "Welcome to smart event manager pls login to proceed" << endl;
    cout << "1. login as Admin" << endl;
    cout << "2. Login as user" << endl;
    bool isAdmin;


cin >> log;
cin.ignore(); 

if (log == '1')
{
    string username, password;
    adminacc admin;
    bool isAdmin = false;


    cout << "Enter Admin Username: ";
    getline(cin, username);  
    cout << "Enter Admin Password: ";
    getline(cin, password);  

    if (username == admin.adm && password == admin.pass)
    {
        cout << "Login successful! Welcome Admin.\n";
        isAdmin = true;
    }
    else
    {
        cout << "Invalid username or password. Exiting...\n";
        return 0;
    }
}

    // Preloaded 10 events on the same date
    string sameDate = "17-08-2025";
    vector<string> times = {"16:00", "12:15", "11:00", "10:30", "13:00", "14:30", "15:45", "09:00", "17:15", "18:30"};
    vector<string> names = {"Meeting", "Workshop", "Call", "Lunch", "Presentation", "Discussion", "Brainstorm", "Training", "Review", "Party"};
    vector<string> types = {"Work", "Work", "Work", "Personal", "Work", "Work", "Work", "Work", "Work", "Personal"};
    vector<string> locations = {"Room A", "Room B", "Zoom", "Cafeteria", "Hall", "Room C", "Room D", "Zoom", "Hall", "Home"};

    for (int i = 0; i < 10; i++)
    {
        Event e;
        e.id = idCounter++;
        e.name = names[i];
        e.date = sameDate;
        e.time = times[i];
        e.type = types[i];
        e.location = locations[i];
        bst.addEvent(e);
    }

    while (true)
    {
        if (isAdmin)
            adminrMenu();
        else
            usermenu();

        cin >> choice;
        cin.ignore();

        if (choice == 0)
        {
            cout << "Exiting...\n";
            break;
        }

        if (choice == 1)
            bst.viewEvents();
        else if (choice == 2)
        {
            cout << "Enter 1 for searching by name\n";
            cout << "Enter 2 for searching by type\n";
            int par;
            cin >> par;
            cin.ignore();
            string keyword;
            if (par == 1)
            {
                cout << "Enter name keyword: ";
                getline(cin, keyword);
                bst.searchEvent(keyword, "name");
            }
            else
            {
                cout << "Enter type keyword: ";
                getline(cin, keyword);
                bst.searchEvent(keyword, "type");
            }
        }
        else if (choice == 3)
        {
            string date;
            cout << "Enter date (DD-MM-YYYY): ";
            cin >> date;
            bst.viewEventsByDate(date);
        }
        else if (choice == 4)
            bst.viewTodaysEvents();

        // Admin-only features
        else if (isAdmin)
        {
            if (choice == 5)
            {
                Event e;
                e.id = idCounter++;
                cout << "Enter Event Name: ";
                getline(cin, e.name);
                cout << "Enter Date (DD-MM-YYYY): ";
                cin >> e.date;
                cout << "Enter Time (HH:MM): ";
                cin >> e.time;
                cin.ignore();
                cout << "Enter Type: ";
                getline(cin, e.type);
                cout << "Enter Location: ";
                getline(cin, e.location);
                bst.addEvent(e);
                cout << "Event Added!\n";
                bst.saveToFile();
            }
            else if (choice == 6)
            {
                string date, time;
                cout << "Enter date of the event to edit (DD-MM-YYYY): ";
                cin >> date;
                cout << "Enter time of the event to edit (HH:MM): ";
                cin >> time;
                bst.editEvent(date, time);
                bst.saveToFile();
            }
            else if (choice == 7)
            {
                string date, time;
                cout << "Enter Date (DD-MM-YYYY): ";
                cin >> date;
                cout << "Enter Time (HH:MM): ";
                cin >> time;
                bst.deleteEvent(date, time);
                cout << "Event Deleted (if existed).\n";
                bst.saveToFile();
            }
            else
                cout << "Invalid choice, try again.\n";
        }
        else
        {
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}
