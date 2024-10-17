// See comments in MainMenu.h for general details on what each functin does.
#include <iostream>
#include "MainMenu.h"


// clear any characters from the (keyboard) input buffer
void ignoreLine() {
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// read and return an int from the console.  Keep trying if invalid until an int is returned.
int getIntFromUser() {
	while (true) {
		int guess;
		std::cin >> guess;

		if (std::cin.fail()) {
			std::cin.clear();
			ignoreLine();
			std::cerr << "Invalid input. Input must be a valid int.  Please try again.\n";
		}
		else
		{
			ignoreLine();
			return guess;
		}
	}
}

// output the menu choices to the console.
void displayMainMenu()
{
	std::cout << "\n";
	std::cout << "--- MENU ---\n";
	std::cout << "1) View Employees\n";
	std::cout << "2) Add Employee\n";
	std::cout << "3) Remove Employee\n";
	std::cout << "0) Exit\n";
	std::cout << "-------------\n";
	std::cout << "Select:";
}

// Attempt to read an int from the keyboard.  
// If there was a failure, clear the buffer and return -1.
Command getMenuCommand() {
	// read user input
	int input{ -1 };  // default 
	std::cin >> input;
	if (std::cin.fail()) {
		std::cin.clear();	// clear the failure
		input = -1;			// indicate an error
	}
	ignoreLine();	// ignore any extra input

	Command command = Command::invalid;	// default
	// match input with specific commands
	switch (input) {
	case 1: command = Command::viewEmployees; break;
	case 2: command = Command::addEmployee; break;
	case 3: command = Command::removeEmployee; break;
	case 0: command = Command::exit; break;
	}
	return command;
}

// This function is called when a user picks a selection from the menu.
// It determines what action to take depending on the menuItemSelected, 
// and calls the appropriate function.
// Returns true the selection was a request to exit menu, false otherwise.
void handleMenuInput(Employee*& pHead, Command command)
{
	switch (command)
	{
	case Command::viewEmployees:
		std::cout << ">> View Employees:\n";
		viewEmployees(pHead);
		break;
	case Command::addEmployee: {
		std::cout << ">> Add Employee:\n";
		std::cout << "Enter name:";
		std::string name;
		std::getline(std::cin, name);
		addNewEmployee(pHead, name);
	}
							 break;
	case Command::removeEmployee:
		std::cout << ">> Remove Employee:\n";
		std::cout << "Enter id:";
		//removeEmployee(pHead, getIntFromUser());
		break;
	case Command::exit:
		std::cout << "Exiting\n";
		break;
	default:
		std::cout << "Invalid input.\n";
		break;
	}
}

// =============================================================================
// Implement the functions below by following the given directions:	
// As you implement these, you will have to add forward declarations for
// each one in MainMenu.h
// The function name has been given.
// In some cases, the first parameter has been given.
// =============================================================================

// Create a new employee struct on the heap.
// Use a static local variable to generate unique employee id's with (initialize
// it and then increment it every time this function is called). 
// Dynamically allocate memory for an employee struct from the heap.  
//		 Initialize it with:
//				-the unique id,
//				-the name passed in as a paramter,
//				-nullptr.
// - param 1: a string - the employee's name.
// - return: a pointer to the dynamically allocated Employee struct 
Employee* createEmployee(const std::string& name)
{
	static int id{ 0 };
	Employee* employee{ new Employee };
	employee->id = id;
	employee->name = name;
	employee->pNext = nullptr;

	id++;
	return employee;
}


// Create a new employee node, then add it to the list
// Use createEmployee() for node creation.
// Insert the new employee node at the front of the linked list
// It will help to draw this out!
// - param 1: (given) a pointer to the front of the list of employees (passed
//            by reference)
// - param 2: a string - the employee's name.
// - return: nothing
void addNewEmployee(Employee*& pHead, const std::string& name)
{
	Employee* newEmployee{ createEmployee(name) };

	// Handle case where head is nullptr
	if (!pHead) {
		pHead = newEmployee;
		return;
	}

	Employee* employeeAtBack{ pHead };
	while (employeeAtBack->pNext) {
		employeeAtBack = employeeAtBack->pNext;
	}

	employeeAtBack->pNext = newEmployee;
}


// Output a list of all employees to the console.
// If the list is empty (nullptr), print "empty list.\n" 
// If it isn't empty, print each employee's id and name to the console:
//    eg: [id: 5, name: Mary]\n
// Create a pointer to traverse the list, point it at the front of the list,
// process the node (output as shown above), and advance the pointer through the
// list.  Repeat until you process the last node.
// -param 1: (given) a pointer to an Employee struct (representing the front 
//			 of the list of employees.
// return: nothing
void viewEmployees(Employee*& pHead)
{
	if (!pHead) return;

	Employee* currentEmployee{ pHead };

	while (currentEmployee) {
		std::cout << "[id: " << currentEmployee->id << ", name: " << currentEmployee->name << "]\n";
		currentEmployee = currentEmployee->pNext;
	}
}


// Search through the list for a node with the given id.  
// Tip: Use a pointer to iterate through the linked list and another
//      pointer to track the parent. 
// - param 1: (given) a pointer to an Employee struct (representing the front  
//			  of the list of employees
// - param 2: an int (the id of the employee we're searching for). 
// - return: a NodeInfo struct. It contains a pointer to the node that matches  
//			 the id passed in and a pointer to that node's parent. 
//           If node not found, pointers inside NodeInfo should both be nullptr.
//           If node is first in the list, NodeInfo.pParent should be nullptr.
NodeInfo* getNodeInfo(Employee*& pHead, const int& employeeId) 
{
	NodeInfo* nodeInfo{};
	nodeInfo->pNode = nullptr;
	nodeInfo->pParent = nullptr;

	Employee* parent{ nullptr };
	Employee* child{ pHead };
	while (child) {
		if (child->id == employeeId) {
			nodeInfo->pNode = child;
			nodeInfo->pParent = parent;
			break;
		}
		parent = child;
		child = child->pNext;
	}
	return nodeInfo;
}


// Removes an employee node with the given id from the list.
// Try to find a node with the given id (use getNodeInfo()). 
//	1) If node not found, output "Error: employee id:# not found\n". 
//  2) If node found, but parent is nullptr, the node is first in the list:
//			-Set the pHead to point to the second node in the list.
//			-Output "removed id:# \n" to the console
//			-Deallocate the node (return it to the heap)
//  3) otherwise the node was found and a parent exists
//			-Set the parent's next pointer to the next pointer of the node we want to delete
//			-Output a "removed id: #n" message to the console
//			-Deallocate the node (return it to the heap).
// - param 1: (given) a pointer to the front of the list of employees (passed by reference)
// - param 2: an int (the id of the employee we're searching for). 
// - return: nothing
//removeEmployee(Employee*& pHead);

// Removes all employees in an Employee linked-list
// - param: A pointer to the head of a linked employee list
// - return: nothing
void removeAllEmployees(Employee*& pHead)
{
	if (!pHead)
		return;

	if (pHead->pNext)
		removeAllEmployees(pHead->pNext);

	delete pHead;
	pHead = nullptr;
}