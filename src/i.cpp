// Milestone 5
// Filename: i.cpp
// Author:   William Seto
// Date:     April 15 2018
// Description:
/*
Implementation file for i.h. Contains the Item and ItemManager classes. Draws 
a graphviz image.
*/

#include <iostream>
#include <string>
#include <vector>
#include "util.h"
#include "i.h"

// Item Class--------------------------------------------------------------------------------------
// Default Constructor
Item::Item() {}

// Initializing Constructor
Item::Item(std::vector<std::string> p_field_list) {
	switch (p_field_list.size()) {
	case 5:
		m_description = p_field_list[4];
	case 4:
		// sequence
		if (sequenceValid(p_field_list[3])) {
			m_sequential_code = p_field_list[3];
		}
		else {
			throw std::string("Expected a sequential code, found '") + p_field_list[3] + "'";
		}

		// remover
		if (taskValid(p_field_list[2])) {
			m_remover_task = p_field_list[2];
		}
		else {
			throw std::string("Expected a remover task, found '") + p_field_list[2] + "'";
		}

		// installer
		if (taskValid(p_field_list[1])) {
			m_installer_task = p_field_list[1];
		}
		else {
			throw std::string("Expected an installer task, found '") + p_field_list[1] + "'";
		}
			
		// item name
		if (itemValid(p_field_list[0])) {
			m_item_name = p_field_list[0];
		}
		else {
			throw std::string("Expected an item name, found '") + p_field_list[0] + "'";
		}
		break;
	default:
		throw std::string("Expected 4 or 5 fields, found") + std::to_string(p_field_list.size());
		break;
	}
}
	
// printer
void Item::print() {
	std::cout << "item/installer/remover/sequence/description = /";
	std::cout << m_item_name << "/";
	std::cout << m_installer_task << "/";
	std::cout << m_remover_task << "/";
	std::cout << m_sequential_code << "/";
	std::cout << m_description << "/";
	std::cout << std::endl;
}

//grapher
void Item::graph(std::fstream& gv) {
	gv << '"' << "Item\n" << m_item_name << '"' << "->" << '"' << "Installer\n" << m_installer_task << '"' << "[color=green];\n";

	gv << '"' << "Item\n" << m_item_name << '"' << "->" << '"' << "Remover\n" << m_remover_task << '"' << "[color=red];\n";
}

// getItem returns the string stored in m_item_name
std::string& Item::getItem() {
	return m_item_name;
}

// getInstaller returns the string stored in m_installer_task
std::string& Item::getInstaller() {
	return m_installer_task;
}

// getRemover returns the string stored in m_remover_task
std::string& Item::getRemover() {
	return m_remover_task;
}

// ItemManager Class-------------------------------------------------------------------------------

// Default Constructor
ItemManager::ItemManager() {}

// Initializing Constructor
ItemManager::ItemManager(std::vector<std::vector<std::string>>& p_item_list) {
	for (auto& e : p_item_list) {
		try {
			m_item_list.push_back(Item(e));
		}
		catch (std::string& err) {
			std::cerr << "Error: " << err << std::endl;
		}
	}
}

// printer
void ItemManager::print() {
	for (auto& i : m_item_list) {
		i.print();
	}
}

// grapher
void ItemManager::graph(std::string p_filename) {
	std::fstream gv(p_filename, std::ios::out | std::ios::trunc);
	if (gv.is_open()) {
		gv << "digraph item {\n";
		for (auto& i : m_item_list) {
			i.graph(gv);
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

// find
Item* ItemManager::find(std::string p_item_name) {
	for (size_t i = 0; i < m_item_list.size(); i++) {
		if (m_item_list[i].getItem() == p_item_name) {
			return &m_item_list[i];
		}
	}
	return nullptr;
}

// validate
bool ItemManager::validate(TaskManager& tm) {
	int errors = 0;
	for (auto& i : m_item_list) {
		std::string installer = i.getInstaller();
		if (tm.find(installer) == nullptr) {
			errors++;
			std::cerr << "Installer task '" << installer << "' is missing (not found)" << std::endl;
		}

		std::string remover = i.getRemover();
		if (tm.find(remover) == nullptr) {
			errors++;
			std::cerr << "Remover task '" << remover << "' is missing (not found)" << std::endl;
		}
	}
	return errors ? false : true;
}