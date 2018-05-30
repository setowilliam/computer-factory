// Milestone 5
// Filename: util.cpp
// Author:   William Seto
// Date:     April 15 2018
// Description:
/*
Implementation file for util.h. Contains the definitions for the various utility
functions.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "util.h"

void trim(std::string& s, char trimChar = ' ')
{
	while (not s.empty() and s[0] == trimChar)
		s.erase(0, 1);

	while (not s.empty() and s[s.size() - 1] == trimChar)
		s.erase(s.size() - 1, 1);
}

void csvRead(std::string filename, char delim, std::vector<std::vector<std::string>> &csv) {
	std::ifstream fin(filename);
	if (fin) {
		std::string line;
		std::vector<std::string> fields;
		while (std::getline(fin, line)) {
			auto cr = line.find('\r');
			if (cr != std::string::npos) {
				line.erase(cr, 1);
			}
			std::string field;
			for (size_t i = 0; i < line.size(); i++) {
				if (line[i] != delim) {
					field += line[i];
				}
				else {
					trim(field);
					fields.push_back(move(field)); // push_back the field onto vector of columns
				}
			}
			trim(field);
			fields.push_back(move(field)); // push_back the last field onto vector of columns
			csv.push_back(move(fields)); // push_back the row to the vector of rows
		}
		fin.close();
	}
	else {
		throw std::string("Cannot open file '") + filename + "'";
	}
}

bool taskValid(const std::string s) {
	if (s.empty()) {
		return false;
	}

	for (auto c : s) {
		if (not (isalpha(c) or c == ' ' or c == '-' or c == '#')) {
			return false;
		}
	}

	return true;
}

bool slotValid(const std::string s) {
	for (auto c : s) {
		if (not (isdigit(c) or c == ' ' or c == '-' or c == '#')) {
			return false;
		}
	}

	return true;
}

bool sequenceValid(const std::string s) {
	for (auto c : s) {
		if (not (isdigit(c))) {
			return false;
		}
	}
	return true;
}

bool itemValid(const std::string s) {
	if (s.empty()) {
		return false;
	}
	return true;
}