/*
	Exception set up for the  CollatzConjecture object
	
	Sets up a generic exception for the CollatzConjecture
	
	Author: Greg Stewart
	Copyright: 2014
	
	Start date: 8/10/14
*/
#ifndef COLLATZCONJEXCEPTION_H
#define COLLATZCONJEXCEPTION_H
//includes
#include <stdexcept>//for runtime_error

//usings
using namespace std;

//Thrown when an invalid int is passed to a constructor
class CollatzConjException : public runtime_error {
	public:
	CollatzConjException() : runtime_error("An invalid integer was passed. Not Greater than or equal to zero."){
	}
};//class CollatzConjException
#endif
