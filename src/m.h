// Milestone 5
// Filename: m.h
// Author: William Seto
// Date: April 15 2018

#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include "j.h"
#include "t.h"
#include "i.h"

class Machine : public Task {
	int incoming = 0; // C++14
	std::queue<Job> m_job_queue;
	bool bInstaller = false;
	bool bRemover = false;
public:
	Machine();

	// source-sink functions
	Machine(Task&);
	void incIncoming();
	bool isSource();
	bool isSink();

	bool isInstaller();
	bool isRemover();
	
	// jobQ functions
	void addJob(Job&&);
	size_t jobCount();
	Job* removeCurrentJob();

	Job getJob() {
		Job j = std::move(m_job_queue.front());
		m_job_queue.pop();
		return std::move(j);
	};

	void classify(ItemManager& im) {
		for (size_t i = 0; i < im.count(); i++) {
			std::string installer = im.getItem(i).getInstaller();
			if (installer == getTask()) {
				bInstaller = true;
			}

			std::string remover = im.getItem(i).getRemover();
			if (remover == getTask()) {
				bRemover = true;
			}

			if (bInstaller and bRemover) {
				throw std::string(" machine ") + getTask() + " is both an installer and a remover. Fix item data and resubmit.\n";
			}
		}
	}
};