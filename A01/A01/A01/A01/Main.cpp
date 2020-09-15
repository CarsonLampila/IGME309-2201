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
	intQueue.Push(34);
	intQueue.Push(35);
	intQueue.Push(36);
	intQueue.Push(37);
	intQueue.Print();
	intQueue.Pop();
	intQueue.Pop();
	intQueue.Print();
	intQueue.Push(32);
	intQueue.Push(31);
	intQueue.Print();
	intQueue.Pop();
	intQueue.Print();



	// String Queue
	cout << endl;
	cout << "--------------#-----------------" << endl;
	cout << "String Queue: |" << endl;
	cout << "--------------#" << endl;
	Queue<string> stringQueue;
	stringQueue.Push("Gomez");
	stringQueue.Push("Morticia");
	stringQueue.Push("Wednesday");
	stringQueue.Push("Pugsley");
	stringQueue.Print();
	stringQueue.Pop();
	stringQueue.Pop();
	stringQueue.Print();
	stringQueue.Push("Lurch");
	stringQueue.Push("Fester");
	stringQueue.Push("Grandmama");
	stringQueue.Print();
	stringQueue.Pop();
	stringQueue.Print();



	// Char Queue
	cout << endl;
	cout << "------------#-------------------" << endl;
	cout << "Char Queue: |" << endl;
	cout << "------------#" << endl;
	Queue<char> charQueue;
	charQueue.Push('e');
	charQueue.Push('d');
	charQueue.Push('i');
	charQueue.Push('r');
	charQueue.Print();
	charQueue.Pop();
	charQueue.Pop();
	charQueue.Pop();
	charQueue.Pop();
	charQueue.Pop();
	charQueue.Print();
	charQueue.Push('x');
	charQueue.Push('e');
	charQueue.Push('m');
	charQueue.Push('o');
	charQueue.Push('r');
	charQueue.Push('h');
	charQueue.Print();
	charQueue.Pop();
	charQueue.Print();
	charQueue.Push('c');
	charQueue.Print();



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