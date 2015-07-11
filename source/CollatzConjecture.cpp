/*
	Functions for the CollatzConjecture 
	
	Author: Greg Stewart
	Start:  4/16/14
*/

//includes
#include "CollatzConjecture.h"//header file for the object
//all other includes in header file ^


//usings
using namespace std;

////////////////////////////
////	Public methods	////
////////////////////////////	
/*
	Constructors
*/
CollatzConjecture::CollatzConjecture(){
	unsigned long long int x = 0;
	setNewStart(x);
}

CollatzConjecture::CollatzConjecture(unsigned long long int x){
	setNewStart(x);
}

CollatzConjecture::CollatzConjecture(int x){
	if(x < 0){//throw exception if not valid int
		throw CollatzConjException();
	}
	setNewStart(x);
}

CollatzConjecture::CollatzConjecture(CollatzConjecture& y){
	setNewStart(y.getStart());
}

/*
	Setters
*/

//sets a new start number, re-evaluates series	
void CollatzConjecture::setNewStart(unsigned long long int x){
	startNum = x;
	if(x != 0){//if x is not zero, so it does not infinitely loop
		newConjecture();	
	}else{
		highNum = 0;
		numInSet= 0;
	}
}//setNewStart(unsigned long long int)
void CollatzConjecture::setNewStart(int x){
	if(x < 0){//so it is a valid number
		throw CollatzConjException();
	}
	setNewStart((unsigned long long int)x);
}//setNewStart(int)
	
/*
	Getters
*/

//gets the start number
unsigned long long int CollatzConjecture::getStart(){
	return startNum;
}

//gets the high number
unsigned long long int CollatzConjecture::getHigh(){
	return highNum;
}

//gets the number of numbers in the series
unsigned long long int CollatzConjecture::getNumInSet(){
	return numInSet;
}

/*
print(char,char)
prints all information about the series, format based on input
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
void CollatzConjecture::print(char op1,char op2){
	switch(op1){
		case 'l':{//'list', for outputting lists of conjectures
			switch(op2){
				case 'h':{//'header', prints header for the list
					cout<<left<<setw(5)<<"n"<<setw(15)<<"Number In Set"<<
			    		    setw(18)<<"Highest # in Set"<<endl;
					break;
				}case 'i':{//'item', prints this conjecture's info in listing form
					cout<<left<<setw(10)<<formatWithCommas(getStart())<<
			    		    setw(18)<<formatWithCommas(getHigh())<<setw(15)<<
			    		    formatWithCommas(getNumInSet())<<endl;
			    		break;
				}
			}//inner switch
			break;
		}case 'b':{//'basic' output. simple, easy to read
			switch(op2){
				case 't':{//'tabbed' printing
					cout<<"\tStart: ------------------------ "<<formatWithCommas(getStart())<<std::endl;
					cout<<"\tNumber in series:               "<<formatWithCommas(getNumInSet())<<std::endl;
					cout<<"\tHighest number encountered: --- "<<formatWithCommas(getHigh())<<std::endl;
					break;
				}default:{
					cout<<"Start: ------------------------ "<<formatWithCommas(getStart())<<std::endl;
					cout<<"Number in series:               "<<formatWithCommas(getNumInSet())<<std::endl;
					cout<<"Highest number encountered: --- "<<formatWithCommas(getHigh())<<std::endl;
					break;
				}
			}//inner switch
			break;
		}default:{//if some other options were entered
			print('b',' ');
			break;
		}//default case
	}//switch
}//print(char,char)
//simply calls the other print() method with the default option
void CollatzConjecture::print(){
	print('b',' ');
}
/*
	the toString() method
	
	Similar to the idea of the print() method above, except instead of 
		printing the stuff, it returns it as a string. Uses the sstream
		method of stream manipulation	
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
string CollatzConjecture::toString(char op1,char op2){
	stringstream output;//create stringstream to add onto
	switch(op1){
		case 'l':{//'list', for outputting lists of conjectures
			switch(op2){
				case 'h':{//'header', prints header for the list
					output<<left<<setw(5)<<"n"<<setw(15)<<"Number In Set"<<
			    		    setw(18)<<"Highest # in Set"<<endl;
					break;
				}case 'i':{//'item', prints this conjecture's info in listing form
					output<<left<<setw(10)<<formatWithCommas(getStart())<<
			    		    setw(18)<<formatWithCommas(getHigh())<<setw(15)<<
			    		    formatWithCommas(getNumInSet())<<endl;
			    		break;
				}
			}//inner switch
			break;
		}case 'b':{//'basic' output. simple, easy to read
			switch(op2){
				case 't':{//'tabbed' printing
					output<<"\tStart: ------------------------ "<<formatWithCommas(getStart())<<std::endl;
					output<<"\tNumber in series:               "<<formatWithCommas(getNumInSet())<<std::endl;
					output<<"\tHighest number encountered: --- "<<formatWithCommas(getHigh())<<std::endl;
					break;
				}default:{
					output<<"Start: ------------------------ "<<formatWithCommas(getStart())<<std::endl;
					output<<"Number in series:               "<<formatWithCommas(getNumInSet())<<std::endl;
					output<<"Highest number encountered: --- "<<formatWithCommas(getHigh())<<std::endl;
					break;
				}
			}//inner switch
			break;
		}default:{//if some other options were entered
			print('b',' ');
			break;
		}//default case
	}//switch
	return output.str();
}
//simply returns a default option set for the more complex toString(char,char) method
string CollatzConjecture::toString(){
	return toString('b',' ');
}


/*
	Other
*/
	
////////////////////////////////
////	Private Methods     ////
////////////////////////////////

//recursive function to do the conjecture		
void CollatzConjecture::doCollatz(unsigned long long int num){
	if(num != 1){//if the number isn't 1 (where we want to go)
		//move count up one, test for highest number
		numInSet++;
		if(num > highNum){
			highNum = num;
		}
		//decide on what do do based on number
		if(num%2 != 0){//if it isnt divisible by two
			num = (num * 3) + 1;
			doCollatz(num);
		}else{//else
			num = num / 2;
			doCollatz(num);
		}
	}//if != 1
}//doCollatz

//function to set up the new conjecture and make it
void CollatzConjecture::newConjecture(){
	//reset initial numbers
	highNum = startNum;
	numInSet=1;
	unsigned long long int num = startNum;
	doCollatz(num);
}//newConjecture

//so we can format appropriately for larger numbers
string CollatzConjecture::formatWithCommas(unsigned long long int value){
    stringstream ss;
    ss.imbue(locale(""));
    ss << fixed << value;
    return ss.str();
}

/*
	Operators
*/
bool CollatzConjecture::operator==(CollatzConjecture &input){
	if(getStart() == input.getStart()){
		return true;
	}else{
		return false;
	}
}// == operator
bool CollatzConjecture::operator!=(CollatzConjecture &input){
	if(getStart() != input.getStart()){
		return true;
	}else{
		return false;
	}
}// != operator
