Collatzer V1.1 Readme
Author: Greg Stewart
Copyright 2014 Greg Stewart

For information on this project and more, visit:

http://www.gjstewart.net/projects/

To find out what a CollatzConjecture is:
http://en.wikipedia.org/wiki/Collatz_conjecture

Additional Documentation in the Documentation folder.
	Included:
	-How to Compile
	-CollatzConjecture object
	-Flow diagram of program

'outputs' folder needs to be present in the same running directory for
	outputting to a text file.

Written in C++, this program is made to record and display information about
	Collatz Conjectures. Only limit to the number of conjectures able to 
	be generated is the disk space available on the computer it is being
	run on, and the size of an unsigned long long integer.

An exercise in file input and output in c++. Generates collatz conjecture 
	objects	and stores them in a binary random access file, for future 
	reference. Uses this file to output information about the conjecture. 
	Able to also show information about a single conjecture, without the use
	of the data file.

Uses a relatively simple recursive function to generate the conjecture, found in
	the CollatzConjecture object.

Can output to a plain text file for viewing, and outputs statistics about the
	conjectures being outputted to this text file.

Can verify the data file's integrity in case of possible corruption.

Can automatically shut down the computer for use when generating many 
	conjectures.
	-on linux, must be run as sudo user

Source code provided, code present for both windows and linux use. Compiled for
	windows and Ubuntu linux (x86).
	
For legal information, visit my website's download page, and click on 'Software Information'