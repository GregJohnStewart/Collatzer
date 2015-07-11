/*
	classes to manage and interact with the 'conjectures.dat' file

	also includes other worker classes
	
	Author: Greg Stewart
	Date started: 5/13/14
	
	V1.1

*/
#ifndef RAFCLASSES_H
#define RAFCLASSES_H
/*
	Includes
*/

//my classes and methods
#include "CollatzConjecture.h"// CollatzConjecture class definition


//c++ libraries
#include <iostream>//to do console output
#include <iomanip>//for stream manipulation
#include <cstdlib>
#include <fstream>//for file streams
#include <ostream>//for text streams?
#include <sstream>//string streams
#include <limits>//for user input stuff

//usings
using namespace std;


/*********************************
	classes to get inputs

*********************************/
/*
	To get an input (char) fron the user

	def = default character if failed
*/
char getCharFromUser(char def){
	char input = ' ';
	//get input (in the form of an int)
	try{//get the input
		cin >> input;
	}
	catch (char* error){
		input = def;
	}//try/catch
	cin.clear(); //clear stream
	cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignore leftover data
	//cout << "Input: " << choice;//debugging
	
	return input;
}//getCharFromUser(char def)
//function to call other one with a space as def
char getCharFromUser(){
	return getCharFromUser(' ');
}

/*
To get an integer input from the user

i = default if failed
*/

//defaults to i
int getIntFromUser(int i){
	int input;
	//get input (in the form of an int)
	try{//get the input
		cin >> input;
	}
	catch (int* error){
		input = i;
	}//try/catch
	cin.clear(); //clear stream
	cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignore leftover data
	//cout << "Input: " << choice;//debugging
	cout << "Returning: " <<input <<endl;
	return input;
}//getIntFromUser(int i)
//calls other function with i = 0
int getIntFromUser(){
	return getIntFromUser(0);
}

/*
To get an unsigned input fron the user

i = default if failed
*/
unsigned int getUIntFromUser(int i){
	int input;//input from the user
	unsigned int uInput;//what we will pass back after making sure validity
	//get input (in the form of an int)
	try{//get the input
		cin >> input;
	}
	catch (int* error){
		input = 0;
	}//try/catch
	cin.clear(); //clear stream
	cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignore leftover data
	//cout << "Input: " << choice;//debugging

	//get absolute value, to eliminate negatives
	uInput = abs(input);

	return uInput;
}//getUIntFromUser(int i)
//calls other function with i = 0
int getUIntFromUser(){
	return getUIntFromUser(0);
}


//class to format ints to ints with commas for readability
string FormatWithCommas(unsigned long long int value){
    stringstream ss;
    ss.imbue(locale(""));
    ss << fixed << value;
    return ss.str();
}
/*
	Takes in a number and separates it into appropriate TB,GB,MB,KB data sets
*/
string figureSpace(unsigned long long int number,int sizeOfObj){
	//data necessary
	unsigned long long int result = sizeOfObj * number;
	unsigned long long int terabytes = 0;
	unsigned long long int gigabytes = 0;
	unsigned long long int megabytes = 0;
	unsigned long long int kilobytes = 0;
	unsigned long long int bytes = 0;
	stringstream sstm;//output string
	
	//calculate how many of each and take that number away from result
	if((result >= 1000000000000)){
		terabytes = result / 1000000000000;
		result -= terabytes * 1000000000000;
	}
	if((result >= 1000000000)){
		gigabytes = result / 1000000000;
		result -= gigabytes * 1000000000;
	}
	if((result >= 1000000)){
		megabytes = result / 1000000;
		result -= megabytes * 1000000;
	}
	if((result >= 1000)){
		kilobytes = result / 1000;
		result -= kilobytes * 1000;
	}
	//put what is left into bytes
	bytes = result;
	
	/*
	//debugging
	cout<<"\tNumbers:"<<endl<<
	      "\t\tTerabytes: "<<terabytes<<endl<<
	      "\t\tGigabytes: "<<gigabytes<<endl<<
	      "\t\tMegabytes: "<<megabytes<<endl<<
	      "\t\tKilobytes: "<<kilobytes<<endl<<
	      "\t\tbytes: "<<bytes<<endl;
	*/
	//output appropriately
	//cout<<"\tSize before: "<<sizeCalculated.c_str()<<endl;
	if(terabytes > 0){
		sstm << FormatWithCommas(terabytes) << "tb ";
	}
	//cout<<"\tSize before (1): "<<sizeCalculated.c_str()<<endl;
	if(gigabytes > 0){
		sstm << gigabytes << "gb ";
	}
	//cout<<"\tSize before (2): "<<sizeCalculated.c_str()<<endl;
	if(megabytes > 0){
		sstm << megabytes << "mb ";
	}
	//cout<<"\tSize before (3): "<<sizeCalculated.c_str()<<endl;
	if(kilobytes > 0){
		sstm << kilobytes << "kb ";
	}
	//cout<<"\tSize before (4): "<<sizeCalculated.c_str()<<endl;
	if(bytes > 0){
		sstm << bytes << "bytes ";
	}
	//cout<<"\tSize before (5): "<<sizeCalculated.c_str()<<endl;
	if(sstm.str() == ""){
		sstm << "0bytes";
	}
	return sstm.str();
}//figureSpace

