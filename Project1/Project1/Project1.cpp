// Project1.cpp : Defines the entry point for the console application.
//

//git git git
//manpoifkjwels.
//wlkneglkwne

#include "myscheduler.h"

int main(int argc, char* argv[])
{
	MyScheduler T1(FCFS, 2);
	MyScheduler T2(STRFwoP, 2);
	MyScheduler T3(STRFwP, 2);
	MyScheduler T4(PBS, 2);


	T1.CreateThread(0, 15, -1, 1);
	T1.CreateThread(5, 5, -1, 2);
	T1.CreateThread(12, 10, -1, 3);
	T1.CreateThread(15, 2, -1, 4);
	T1.CreateThread(30, 1, -1, 5);

	T2.CreateThread(0, 15, -1, 1);
	T2.CreateThread(5, 5, -1, 2);
	T2.CreateThread(12, 10, -1, 3);
	T2.CreateThread(15, 2, -1, 4);
	T2.CreateThread(30, 1, -1, 5);

	T3.CreateThread(0, 15, -1, 1);
	T3.CreateThread(5, 5, -1, 2);
	T3.CreateThread(12, 10, -1, 3);
	T3.CreateThread(15, 2, -1, 4);
	T3.CreateThread(30, 1, -1, 5);

	T4.CreateThread(0, 15, 7, 1);
	T4.CreateThread(5, 5, 4, 2);
	T4.CreateThread(12, 10, 6, 3);
	T4.CreateThread(15, 2, 3, 4);
	T4.CreateThread(30, 1, 5, 5);

	return 0;
}

