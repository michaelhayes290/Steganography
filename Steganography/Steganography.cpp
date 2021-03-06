// Created by Michael Hayes
// Steganography program to hide text within an image, and extract hidden text from an image
// Uses code from the lodePNG library (All rights to respective authors)

#include "stdafx.h"
#include "lodepng.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <iomanip>
#include <fstream>
#include <string>
#include <stdlib.h>


using namespace std;

int main(int argc, char *argv[])
{
	string originalImageName, modifiedImageName, textFileName;
	
	//they didn't provide the correct number of parameters
	if (argc < 3 || argc > 5) {
		cout << "Usage:\nENCRYPT: Steganography -e <original image name> <modified image name> <input ASCII text file name>\nDECRYPT: Steganography -d <modified image name> <output ASCII text file name>\n";
	}
	//they are trying to decrypt with too many parameters
	else if (argc == 5 && string(argv[1]) == "-d") {
		cout << "Usage: Steganography -d <modified image name> <output ASCII text file name>";
	}
	//everything is correct
	else {
		string fileContents = "";
		bool encrypt = false;
		bool hasText = true;
		if (string(argv[1]) == "-e") {
			encrypt = true;
		}
		//there was no text specified
		if (argc == 4 && encrypt) {
			//if we are encrypting
			cout << "Enter Text to be stored in the image:" << endl;
			getline(cin, fileContents);
			hasText = false;
		}
		else if (argc == 3 && !encrypt) {
			hasText = false;
		}
		//there were command-line arguments
		if (encrypt) {
			originalImageName = string(argv[2]);
			modifiedImageName = string(argv[3]);
			if (hasText) {
				textFileName = string(argv[4]);
			}
		}
		//we are decrypting
		else {
			originalImageName = string(argv[2]);
			if (hasText) {
				textFileName = string(argv[3]);
			}
		}

		//Start doing work
		if (encrypt) {
			//get the image file
			vector<unsigned char> image; //the raw pixels
			unsigned width, height;
			//decode
			unsigned error = lodepng::decode(image, width, height, originalImageName);
			//if there's an error, display it
			if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

			//get the characters from the file
			string line;
			ifstream myfile;
			if (fileContents == "") {
				myfile.open(textFileName.c_str(), ios::in);
				if (myfile.is_open())
				{
					while (getline(myfile, line))
					{
						fileContents += line + "\n";
					}
					myfile.close();
				}
				else
				{
					cout << "Unable to open text file" << "\n";
				}
			}
			
			//make the new image
			vector<unsigned char> newImage = image; //the raw pixels
			int j = 0;
			for (int i = 0; i < fileContents.size(); i++) {
				for (int k = 7; k >= 0; k--) {
					if ((fileContents[i] >> k) & 0x01 == 1) {
						newImage[j] ^= (-1 ^ newImage[j]) & (1UL << 0);
						j++;
					}
					else {
						newImage[j] ^= (-0 ^ newImage[j]) & (1UL << 0);
						j++;
					}
				}
			}
			//pad the next 8 bits with 0's to signify the end of the message
			for (int i = 0; i < 8; i++) {
				newImage[j] ^= (-0 ^ newImage[j]) & (1UL << 0);
				j++;
			}
			error = lodepng::encode(modifiedImageName, newImage, width, height);

			//if there's an error, display it
			if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		}
		else {
			//get the image file
			vector<unsigned char> image; //the raw pixels
			unsigned width, height;
			//decode
			unsigned error = lodepng::decode(image, width, height, originalImageName);
			//if there's an error, display it
			if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

			//move characters to the message
			vector<char> message;
			int j = 7;
			char temp;
			for (int i = 0; i <image.size(); i++) {
				if ((image[i] >> 0) & 0x01 == 1) {
					temp ^= (-1 ^ temp) & (1UL << j);
					j--;
				}
				else {
					temp ^= (-0 ^ temp) & (1UL << j);
					j--;
				}
				if (j == -1) {
					if (temp == 00000000) {
						break;
					}
					message.push_back(temp);
					j = 7;
				}
			}
			string s(message.begin(), message.end());
			if (!hasText) {
				cout << s;
			}
			else {
				ofstream myfile;
				myfile.open(textFileName);
				myfile << s;
				myfile.close();
			}
		}
	}
	cout << endl;
	system("PAUSE");

	return 0;
}