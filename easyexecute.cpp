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

#include "easyexecute.h"

//here we globally define the maximum number of command line arguments
//Trials indicate Windows is somewhere above 20 but below 255, linux is somewhere between 255 and 1024??
#define MAX_ARGS 64


//FUNCTION:	easyExec()
//Purpose:	constructor for easy exec class
easyExec::easyExec()
{
	//default values
	classDeclarationTimeVal = time(NULL);
	commandVal = "";
	executed = false;
	finished = false;
}

//FUNCTION:	easyExec(std::string command)
//Purpose:	constructor for easy exec class
//		command passed is stored in class
easyExec::easyExec(std::string command)
{
	//default values
	classDeclarationTimeVal = time(NULL);
	commandVal = command;
	executed = false;
	finished = false;
}

//FUNCTION:	executeWait()
//Purpose:	executes the class' contained command
//		using fork/exec.  Blocks and waits
//		till command completes before
//		returning
int easyExec::executeWait()
{
	
	child_pid = fork();
	if ( child_pid == 0 )
	{
		//create our arguments list from the command string
		char *arguments[MAX_ARGS];
		char *commandPtr = new char[commandVal.length()+1];
		strcpy(commandPtr,commandVal.c_str());
		parse(commandPtr,arguments);

  		// in the child, do exec  
  		execvp(*arguments, arguments);
		delete commandPtr;
		exit(1);
	}
	else if ( child_pid != (pid_t)-1 ) 
	{
       		// in the parent
       		executed = true;
		finished = false;
		startTimeVal = time(NULL);
       		int status;
		while(waitpid(child_pid, &status, WNOHANG) == 0 )
			sleep(1);
		finished = true;
		endTimeVal = time(NULL);
		totalTimeVal = endTimeVal - startTimeVal;
		return status;
	}
       else {
         	// still in parent, but there is no child
         	// this should not happen
         	return 0;
         }
	return 1;
}


//FUNCTION	executeWait(std::string command)
//Purpose	executes the given command
//		using fork/exec.  Blocks and
//		waits till command completes before
//		returning
int easyExec::executeWait(std::string command)
{
	commandVal = command;
	return executeWait();
}


//FUNCTION:	executeAndGo()
//Purpose:	executes the class' command and continues on while the command executes in
//		its own thread.  Child thread will exit after command completes
int easyExec::executeAndGo()
{
        child_pid = fork();
        if ( child_pid == 0 )
        {
                //create our arguments list from the command string
	        char *arguments[MAX_ARGS];
                char *commandPtr = new char[commandVal.length()+1];
                strcpy(commandPtr,commandVal.c_str());
                parse(commandPtr,arguments);
             
		// in the child, do exec
                execvp(*arguments, arguments);
                delete commandPtr;
                exit(1);
        }
        else if ( child_pid != (pid_t)-1 )
        {
		//in the parent process
                executed = true;
                startTimeVal = time(NULL);
                int status;
	
		//as we are doing this asynchronous, it should have "finished" by now
                finished = 1;
                endTimeVal = time(NULL);
                totalTimeVal = endTimeVal - startTimeVal;
	}
	else
	{
		//we are in the parent but the child didn't spawn, major error!
		return 0;
	}                
	return 1;
}

//FUNCTION	executeAndGo(std::string command)
//Purpose:	sets the given command as the class command, and executes asychronously
int easyExec::executeAndGo(std::string command)
{
	commandVal = command;
	return executeAndGo();
}

//FUNCTION:	executeSystem()
//Purpose:	Executes the class' command using system
//		this is dangerous, imagine a "rm -rf /home/" with this!
int  easyExec::executeSystem()
{
	//set up for the unsafe execute
	finished = 0;
	executed = true;
	startTimeVal = time(NULL);
	int sysRetVal;

	//execute (not safe!!)
	sysRetVal = system(commandVal.c_str());

	//finish it up and set time values
	finished = 1;
	endTimeVal = time(NULL);
	totalTimeVal = endTimeVal - startTimeVal;

	return sysRetVal;

}

//FUNCTION:	executeSystem(std::string command)
//Purpose:	Sets the given command as the class command, and executes dangerously (system!)
int  easyExec::executeSystem(std::string command)
{
	commandVal = command;
	return executeSystem();
}

//FUNCTION:	elapsedTime()
//Purpose:	return number of seconds since begining
//		of execution.  If hasn't executed, 
//		returns -1
time_t  easyExec::elapsedTime()
{
	if (!executed)
		return -1;
	else if (!finished)
		return (time(NULL) - startTimeVal);
	else
		return totalTimeVal;
}

//FUNCTION:	startTime()
//Purpose:	return the time command was executed
//		If hasn't executed, return -1
time_t  easyExec::startTime()
{
	if (!executed)
		return -1;
	else
		return startTimeVal;
}

//FUNCTION:	endTime()
//Purpose:	return the time command finished
//		If hasn't finished, return -1
time_t  easyExec::endTime()
{
	if (!finished)
		return -1;
	else
		return endTimeVal;
}

//FUNCTION:	totalRunTime()
//Purpose	return the total time taken executing command
//		If hasn't executed, returns -1.
//		If hasn't finished, returns elapsed time
time_t  easyExec::totalRunTime()
{
	if(!executed)
		return -1;
	else if(!finished)
		return elapsedTime();
	else
		return totalTimeVal;
}

//FUNCTION: 	classDeclarationTime
//Purpose:	return time that class was instantiated
time_t easyExec::classDeclarationTime()
{
	return classDeclarationTimeVal;
}


//FUNCTION:	command()
//Purpose:	return command contained in class
std::string easyExec::command()
{
	return commandVal;
}


//FUNCTION:	parse(char *line, char **argv)
//Purpose	parse the line passed into the char array passed
//		this will later be used with execvp
//		Users of this library should not need to execute this function directly
//		Thanks to MTU.edu's cs441 syllabus for such a petite solution
//		Or to the linux kernel, the crediting is ambiguous


void  easyExec::parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}


