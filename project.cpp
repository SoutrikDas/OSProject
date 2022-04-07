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
        last_used = -1;
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
string check_table(int page_no)
{
    // resundant to get_address_from_PT
    string mystring;
    ifstream page_num;          // why are you doing this bro ?
    page_num.open("table.txt"); // page_no filename.txt
    string addr = "";
    if (page_num.is_open())
    {
        while (getline(page_num, mystring))
        {
            if (mystring[0] == page_no + '0') // int(3) + '0' = ascii(3)
            {
                for (int i = 1 + get_digits(page_no); i < mystring.size(); i++)
                {
                    addr += mystring[i];
                }
                cout << "  string name = " << mystring;
                break;
            }
        }
    }
    ifstream page;
    page.open(addr);
    if (page.is_open())
    {
        while (getline(page, mystring))
        {
            cout << mystring << endl;
        }
    }
    return addr;
}
void get_page(int page_no, vector<pair<int, string>> &disk_mem, int m, map<int, int> last_used)
{
    // checks if page no. is in disk mem
    int n = disk_mem.size();
    for (int i = 0; i < n; i++)
    {
        if (disk_mem[i].first == page_no)
        {
            cout << "HIT\n";
            cout << "Page contains: \n";
            ifstream page;
            page.open(disk_mem[i].second);
            string mystring;
            if (page.is_open())
            {
                while (getline(page, mystring))
                {
                    cout << mystring << endl;
                }
            }
            last_used[page_no]++;
            return;
        }
    }
    cout << "MISS\n";
    string addr = get_address_from_PT(page_no);
    int x = -1;
    int y = INT_MAX;
    int mx = INT_MIN;
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
    if (n < m)
    {
        disk_mem.push_back({page_no, addr});
        last_used[page_no] = mx + 1;
        cout << "!" << page_no << "!" << addr << "!!!!" << endl;
        return;
    }
    else
    {
        cout << "::\n";
        for (int i = 0; i < n; i++)
        {
            if (disk_mem[i].first == x)
            {
                disk_mem[i].first = page_no;
                disk_mem[i].second = addr;
                last_used.erase(x);
                mx += 1;
                cout << "!!!" << x << "!!" << y << "!!" << mx << endl;
                last_used[page_no] = mx + 1;
                break;
            }
        }
        return;
    }
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
        while (getline(myfile, mystring)) // fetch one line
        {
            if (mystring[0] == divide_symbol) // check if it starts with #
            {
                if (counter > 0)
                {
                    ofstream tempfile; // open a new file and usme store kardo 
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
            cout << "This is the extracted page no. bro" << temppgno << endl;
            ss << temppgno;
            ss >> tmp;
            if (temppgno.compare(to_string(page_num))==0)
            {
                cout << "Got the page no.\n";
                cout<<"------- FOR PN="<<page_num<<" Address="<<string(tempstring.begin()+1 + get_digits(page_num), tempstring.end())<<endl;
                return string(tempstring.begin()+1 + get_digits(page_num), tempstring.end()); // yaha 1 mat bhulna 
            }
        }
    }
    return "Page Fault, Page Adress Not Found";
}
string get_data_from_PN(int page_num)
{
    // directly hands over the data inside the page stored in hard disk
    ifstream tempstream;
    cout<<"Start of getdatafrom PN\n";
    string data = "", tmpstring = "";
    cout<<"Addres ="<<get_address_from_PT(page_num)<<endl;
    tempstream.open(get_address_from_PT(page_num));
    if (tempstream.is_open())
    {
        while (getline(tempstream, tmpstring)) // reading each line
        {
            cout<<"tempstring = "<<tmpstring<<endl;
            cout<<"Data = "<<data<<endl;
            data += tmpstring + "\n";
        }
    }
    cout<<" \n-----returned data = ";
    cout<<data<<endl;
    return data;
}
Page get_page_from_PN(int page_num)
{
    return Page(page_num, get_data_from_PN(page_num));
}
void merge()
{
    ifstream myfile;
    myfile.open("laptop_list.txt");
    string mystring;
    if (myfile.is_open())
    {
        while (getline(myfile, mystring))
        {
            cout << mystring << endl;
        }
    }
}
int check_page_in_PM(int page_num, vector<Page> &primary_mem)
{
    // returns index of required page_num inside Primary mem , -1 if not found
    for( int i = 0 ; i<primary_mem.size(); i++)
    {
        if (primary_mem[i].ind == page_num) // IS IT SAME AS OUR PAGE NO
        {
            return i;
        }
    }
    return -1;
}
int clck = 0;
bool isEmpty(vector<Page> &primary_mem)
{
    // checks if the Page vector is empty or not
    for (auto i : primary_mem)
    {
        if (i.ind != -1)
            return false;
    }
    return true;
}
int get_replace_index(vector<Page> &primary_mem)
{
    // returns the index of Least Recently used Page in Primary memory
    int min = primary_mem[0].last_used, i, temp = 0;
    int n = primary_mem.size();
    for (i = 1; i < n; i++)
    {
        if (min > primary_mem[i].last_used)
        {
            min = primary_mem[i].last_used;
            temp = i;
        }
    }
    cout<<" Replacable index = "<<temp<<endl;
    return temp;
    
}
int load_page(int page_num, vector<Page> &primary_mem)
{
    // har baar ek ek karke load karega ye banda
    int temp = get_replace_index(primary_mem); // LRU implement  
    primary_mem[temp] = get_page_from_PN(page_num);
    primary_mem[temp].last_used = clck;
    cout<<"Page loaded"<<primary_mem[temp].data<<" ind ="<<primary_mem[temp].ind<<endl;
    //also need to increase the last used variable ;

    return 0;
}
int main()
{
    cout << "Enter File name to divide\n";
    getline(cin, file_name); // take laptop_list
    cout << "Enter divide symbol\n";
    cin >> divide_symbol; // # 
    // cout << file_name;
    divide(file_name, divide_symbol); // divide the big file 

    cout << "Enter number of pages to be stored in primary memory: ";
    vector<pair<int, string>> disk_mem;
    int primary_mem_size;
    cin >> primary_mem_size;
    vector<Page> prim_mem(primary_mem_size, Page()); // alternate to disk_mem
    map<int, int> last_used;
    while (true)
    {
        clck++;
        cout << "Enter page no: ";
        int p;
        cin >> p;
        // get_page(p,disk_mem,primary_mem_size,last_used);
        int temp = check_page_in_PM(p, prim_mem);
        cout<<"Temp = checkpageinPM resposne"<<temp<<endl;
        cout << "Returned Page ind=" << prim_mem[temp].ind << " data=" << prim_mem[temp].data << endl;
        cout<<"---Data of page"<<p<<"= "<<get_data_from_PN(p)<<endl;
        // if (temp == -1)
        // {
        //     cout<<"Page not found in MM\n";
        //     load_page(p, prim_mem);
        // }
        // else
        // {
        //     cout<<"Page found bro !!!!\n";
        //     cout<<"Data = "<<prim_mem[temp].data<<endl;
        // }

        if (p == -1)
            return 0;
    }
    // while (true)
    // {
    //     cout << "Enter operation to perform:\n1.Get entire file\n2.Get a page\n3.Exit";
    //     int c;
    //     cin >> c;
    //     if (c == 1)
    //     {
    //         merge();
    //     }
    //     else if (c == 2)
    //     {
    //         get_page();
    //     }
    //     else
    //     {
    //         break;
    //     }
    // }
    return 0;
}
