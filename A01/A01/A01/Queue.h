#pragma once
#include <iostream>

using namespace std;

template <class N>
class Queue
{
public:
	// Global variables
	N* current;
	int size;
	int top;


// Default Constructor
Queue()
{
	// Assign default values
	size = 2;
	top = 0;
	current = new N[size];
}


// Copy Constructor
Queue(const Queue& prev)
{
	// Copy values
	size = prev.size;
	top = prev.top;
	current = new N[size];
	for (int i = 0; i < top; i++)
	{
		current[i] = prev.current[i];
	}
}


// Copy Assignment
Queue& operator= (const Queue& prev)
{
	// Clean before copy
	delete[] current;
	current = nullptr;

	// If this is different from prev
	if (this != &prev)
	{
		// Copy values
		size = prev.size;
		top = prev.top;
		current = new N[size];
		for (int i = 0; i < top; i++)
		{
			current[i] = prev.current[i];
		}
	}
	return *this;
}


// Destructor
~Queue()
{
	delete[] current;
	current = nullptr;
}


	// Push to queue
	void Push(N data)
	{
		cout << endl;

		// Find current length and add 1 for next position
		int length = GetSize() + 1;

		// Ensure queue is big enough
		if (size <= length)
		{
			// Create bigger transfer array
			N* transfer = new N[size * 2];

			// Add all current items to transfer
			for (int i = 0; i < size; i++)
			{
				transfer[i] = current[i];
			}

			// Clean
			delete[] current;
			current = nullptr;

			// Update current and size
			current = transfer;
			size = size * 2;

			// Clean
			transfer = nullptr;

		}
		// Add to queue and print to console
		current[length] = data;
		cout << current[length] << " was added" << endl;
		top++;
	}


	// Pop top of queue
	void Pop()
	{
		cout << endl;
		// Check if not empty
		if (IsEmpty() == false)
		{
			// Display then remove
			cout << current[0] << " was removed" << endl;

			for (int i = 0; i < GetSize(); i++)
			{
				current[i] = current[i + 1];
			}
			top--;
		}
		else
		{
			// Nothing to remove
			cout << "No elements left to remove" << endl;
		}
	}


	// Print queue contents
	void Print()
	{
		IntOrder();
		cout << endl;
		cout << "Current queue:" << endl;
		// Check if not empty
		if (IsEmpty() == false)
		{
			// Loop and display queue contents
			for (int i = 0; i < GetSize() + 1; i++)
			{
				cout << "     " << current[i] << endl;
			}
		}
		else
		{
			// Nothing to display
			cout << "Queue is empty" << endl;
		}
	}


	// Get size of queue
	int GetSize()
	{
		// Calculate length
		int length = top - 1;
		return length;
	}


	// Check if queue is empty
	bool IsEmpty()
	{
		// Check if top is currently 0
		if (top == 0)
		{
			return true;
		}
		return false;
	}

	// Orders in numerical order
	void IntOrder()
	{
		// Defualt values
		N temp;
		bool finish = false;
		bool change = false;

		// Loop until no changes in order are made
		while (finish == false)
		{
			// Loop through all array elements
			for (int i = 0; i < GetSize(); i++)
			{
				// Check if first element is greater than second element
				if (current[i] > current[i + 1])
				{
					// Excahge places of two elements
					temp = current[i];
					current[i] = current[i + 1];
					current[i + 1] = temp;
					change = true;
				}
			}
			if (change == false)
			{
				finish = true;
			}
			change = false;
		}
	}
};

