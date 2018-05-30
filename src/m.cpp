// Milestone 5
// Filename: m.cpp
// Author: William Seto
// Date: April 15 2018

#include "m.h"

// Default constructor
Machine::Machine() {}

// source-sink functions
// Initializing constructor.
Machine::Machine(Task& t) : Task(t) {}
void Machine::incIncoming() {
	incoming++;
}
bool Machine::isSource() {
	return incoming == 0;
}
bool Machine::isSink() {
	return getPass().empty() and getFail().empty();
}

bool Machine::isInstaller() {
	return bInstaller;
}

bool Machine::isRemover() {
	return bRemover;
}

// jobQ functions
void Machine::addJob(Job&& p_job_queue) {
	m_job_queue.push(std::move(p_job_queue));
}

size_t Machine::jobCount() {
	return m_job_queue.size();
}

Job* Machine::removeCurrentJob() {
	Job* temp = new Job(m_job_queue.front());
	m_job_queue.pop();
	return temp;
}