/*
	Takes in a double value, the time taken, and splits it into hours, minutes, and seconds
*/
string figureTime(double elapsed_time){
	stringstream sstm;
	
	//take out the hours
	int elapsed_hrs = elapsed_time/3600;
	if(elapsed_hrs > 0){
		elapsed_time -= elapsed_hrs * 3600;
	}
	
	//take out the minutes
	int elapsed_min = elapsed_time/60;
	if(elapsed_min > 0){
		elapsed_time -= elapsed_min * 60;
	}
	
	//what is left over put into seconds
	int elapsed_sec = elapsed_time;
	
	//output hours, minutes, and seconds where applicable
	if(elapsed_hrs > 0){
		sstm << FormatWithCommas(elapsed_hrs)<<"h ";
	}else if(elapsed_min > 0){
		sstm << elapsed_min <<"m ";
	}else{
		sstm << elapsed_time <<"s";
	}
	
	return sstm.str();
}//figureTime
/*
	returns the conjecture with the highest individual number in its set, 
		in a range of conjectures
*/
int getHighNumOfConj(fstream &readFromFile,int x,int y){
	//instance conjectures
	CollatzConjecture temp;//conjecture gotten from file
	CollatzConjecture test;//conjecture to test against
	
	//to prevent -1 from happening when navigating the file
	if(x == 1){
		x = 2;
	}
	
	//calculate number to generate
	int numToGen = (y - x) + 2;
	
	//navigate to beginning of where we are starting
	readFromFile.seekg((x - 2) * sizeof(CollatzConjecture));
	
	//read the first one off into temp
	readFromFile.read(reinterpret_cast<char *>(&temp),sizeof(CollatzConjecture));
	
	//make test = temp
	test = CollatzConjecture(temp);
	
	//counting number
	int i = 1;
	//actual loop to go through file
	do{
		//cout<<i<<endl;//debugging
		
		//read single record into test
		readFromFile.read(reinterpret_cast<char *>(&test),sizeof(CollatzConjecture));
		//test.print();//debugging
		//cout<<endl;//debugging
		
		//test to see if the new conjecture's highest number is larger
		if(test.getHigh() > temp.getHigh()){
			temp = CollatzConjecture(test);
			//cout<<"High number now: "<<temp.getHigh()<<endl;//debugging
		}
		//move i up
		i++;
	}while(!readFromFile.eof() && (i < numToGen));//while not end of file
	readFromFile.clear();//clear readFromFile to ensure nothing gets mixed up in the file stream
	//temp.print('t');//debugging
	return temp.getStart();//return temp's start
}

