#pragma once


namespace myUtility {



	/**
	* @brief Convert 1D index to 3D index
	*
	* @param oneDIndex index of the 1D array
	* @param dimX dimension X of the 3D array
	* @param dimY dimension Y of the 3D array
	* @param idxX output index X
	* @param idxY output index Y
	* @param idxZ output index Z
	*/
	void oneDTo3D(const int& oneDIndex, const int& dimX, const int& dimY, int& idxX, int& idxY, int& idxZ);

	/**
	* @brief Convert 3D index to 1D index
	*
	* @param idxX index X of the 3D array
	* @param idxY index Y of the 3D array
	* @param idxZ index Z of the 3D arary
	* @param dimX dimension X of the 3D array
	* @param dimY dimension Y of the 3D array
	* @param oneDIndex output index of the 1D array
	*/
	void treeDto1D(const int& idxX, const int& idxY, const int& idxZ, const int& dimX, const int& dimY, int& oneDIndex);

}