#include<bits/stdc++.h>
#include<fstream>
#include<string>
using namespace std;



int main() 
{
    ifstream myfile;
    int counter = 0;
     myfile.open("laptop_list.txt");
     string mystring;
     string tmpstring;

    if ( myfile.is_open() ) { 
        while ( getline (myfile,mystring) )
{
    if( mystring[0]=='#')
    {
        ofstream tempfile;
        tempfile.open(to_string(counter)+".txt");
        tempfile<<tmpstring;
        tempfile.close();
        tmpstring=mystring;
        counter++;
        
    }
    else
    {
        // tmpstring.copy(mystring);
        tmpstring +=mystring;
    }
//Printing the output of the file contents
cout << mystring << '\n';
}
}
ofstream tempfile;
        tempfile.open(to_string(counter)+".txt");
        tempfile<<tmpstring;
        tempfile.close();


    cout<<"hello world";
    return 0;
}