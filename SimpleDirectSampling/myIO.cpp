//#include "IO.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <climits>
#include <vector>
#include "myIO.h"


namespace myIO {


	/**
	* read hard data from txt file
	* every line in the txt file is "x y z attr"
	*
	*/
	bool readHardData(const std::string& fileName, std::vector<std::vector<int> > & hd)
	{
		std::ifstream file;
		file.open(fileName, std::ios::in);
		if (!file.is_open()) {
			// debig information should be written from function that calls readTI..
			//std::cerr << "Can't open " << fileName << " !\n";
			return false;
		}

		int x;
		int y;
		int z;
		int attr;
		std::string str;
		while (getline(file, str)) {
			std::stringstream ss(str);
			std::string s;
			ss >> x >> y >> z >> attr;
			hd.push_back(std::vector<int>{x, y, z, attr});
		}

		return true;
	}

	/**
	* @brief Write simulation 2D grid result into txt file
	* override for 2D
	*
	* @param fileName destination's file name
	* @param sg the simulation grid which is a 3D float vector
	* dimention is got from the size of sg
	*/
	void writeToASCIIFile(const std::string& fileName, const std::vector<std::vector<int> >& sg) {
		std::ofstream aFile(fileName);
		int dimY = sg.size();
		int dimX = sg[0].size();
		// Header
		aFile << dimX << " " << dimY << std::endl;
		int cnt = 0;
		for (int y = 0; y < dimY; y++) {
			for (int x = 0; x < dimX; x++) {
				aFile << sg[y][x] << " ";
			}
			aFile << std::endl;
		}

		// Close the file stream explicitly
		aFile.close();
	}

	/**
	* for visual debug by python
	* just write the array to a file without dimensions
	*/
	void save2txt(const std::string & fileName, const std::vector<std::vector<int>>& sg)
	{
		std::ofstream aFile(fileName);
		int dimY = sg.size();
		int dimX = sg[0].size();
		int cnt = 0;
		for (int y = 0; y < dimY; y++) {
			for (int x = 0; x < dimX; x++) {
				aFile << sg[y][x] << " ";
			}
			aFile << std::endl;
		}
		// Close the file stream explicitly
		aFile.close();
	}





}