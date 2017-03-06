#pragma once
//myschedule.h
/* Students need to define their own data structure to contain
   and access objects from 'thread class'. The 'CreateThread()' 
   function should insert the threads into the student defined
   data structure after creating them.
   They are allowed to add any additional functionality (only 
   declaration in this file, define in 'myschedule.cpp')
   which they might find helpful.*/
//NAMES
//Sidd Shanmugam
//Albert Wilson
//Ryan Byrne
#include "scheduler.h"
#include <list>

//Define your data structure here.

class MyScheduler: public Scheduler {
public:
	MyScheduler(Policy p, unsigned int n) : Scheduler(p, n) {}
	bool Dispatch() override; //Function to implement scheduling policy and to keep a check on processed threads
	void CreateThread(int arriving_time, int remaining_time, int priority, int tid) override; //Function to create threads and insert them in student defined data structure

	//Declare additional methods(s) below if needed.
	bool compareTimeRemaining(ThreadDescriptorBlock thread01, ThreadDescriptorBlock thread02);
	bool comparePriority(ThreadDescriptorBlock thread01, ThreadDescriptorBlock thread02);
	void addThreadtoCPU(list<ThreadDescriptorBlock>::iterator iter, int index);
	void removeThreadFromCPUAddToList(int index);
	
private:
	list<ThreadDescriptorBlock> thread_list;
};
