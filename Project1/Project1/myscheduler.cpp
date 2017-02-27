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

			if (itr->arriving_time <= timer) {	//examine all threads which have arrived

				for (int i = 0; i < num_cpu && !thread_list.empty(); i++) { //loop through cpus

					if (CPUs[i] != NULL) { //if the current CPU is full, check the priority of the task, to see if we need to swap priorities.

						if (CPUs[i]->priority > itr->priority) { //lower integer equals higher priority

							//create thread with the attributes of CPUs[i] and add it to thread_list
							ThreadDescriptorBlock thread = ThreadDescriptorBlock();
							thread.arriving_time = CPUs[i]->arriving_time;
							thread.priority = CPUs[i]->priority;
							thread.remaining_time = CPUs[i]->remaining_time;
							thread.tid = CPUs[i]->tid;

							thread_list.push_front(thread); //add the thread from CPUs to the thread_list so it can execute again later

							//insert new thread into CPU array
							CPUs[i] = new ThreadDescriptorBlock();
							CPUs[i]->arriving_time = itr->arriving_time;
							CPUs[i]->priority = itr->priority;
							CPUs[i]->remaining_time = itr->remaining_time;
							CPUs[i]->tid = itr->tid;

							itr = thread_list.erase(itr);

							if (thread_list.size() > 0) {
								itr = thread_list.begin();
							}
						}
					}
					else {

						CPUs[i] = new ThreadDescriptorBlock();
						CPUs[i]->arriving_time = itr->arriving_time;
						CPUs[i]->priority = itr->priority;
						CPUs[i]->remaining_time = itr->remaining_time;
						CPUs[i]->tid = itr->tid;

						itr = thread_list.erase(itr);

						if (thread_list.size() > 0) {
							itr = thread_list.begin();
						}

					}
				}
			}
			else { //if we are looking at Threads that have not arrived yet
				break;
			}

			if (thread_list.size() > 0)
				++itr;
		}

		break;
	}
	default:
		cout << "Invalid policy!";
		throw 0;
	}
	return true;
}
