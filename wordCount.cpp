#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main()
{
int count =0;
string words;
string word;
string filename;
cout<< "Enter file name to read from: " ;
cin >> filename;

ifstream myReadFile;
 myReadFile.open(filename.data()); 
 
if (myReadFile.fail()) {
     cout <<"\nFailed to open file "<<filename;
     return 1;
    }
cout<< "Enter a word to search: ";
cin>>word;
 if (myReadFile.is_open()) {
	
	while (!myReadFile.eof()) {	
    myReadFile >> words; 
    if(words==word)
	{
	count++;
	}
 }
}
 myReadFile.close();
 //cout<<words;
 int size = sizeof(words); 
 
 if (count==0)
 {
 cout<< "Word \""<< word << " \"is not found in the file";
 }
 else
 {
 cout<< "Word \"" << word << "\" count in the file is: "<< count;
 }
 }                


