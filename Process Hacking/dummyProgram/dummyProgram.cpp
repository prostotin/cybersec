#include <iostream>
#include <stdlib.h>

#include "Windows.h"

int main()
{
	int accountBalance = 12345; //Starting account balance

	for (;;) {
		std::cout << "Your account balance is: " << accountBalance << std::endl; //display balance
		std::cout << "Press ENTER to refresh"; 
		getchar(); //
		std::cout << "---------" << std::endl;

	}

}
