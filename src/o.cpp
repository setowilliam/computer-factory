// Milestone 5
// Filename: o.cpp
// Author:   William Seto
// Date:     April 15 2018
// Description:
/*
Contains the Order and OrderManager classes. Draws a graphviz image.
*/

#include <iostream>
#include <string>
#include <vector>
#include "util.h"
#include "o.h"
#include "i.h"

// Order Class-------------------------------------------------------------------------------------

// Default Constructor
Order::Order() {}

// Initializing Constructor
Order::Order(std::vector<std::string> p_field_list) {
	if (p_field_list.size() < 3) {
		throw std::string("Expected 3 or more fields, found") + std::to_string(p_field_list.size());
	}

	// customer name
	m_customer_name = p_field_list[0];

	// product name
	m_product_name = p_field_list[1];

	// order list
	for (int i = 2; i < p_field_list.size(); i++) {
		if (itemValid(p_field_list[i])) {
			m_items_ordered.push_back(p_field_list[i]);
		}
		else {
			throw std::string("expected a order, found '") + p_field_list[i] + "'";
		}
	}
}

// printer
void Order::print() {
	std::cout << "customer/product/items ordered .../ = /";
	std::cout << m_customer_name << "/";
	std::cout << m_product_name << "/";
	for (auto& item : m_items_ordered) {
		std::cout << item << "/";
	}
	std::cout << std::endl;
}

//grapher
void Order::graph(std::fstream& gv) {
	for (auto& item : m_items_ordered) {
		gv << '"' << m_customer_name << "\n" << m_product_name << '"' << "->" << '"' << "Item\n" << item << '"' << "[color=blue];\n";
	}
}

size_t Order::getItemsOrderedSize() {
	return m_items_ordered.size();
}

std::string& Order::getItem(int i) {
	return m_items_ordered[i];
}

std::string Order::getCustomerName() {
	return m_customer_name;
}

std::string Order::getProductName() {
	return m_product_name;
}

//-------------------------------------------------------------------------------------------------

// OrderManager Class------------------------------------------------------------------------------

// Default Constructor
OrderManager::OrderManager() {}

// Initializing Constructor
OrderManager::OrderManager(std::vector<std::vector<std::string>>& p_order_list) {
	for (auto& e : p_order_list) {
		try {
			m_order_list.push_back(Order(e));
		}
		catch (std::string& err) {
			std::cerr << "Error: " << err << std::endl;
		}
	}
}

// printer
void OrderManager::print() {
	for (auto& o : m_order_list) {
		o.print();
	}
}

// grapher
void OrderManager::graph(std::string p_filename) {
	std::fstream gv(p_filename, std::ios::out | std::ios::trunc);
	if (gv.is_open()) {
		gv << "digraph order {\n";
		for (auto& o : m_order_list) {
			o.graph(gv);
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

// validate
bool OrderManager::validate(ItemManager& im) {
	int errors = 0;
	for (auto& o : m_order_list) {
		for (size_t i = 0; i < o.getItemsOrderedSize(); i++) {
			if (im.find(o.getItem(i)) == nullptr) {
				errors++;
				std::cerr << "Cannot find item '" << o.getItem(i) << "' (missing)" << std::endl;
			}
		}
	}
	return errors ? false : true;
}

size_t OrderManager::count() {
	return m_order_list.size();
}

Order& OrderManager::getOrder(size_t i) {
	return m_order_list[i];
}

//-------------------------------------------------------------------------------------------------