/*
	returns conjecture with largest set
*/
int getHighNumInSet(fstream &readFromFile,int x,int y){
	//cout<<"getHighNumInSet() called."<<endl;//debugging
	
	//instance conjectures
	CollatzConjecture temp;
	CollatzConjecture test;
	
	//to prevent -1 from happening when navigating the file
	if(x == 1){
		x = 2;
	}
	
	//calculate number to generate
	int numToGen = (y - x) + 2;
	
	//navigate to beginning of where we are starting
	readFromFile.seekg((x - 2) * sizeof(CollatzConjecture));
	
	//read the first one off into temp
	readFromFile.read(reinterpret_cast<char *>(&temp),sizeof(CollatzConjecture));
	
	//make test = temp
	test = CollatzConjecture(temp);
	
	//counter
	int i = 1;
	while(!readFromFile.eof() & i < numToGen){
		//cout<<i<<endl;//debugging
		
		//read single record into test
		readFromFile.read(reinterpret_cast<char *>(&test),sizeof(CollatzConjecture));
		
		//test.print();//debugging
		//cout<<endl;//debugging
		
		//test to see if the new conjecture's set is larger
		if(test.getNumInSet() > temp.getNumInSet()){
			temp = CollatzConjecture(test);
			//cout<<"Highest number in set now: "<<temp.getNumInSet()<<endl;//debugging
		}
		//move i up
		i++;
	}//while not end of file
	readFromFile.clear();//clear readFromFile to ensure nothing gets mixed up in the file stream
	//temp.print('t');//debugging
	return temp.getStart();//return start of temp
}//getHighNumInSet()
/*
	Function to search the data file to find how many conjectures there are
		with x steps
	takes in the number of steps and the filestream
	
	returns the number of conjectures
*/
unsigned long long int findNumWithHighNum(unsigned long long int highNum,fstream &readFromFile,unsigned long long int lowBounds,unsigned long long int highBounds){
	//instance variables
	unsigned long long int count = 0;//number of stepNum encountered
	unsigned long long int place = lowBounds;//counting place
	CollatzConjecture temp;//temporary holder of current conjecture being messed with
	//set reader to beginning of file and get the first entry
	readFromFile.seekg((lowBounds - 1) * sizeof(CollatzConjecture));
	readFromFile.read(reinterpret_cast<char *>(&temp),sizeof(CollatzConjecture));
	//loop to go through file to the end
	do{
		//test to see if we need to increment count
		if(temp.getHigh() == highNum){
			count++;
		}
		//move up the one we are on
		place++;
		readFromFile.read(reinterpret_cast<char *>(&temp),sizeof(CollatzConjecture));
	}while(place <= highBounds);
	
	readFromFile.clear();//clear readFromFile to ensure nothing gets mixed up in the file stream
	return count;
}//findNumWithHighNum

