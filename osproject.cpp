#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
string get_address_from_PT(int);
string file_name;
char divide_symbol;
stringstream ss;
const string file_ext = ".txt";
class Page
{
public:
    int ind;
    string data;
    int last_used;
    static int clock;
    Page()
    {
        ind = -1;
        data = "";
    }
    Page(int i, string s)
    {
        cout << "Page created\n";
        ind = i;
        data = s;
    }
};

int get_digits(int number)
{
    int count = 0;
    while (number != 0)
    {
        number = number / 10;
        count++;
    }
    return count;
}

void display_page(string &addr, int pchoice)
{
    if (!pchoice)
        cout << "Page contains:\n";
    ifstream page;
    page.open(addr);
    string mystring = "";
    if (page.is_open())
    {
        while (getline(page, mystring))
        {
            cout << mystring << endl;
        }
    }
}
void get_page(int page_no, vector<pair<int, string>> &disk_mem, int m, map<int, int> &last_used, int pchoice)
{
    // checks if page no. is in disk mem
    int n = disk_mem.size();
    int x = -1;
    int y = INT_MAX;
    int mx = 0;
    for (int i = 0; i < n; i++)
    {
        if (last_used[disk_mem[i].first] > mx)
        {
            mx = last_used[disk_mem[i].first];
        }
        if (last_used[disk_mem[i].first] < y)
        {
            y = last_used[disk_mem[i].first];
            x = disk_mem[i].first;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (disk_mem[i].first == page_no)
        {
            if (!pchoice)
                cout << "\nPAGE HIT\n";
            display_page(disk_mem[i].second, pchoice);
            last_used[page_no] = mx + 1;
            return;
        }
    }
    if (!pchoice)
        cout << "\nPAGE MISS\n";
    string addr = get_address_from_PT(page_no);
    display_page(addr, pchoice);
    if (n < m)
    {
        disk_mem.push_back({page_no, addr});
        last_used[page_no] = mx + 1;
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            if (disk_mem[i].first == x)
            {
                disk_mem[i].first = page_no;
                disk_mem[i].second = addr;
                last_used.erase(x);
                mx += 1;
                last_used[page_no] += mx + 1;
                break;
            }
        }
    }
    if (!pchoice)
    {
        cout << "\nDisk now contains pages: ";
        for (auto i : last_used)
        {
            cout << i.first << " ";
        }
    }
    cout << endl;
}
void divide(string file_name, char divide_symbol)
{
    // takes file path , opens file , divides into multiple files using symbol
    ifstream myfile;
    int counter = 0;
    string file_ext = ".txt";

    myfile.open(file_name + file_ext);

    string mystring;
    string tmpstring = "";
    ofstream content;
    ofstream table;
    content.open(file_name + "_content" + file_ext);
    table.open(file_name + "_table" + file_ext);
    if (myfile.is_open())
    {
        while (getline(myfile, mystring))
        {
            if (mystring[0] == divide_symbol)
            {
                if (counter > 0)
                {
                    ofstream tempfile;
                    tempfile.open(file_name + "_" + to_string(counter) + file_ext);
                    tempfile << tmpstring;
                    tempfile.close();
                }
                tmpstring = mystring;
                counter++;
                content << to_string(counter) + " " << mystring + "\n";
                table << to_string(counter) + " " + file_name + "_" + to_string(counter) + file_ext << endl;
            }
            else
            {
                tmpstring += "\n" + mystring;
            }
            cout << mystring << endl;
        }
        ofstream tempfile;
        tempfile.open(file_name + "_" + to_string(counter) + file_ext);
        tempfile << tmpstring;
        tempfile.close();
        content.close();
        table.close();
    }
}

string get_address_from_PT(int page_num)
{
    // takes page no and returns the page file name

    string PT_name = file_name + "_table" + file_ext;
    ifstream table;
    table.open(PT_name);
    string tempstring;
    string temppgno;
    int tmp;
    if (table.is_open())
    {
        while (getline(table, tempstring))
        {
            temppgno = string(tempstring.begin(), tempstring.begin() + get_digits(page_num));
            if (temppgno.compare(to_string(page_num)) == 0)
            {
                return string(tempstring.begin() + get_digits(page_num) + 1, tempstring.end());
            }
        }
    }
    return "Not Found";
}
//  loads each page one by one and print everything
void merge(vector<pair<int, string>> &disk_mem, int m, map<int, int> &last_used)
{
    ifstream content;
    content.open("laptop_list_content.txt");
    string mystring;
    int c = 1;
    if (content.is_open())
    {
        while (getline(content, mystring))
        {
            // cout << mystring << endl;
            get_page(c, disk_mem, m, last_used, 1);
            c++;
        }
    }
}
int main()
{
    cout << "Enter File name to divide\n";
    getline(cin, file_name);
    cout << "Enter divide symbol\n";
    cin >> divide_symbol;
    cout << file_name << endl;
    divide(file_name, divide_symbol);

    cout << "Enter number of pages to be stored in primary memory: ";
    // no of frames in memory , usually we enter 3 for this
    vector<pair<int, string>> disk_mem;
    int m;
    cin >> m;
    // alternate to disk_mem
    vector<Page> prim_mem(m, Page());
    // time is the second int , page no. is the first int
    map<int, int> last_used;

    while (true)
    {
        cout << "Enter operation to perform:\n1.Get entire file\n2.Get a page\n3.Exit\n___";
        int c;
        cin >> c;
        if (c == 1)
        {
            merge(disk_mem, m, last_used);
        }
        else if (c == 2)
        {
            cout << "Content:\n";
            ifstream content;
            content.open("laptop_list_content.txt");
            string mystring;
            if (content.is_open())
            {
                while (getline(content, mystring))
                {
                    cout << mystring << endl;
                }
            }
            while (true)
            {
                cout << "\nEnter page no: \t*Enter 0 to exit*\n___";
                int p;
                cin >> p;
                if (p == 0)
                {
                    break;
                }
                get_page(p, disk_mem, m, last_used, 0);
            }
        }
        else if (c == 3)
        {
            break;
        }
        else
        {
            cout << "Wrong option\n";
        }
    }
    return 0;
}
