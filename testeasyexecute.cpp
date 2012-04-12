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

/*	This is a test driver program for the easy execute class/library
 *	Just a bunch of unit test to ensure program functionality
 *	Should be pretty easy to figure out, as was the intention of
 *	writing the easyExec class in the first place.
 *	So, no more worrying about blocking vs non-blocking executes, parallel
 *	execution, timing the command executed etc, it's all in here.
 *	Any Questions, feel free to hit hunter@hunterdavis.com
 *
*/

#include "easyexecute.h"

int main(int argc, char* argv[])
{
	//Can we declare an easyExec type?
	easyExec declarationTest;
	
	//Can we retrieve the class instantiation time
	std::cout << "dectest class instantiation time: " << 
	declarationTest.classDeclarationTime() << std::endl;

	//Can we declare an easyExec(std::string command)?
	std::string lscommand = "sleep 3";
	easyExec commanddeclarationTest(lscommand);

	//Can we retrieve the class command from both instantiations?
	std::string retrcommand1, retrcommand2;
	retrcommand1 = declarationTest.command();
	retrcommand2 = commanddeclarationTest.command();
	std::cout << "no command: " << retrcommand1 << std::endl;
	std::cout << "command: " << retrcommand2 << std::endl;
	

	//Can we execute and wait on a command
	commanddeclarationTest.executeWait();
	
	//Can we retrieve the command start, end, and total time
	std::cout << "Execute and Wait till Command Completes\n";
	std::cout << "Command: " << commanddeclarationTest.command() << std::endl;
	std::cout << "Start time: " << commanddeclarationTest.startTime() << std::endl;
	std::cout << "End time: " << commanddeclarationTest.endTime() << std::endl;
	std::cout << "Total time: " << commanddeclarationTest.totalRunTime() << std::endl;


	//can we execute and continue on after a command
	std::cout << "Execute and Continue On Immediately\n";
	commanddeclarationTest.executeAndGo();
	std::cout << "Command: " << commanddeclarationTest.command() << std::endl;
	std::cout << "Start time: " << commanddeclarationTest.startTime() << std::endl;
	std::cout << "End time: " << commanddeclarationTest.endTime() << std::endl;
	std::cout << "Total Time: " << commanddeclarationTest.totalRunTime() << std::endl;

	//can we execute unsafely (system!)
	std::cout << "Execute unsafely with system command\n";
	commanddeclarationTest.executeSystem();
	std::cout << "Command: " << commanddeclarationTest.command() << std::endl;
	std::cout << "Start Time: " << commanddeclarationTest.startTime() << std::endl;
	std::cout << "End Time: " << commanddeclarationTest.endTime() << std::endl;
	std::cout << "Total Time: " << commanddeclarationTest.totalRunTime() << std::endl;



	return 1;
}

