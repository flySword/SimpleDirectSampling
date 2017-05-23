//#include "IO.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <climits>
#include <vector>
#include "myIO.h"
#include "myUtility.h"

namespace myIO {
	/**
	* @brief check if file exists
	*
	* @param fileName source's file name
	*
	* @return true if filename exists
	*/
	bool file_exist(const std::string& fileName)
	{
		std::ifstream infile(fileName);
		return infile.good();
	}





	/**
	* @brief Write simulation 3D grid result into txt file
	* some parameters have not been used now
	*
	* @param fileName destination's file name
	* @param sg the simulation grid which is a 3D float vector
	* @param sgDimX dimension X of SG
	* @param sgDimY dimension Y of SG
	* @param sgDimZ dimension Z of SG
	* @param minWorldX minimal GeoGraphic X coordinate of the TI
	* @param minWorldY minimal GeoGraphic Y coordinate of the TI
	* @param minWorldZ minimal GeoGraphic Z coordinate of the TI
	* @param stepX voxel size in X dimension
	* @param stepY voxel size in Y dimension
	* @param stepZ voxel size in Z dimension
	*/
	void writeToASCIIFile(const std::string& fileName, const std::vector<std::vector<std::vector<float> > >& sg,
		const int& dimX, const int& dimY, const int& dimZ,
		const float& minWorldX, const float& minWorldY, const float& minWorldZ,
		const float& stepX, const float& stepY, const float& stepZ) {
		std::ofstream aFile(fileName);
		// Header
		aFile << dimX << " " << dimY << " " << dimZ << std::endl;
		int cnt = 0;
		for (int z = 0; z < dimZ; z++) {
			for (int y = 0; y < dimY; y++) {
				for (int x = 0; x < dimX; x++) {
					aFile << sg[z][y][x] << " ";
				}
			}
		}
		// Close the file stream explicitly
		aFile.close();
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
	* @brief Write simulation 1D grid result into txt file
	* override for 1D, just for some tests and logs
	* 
	* @param fileName destination's file name
	* @param sg the simulation grid which is a 3D float vector
	* dimention is got from the size of sg
	*/
	void writeToASCIIFile(const std::string& fileName, const std::vector<int> & sg) {
		std::ofstream aFile(fileName);
		int dim = sg.size();
		// Header
		aFile << dim << std::endl;
		for (int x = 0; x < dim; x++) {
			aFile << sg[x] << " ";
		}

		// Close the file stream explicitly
		aFile.close();
	}

	/**
	* @brief Write simulation 3D grid result into file
	*
	* @param fileName destination's file name
	* @param sg the simulation grid which is a 3D float vector
	*/
	void writeToVTKFile(const std::string& fileName, const std::vector<std::vector<std::vector<float>>>& sg) {

		std::ofstream aFile(fileName);
		int sgDimZ = sg.size();
		int sgDimY = sg[0].size();
		int sgDimX = sg[0][0].size();

		// Header
		aFile << "# vtk DataFile Version 2.0" << std::endl;
		aFile << "Volume example" << std::endl;
		aFile << "ASCII" << std::endl;
		aFile << "DATASET STRUCTURED_POINTS" << std::endl;
		aFile << "DIMENSIONS " << sgDimX << " " << sgDimY << " " << sgDimZ << std::endl;
		aFile << "ASPECT_RATIO 1 1 1" << std::endl;
		aFile << "ORIGIN 0 0 0" << std::endl;
		aFile << "ASPECT_RATIO 1 1 1" << std::endl;
		aFile << "POINT_DATA " << sgDimX*sgDimY*sgDimZ << std::endl;
		aFile << "SCALARS volume_scalars int 1" << std::endl;
		aFile << "LOOKUP_TABLE default" << std::endl;

		for (int z = 0; z < sgDimZ; z++) {
			for (int y = 0; y < sgDimY; y++) {
				for (int x = 0; x < sgDimX; x++) {
					aFile << sg[z][y][x] << std::endl;
				}
			}
		}


		// Close the file stream explicitly
		aFile.close();
	}




	/**
	* @brief Read a VTK file and put data inside a training image
	*
	* @param fileName source's file name
	* @param ti the training image
	*
	* @return true if the reading process is sucessful
	*/
	bool readTIFromVTKFile(const std::string& fileName, std::vector<std::vector<std::vector<float>>>& ti) {
		std::ifstream file;
		file.open(fileName, std::ios::in);
		if (!file.is_open()) {
			// debig information should be written from function that calls readTI..
			//std::cerr << "Can't open " << fileName << " !\n";
			return false;
		}

		std::vector<int> dimensions;
		std::string str;
		//Header processing
		for (int i = 0; str != "LOOKUP_TABLE default"; i++) {
			getline(file, str);
			if (i == 4) {

				//cout << str << endl;
				std::stringstream ss(str);
				std::string s;
				int j = 0;
				while (getline(ss, s, ' ')) {
					if (j == 0) {
						j = 1;
						continue;
					}
					if (!s.empty()) {
						dimensions.push_back(stoi(s));
					}

				}
			}

		}

		ti.resize(dimensions[2]);
		for (int i = 0; i < dimensions[2]; i++) {
			ti[i].resize(dimensions[1]);
			for (int j = 0; j < dimensions[1]; j++) {
				ti[i][j].resize(dimensions[0]);
			}
		}

		//Putting data inside
		int dataCnt = 0, idxX = 0, idxY = 0, idxZ;
		float dataValue;
		while (getline(file, str)) {

			dataValue = stof(str);
			myUtility::oneDTo3D(dataCnt, dimensions[0], dimensions[1], idxX, idxY, idxZ);
			ti[idxZ][idxY][idxX] = dataValue;
			++dataCnt;
		}
		return true;
	}


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
	//template<typename T>
	//bool readTIFromTxtFile(const std::string& fileName, std::vector<std::vector<T>>& ti) {
	//	std::ifstream file;
	//	file.open(fileName, std::ios::in);
	//	if (!file.is_open()) {
	//		// debig information should be written from function that calls readTI..
	//		//std::cerr << "Can't open " << fileName << " !\n";
	//		return false;
	//	}

	//	int dimX, dimY;
	//	file >> dimX >> dimY;

	//	ti.resize(dimY);
	//	for (int i = 0; i < dimY; i++) {
	//		ti[i].resize(dimX);
	//		for (int j = 0; j < dimX; j++) {
	//			file >> ti[i][j];
	//		}
	//	}


	//	return true;
	//}




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

	// for 2d function, some 2d function is override above
	namespace for2d {

		/**
		* read hard data from txt file
		* every line in the txt file is "x y attr"
		*
		*/
		//template <typename T>
		//bool readHardData(const std::string & fileName, std::vector<std::vector<T>>& hd)
		//{
		//	std::ifstream file;
		//	file.open(fileName, std::ios::in);
		//	if (!file.is_open()) {
		//		// debig information should be written from function that calls readTI..
		//		//std::cerr << "Can't open " << fileName << " !\n";
		//		return false;
		//	}

		//	int x;
		//	int y;

		//	int attr;
		//	std::string str;
		//	while (getline(file, str)) {
		//		std::stringstream ss(str);
		//		std::string s;
		//		ss >> x >> y >> attr;
		//		hd.push_back(std::vector<int>{x, y, attr});
		//	}

		//	return true;
		//}




	}

}