/*
	Function to search the data file to find how many conjectures there are
		with x steps
	takes in the number of steps and the filestream
	
	returns the number of conjectures
*/
unsigned long long int findNumWithXStep(unsigned long long int stepNum,fstream &readFromFile,unsigned long long int lowBounds,unsigned long long int highBounds){
	//instance variables
	unsigned long long int count = 0;//number of stepNum encountered
	unsigned long long int place = lowBounds;//counting place
	CollatzConjecture temp;//temporary holder of current conjecture being messed with
	//set reader to beginning of file and get the first entry
	readFromFile.seekg((lowBounds - 1) * sizeof(CollatzConjecture));
	readFromFile.read(reinterpret_cast<char *>(&temp),sizeof(CollatzConjecture));
	//loop to go through file to the end
	do{
		//test to see if we need to increment count
		if(temp.getNumInSet() == stepNum){
			count++;
		}
		//move up the one we are on
		place++;
		readFromFile.read(reinterpret_cast<char *>(&temp),sizeof(CollatzConjecture));
	}while(place <= highBounds);
	
	readFromFile.clear();//clear readFromFile to ensure nothing gets mixed up in the file stream
	return count;
}//findNumWithXStep
/*
	to create a text file listing all the conjectures
*/
bool createTextFile(fstream &readFromFile,int numGenerated){
	
	//create text file to output to
	ostringstream convert;
	//create custom filename based on what is being generated
	convert << numGenerated; 
	string fileName ="outputs/conjectures_to_"+ convert.str()+".txt";
	//make the file
	ofstream outPrintFile(fileName.c_str(), ios::out);
	
	//if the file couldnt be made/opened
	if ( !outPrintFile ){
		return false;
	}else{
		cout<<"File being saved as: \""<<fileName<<"\"."<<endl;
	}
	
	//set up headers, for beginning of text file list
	cout<<"Finding highest number of all the sets..."<<endl;
	CollatzConjecture highestNumberInSet;
	highestNumberInSet = CollatzConjecture(getHighNumOfConj(readFromFile,1,numGenerated));
	cout<<" Done."<<endl;
	
	cout<<"Finging number of conjectures with this number..."<<endl;
	unsigned long long int numWithHigh = findNumWithHighNum(highestNumberInSet.getHigh(),readFromFile,1,numGenerated);
	cout<<" Done."<<endl;
	
	cout<<"Finding the conjecture with the biggest set..."<<endl;
	CollatzConjecture biggestSet;
	biggestSet = CollatzConjecture(getHighNumInSet(readFromFile,1,numGenerated));
	cout<<" Done."<<endl;
	
	cout<<"Finding the number of conjectures with this sized set..."<<endl;
	unsigned long long int numWithHighStep = findNumWithXStep(biggestSet.getNumInSet(),readFromFile,1,numGenerated);
	cout<<" Done."<<endl;
	
	//output headers to file
	outPrintFile<<"Collatz Conjectures 1 - "<<FormatWithCommas(numGenerated)<<endl<<
				  "Generated with Collatzer by Greg Stewart"<<endl<<endl;
				  
	outPrintFile<<"Conjecture with largest number encountered in its set:"<<endl
				<<left<<setw(29)<<"Start:"<<setw(18)<<FormatWithCommas(highestNumberInSet.getStart())<<endl
				<<setw(29)<<"Number In set:"<<setw(18)<<FormatWithCommas(highestNumberInSet.getNumInSet())<<endl
				<<setw(29)<<"Highest Number Encountered:"<<FormatWithCommas(highestNumberInSet.getHigh())<<endl<<endl; 
	outPrintFile<<"Number of times the largest number encountered appears: "<<FormatWithCommas(numWithHigh)<<endl<<endl;
				
	outPrintFile<<"Conjecture with the largest set:"<<endl
				<<left<<setw(29)<<"Start:"<<setw(18)<<FormatWithCommas(biggestSet.getStart())<<endl
				<<setw(29)<<"Number In set:"<<setw(18)<<FormatWithCommas(biggestSet.getNumInSet())<<endl
				<<setw(29)<<"Highest Number Encountered:"<<FormatWithCommas(biggestSet.getHigh())<<endl<<endl;
	outPrintFile<<"Number of sets that have this number in them: "<<FormatWithCommas(numWithHighStep)<<endl<<endl;
	
	outPrintFile<<left<<setw(18)<<"Starting Num"<<setw(18)<<"Highest # in Set"<<setw(18)<<"# Of Steps"<<endl;
	
	cout<<"Now writing conjectures out to file..."<<endl;
	//set file-position pointer to beginning of readFromFile
	readFromFile.seekg(0);
	
	//read first record from data file
	CollatzConjecture conj;
	readFromFile.read( reinterpret_cast<char *>(&conj),sizeof(CollatzConjecture));
	//conj.toString(0);//debugging
	
	//print out frame for loading bar
	cout<<"Progress:"<<endl<<
		"|0%          25%        50%        75%       100%|"<<endl;
	
	//do math for loading bar
	int updateFreq = numGenerated/50;
	int count = 0;
	
	//copy all records from record file into text file
	while( !readFromFile.eof()){
		//write single record to text file
		if(conj.getStart() != 0){//skip possible bad one (can't start at zero)
			
			//output to file
			outPrintFile<<left<<setw(18)<<FormatWithCommas(conj.getStart())<<setw(18)<<FormatWithCommas(conj.getHigh())
						<<setw(18)<<FormatWithCommas(conj.getNumInSet())<<endl;			
			//move count up
			count++;
			//see if we need to add to the progress bar
			if(count >= updateFreq){
				cout<<"#"<<flush;
				count = 0;
			}
			//conj.toString(1);//debugging
		}
		
		//read next record from file
		readFromFile.read(reinterpret_cast<char *>(&conj),sizeof(CollatzConjecture));
		
	}//while not end of file
	cout<<" Done."<<endl;
	readFromFile.clear();//clear readFromFile to ensure nothing gets mixed up in the file stream
	return true;
}//createTextFile

