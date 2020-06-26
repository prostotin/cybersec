#include <iostream>
#include "Windows.h"
int main()
{
	int dummyProcId = 7868; //dummy program process ID.
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dummyProcId); //open handle with all access, including reading and writing. 
	int accountBalance = 0; // a variable to store the read information
	ReadProcessMemory(processHandle, (LPCVOID)(0x00CFFAE8), &accountBalance, sizeof(int), NULL); //read whats stored at 00CFFAE8 into accountBalance.
	std::cout << "Read account balance: " << accountBalance << std::endl; //print the result
	std::cout << "Press ENTER to modify balance"; 
	getchar(); 
	int desiredBalance = 999999; //the value that we will be writing to the address
	WriteProcessMemory(processHandle, (LPVOID)(0x00CFFAE8), &desiredBalance, sizeof(int), NULL); //writes to 0x00CFFAE8
	std::cout << "Value successfully written" << std::endl;
	getchar();

}

