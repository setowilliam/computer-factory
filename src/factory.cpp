// Milestone 5
// Filename: factory.cpp
// Author: William Seto
// Date: April 15 2018

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "util.h"
#include "i.h"
#include "o.h"
#include "t.h"
#include "m.h"
#include "j.h"

// Factory Class
class Factory {
	std::vector<Machine> m_machine_list;
public:
	Factory(ItemManager& im, OrderManager& om, TaskManager& tm) {
		// step 1 - for each task create a machine on the m_machine_list
		for (size_t i = 0; i < tm.count(); i++) {
			m_machine_list.push_back(tm.getTask(i));
		}
#if (1)
		for (auto& m : m_machine_list) { // Print out names just to check
			std::cout << m.getTask() << std::endl;
		}
#endif
		// step 2, Identify sink node
		for (auto& m : m_machine_list) {
			auto find = [&](std::string name) -> Machine* {
				for (size_t i = 0; i < m_machine_list.size(); i++) {
					if (m_machine_list[i].getTask() == name) {
						return &m_machine_list[i];
					}
				}
				return nullptr;
			};

			std::string pass = m.getPass();
			std::string fail = m.getFail();
			if (not pass.empty()) { // increment incoming for pass
				find(pass)->incIncoming();
			}
			if (not fail.empty()) { // increment incoming for fail
				find(fail)->incIncoming();
			}
		}

#if (1)
		for (auto& m : m_machine_list) {
			std::cout << m.getTask();
			if (m.isSource()) {
				std::cout << " <-- SOURCE";
			}
			if (m.isSink()) {
				std::cout << " <-- SINK";
			}
			std::cout << std::endl;
		}
#endif

		int sourceNode = -1;
		for (size_t i = 0; i < m_machine_list.size(); i++) {
			if (m_machine_list[i].isSource()) {
				if (sourceNode == -1) {
					sourceNode = i; // First source node we have found
				}
				else { // There are multiple nodes
					throw std::string("multiple source nodes. Fix task data and resubmit!");
				}
			}
		}
		if (sourceNode == -1) {
			throw std::string("missing source node. Fix task data and resubmit!");
		}
		std::cout << "SourceNode = " << sourceNode << " (" << m_machine_list[sourceNode].getTask() << ")" << std::endl;

		// step 3. add jobs onto sourceNode jobQ
		for (size_t i = 0; i < om.count(); i++) {
			m_machine_list[sourceNode].addJob(std::move(Job(om.getOrder(i))));
		}

		auto activeMachines = [&] {
			int ret = 0;
			for (auto& m : m_machine_list) {
				if (m.jobCount()) {
					ret += m.jobCount();
					//std::cout << m.getTask() << " has " << m.jobCount() << " jobs." << std::endl;
				}
			}
			return ret;
		};
		
		for (auto& e : m_machine_list) {
			e.classify(im);
		}

		// continue looping while there are incomplete jobs
		int time = 0;
		while (true) {
			if (activeMachines() == 0) {
				break;
			}

			for (auto& m : m_machine_list) {
				if (m.jobCount() == 0) {
					continue;
				}
				Job job = m.getJob();

				if (m.isSink()) {
					if (job.complete()) {
						std::cout << "Job " << job.getProductName() << " for " << job.getCustomerName() << " is complete at " << m.getTask() << " (" << time << " seconds)" << std::endl;
					}
					else {
						std::cout << "Job " << job.getProductName() << " for " << job.getCustomerName() << " is not complete. Rerouting back to sourceNode" << std::endl;
						m_machine_list[sourceNode].addJob(std::move(job));
					}
				}

				bool didSomething = false;
				if (m.isInstaller()) {
					for (size_t i = 0; i < job.getItemsOrderedSize(); i++) {
						if (job.getInstalled(i) == true) {
							continue;
						}
						std::string item = job.getItem(i);
						std::string installer = im.find(item)->getInstaller();
						if (installer == m.getTask()) {
							job.setInstalled(i, true);
							didSomething = true;
							time += std::stoi(m.getSlot());
							break;
						}
					}
				}

				if (m.isRemover()) {
					for (size_t i = job.getItemsOrderedSize() - 1; i >= 0;  i--) {
						if (job.getInstalled(i) == false) {
							continue;
						}
						std::string item = job.getItem(i);
						std::string remover = im.find(item)->getRemover();
						if (remover == m.getTask()) {
							job.setInstalled(i, false);
							didSomething = true;
							time += std::stoi(m.getSlot());
							break;
						}
					}
				}

				auto route = [&](std::string name) {
					for (auto& m : m_machine_list) {
						if (m.getTask() == name) {
							m.addJob(std::move(job));
							break;
						}
					}
				};
				std::string pass = m.getPass();
				std::string fail = m.getFail();

				if (not didSomething) {
					route(pass);
					continue;
				}

				if (fail.empty()) {
					route(pass);
					continue;
				}

				int random = (rand() % (10 + 1));
				if (random > 5) {
					route(pass);
					continue;
				}
				else {
					route(fail);
					continue;
				}

			} // bottom of machine loop
		} // end of time loop
		std::cout << "All done" << std::endl;
	}
};
void app(int argc, char* argv[]) {
	std::srand(std::time(nullptr));
	if (argc != 5) {
		throw std::string("usage: ") + argv[0] + " item order task delim\n";
	}

	std::string filenameItem = argv[1];
	std::string filenameOrder = argv[2];
	std::string filenameTask = argv[3];
	char delim = argv[4][0];

	std::vector<std::vector<std::string>> dataItem;
	std::vector<std::vector<std::string>> dataOrder;
	std::vector<std::vector<std::string>> dataTask;

	csvRead(filenameItem, delim, dataItem);
	csvRead(filenameOrder, delim, dataOrder);
	csvRead(filenameTask, delim, dataTask);

	ItemManager im(dataItem);
	OrderManager om(dataOrder);
	TaskManager tm(dataTask);

	im.print();
	om.print();
	tm.print();

	im.graph(filenameItem + ".gv");
	om.graph(filenameOrder + ".gv");
	tm.graph(filenameTask + ".gv");

	if (im.validate(tm) and tm.validate() and om.validate(im)) {
		std::cout << "Good to go! --- no errors" << std::endl;
	}
	else {
		throw std::string("An error occurred");
	}

	Factory factory(im, om, tm);
}

int main(int argc, char** argv) {
	try {
		app(argc, argv);
		std::cout << "Press enter to exit...\n";
		std::cin.get();
	}
	catch (const std::string& e) {
		std::cerr << "It threw: " + e << std::endl;
	}
}