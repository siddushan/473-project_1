//myschedule.cpp
/*Define all the functions in 'myschedule.h' here.*/
#include "myscheduler.h"
#include <algorithm>

bool sort_by_arriving_time(ThreadDescriptorBlock thread1, ThreadDescriptorBlock thread2) {
	return thread1.arriving_time < thread2.arriving_time;
}

bool sort_by_remaining_time(ThreadDescriptorBlock thread1, ThreadDescriptorBlock thread2)
{
	return thread1.remaining_time < thread1.remaining_time;
}


void MyScheduler::CreateThread(int arriving_time, int remaining_time, int priority, int tid) //Thread ID not Process ID
{
	//Function to Create Thread(s) and insert them in the student
	ThreadDescriptorBlock thread = ThreadDescriptorBlock();
	thread.arriving_time = arriving_time;
	thread.remaining_time = remaining_time;
	thread.priority = priority;
	thread.tid = tid;

	thread_list.push_back(thread);
}

bool MyScheduler::Dispatch()
{
	//Todo: Check and remove finished threads
	//Todo: Check if all the threads are finished; if so, return false
	switch (policy)
	{
	case FCFS:		//First Come First Serve

		break;
	case STRFwoP:	//Shortest Time Remaining First, without preemption

		break;
	case STRFwP:	//Shortest Time Remaining First, with preemption

		break;
	case PBS:		//Priority Based Scheduling, with preemption
	{ //Curly braces are here cause I was getting a weird error without them. 
		thread_list.sort(sort_by_arriving_time); //sort the threads by arrival time

		list<ThreadDescriptorBlock>::iterator itr = thread_list.begin();

		while (itr != thread_list.end() && !thread_list.empty()) { //loop through ThreadDescriptorBlocks

			int CPUs_index = 0;

			while ((CPUs_index < num_cpu) && (!thread_list.empty())) {	//examine all threads which have arrived

				if (itr->arriving_time <= timer) {

					if (CPUs[CPUs_index] != NULL) { //if the current CPU is full, check the priority of the task, to see if we need to swap priorities.

						if (CPUs[CPUs_index]->priority > itr->priority) { //lower integer equals higher priority

							//create thread with the attributes of CPUs[i] and add it to thread_list
							ThreadDescriptorBlock thread = ThreadDescriptorBlock();
							thread.arriving_time = CPUs[CPUs_index]->arriving_time;
							thread.priority = CPUs[CPUs_index]->priority;
							thread.remaining_time = CPUs[CPUs_index]->remaining_time;
							thread.tid = CPUs[CPUs_index]->tid;

							thread_list.push_front(thread); //add the thread from CPUs to the thread_list so it can execute again later

							//insert new thread into CPU array
							CPUs[CPUs_index] = new ThreadDescriptorBlock();
							CPUs[CPUs_index]->arriving_time = itr->arriving_time;
							CPUs[CPUs_index]->priority = itr->priority;
							CPUs[CPUs_index]->remaining_time = itr->remaining_time;
							CPUs[CPUs_index]->tid = itr->tid;

							itr = thread_list.erase(itr);

							if (thread_list.size() > 0) {
								itr = thread_list.begin();
							}
						}
					}
					else {

						CPUs[CPUs_index] = new ThreadDescriptorBlock();
						CPUs[CPUs_index]->arriving_time = itr->arriving_time;
						CPUs[CPUs_index]->priority = itr->priority;
						CPUs[CPUs_index]->remaining_time = itr->remaining_time;
						CPUs[CPUs_index]->tid = itr->tid;

						itr = thread_list.erase(itr);

						if (thread_list.size() > 0) {
							itr = thread_list.begin();
						}

					}
				}

				if (thread_list.size() < 0) {
					break;
				}
				else {
					CPUs_index++; //increment index of the CPUs array
				}

			}

			if (thread_list.size() > 0) {
				++itr; //iterate thread_list done in the outer while loop
			}
		}

		break;
	}
	default:
		cout << "Invalid policy!";
		throw 0;
	}
	return true;
}
