/*
 * Created by Emily Ojeda, 08/13/18. 
 * To be used on the rate table for calibrating ground based magnetometer's as part of research conducted by the University of Michigan's Space Research Department.
 * This program reads data from readfile.txt. 
 * The data in the file can be edited by the user, and it is used by the main program to set values of target angle, range of error, and pause time between each test. 
 * The data is then sent to the main program (motor) using the int system(command) command. 
 */
#include <bits/stdc++.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <string>
using namespace std;

int main()
{
	int i = 0;
	int pause = 0;
	ifstream file;
	string data;
	string filename;
	string line;
	string exec_file;
	string command;

	// filename of the file
	filename = "readfile.txt";
	exec_file = "./angle";

	// opening file
	file.open (filename.c_str());
	getline (file, line);
	cout << "skipped " << line << endl; // cout << data << endl;

	// extracting words form the file
	while (file >> data) //while (file >> data)
	{
		if ((i % 3) ==0)
		{
			cout << exec_file << " ";
			command = exec_file + " ";
		}
		cout << data << " ";
		if (i < 2)
		{
			command = command + data + " ";
		}
		if ((i % 3) == 2)
		{
			cout << endl;
			cout << "Emily's command beging: " << command << " End of Emily's command\n";
			system (command.c_str());
		}

		i++;
		if (i > 2)
		{
			pause = stoi(data);
			cout << "Pause Time: "<< pause << "\n";
			cout << endl;
			sleep(pause);
			i = 0;
		}

	}
	cout <<"\n";
	return 0;
}
