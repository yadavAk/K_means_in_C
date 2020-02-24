How to run the Program:-On gcc compiler, follow following steps to run the program:
			(i) Open command window and go to location of file using 'cd' command.
			(ii) Now type 'gcc -o filename.exe filename.c' command to compile 
			the program.
			(iii) Finally type 'filename.exe' command to execute the program.

Problem 1:
	INPUT FORMAT:   First input will be text file of name "english.txt" at the same address
			where my C file is present.
			Second input will be on command line which is positive integer denoting
			no. of clusters user want to make(for best output no. of clusters should
			be >20 and <50).
	OUTPUT FORMAT:  First few lines of output will be representative strings.
			Second output is positve integers denoting no. of correctly spam messages
			detected out of 41 spam messages and no. of correctly ham messages detected
			out of 502 ham messages.
			Last output is floating number denoting percentage accuracy which is equal
			to sum of correctly spam messages detected plus no. of correctly ham message
			detected divided by total no. of test messages.

