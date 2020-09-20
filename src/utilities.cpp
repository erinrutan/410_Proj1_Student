/***
 * KP Utils
 */
#include <numeric>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include "utilities.h"
#include "stdlib.h"

using namespace std;

//********************** private to this compilation unit **********************

//if myString does not contain a string rep of number returns o
//if int not large enough has undefined behaviour, very fragile
int stringToInt(const char *myString) {
	return atoi(myString);
}

std::vector<process_stats> data;

void trim(string& s) {
   size_t p = s.find_first_not_of(" \t");
   s.erase(0, p);

   p = s.find_last_not_of(" \t");
   if (string::npos != p)
      s.erase(p+1);
}

int loadData(const char* filename, bool ignoreFirstRow) {
	ifstream myfile;
	myfile.open(filename);
	if (!myfile.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}

	data.clear();
	string line;
	if (ignoreFirstRow) {
		string ignoreLine;
		getline(myfile, ignoreLine);
	}
	while (getline(myfile, line)) {
		istringstream ss(line);
		process_stats temp;
		string tempVal;

		getline(ss, tempVal, ',');
		trim(tempVal);
		if (tempVal == "" || (tempVal.find(" ") != string::npos)) {
			//skip to next row, bad data
			continue;
		}
		temp.process_number = stringToInt(tempVal.c_str());
		getline(ss, tempVal, ',');
		trim(tempVal);
		if (tempVal == "" || (tempVal.find(" ") != string::npos)) {
			//skip to next row, bad data
			continue;
		}
		temp.start_time = stringToInt(tempVal.c_str());
		getline(ss, tempVal, ',');
		trim(tempVal);
		if (tempVal == "" || (tempVal.find(" ") != string::npos)) {
			//skip to next row, bad data
			continue;
		}
		temp.cpu_time = stringToInt(tempVal.c_str());
		getline(ss, tempVal, ',');
		trim(tempVal);
		if (tempVal == "" || (tempVal.find(" ") != string::npos)) {
			//skip to next row, bad data
			continue;
		}
		temp.io_time = stringToInt(tempVal.c_str());
		data.push_back(temp);
	}

	myfile.close();
	return SUCCESS;
}


//will sort according to user preference
void sortData(SORT_ORDER mySortOrder) {
	auto compare = [mySortOrder](const process_stats &a, const process_stats &b) {
		switch (mySortOrder) {
				case SORT_ORDER::PROCESS_NUMBER:
					return a.process_number < b.process_number;
				case SORT_ORDER::START_TIME:
					return a.start_time < b.start_time;
				case SORT_ORDER::CPU_TIME:
					return a.cpu_time < b.cpu_time;
				case SORT_ORDER::IO_TIME:
					return a.io_time < b.io_time;
				default:
					break;
			}
		};
	sort(begin(data),end(data), compare);
}

process_stats getNext() {
	process_stats myFirst;
	myFirst = data.front();
	data.erase(data.begin());
	return myFirst;
}

//returns number of process_stats structs in the vector holding them
int getNumbRows(){
	return data.size();
}
