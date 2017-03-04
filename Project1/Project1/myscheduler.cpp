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
				list<ThreadDescriptorBlock>::iterator itr_srtf, itr_lowest;// iterator through thread list
		ThreadDescriptorBlock thread_off = ThreadDescriptorBlock();
		int current_cpu = 0, cpu_highest, highest_time_on_cpu=0,lowest_time_in_list=0, temp;
		bool cpu_avail=false;

		thread_list.sort(sort_by_arriving_time);//sort thread list according to arrival time
				
		for (int t = 0; t < num_cpu; t++) {//check to see if there is an idle cpu
			if (CPUs[t] == NULL) {
				current_cpu = t;
				cpu_avail = true;
				break;
			}
			
			else cpu_avail = false;
		}
		if (!cpu_avail) {//find cpu with thread that has highest time
			for (int t = 0; t < num_cpu; t++) {
				if (CPUs[t] != NULL) {
					if (CPUs[t]->remaining_time > highest_time_on_cpu) {
						highest_time_on_cpu = CPUs[t]->remaining_time;
						current_cpu = t;
					}
				}
			}
		}
		if (!thread_list.empty()) {
			itr_srtf = thread_list.begin();
			itr_lowest = itr_srtf;

			while (itr_srtf != thread_list.end()) {//find thread with lowest time in list
				if (itr_srtf->arriving_time <= timer&&itr_srtf->remaining_time < itr_lowest->remaining_time)
					itr_lowest = itr_srtf;
				itr_srtf++;
			}
			itr_srtf = itr_lowest;
						
			while ((!thread_list.empty() )&&(cpu_avail || highest_time_on_cpu > itr_srtf->remaining_time) && itr_srtf->arriving_time<=timer) {
				//place thread on cpu then check again and continue to place theads on cpu if needed
				if (!cpu_avail) { //swap
					 
					

					
					removeThreadFromCPUAddToList(current_cpu);
					
					addThreadtoCPU(itr_srtf, current_cpu);

					itr_srtf = thread_list.erase(itr_srtf);

				}
				else {//put thread on idle cpu
					addThreadtoCPU(itr_srtf, current_cpu);

					itr_srtf = thread_list.erase(itr_srtf);
					cpu_avail = false;
				
				}

				for (int t = 0; t < num_cpu; t++) {//check to see if there is an empty cpu
					if (CPUs[t] == NULL) {
						current_cpu = t;
						cpu_avail = true;
						break;
					}
					else cpu_avail = false;
				}

				if (!cpu_avail) {//find cpu with highest time
					highest_time_on_cpu = 0;
					for (int t = 0; t < num_cpu; t++) {
						if (CPUs[t] != NULL) {
							if (CPUs[t]->remaining_time > highest_time_on_cpu) {
								highest_time_on_cpu = CPUs[t]->remaining_time;
								cpu_highest = t;
							}
						}
					}
					current_cpu = cpu_highest;
				}

				if (!thread_list.empty()) {
					itr_srtf = thread_list.begin();
					itr_lowest = itr_srtf;

					while (itr_srtf != thread_list.end()) {//find thread with lowest time
						if (itr_srtf->arriving_time <= timer&&itr_srtf->remaining_time < itr_lowest->remaining_time)
							itr_lowest = itr_srtf;
						itr_srtf++;
					}
					itr_srtf = itr_lowest;
				}

			}
		}

		bool done = true;
		for (int t = 0; t < num_cpu; t++) {
			if (CPUs[t] != NULL)
				return true;
		}
		return false;
			
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
