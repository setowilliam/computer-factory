// Milestone 5
// Filename: task.cpp
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
#include "t.h"

// Task Class--------------------------------------------------------------------------------------

// Default Constructor
Task::Task() {}

// Initializing Constructor
Task::Task(std::vector<std::string> p_field_list) {
	switch (p_field_list.size()) {
	case 4:
		if (taskValid(p_field_list[3])) {
			m_fail = p_field_list[3];
		}
		else {
			throw std::string("Expected a fail task, found '") + p_field_list[3] + "'";
		}
	case 3:
		if (taskValid(p_field_list[2])) {
			m_pass = p_field_list[2];
		}
		else {
			throw std::string("Expected a pass task, found '") + p_field_list[2] + "'";
		}
	case 2:
		if (slotValid(p_field_list[1])) {
			m_slot = p_field_list[1];
		}
		else {
			throw std::string("Expected a slots, found '") + p_field_list[1] + "'";
		}
	case 1:
		if (taskValid(p_field_list[0])) {
			m_task = p_field_list[0];
		}
		else {
			throw std::string("Expected a task, found '") + p_field_list[0] + "'";
		}
		break;
	default:
		throw std::string("Expected 1, 2, 3, or 4 fields");
		break;
	}
}

// printer
void Task::print() {
	std::cout << "/task/slots/pass/fail = /";
	std::cout << m_task << "/";
	std::cout << m_slot << "/";
	std::cout << m_pass << "/";
	std::cout << m_fail << "/";
	std::cout << std::endl;
}

//grapher
void Task::graph(std::fstream& gv) {
	if (not m_pass.empty()) {
		gv << '"' << m_task << '"' << "->" << '"' << m_pass << '"' << "[color=green];\n";
	}
	if (not m_fail.empty()) {
		gv << '"' << m_task << '"' << "->" << '"' << m_fail << '"' << "[color=red];\n";
	}
	if (m_pass.empty() && m_fail.empty()) {
		gv << '"' << m_task << '"' << "[shape=box];\n";
	}
}

// getPass returns the current string stored in m_pass
std::string& Task::getPass() {
	return m_pass;
}

// getFail returns the current string stored in m_fail
std::string& Task::getFail() {
	return m_fail;
}

// getTask returns the current string stored in m_task
std::string& Task::getTask() {
	return m_task;
}

std::string& Task::getSlot() {
	return m_slot;
}

// TaskManager Class-------------------------------------------------------------------------------

// Default Constructor
TaskManager::TaskManager() {}

// Initializing Constructor
TaskManager::TaskManager(std::vector<std::vector<std::string>>& p_task_list) {
	for (auto& e : p_task_list) {
		try {
			m_task_list.push_back(Task(e));
		}
		catch (std::string& err) {
			std::cerr << "Error: " << err << std::endl;
		}
	}
}

// printer
void TaskManager::print() {
	for (auto& t : m_task_list) {
		t.print();
	}
}

// grapher
void TaskManager::graph(std::string p_filename) {
	std::fstream gv(p_filename, std::ios::out | std::ios::trunc);
	if (gv.is_open()) {
		gv << "digraph task {\n";
		for (auto& t : m_task_list) {
			t.graph(gv);
		}
		gv << "}\n";
		gv.close();

#ifdef __unix
		std::string dot = "dot";
#else
		std::string dot = "C:/\"Program Files (x86)\"/Graphviz2.38/bin/dot.exe";
#endif
		std::string cmd = dot;
		cmd += " -Tpng -O " + p_filename;

		std::cout << "Running command >'" << cmd << "'\n";
		std::cout << "The operating system says dot returned '" << system(cmd.c_str()) << "' (0 is good --> dot executed successfully)\n";
	}
}

// find searches for a given task in the task list. If found, it returns a 
// pointer to the matching task in the task list. If not, it returns
// nullptr.
Task* TaskManager::find(std::string p_task) {
	for (size_t i = 0; i < m_task_list.size(); i++) {
		if (m_task_list[i].getTask() == p_task) {
			return &m_task_list[i];
		}
	}
	return nullptr;
}

// validate checks for referential integrity in the task list.
bool TaskManager::validate() {
	int errors = 0;

	for (auto& t : m_task_list) {
		std::string pass = t.getPass();
		if (not pass.empty()) {
			if (find(pass) == nullptr) {
				errors++;
				std::cerr << "Pass task '" << pass << "; is missing (not found)" << std::endl;
			}
		}

		std::string fail = t.getFail();
		if (not fail.empty()) {
			if (find(fail) == nullptr) {
				errors++;
				std::cerr << "Fail task '" << fail << "' is missing (not found)" << std::endl;
			}
		}
	}
	return errors ? false : true;
}

size_t TaskManager::count() {
	return m_task_list.size();
}

Task& TaskManager::getTask(size_t i) {
	return m_task_list[i];
}