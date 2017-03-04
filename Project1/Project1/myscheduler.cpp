//myschedule.cpp
/*Define all the functions in 'myschedule.h' here.*/
#include "myscheduler.h"
#include <algorithm>

bool sort_by_arriving_time(ThreadDescriptorBlock thread1, ThreadDescriptorBlock thread2) {
	return thread1.arriving_time < thread2.arriving_time;
}

bool sort_by_remaining_time(ThreadDescriptorBlock thread1, ThreadDescriptorBlock thread2)
{
	return thread1.remaining_time < thread2.remaining_time;
}

void MyScheduler::addThreadtoCPU(list<ThreadDescriptorBlock>::iterator iter, int index) {

	//insert new thread into CPU array
	CPUs[index] = new ThreadDescriptorBlock();
	CPUs[index]->arriving_time = iter->arriving_time;
	CPUs[index]->priority = iter->priority;
	CPUs[index]->remaining_time = iter->remaining_time;
	CPUs[index]->tid = iter->tid;

}

void MyScheduler::removeThreadFromCPUAddToList(int index) {

	//create thread with the attributes of CPUs[i] and add it to thread_list
	ThreadDescriptorBlock thread = ThreadDescriptorBlock();
	thread.arriving_time = CPUs[index]->arriving_time;
	thread.priority = CPUs[index]->priority;
	thread.remaining_time = CPUs[index]->remaining_time;
	thread.tid = CPUs[index]->tid;

	thread_list.push_front(thread); //add the thread from CPUs to the thread_list so it can execute again later
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
	if (thread_list.empty())
	{
		bool isDone = true;
		for (unsigned int i = 0; i < num_cpu; i++)
		{
			if (CPUs[i] != NULL)
			{
				isDone = false;
				break;
			}
		}
		if (isDone)
			return false;
	}
	
	switch (policy)
	{
	case FCFS:		//First Come First Serve
	{
		thread_list.sort(sort_by_arriving_time); //sort by arrival time
		
		for (list<ThreadDescriptorBlock>::iterator itr = thread_list.begin(); itr != thread_list.end() && !thread_list.empty();)
		{
			if (itr->arriving_time <= timer)
			{
				for (unsigned int i = 0; i < num_cpu; i++)
				{
					if (CPUs[i] == NULL)
					{
						addThreadtoCPU(itr, i);
						itr = thread_list.erase(itr);
						itr = thread_list.begin();
						break;
					}
				}
			}
			else
				itr++;
		}
	}
	break;
	case STRFwoP:	//Shortest Time Remaining First, without preemption
	{
		thread_list.sort(sort_by_remaining_time);
		for (list<ThreadDescriptorBlock>::iterator itr = thread_list.begin(); itr != thread_list.end() && !thread_list.empty();)
		{
			if (itr->arriving_time <= timer)
			{
				for (unsigned int i = 0; i < num_cpu; i++)
				{
					if (CPUs[i] == NULL)
					{
						addThreadtoCPU(itr, i);
						itr = thread_list.erase(itr);
						itr = thread_list.begin();
						break;
					}
				}
				if (itr == thread_list.end())
				{
					break;
				}
				else
					itr++;
			}
			else
				itr++;
		}
	}
		break;
	case STRFwP:{	//Shortest Time Remaining First, with preemption
		list<ThreadDescriptorBlock>::iterator itr_srtf;// iterator through thread list
		ThreadDescriptorBlock thread_off = ThreadDescriptorBlock();
		int current_cpu = 0;
		bool cpu_avail=false;

		thread_list.sort(sort_by_arriving_time);//sort thread list according to arrival time
		itr_srtf = thread_list.begin();
		
		while (itr_srtf != thread_list.end() && !thread_list.empty()) {//
			if (itr_srtf->arriving_time <= timer) {//only considering threads that have arrived
				//iterate through cpus to check availability
				for (current_cpu = 0; (current_cpu < num_cpu)&& (thread_list.size() != 0)&&(itr_srtf->arriving_time <= timer); current_cpu++) {	
				
					cpu_avail = false;

					for (int t = 0; t < num_cpu; t++) {//before atempting to swap chek if there is an open cpu
						if (CPUs[t] == NULL) {
							cpu_avail = true;
							current_cpu = t;//set current cpu to the one available
						}
					}
					if (!cpu_avail&&CPUs[current_cpu] != NULL) {// check if anything is on the cpu if none are open
							
						if (CPUs[current_cpu]->remaining_time>itr_srtf->remaining_time) {//check if swap is needed otherwse do nothing

							thread_off.tid = CPUs[current_cpu]->tid;
							thread_off.arriving_time = CPUs[current_cpu]->arriving_time;
							thread_off.remaining_time = CPUs[current_cpu]->remaining_time;
							thread_off.priority = CPUs[current_cpu]->priority;

							thread_list.push_front(thread_off);//thread removed from cpu added back to front of queue

							CPUs[current_cpu] = new ThreadDescriptorBlock();
							CPUs[current_cpu]->tid = itr_srtf->tid;
							CPUs[current_cpu]->remaining_time = itr_srtf->remaining_time;
							CPUs[current_cpu]->priority = itr_srtf->priority;
							CPUs[current_cpu]->arriving_time = itr_srtf->arriving_time;
								
							itr_srtf = thread_list.erase(itr_srtf);
									
							if (!thread_list.empty()&&itr_srtf == thread_list.end()) {//go to next thread in queue if possible
								itr_srtf = thread_list.begin();
							}
						}
					}
					else {//Put thread on cpu if nothing is on it
						CPUs[current_cpu] = new ThreadDescriptorBlock();
						CPUs[current_cpu]->tid = itr_srtf->tid;
						CPUs[current_cpu]->remaining_time = itr_srtf->remaining_time;
						CPUs[current_cpu]->priority = itr_srtf->priority;
						CPUs[current_cpu]->arriving_time = itr_srtf->arriving_time;

						itr_srtf = thread_list.erase(itr_srtf);
						cpu_avail = false;
						if (!thread_list.empty()&&itr_srtf==thread_list.end()) {//go to next thread in queue if possible
							itr_srtf = thread_list.begin();
						}
					}		
				}
			}
				
			else { break; }//break to avoid threads that have not arrived yet
			if (thread_list.size() > 0 &&!cpu_avail) {//go to next thread in queue if possible
				++itr_srtf;
			}		
		}

		
		if (thread_list.empty()) { return false; }//return false all threads have completed
			
	}
		break;
	case PBS:		//Priority Based Scheduling, with preemption
	{ //Curly braces are here cause I was getting a weird error without them. 
		thread_list.sort(sort_by_arriving_time); //sort the threads by arrival time

		list<ThreadDescriptorBlock>::iterator itr = thread_list.begin();

		while (itr != thread_list.end() && !thread_list.empty()) { //loop through ThreadDescriptorBlocks

			int CPUs_index = 0;

			while ((CPUs_index < num_cpu) && (!thread_list.empty())) {	

				if (itr->arriving_time <= timer) { //examine all threads which have arrived

					if (CPUs[CPUs_index] != NULL) { //if the current CPU is full, check the priority of the task, to see if we need to swap priorities.

						if (CPUs[CPUs_index]->priority > itr->priority) { //lower integer equals higher priority

							removeThreadFromCPUAddToList(CPUs_index);

							addThreadtoCPU(itr, CPUs_index);

							itr = thread_list.erase(itr);

							if (thread_list.size() > 0) {
								itr = thread_list.begin();
								continue; //this continue statement loops back to the outer while loop early
							}
						}
					}
					else {

						addThreadtoCPU(itr, CPUs_index);

						itr = thread_list.erase(itr);

						if (thread_list.size() > 0) {
							itr = thread_list.begin();
							continue; //this continue statement loops back to the outer while loop early
						}

					}
				}

				CPUs_index++; //increment index of the CPUs array

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
