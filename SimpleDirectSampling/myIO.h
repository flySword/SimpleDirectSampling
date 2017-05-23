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
	* @brief Write simulation 2D grid result into txt file
	* override for 2D
	*
	* @param fileName destination's file name
	* @param sg the simulation grid which is a 3D float vector
	* dimention is got from the size of sg
	*/
	void writeToASCIIFile(const std::string& fileName, const std::vector<std::vector<int> >& sg);


	/**
	* @brief Write simulation 1D grid result into txt file
	* override for 1D, just for some tests and logs
	*
	* @param fileName destination's file name
	* @param sg the simulation grid which is a 3D float vector
	* dimention is got from the size of sg
	*/
	void writeToASCIIFile(const std::string& fileName, const std::vector<int> & sg);




	/**
	* @brief Read a txt file and put data inside a training image
	* just for simple test for 2d
	*
	* @param fileName source's file name
	* @param ti the training image
	*
	* @return true if the reading process is sucessful
	*/
	/**
	* @brief Read a txt file and put data inside a training image
	* just for simple test for 2d
	*
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

	namespace for2d {
		/**
		*  read data by line
		*  hd[0] contains {x,y,attr}
		*/
		template<typename T>
		bool readHardData(const std::string & fileName, std::vector<std::vector<T>>& hd)
		{
			std::ifstream file;
			file.open(fileName, std::ios::in);
			if (!file.is_open()) {
				// debig information should be written from function that calls readTI..
				//std::cerr << "Can't open " << fileName << " !\n";
				return false;
			}

			T x;
			T y;

			T attr;
			std::string str;
			while (getline(file, str)) {
				std::stringstream ss(str);
				std::string s;
				ss >> x >> y >> attr;
				hd.push_back(std::vector<T>{x, y, attr});
			}

			return true;
		}
	}
}