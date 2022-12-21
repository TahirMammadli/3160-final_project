

#include <ctime>              
#include <iostream>           
#include <map>       
#include <string>
#include "Parser.h"          
#include "Symbol_Table_Row.h"

using namespace std;



int main (int argc, char *argv[])
{
     // A Symbol Table map:
     //     The name of an identifier (string) is the key, and a Symbol_Table_Row is a value.
     map <string, Symbol_Table_Row> symbolTable;
     // String holding the name of the file that the user wants to interpret:
     string fileName;
     // Two time types:
     time_t timer1, timer2;
     
 
 	 
     if (argc > 1)
     {  
         fileName = argv[1];
         cout << endl << "The entered filename is:" << endl;
     }
     else
     {
 ;
        fileName="test.txt";

	 }



      Parser (fileName, &symbolTable);
     

     return 0;	
}



