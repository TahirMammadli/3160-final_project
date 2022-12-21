
#ifndef PARSER_H
#define PARSER_H

#include <iostream>            
#include <map>
#include <string>              
#include "Semantics.h"       
#include "Symbol_Table_Row.h"  
#include "Tokenizer.h"        

using namespace std;

class Parser
{
private:
     /* Data Fields: */
     map<string, Symbol_Table_Row> *symbolTable;
     Token token_t;                 
     Tokenizer Token_Izer;                    
     Semantics Semetic_Izer;                 
     int ER;                                     
     int O_Parenthes;                      

public:
     /* Constructor */
     Parser(string fileName, map<string, Symbol_Table_Row> *symbolTable)
     {
          this->symbolTable = symbolTable;
          ER = 0;
          O_Parenthes = 0;
          token_t = Token('0', "");
          Token_Izer = Tokenizer(fileName, symbolTable);

          Semetic_Izer = Semantics(this->symbolTable, &Token_Izer, &ER);

          Token_Izer.next(token_t); // Retrieve the first token.

          while (token_t.getType() != '5')
               if (matchID() && matchAssign() && matchExp() && Semetic_Izer.process(token_t, 4))
               {
                    if (O_Parenthes > 0)
                         printSyntaxError();
                    Token_Izer.next(token_t); // Retrieve the next token.
               }
               else
                    skipAfterNextSemicolon();

          if (ER > 0)
               printErrorsFooter();
          else
               for (map<string, Symbol_Table_Row>::iterator it = (*symbolTable).begin();
                    it != (*symbolTable).end();
                    it++)
                    cout << it->first << " = " << (it->second).getValue() << endl;
          // printValuesOfVariables ();
     }

     void skipAfterNextSemicolon(void)
     {
          while (token_t.getType() != '5' && token_t.getName()[0] != ';')
               Token_Izer.next(token_t); // Retrieve the next token.
          Token_Izer.next(token_t);      // We need the token that is right after the semicolon.
          Semetic_Izer.initialize();     // Clear the information in the Semantic Analyzer.
          O_Parenthes = 0;
     }

     void printErrorsFooter(void)
     {
          cout << "Totall of: " << ER << " Errors."
               << endl
               << "--------------------------------------------------------" << endl;
     }

     // char message, char type = 's'
     void printSyntaxError()
     {

          ER++;
          cout << "Syntax Error on line " << Token_Izer.getCurrLine() << endl;

         
          cout << endl
               << "--------------------------------------------------------" << endl;
     }

     bool matchID(void)
     {
          if (token_t.getType() == '1')
          {
               Semetic_Izer.process(token_t, -1); // Call the Semantic Analyzer to process this.
               Token_Izer.next(token_t);
               return true;
          }
          else if (token_t.getType() == '3' && token_t.getName()[0] == ';')
          {
               printSyntaxError();
               return false;
          }
          else
          {
               printSyntaxError();
               return false;
          }
       
          Token_Izer.next(token_t); // Retrieve the next token.
          return false;
     }

     bool matchAssign(void)
     {
          if (token_t.getType() == '3' && token_t.getName()[0] == '=')
          {
               Token_Izer.next(token_t); // Retrieve the next token.
               return true;
          }
          else if (token_t.getType() == '3' && token_t.getName()[0] == ';')
          {
               printSyntaxError();
               return false;
          }
          else
          {
               printSyntaxError();
               return false;
          }
        
          
          Token_Izer.next(token_t); // Retrieve the next token.
          return false;
     }

     bool matchExp(void)
     {
          if (token_t.getType() == '5')
          {
               printSyntaxError();
               return false;
          }

          if (matchFact() && matchTermPrime() && matchExpPrime())
               return true;
          return false;
     }

     bool matchSemicolon(void)
     {
          Semetic_Izer.process(token_t, 4); // Call the Semantic Analyzer to process this.
          return true;
     }

   

     bool matchExpPrime(void)
     {
          switch (token_t.getType())
          {
          case '0':
               printSyntaxError();
               break;
          case '1':
          case '2':
               printSyntaxError();
               break;
          case '3':
               switch (token_t.getName()[0])
               {
               case ';':
               case '*':
                    return true;
               case '(':
                    printSyntaxError();
                    break;
               case ')':
                    if (O_Parenthes > 0)
                    {
                         O_Parenthes--;
                         Semetic_Izer.process(token_t, 4); // Call the Semantic Analyzer.
                         Token_Izer.next(token_t);         // Retrieve the next token.
                         return true;
                    }
                    else
                    {
                         printSyntaxError();
                         return false;
                    }
               case '+':
               case '-':
                    Semetic_Izer.process(token_t, 3); // Call the Semantic Analyzer.
                    Token_Izer.next(token_t);         // Retrieve the next token.
                    if (matchExp())
                         return true;
                    return false;
               case '=':
                    printSyntaxError();
                    // No default case since we covered all possible cases for type 3.
               }
               break;
          case '5':
               printSyntaxError();
          }
          return false;
     }

     bool matchFact(void)
     {
          switch (token_t.getType())
          {
          case '1':
          case '2':
               Semetic_Izer.process(token_t, -1); // Call the Semantic Analyzer to process this.
               Token_Izer.next(token_t);          // Retrieve the next token.
               return true;
          case '0':
               printSyntaxError();
               break;
          case '3':
               switch (token_t.getName()[0])
               {
               case ';':
                    printSyntaxError();
                    break;
               case ')':
                    printSyntaxError();
                    break;
               case '(':
                    O_Parenthes++;
                    Semetic_Izer.process(token_t, 0); // Call the Semantic Analyzer.
                    Token_Izer.next(token_t);         // Retrieve the next token.
                    if (matchExp())
                         return true;
                    return false;
               case '+':
               case '-':
                    Semetic_Izer.process(token_t, 1); // Call the Semantic Analyzer.
                    Token_Izer.next(token_t);         // Retrieve the next token.
                    if (matchFact())
                         return true;
                    return false;
               case '=':
                    printSyntaxError();
                    break;
               case '*':
                    printSyntaxError();
                    // No default case since we covered all possible cases for type 3.
               }
               break;
          case '5':
               printSyntaxError();
          }
          return false;
     }

     bool matchTermPrime(void)
     {
          switch (token_t.getType())
          {
          case '0':
               printSyntaxError();
               break;
          case '1':
          case '2':
               printSyntaxError();
               break;
          case '3':
               switch (token_t.getName()[0])
               {
               case '*':
                    Semetic_Izer.process(token_t, 2); // Call the Semantic Analyzer.
                    Token_Izer.next(token_t);         // Retrieve the next token.
                    if (matchFact() && matchTermPrime())
                         return true;
                    return false;
               case ')':
                    if (O_Parenthes > 0)
                    {
                         O_Parenthes--;
                         Semetic_Izer.process(token_t, 4); // Call the Semantic Analyzer.
                         Token_Izer.next(token_t);         // Retrieve the next token.
                         return true;
                    }
                    else
                    {
                         printSyntaxError();
                         return false;
                    }
               case '+':
               case '-':
               case ';':
                    return true;
               case '(':
                    printSyntaxError();
                    break;
               case '=':
                    printSyntaxError();
                    // No default case since we covered all possible cases for type 3.
               }
               break;
          case '5':
               printSyntaxError();
          }
          return false;
     }

   
};

#endif
