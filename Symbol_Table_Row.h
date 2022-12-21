

#ifndef SYMBOL_TABLE_ROW_H
#define SYMBOL_TABLE_ROW_H

using namespace std;


class Symbol_Table_Row
{
	private:
        /* Data Fields: */
        bool isInit;         // Has the variable (identifier) been initialized before use?
        long long intValue;  // The value that corresponds to an identifier or literal

	public:
        /* Constructor */
        Symbol_Table_Row (bool isInitialized = false, long long intValue = 0)
        {
            this -> isInit = isInitialized; 
            this -> intValue = intValue;
        }
		

        bool isInitialized (void) { return isInit; }
        

        long long getValue (void) { return intValue; }
		

        void initialize (void) { isInit = true; }
        

        void setValue (long long intValue) { this -> intValue = intValue; }
}; 

#endif