/*
	To create a textfile containing only the conjectures within a range
*/
bool createTextFileRange(fstream &readFromFile,int x,int y){
	
	//make sure correct inputs
	if(x == 0){
		x = 1;
	}
	if(x > y){
		cout<<"X is Greater than Y"<<endl;
		return false;
	}
	//setup text file
	ostringstream convert;
	//make custom string for file name
	convert << x; 
	string fileName ="outputs/conjectures_"+ convert.str();
	convert.str("");
	convert << y;
	fileName += "_to_"+ convert.str() +".txt";
	//create actual text file and open it
	ofstream outPrintFile(fileName.c_str(), ios::out);
	cout<<"File being saved as \""<<fileName<<"\"."<<endl;
	//break out if it failed
	if ( !outPrintFile ){
		return false;
	}
	
	//set up headers
	cout<<"Finding highest number of all the sets..."<<endl;
	CollatzConjecture highestNumberInSet;
	highestNumberInSet = CollatzConjecture(getHighNumOfConj(readFromFile,x,y));
	cout<<" Done."<<endl;
	
	cout<<"Finding number of conjectures with this number..."<<endl;//debugging
	unsigned long long int numWithHigh = findNumWithHighNum(highestNumberInSet.getHigh(),readFromFile,x,y);
	cout<<" Done."<<endl;
	
	cout<<"Finding the conjecture with the biggest set..."<<endl;
	CollatzConjecture biggestSet;
	biggestSet = CollatzConjecture(getHighNumInSet(readFromFile,x,y));
	cout<<" Done."<<endl;
	
	cout<<"Finding the number of conjectures with this sized set..."<<endl;
	unsigned long long int numWithHighStep = findNumWithXStep(biggestSet.getNumInSet(),readFromFile,x,y);
	cout<<" Done."<<endl;
	
	//write headers to file
	outPrintFile<<"Collatz Conjectures "<<FormatWithCommas(x)<<" - "<<FormatWithCommas(y)<<endl<<
				  "Generated with Collatzer by Greg Stewart"<<endl<<endl;
				  
	outPrintFile<<"Conjecture with largest number encountered in its set:"<<endl
				<<left<<setw(29)<<"Start:"<<setw(18)<<FormatWithCommas(highestNumberInSet.getStart())<<endl
				<<setw(29)<<"Number In set:"<<setw(18)<<FormatWithCommas(highestNumberInSet.getNumInSet())<<endl
				<<setw(29)<<"Highest Number Encountered:"<<FormatWithCommas(highestNumberInSet.getHigh())<<endl<<endl; 
	outPrintFile<<"Number of times the largest number encountered appears: "<<FormatWithCommas(numWithHigh)<<endl<<endl;
				
	outPrintFile<<"Conjecture with the largest set:"<<endl
				<<left<<setw(29)<<"Start:"<<setw(18)<<FormatWithCommas(biggestSet.getStart())<<endl
				<<setw(29)<<"Number In set:"<<setw(18)<<FormatWithCommas(biggestSet.getNumInSet())<<endl
				<<setw(29)<<"Highest Number Encountered:"<<FormatWithCommas(biggestSet.getHigh())<<endl<<endl;
	outPrintFile<<"Number of sets that have this number in them: "<<FormatWithCommas(numWithHighStep)<<endl<<endl;
	
	outPrintFile<<left<<setw(18)<<"Starting Num"<<setw(18)<<"Highest # in Set"<<setw(18)<<"# Of Steps"<<endl;
	
	cout<<"Now writing conjectures out to file..."<<endl;
	//set file-position pointer to where it needs to be
	readFromFile.seekg((x - 1) * sizeof(CollatzConjecture));
	
	//read first record from data file
	CollatzConjecture conj;
	readFromFile.read( reinterpret_cast<char *>(&conj),sizeof(CollatzConjecture));
	//conj.toString(0);//debugging
	
	//copy all records from record file into text file
	while(x <= y & !readFromFile.eof()){
		//write single record to text file
		if(conj.getStart() != 0){//skip possible bad one (can't start at zero)
			
			//output to file
			outPrintFile<<left<<setw(18)<<FormatWithCommas(conj.getStart())<<setw(18)<<FormatWithCommas(conj.getHigh())
						<<setw(18)<<FormatWithCommas(conj.getNumInSet())<<endl;
			
			//conj.toString(1);//debugging
		}
		
		//read next record from file
		readFromFile.read(reinterpret_cast<char *>(&conj),sizeof(CollatzConjecture));
		x++;
	}//while not end of file
	cout<<" Done."<<endl;
	readFromFile.clear();//clear readFromFile to ensure nothing gets mixed up in the file stream
	return true;
}//createTextFile

