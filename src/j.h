// Milestone 5
// Filename: j.h
// Author: William Seto
// Date: April 15 2018

#pragma once

#include "o.h"

class Job : public Order {
	std::vector<bool> m_installed; // data element that flags whether an item has been installed
public:
	Job() {}
	Job(Order& o) : Order(o) {
		// initialize m_installed to be false for all items
		for (size_t i = 0; i < getItemsOrderedSize(); i++) {
			m_installed.push_back(false);
		}
	}

	void setInstalled(int i, bool v) {
		m_installed[i] = v;
	}

	bool getInstalled(int i) {
		return m_installed[i];
	}

	bool complete() {
		for (auto e : m_installed) {
			if (not e) {
				return false;
			}
		}
		return true;
	}
 };