#pragma once
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
#include "i.h"

class Order {
	std::string m_customer_name;
	std::string m_product_name;
	std::vector<std::string> m_items_ordered;
public:
	// Default Constructor
	Order();

	// Initializing Constructor
	Order(std::vector<std::string> p_field_list);

	// printer
	void print();

	//grapher
	void graph(std::fstream& gv);

	size_t getItemsOrderedSize();
	std::string& getItem(int);

	std::string getCustomerName();
	std::string getProductName();
};

class OrderManager {
	std::vector<Order> m_order_list;
public:
	// Default Constructor
	OrderManager();

	// Initializing Constructor
	OrderManager(std::vector<std::vector<std::string>>& p_order_list);

	// printer
	void print();

	// grapher
	void graph(std::string p_filename);

	// validate
	bool validate(ItemManager&);

	size_t count();

	Order& getOrder(size_t i);
};