/*
	to create a new record
*/
bool newRecord(fstream &insertInFile,CollatzConjecture &conj){
	
	
	//obtain new number of conj to create
	unsigned long long int startNum = conj.getStart();
	
	//move file-position pointer to correct record in file
	insertInFile.seekg((startNum - 1) * sizeof(CollatzConjecture));
	
	//read record from file
	CollatzConjecture conj2;
	insertInFile.read(reinterpret_cast<char *>(&conj2),sizeof(CollatzConjecture));
	
	//clear to not mix things up if empty file
	insertInFile.clear();
	
	//create record, if record does not previously exist
	if(conj2.getStart() == 0){
	
		//cout<<"Writing to file... ";//debugging
		
		//move file-position pointer to correct record in file
		insertInFile.seekp((startNum - 1) * sizeof(CollatzConjecture));
		
		//insert record in file
		insertInFile.write(reinterpret_cast<const char *>(&conj),sizeof(CollatzConjecture));
		
		
		//testing for failiures
		if(!insertInFile | insertInFile.fail()){
			//cout<<"ERROR: Writing conjecture starting at "<<conj.getStart()<<" failed."<<endl;
			return false;
		}else{ 
			//cout<<"Writing conjecture starting at "<<conj.getStart()<<" succeded."<<endl;
			return true;
		}
	}else{
		//cout<<"Conjecture starting at "<<conj.getStart()<<" already present."<<endl;//debugging
		//conj.toString();//debugging
		//cout<<endl<<endl;//debugging
		
		return false;
	}
	
}//newRecord

/*
	createUpTo
		creates conjectures up to the given number
		also does the progress bar
*/
bool createFromTo(unsigned long long int lowNum,unsigned long long int highNum,fstream &theFileStream){
	if(lowNum == 0){
		lowNum = 1;
		//return false;
	}
	if(highNum <= lowNum){
		cout<<"the lower bounds is greater than the upper"<<endl;
		return false;
	}
	
	time_t start,end;//instanciate start and end time objects

	//output range and number to be generated
	cout<<endl<<"Generation starting at "<<FormatWithCommas(lowNum)<<" and going to "<<FormatWithCommas(highNum)<<endl;
	int numGenerating = (highNum - lowNum) + 1;
	cout<<"Total conjectures to be generated: "<<FormatWithCommas(numGenerating)<<endl;
	cout<<"Estimated Disk use when complete: "<<figureSpace(highNum,sizeof(CollatzConjecture))<<endl;
	//print out frame for loading bar
	cout<<"Progress:"<<endl<<
		"|0%          25%        50%        75%       100%|"<<endl;
	
	//setup math for loading bar
	int updateFreq = numGenerating/50;
	int count = 0;
	
	//instance variables for loading bar
	bool result = true;
	CollatzConjecture temp;
	
	//take starting time
	time (&start);
	
	//do the actual generation loop
	while(lowNum <= highNum & result){//generation loop
			
		//make new conjecture
		temp = CollatzConjecture(lowNum);
	
		//insert into file
		result = newRecord(theFileStream,temp);
		
		//move i up one
		lowNum++;
		//move count up one
		count++;
		//see if we need to add to the progress bar
		if(count >= updateFreq){
			cout<<"#"<<flush;
			count = 0;
		}
	}//generation loop
	
	time (&end);//take end time
	cout<<endl;//spacer
	
	//calculate time spent (in seconds)
	double elapsed_time = difftime (end,start);
	
	//separate into hours, minutes, and seconds
	string timeString = figureTime(elapsed_time);
	
	//output
	cout<<"Time elapsed: "<<timeString<<endl;
	
	if(result){//if it worked
		return true;
	}else{//if it failed
		return false;
	}
	
	
	
}//if not already generated to that number

