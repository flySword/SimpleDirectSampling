#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <climits>
#include <vector>

namespace myIO {



	/**
	* @brief Read a txt file and put data inside a training image
	*   just for simple test for 2d
	* @param fileName source's file name
	* @param ti the training image
	*
	* @return true if the reading process is sucessful
	*/
	template<typename T>
	bool readTIFromTxtFile(const std::string& fileName, std::vector<std::vector<T>>& ti) {
		std::ifstream file;
		file.open(fileName, std::ios::in);
		if (!file.is_open()) {
			// debig information should be written from function that calls readTI..
			//std::cerr << "Can't open " << fileName << " !\n";
			return false;
		}

		int dimX, dimY;
		file >> dimX >> dimY;

		ti.resize(dimY);
		for (int i = 0; i < dimY; i++) {
			ti[i].resize(dimX);
			for (int j = 0; j < dimX; j++) {
				file >> ti[i][j];
			}
		}
		return true;
	}


	bool readHardData(const std::string& fileName, std::vector<std::vector<int> > & hd);



	/**
	* @brief Write simulation 2D grid result into txt file
	* override for 2D
	*
	* @param fileName destination's file name
	* @param sg the simulation grid which is a 3D float vector
	* dimention is got from the size of sg
	*/
	void writeToASCIIFile(const std::string& fileName, const std::vector<std::vector<int> >& sg);

	/** 
	* for visual debug by python
	* just write the array to a file without dimensions
	*/
	void save2txt(const std::string& fileName, const std::vector<std::vector<int> >& sg);
	
}