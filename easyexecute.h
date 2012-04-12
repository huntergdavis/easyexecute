/*
    Copyright (C) 2007  Hunter Davis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef EXECUTE_H_
#define EXECUTE_H_

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

// CLASS: 	easyExec
// FUNCTION:    An easy to use abstracted class for executing
// 		external programs from within c++ source.
// 		Time related information is stored with the 
// 		structure, and various methods of executing
// 		(blocking, non-blocking, system()) are included.
// AUTHOR: 	Hunter Davis
// Date:   	11/16/07
// LICENSE:	GPLV3 - see licence.txt for more info
//

class easyExec{
public:
	easyExec();
	easyExec(std::string command);
	int executeWait();
	int executeWait(std::string command);
	int executeAndGo();
	int executeAndGo(std::string command);

	//only use system() execute if you know what you are doing
	//the above executeWait and executeAndGo functions are safer
	//as they use fork
	int executeSystem();
	int executeSystem(std::string command);

	time_t elapsedTime();
	time_t startTime();
	time_t endTime();
	time_t totalRunTime();
	time_t classDeclarationTime();


	std::string command();

private:
	pid_t child_pid;
	std::string commandVal;

	time_t classDeclarationTimeVal;
	time_t startTimeVal;
	time_t endTimeVal;
	time_t elapsedTimeVal;
	time_t totalTimeVal;

	bool executed;
	bool finished;
	//parse our string to command args
	void  parse(char *line, char **argv);
};
#endif

