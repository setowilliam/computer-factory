#pragma once
// Milestone 5
// Filename: i.h
// Author:   William Seto
// Date:     April 15 2018
// Description:
/*
Header file for item.cpp. Contains the declarations for the Item and 
ItemManager classes.
*/

#include <iostream>
#include <string>
#include <vector>
#include "util.h"
#include "t.h"

class Item {
	std::string m_item_name;
	std::string m_installer_task;
	std::string m_remover_task;
	std::string m_sequential_code;
	std::string m_description;
public:
	// Default Constructor
	Item();

	// Initializing Constructor
	Item(std::vector<std::string>);

	// printer
	void print();

	//grapher
	void graph(std::fstream&);

	std::string& getItem();
	std::string& getInstaller();
	std::string& getRemover();
};

class ItemManager {
	std::vector<Item> m_item_list;
public:
	// Default Constructor
	ItemManager();

	// Initializing Constructor
	ItemManager(std::vector<std::vector<std::string>>&);

	// printer
	void print();

	// grapher
	void graph(std::string);

	// find
	Item* find(std::string);

	// validate
	bool validate(TaskManager&);

	size_t count() {
		return m_item_list.size();
	}

	Item& getItem(size_t i) {
		return m_item_list[i];
	}
};