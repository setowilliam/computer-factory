// Milestone 5
// Filename: util.h
// Author:   William Seto
// Date:     April 15 2018
// Description:
/*
Header file for util.cpp. Contains declarations for various utility functions.
*/

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void trim(std::string&, char);
void csvRead(std::string, char, std::vector<std::vector<std::string>>&);
bool taskValid(const std::string);
bool slotValid(const std::string);
bool sequenceValid(const std::string);
bool itemValid(const std::string);