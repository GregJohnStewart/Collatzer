/*
	Collatz Conjecture Generator
		
		Calculates many conjecture series at once, outputs it to a file
		
		can output to a text file for easy output
		
	Author: Greg Stewart
	
	Start: 4/15/14

	V1.1
	
	Be sure to check out the documentation for more
		information on this program!
	
	http://gjstewart.net/projects/collatzer.html
	
TODO: List major:
      
      List minor:
      	-in options processing, check for vald inputs
*/

/*
	includes
*/	

//my custom classes and methods
#include "CollatzConjecture.h"//the CollatzConjecture object
#include "RAFclasses.h"//for all the file I/O

//c++ libraries
#include <iostream>//to do console stuff
#include <fstream>//to deal with the file
#include <limits>//to deal with wrong inputs
#include <time.h>//for time measurment
#include <string>//for strings


//usings
using namespace std;


int main(){
//global variables
bool opened = false;//if the "conjectures.dat" has been opened
bool run = true;//the running loop
char choice = 0;// user's choice
unsigned long long int numInFile = 0;//number on file; only updated where file changed or initially opened
string sizeOnDisk = "";//approximate size of data file on disk
string fileLocation = "";//the location of the working file
bool autoGen = false;//if we are automatically generating up to a certain number
unsigned long long int toAutoGenTo;

//output intro things
cout<<"Collatzer"<<endl<<
	  "Collatz Conjecture Generator"<<endl<<
	  "\tGenerate Collatz Conjectures"<<endl<<
	  "Author/Copyright Greg Stewart"<<endl<<endl<<
	  "NOTE:"<<endl<<
	  "\t- This program scales linerally (O(n)), meaning:"<<endl<<
	  "\t- More conjectures means a longer time taken to deal with them."<<endl<<
	  "\t- More conjectures means a larger data file and outputted text files"<<endl<<endl;

/////////////////////
//  Initial setup  //
/////////////////////

/*
 * See if option files are present:
 * >File Location
 * >automatic generation to a certain number of conjectures
*/
ifstream options;
options.open ("options.txt");
if(!options){
	cout<<"Options file either nonexistent or unable to be opened."<<endl<<endl;
	fileLocation = "conjectures.dat";
}else{
	cout<<"Options.txt file being processed..."<<endl;
	string input = "";//the input string
	int curLine = 0;//the line we are currently at
	int numOfColumn = 27;//where the options start
	while(!options.eof()){//while not reached end of file
		getline(options,input);//get the next line
		curLine++;//move curLine up with it
		if(curLine == 10){
			input = input.substr(numOfColumn,input.length());
			if(input == ""){//if no input
				//set location to default (running directory)
				fileLocation = "conjectures.dat";
				cout<<"\tNo special location defined. Placing in this directory."<<endl;
			}else{//set to location given
				fileLocation = input;
				cout<<"\tData file set to: \""<<fileLocation<<"\""<<endl;
			}
		}else if(curLine == 11){
			input = input.substr(numOfColumn,input.length());
			toAutoGenTo = atoi(input.c_str());
			//cout<<"Value of autoGen: "<<num<<endl;//debug
			
			if(toAutoGenTo == 0){
				cout<<"\tNo valid automatic generation number given."<<endl;
			}else{
				cout<<"\tAutomatic generation set to: "<<FormatWithCommas(toAutoGenTo)<<endl;
				autoGen = true;
			}
		}//separate line option parsing
	}//while not the end of file
	
	cout<<" Done."<<endl<<endl;
}

options.close();

while(!opened){//while the file has yet to be opened
	//open conjectures.dat file
	fstream conjectures(fileLocation.c_str(),ios::in | ios::out | ios::binary);
	
	//set opened to t/f based on how conjectures returned
	if(!conjectures){
		opened = false;
	}else{
		opened = true;
	}
	
	//if it failed to open
	if(!opened){
		
		//output error
		cout<<"ERROR: could not open data file."<<endl
			<<"Be sure it is present and you have correct permissions on it."<<endl;
		run = false;//tell main prog loop not to run
	
		//prompt for what to do
		cout<<endl<<"Try to make empty file? (y/n): ";
		try{
			cin>>choice;
		}catch(char* error){
			choice = '9';
		}
		switch(choice){
			case 'n':
			case 'N':{//just exit the program
				cout<<"Exiting..."<<endl<<endl;
				opened = true;
				break;
			}case 'y':
			case 'Y':{//create "conjectures.dat" file
				cout<<"Making file \""<<fileLocation<<"\"..."<<endl;
			
				ofstream outPrintFile(fileLocation.c_str(), ios::out | ios::binary);
			
				if(!outPrintFile){
					cout<<"File creation failed."<<endl;
				}else{			
					cout<<"File creation complete."<<endl;
				}
				cout<<endl;
				run = true;
				break;
			}default:{
				cout<<"Invalid input. Exiting..."<<endl;
			break;
			}
		}
	
	}else{//when succeeding opening file
		cout<<"\""<<fileLocation<<"\""<<" found and successfully opened!"<<endl<<endl;
		opened = true;
		numInFile = getNumConj(conjectures);
		
		//if set to automatically generate
		if(autoGen){
			bool result = true;
			if(numInFile == 0){//if empty file
				//generate to the given number to go to
				cout<<"Automatically generating to: "<<FormatWithCommas(toAutoGenTo)<<endl;
				cout<<"Empty file, starting from 1"<<endl;
				result = createFromTo(1,toAutoGenTo,conjectures);
				if(result){
					cout<<"Success!"<<endl;
				}else{
					cout<<"Failed."<<endl;
				}
				numInFile = getNumConj(conjectures);
			}else if(numInFile >= toAutoGenTo){//if already generated to that number
				//output this fact
				cout<<"Conjectures 1 - "<<FormatWithCommas(numInFile)<<" already on file."<<endl
				    <<"No need to auto generate."<<endl;
				result = false;
			}else{//if file is not empty, but less than auto
				//generate what is left to do
				cout<<"Automatically generating to: "<<FormatWithCommas(toAutoGenTo)<<endl;
				cout<<"Conjectures 1 - "<<FormatWithCommas(numInFile)<<" already on file."<<endl;
				//set the start of generation to one after the number in the file
				result = createFromTo((numInFile + 1),toAutoGenTo,conjectures);
				if(result){
					cout<<"Success!"<<endl;
				}else{
					cout<<"Failed."<<endl;
				}
				numInFile = getNumConj(conjectures);
			}
			cout<<endl;			
		}//if autoGen
	}//when it succeeds opening the file
	//conjectures.close();
}//while not opened

//open the file
fstream conjectures(fileLocation.c_str(),ios::in | ios::out | ios::binary);


///////////////////////
////   main loop   ////
///////////////////////
//instance variables for just this loop
bool shutdown = false;

while(run & opened){//main running loop
	
	//reset needed variables
	choice = 0;
	//cin.clear();
	
	if(shutdown){//if to shutdown next, tell it to exit loop after one more run through
		run = false;
	}
	
	
	sizeOnDisk = figureSpace(numInFile,sizeof(CollatzConjecture));
	cout<<"\tNumber of conjectures currently on file: "<<endl
	    <<"\t\t"<<FormatWithCommas(numInFile)<<endl;
	cout<<"\tSize of file on disk: "<<endl<<"\t\t"<<sizeOnDisk<<endl<<endl;
	//prompt for what to do
	cout<<"What would you like to do?"<<endl<<
		  "\t1- Generate Conjectures"<<endl<<
		  "\t2- Output to Text File"<<endl<<
		  "\t3- Get Specific Conjecture Info (independent of generated conjectures)"<<endl<<
		  "\t4- View a range of conjectures' Statistics."<<endl<<
		  "\t5- File options"<<endl<<
		  "\t6- Estimate size of data on disk."<<endl;
	if(!shutdown){//if not prompted to shutdown
		cout<<"\t9- Shutdown after next operation."<<endl;
	}else{//if prompted to shutdown
		cout<<"\t9- Cancel automatic shutdown."<<endl;
	}
	cout<<"\t0- Exit"<<endl<<
		  "Choice: ";
	//get input from user
	choice = getCharFromUser();
    	cout << "got from user: " << choice << endl;
    
	cout<<endl;//spacer line
	switch(choice){//main switch
		
		/*
			To generate conjectures
		*/
		case '1':{
			
			unsigned long long int initNum;//initial number of conjectures generated
			int i = -1;	//counting number
			
			do{//loop until valid input
				//prompt for # to generate
				cout<<"To what number would you like to generate up to?"<<endl
					<<"\tNotes:"<<endl
					<<"\t- Any decimal entered will be rounded down to the nearest integer."<<endl
					<<"\t- At least 50 to generate required for loading bar to work correctly"<<endl
					<<"\t- Current number of conjectures on file: "<<FormatWithCommas(numInFile)<<endl
					<<"\t- 0 to skip."<<endl
					<<"Number to generate up to: ";
				
				try{//get inoput
					initNum = getUIntFromUser();
					//cout<<"Number entered: "<<initNum<<endl;//for debugging
					i = 1;//set i to exit loop
				}catch(int* error){
					i = -1;//set i to stay in loop
					cout<<endl<<"Invalid input. Try again."<<endl<<endl;
				}
				
			}while(i < 0);//while not a valid number
			
			//set up instance variables
			CollatzConjecture temp;
			bool result = true;
			
			if(initNum<=0){//to skip generation
				cout<<"Skipped."<<endl;
			}else{//go the generation
				bool result = true;
				if(numInFile == 0){//if empty file
					cout<<"Empty file, starting from 1"<<endl;
					result = createFromTo(1,initNum,conjectures);
					if(result){
						cout<<"Success!"<<endl;
					}else{
						cout<<"Failed."<<endl;
					}
				}else if(numInFile >= initNum){//if already generated to that number
					//output this fact
					cout<<"Conjectures 1 - "<<FormatWithCommas(numInFile)<<" already on file."<<endl
					    <<"\tNo need to generate."<<endl;
					result = false;
				}else{//if file is not empty, but less than auto
					//output this fact
					cout<<"Conjectures 1 - "<<FormatWithCommas(numInFile)<<" already on file."<<endl;
					//set the start of generation to one after the number in the file
					result = createFromTo(numInFile + 1,initNum,conjectures);
					if(result){
						cout<<"Success!"<<endl;
					}else{
						cout<<"Failed."<<endl;
					}
				}
				//recalculate number of conjectures on file
				numInFile = getNumConj(conjectures);
			}//if valid number
			break;
		
		/*
			outputting to text file
		*/
		}case '2':{
			//if there is something in the file
			if(numInFile != 0){
				//output initial notes and prompt
				cout<<"Outputting to Text File."<<endl<<
					  "\tNote:"<<endl<<
					  "\t- All outputs are saved in the \"outputs\" folder."<<endl<<
					  "\t- The \"outputs\" folder needs to exist in the same folder as this program."<<endl;
			
				cout<<"What would you like to do?"<<endl<<
					  "\t1- Output all conjectures to file"<<endl<<
					  "\t2- Output range of conjectures to file"<<endl<<
					  "\t0- Skip"<<endl<<
					  "Choice: ";
				choice = getCharFromUser();
			
				switch(choice){
					case '1':{//output all conjectures to file
						cout<<"Outputting all conjectures to file..."<<endl;
						bool worked = createTextFile(conjectures,numInFile);
						if(worked){
							cout<<endl<<"Done."<<endl;
						}else{
							cout<<"Failed.";
						}
						break;
					
					}case '2':{//output a range of conjectures to file
						//instance vaiables
						unsigned long long int x,y;//high and low bounds of range
						bool correctIn = false;//if they entered within bounds
					
					
						while(!correctIn){//while they haven't entered correctly
							//prompt
							cout<<"Current number of conjectures on file: "<<FormatWithCommas(numInFile)<<endl;
							cout<<"Enter the range you would like to output:"<<endl
								<<"From: ";
							x = getUIntFromUser();
							//cout<<"Number entered: "<<initNum<<endl;//for debugging
							if(x<=0){//if invalid number
								cout<<"Invalid number. Please try again and enter any real number."<<endl<<endl;
								correctIn = false;
							}else{
								correctIn = true;
							
							}
				        	 	if(correctIn){//if first one was valid
								cout<<"To: ";
								y = getUIntFromUser();
								cout<<endl;
								//cout<<"Number entered: "<<initNum<<endl;//for debugging
								if(y<=0 || (x >= y)){//if it is invalid or less than the first number
									cout<<"Invalid number. Please try again and enter any real number, greater than the first number."<<endl<<endl;									correctIn = false;
								}else{
									correctIn = true;
								}
								//cout<<endl;
				        		}//second number input
						}//get x & y
						if(numInFile < y){//if y is greater than total number of conjectures
							//set the upper number to the total number of conjectures
							cout<<"Upper number larger than number of conjectures.\nSetting it to the total number of conjectures."<<endl;
							y = numInFile;
							cout<<endl;
							
							if(x >= y){//if x is now >= y, break out
								cout<<"Lower bounds now greater than or equal to upper bounds, skipping operation."<<endl;
								break;
							}
						}//if y> number of generated conjectures
						
						//output the specified range of conjectures
						cout<<"Outputting to file conjectures "<<x<<" to "<<y<<"..."<<endl;
						bool worked = createTextFileRange(conjectures,x,y);
						//output if it worked or not
						if(worked){
							cout<<"Done."<<endl;
						}else{
							cout<<"Falied."<<endl;
						}
					
						break;
					}case '0':{//skip this operation
						cout<<"Skipping."<<endl;
						break;
					}default:{
						cout<<"Invalid Input. Please try again."<<endl;
						break;
					}
			
				}//switch for what type of output
			}else{//if no conjectures generated
				cout<<"Cannot output conjectures if none generated."<<endl;
			}
			
			break;
		/*
			Getting specific conjecture information
				does not pull from data file
		*/
		}case '3':{
		
			cout<<"Get Specific Conjecture Info"<<endl<<endl;
			//instance variables
			unsigned long long int x = 0;//the starting number to be done
			bool correctIn = false;
			
			
			while(!correctIn){//while getting the number	
				//prompt
				cout<<"Enter conjecture you would like to see the statistics for:"<<endl
					<<"Conjecture's starting number: ";
				
				x = getUIntFromUser(0);
				//cout<<"Number entered: "<<initNum<<endl;//for debugging
				//see if it is valid
				if(x<=0){
					cout<<"Invalid number. Please try again and enter any real number."<<endl<<endl;
					correctIn = false;
				}else{
					correctIn = true;
				}
    	    		}
    	    
    	    		//generate the conjecture based on number given
    	    		CollatzConjecture temp;
    	   		temp = CollatzConjecture(x);
    	    
    	    		//output the information
    	    		temp.print();
    	    
			break;
		/*
			view a number of conjectures' statistics
		*/
		}case '4':{
			if(numInFile > 1){//if there are conjectures on file
				
				//output how many so they know
				cout<<"View a range of conjectures' Statistics."<<endl<<endl;
				cout<<"Number of conjectures on file: "<<FormatWithCommas(numInFile)<<endl<<endl;
				
				int x,y;//upper and lower bounds
				bool correctIn = false;//do we have correct input
				
				
				while(!correctIn){//while we dont have correct input
					//prompt for lower bound
					cout<<"Enter the range you would like to see the statistics for:"<<endl
						<<"From: ";
					x = getIntFromUser();
					//cout<<"Number entered: "<<initNum<<endl;//for debugging
					if(x<=0){
						cout<<"Invalid number. Please try again and enter any real number."<<endl<<endl;
						correctIn = false;
					}else{
						correctIn = true;
					}
			       	
			       		if(correctIn){//if we got the correct input
			       			try{//prompt and get the second number
							cout<<"To: ";
							cin>>y;
							cout<<endl;
							//cout<<"Number entered: "<<initNum<<endl;//for debugging
							if(y<=0 || (x >= y)){
							cout<<"Invalid number. Please try again and enter any real number, greater than the first number."<<endl<<endl;
							correctIn = false;
							}else{
								correctIn = true;
							}
							//cout<<endl;
						}catch(int* error){
							cout<<endl<<"Invalid input. Try again."<<endl<<endl;
							correctIn = false;
						}
						cin.clear(); //clear stream
			    			cin.ignore(numeric_limits<streamsize>::max(), '\n');//ignore leftover data
			    		}// if got x
				}//get x & y
				
				//instance variables
				CollatzConjecture highNum;//conjecture with the highest individual number in its set
				unsigned long long int numWithHigh = 0;//number of numbers with that high number
				CollatzConjecture highInSet;//conjecture with the highest number of values in its set
				unsigned long long int numWithHighStep = 0;//number of conjectures with that number o steps
				int numProcessed = (y - x) + 1;//number to process
				
				//calc # of conjectures
			
				cout<<endl;//spacer
			
				//calc highest number encountered
				cout<<"Calculating highest number encountered in all specified sets..."<<endl;
				highNum = CollatzConjecture(getHighNumOfConj(conjectures,x,y));
				cout<<"Done."<<endl;
				
				cout<<"Finding number of conjectures with this number..."<<endl;//debugging
				numWithHigh = findNumWithHighNum(highNum.getHigh(),conjectures,x,y);
				cout<<" Done."<<endl;
				
				//calculate conjecture with highest number in the set
				cout<<"Calculating the conjecture with the highest number of numbers in its set..."<<endl;
				highInSet = CollatzConjecture(getHighNumInSet(conjectures,x,y));
				cout<<"Done."<<endl;
				
				cout<<"Finding the number of conjectures with this sized set..."<<endl;
				numWithHighStep = findNumWithXStep(highInSet.getNumInSet(),conjectures,x,y);
				cout<<" Done."<<endl;
			
				//output calculated numbers
				cout<<"Finished calculations."<<endl<<endl<<
					  "Number of conjectures on file: "<<FormatWithCommas(numInFile)<<endl<<
					  "Processing conjectures between "<<FormatWithCommas(x)<<" and "<<FormatWithCommas(y)<<"(inclusive)"<<endl<<
					  "Number of conjectures processed: "<<FormatWithCommas(numProcessed)<<endl<<endl<<
					  "The conjecture with the highest number included in its set:  "<<endl;
				highNum.print('b','t');
				cout<<"Number of conjectures that share this high number: "<<FormatWithCommas(numWithHigh)<<endl;
				cout<<endl;//spacer
				cout<<"The conjecture with the highest number of steps in its set:"<<endl;
				highInSet.print('b','t');
				cout<<"Number of conjectures that have this number in their sets: "<<FormatWithCommas(numWithHighStep)<<endl;
				cout<<endl;//spacer
					  
			}else if(numInFile == 1){//simply output 1's conjecture, if they only have one on file
				cout<<"The only conjecture in file is 1."<<endl<<endl;
				
				CollatzConjecture one;
				one = CollatzConjecture(1);
				one.print();
				
				cout<<endl;//spacer
				
			}else{
				cout<<"Cannot output conjecture statistics if none generated."<<endl;
			}
			
			break;
		
		/*
			file options
		*/
		}case '5':{
			//prompt for input
			cout<<"What would you like to do?"<<endl<<
				  "\t1- Delete \""<<fileLocation<<"\""<<endl<<
				  "\t\t- The program will automatically close after deleting."<<endl<<
				  "\t2- Check integrity of \""<<fileLocation<<"\""<<endl<<
				  "\t3- Create \"options.txt\" file."<<endl<<
				  "\t0- Skip"<<endl<<
				  "Choice: ";
			
			choice = getCharFromUser('9');
			
			switch(choice){
				case '1':{//delete generated conjectures
					conjectures.close();
					int result = remove(fileLocation.c_str());
					
					if(result != 0){
						cout<<"Could not delete generated conjectures."<<endl;
					}else{
						cout<<"Deleted."<<endl;
					}
					run = false;
					
					break;
					
				}case '2':{//verify integrity of conjectures.dat
					
					cout<<"Verifying and fixing \""<<fileLocation<<"\"..."<<endl;
					int result = checkFile(conjectures);
					cout<<"Done."<<endl<<endl;
					
					if(result == 0){
						cout<<"No errors found, file is in tact and not corrupted."<<endl<<endl;
					}else{
						cout<<FormatWithCommas(result)<<" out of "<<FormatWithCommas(numInFile)<<" found with errors and fixed."<<endl<<endl;
						
						numInFile = getNumConj(conjectures);
						cout<<"Number of conjectures now on file: "<<FormatWithCommas(numInFile)<<endl;
					}
					break;
				}case '3':{//create the default options.txt file
					bool worked = false;
					worked = createOp();
					if (worked){
						cout << "Options file successfully created. Defaults are written out." << endl;
					}
					else{
						cout << "Options file failed to write. Make sure you have correct permissions and try again." << endl;
					}
					break;
				}case '0':{
					cout<<"File options skipped.";
					break;
				}default:{
					cout<<"Invalid Input. Please try again."<<endl;
					break;
				}
			}//file mngmt switch
			
			
			cout<<endl;
			
			break;
		/*
			disk space Calculator
		*/
		}case '6':{
			//get the size of a CollatzConjecture object
			int sizeOfConj = sizeof(CollatzConjecture);
			
			//output info
			cout<<"Calculate conjecture space."<<endl<<
				  "Note:"<<endl<<
				  "\t- Each conjecture takes up "<<sizeOfConj<<" bytes of disk space."<<endl<<
				  "\t- This tool estimates how much space amounts of conjectures will take."<<endl<<
				  "\t- Largest unit measurement in terabytes"<<endl;
			
			//instance variables
			unsigned long long int initNum = 0;//input number
			bool correctIn = false;//if we have correct input
			do{ 
				//prompt for #
				cout<<"Number: ";
				
				initNum = getUIntFromUser(0);
				//cout<<"Number entered: "<<initNum<<endl;//for debugging
				if(initNum == 0){
					correctIn = false;
				}else{
					correctIn = true;
				}
				//cout<<endl;
			}while(!correctIn);
			
			string spaceNeeded = figureSpace(initNum,sizeOfConj);
			//data necessary
			cout<<"The amount of space "<<FormatWithCommas(initNum)<<" conjectures will take up on the disk:"<<endl
			    <<spaceNeeded<<endl<<endl;
			break;
			
		/*
			Shutdown switch
		*/
		}case '9':{
			//switch the boolean
			shutdown = !shutdown;
			
			//output based on what it will do
			if(shutdown){
				cout<<"System will shutdown after next performed operation."<<endl;
				#ifdef _WIN32//code for windows
				#else//code for UNIX
				cout<<"LINUX: only happens if run as root."<<endl;
				#endif  
			}else{
				cout<<"Automatic shutdown cancelled."<<endl;
				//so the program will keep running
				run = true;
			}
			break;
		/*
			To exit the program
		*/
		}case '0':{
			cout<<"Exiting..."<<endl<<endl;
			run = false;
			break;
		/*
			For invalid input
		*/
		}default:{
			cout<<"Invalid input. Please enter a valid option."<<endl<<endl;
			break;
		}//default
	}//main switch
	cout<<endl;
	conjectures.clear();
	
}//running loop

//close file
conjectures.close();

//if requested, shutdown system.
if(shutdown){
	cout<<"Attempting shutdown..."<<endl;
	#ifdef _WIN32//code for windows
	system("shutdown /s");
	#else//code for UNIX
	system("shutdown -h now");
	#endif  
}

return 0;
}//main
