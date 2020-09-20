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

int loadData(const char* filename, bool ignoreFirstRow) {
	ifstream myfile;
	myfile.open(filename);
	if (!myfile.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}

	string line;
	while (getline(myfile, line)) {
		istringstream ss(line);
		process_stats temp;
		string tempVal;
		if (ignoreFirstRow) {
			string ignoreLine;
			//delete the first row of data
			getline(ss, ignoreLine);
		}
		getline(ss, tempVal, ',');
		if (tempVal == "" || (tempVal.find(" ") != -1)) {
			//skip to next row, bad data
			continue;
		}
		temp.process_number = stringToInt(tempVal.c_str());
		getline(ss, tempVal, ',');
		if (tempVal == "" || (tempVal.find(" ") != -1)) {
			//skip to next row, bad data
			continue;
		}
		temp.start_time = stringToInt(tempVal.c_str());
		getline(ss, tempVal, ',');
		if (tempVal == "" || (tempVal.find(" ") != -1)) {
			//skip to next row, bad data
			continue;
		}
		temp.cpu_time = stringToInt(tempVal.c_str());
		getline(ss, tempVal, ',');
		if (tempVal == "" || (tempVal.find(" ") != -1)) {
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
	for (int i = 0; i < data.size()-1; i++) {
		for (int j = 1; j < data.size(); j++) {
			process_stats a = data[i];
			process_stats b = data[j];
			auto compare = [](const process_stats &a, const process_stats &b) {
				return a.process_number < b.process_number;};
			sort(begin(data),end(data), compare);
		}
	}
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