/*
	to count how many conjectures created
*/
int getNumConj(fstream &readFromFile){
	//output start of function
	cout<<"Calculating number of conjectures on file";
	
	//counting number
	int i = 0;
	//temporart holder of data from file
	//cout<<"\tCreating temp CollatzConjecture"<<endl;//debug
	CollatzConjecture temp;
	
	//cout<<"\t Done"<<endl;//debugging
	
	//set readFromFile to beginning of file
	readFromFile.seekg(0);
	
	//read the first conjecture
	readFromFile.read(reinterpret_cast<char *>(&temp),sizeof(CollatzConjecture));
	
	//output start of calculation
	cout<<"..."<<endl;
	
	//begin counting loop
	while(!readFromFile.eof()){
		//move i up
		i++;
		//read next one
		readFromFile.read(reinterpret_cast<char *>(&temp),sizeof(CollatzConjecture));
		//cout<<i<<endl;//debugging
	}//while not end of file
	//state that the operation is complete
	cout<<"Done."<<endl;
	readFromFile.clear();//clear readFromFile to ensure nothing gets mixed up in the file stream
	return i;//return number counted
}

/*
	Goes through the whole file and fixes any errors (if any)
*/
int checkFile(fstream &readFromFile){
	//needed instance variables
	int numWrong = 0;//overall number wrong
	int i = 1;//counting number in file
	CollatzConjecture temp;//tester of data from file
	CollatzConjecture test;//holder of data from file
	
	//set reader to beginning of file and get the first entry
	readFromFile.seekg(0);
	readFromFile.read(reinterpret_cast<char *>(&test),sizeof(CollatzConjecture));
	//loop to go through file to the end
	do{
		//create fresh conjecture based on where we are
		temp = CollatzConjecture(i);
		
		//test for innacuracy
		if(temp != test){
			//add one to numWrong
			numWrong++;
			//cout<<"Error on "<<i<<endl;//debugging
			//set the entry to correct one
			readFromFile.seekp((i - 1) * sizeof(CollatzConjecture));
			readFromFile.write(reinterpret_cast<const char *>(&temp),sizeof(CollatzConjecture));
		}
		//increment i, for next one
		i++;
		//move up the one we are on
		readFromFile.read(reinterpret_cast<char *>(&test),sizeof(CollatzConjecture));
	}while(!readFromFile.eof());
	
	readFromFile.clear();//clear readFromFile to ensure nothing gets mixed up in the file stream
	return numWrong;//return number of conjectures found wrong
}

/*
	createOp
	
	Generates the default options file.
*/
bool createOp(){
	cout << "Writing out default options tile \"options.txt\"..." << endl;
	const int numLines = 11;
	string options[numLines] = {
		"Collatzer Options File",
		"Notes:",
		"-if entering numbers, no commas",
		"-leave an option empty for default",
		"-do not change anything but the option where you are specified to put it.",
		"	Not following this rule will break the system",
		"-sample options are given to demonstrate use",
		" ",
		"Option name                |<-- start option on this column ",
		"Location of Data file:     conjectures.dat",
		"Automatically generate to: 100" };
	//create options text file
	ofstream outPrintFile("options.txt", ios::out);
	if (!outPrintFile){//in case it fails
		outPrintFile.close();
		return false;
	}
	//loop to write out every line
	for (int i = 0; i < numLines; i++){
		if (i == 0){
			outPrintFile << options[i];
		}
		else{
			outPrintFile << endl << options[i];
		}
		
		if (!outPrintFile){
			outPrintFile.close();
			return false;
		}
	}//writing loop
	cout << " Done." << endl;
	//close file and return true to finish things up
	outPrintFile.close();
	return true;
}
#endif
