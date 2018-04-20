// Steganography.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	//they didn't provide the correct number of parameters
	if (argc == 1 || argc == 3 || argc > 5) {
		cout << "Usage:\nENCRYPT: Steganography -e <original image name> <modified image name> <input ASCII text file name>\nDECRYPT: Steganography -d <modified image name> <output ASCII text file name>\n(-e or -d is required, other command line inputs are optional)\n";
	}
	//they are trying to encrypt with too few parameters
	else if (argc == 4 && string(argv[1]) == "-e") {
		cout << "Usage: Steganography -e <original image name> <modified image name> <input ASCII text file name>";
	}
	//they are trying to decrypt with too many parameters
	else if (argc == 5 && string(argv[1]) == "-d") {
		cout << "Usage: Steganography -d <modified image name> <output ASCII text file name>";
	}
	//everything is correct
	else {
		cout << "Parameters are correct";
	}


	system("PAUSE");

	return 0;
}