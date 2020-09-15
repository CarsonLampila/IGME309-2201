#include <iostream>
#include "Queue.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

using namespace std;

void wrapper();


// Check for memory leak
int main()
{
	wrapper();
	_CrtDumpMemoryLeaks();
}


// Main Code
void wrapper()
{
	// Int Queue
	cout << "-----------#--------------------" << endl;
	cout << "Int Queue: |" << endl;
	cout << "-----------#" << endl;
	Queue<int> intQueue;
	intQueue.Push(33);
	intQueue.Push(35);
	intQueue.Push(34);
	intQueue.Push(37);
	intQueue.Push(36);
	intQueue.Print();
	intQueue.Pop();
	intQueue.Pop();
	intQueue.Print();
	intQueue.Push(32);
	intQueue.Push(31);
	intQueue.Print();
	intQueue.Pop();
	intQueue.Print();

	// Copy Constructor
	cout << endl;
	cout << "----------------------------#---" << endl;
	cout << "Copy Constructor Int Queue: |" << endl;
	cout << "----------------------------#" << endl;
	Queue<int> copy = intQueue;
	copy.Print();
	copy.Pop();
	copy.Pop();
	copy.Print();



	// Copy Assignment
	cout << "---------------------------#----" << endl;
	cout << "Copy Assignment Int Queue: |" << endl;
	cout << "---------------------------#" << endl;
	copy = intQueue;
	copy.Print();

}