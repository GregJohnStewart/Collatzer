/*  
	Header file for the CollatzConjecture series object
	
	Author: Greg Stewart
	Started: 4/16/14
*/
#ifndef COLLATZCONJECTURE_H
#define COLLATZCONJECTURE_H
//includes
#include "CollatzConjException.h"//exceptions for the object

#include <sstream>//string stream
#include <iostream>//for outputs
#include <iomanip>//manipulate outputs

using namespace std;

class CollatzConjecture{
	/*
		Private variables and methods
	*/
	private:
		//instance variables
		unsigned long long int startNum,highNum,numInSet;
		
		//actual recursive function to do the math
		void doCollatz(unsigned long long int);
		
		//to set up for the doCollatz Function
		void newConjecture();
		
		//to format outputs with commas 
		string formatWithCommas(unsigned long long int);
		
	public:
		/*
			Constructors
		*/
		//empty constructor, inistalizes everything to zero
		CollatzConjecture();
		
		//take in an unsigned long long int
		CollatzConjecture(unsigned long long int);
		
		//take in an int
		CollatzConjecture(int);
		
		//take in another CollatzConjecture as a pointer
		CollatzConjecture(CollatzConjecture&);
	
		/*
			Setters
		*/
		//sets a new start number, re-evaluates series	
		void setNewStart(unsigned long long int);
		void setNewStart(int);
		
		/*
			Getters
		*/
		//gets the start number
		unsigned long long int getStart();
		
		//gets the highest number in the series
		unsigned long long int getHigh();
		
		//gets the number of steps in the series
		unsigned long long int getNumInSet();
		
		/*prints all information about the series, format based on input
		  Options:
		  op1:
		  'l':for lists
			op2 choices:
			-'h':the heading for the conjecture list; same for all conjectures
			-'i':the item on the list, the individual conjecture's info
		  'b':for basic output
			op2 choices:
			-'t': for a tabbed over print
			-nothing for default print
		*/
		  void print();//for standard print
		  void print(char,char);//for specific formatting
		//returns a string form of the same formats as the print(char,char)
		// method
		string toString(char,char);
		string toString();
		
		/*
			Other
		*/
		
		/*
			operators
		*/
		//for telling if two conjectures are equal or not
		bool operator==(CollatzConjecture&);
		bool operator!=(CollatzConjecture&);
	
};//class CollatzConjecture
#endif
