#pragma once
// Milestone 5
// Filename: t.h
// Author:   William Seto
// Date:     April 15 2018
// Description:
/*
Contains the Task and Task Manager classes. Draws a graphviz image depicting the task routing information.
*/

#include <iostream>
#include <string>
#include <vector>
#include "util.h"

class Task {
	std::string m_task;
	std::string m_slot;
	std::string m_pass;
	std::string m_fail;
public:
	// Default Constructor
	Task();

	// Initializing Constructor
	Task(std::vector<std::string>);

	// printer
	void print();

	//grapher
	void graph(std::fstream&);

	std::string& getPass();
	std::string& getFail();
	std::string& getTask();
	std::string& getSlot();
};

class TaskManager {
	std::vector<Task> m_task_list;
public:
	// Default Constructor
	TaskManager();

	// Initializing Constructor
	TaskManager(std::vector<std::vector<std::string>>&);

	// printer
	void print();

	// grapher
	void graph(std::string);

	// find
	Task* find(std::string);

	// validate
	bool validate();

	size_t count();

	Task& getTask(size_t i);
};
