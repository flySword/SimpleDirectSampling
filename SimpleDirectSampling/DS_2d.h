#pragma once
# include <vector>

#define SIMULATED_VALUE -999999
#define UNSIMULATIED_VALUE -99999

// parameters 
struct DsParameters {

	int sgDimX;	// dimensions of simulation grid
	int sgDimY;

	int searchRadius;
	int maxPtNum; //max points number of template
	int tolerateDist;// tolerated distance for each point	

	bool isRandomPath; // using random simulation path if true,
					   // else using the unilateral path

	bool isContinuousData;

	int randSeed;

	std::string tiFilename;
	std::string hdFilename;

	// default parameters
	DsParameters() {
		sgDimX = 50;
		sgDimY = 50;
		searchRadius = 25;
		maxPtNum = 7;
		tolerateDist = 8;
		isRandomPath = true;
		isContinuousData = true;
		randSeed = 0;
	}
};



class DS_2d {

public:
	
	// x y dimensions 
	std::vector<int> sgDims;

	// x y dimensions
	std::vector<int> tiDims;


	// simulation grid
	// index by i j
	std::vector<std::vector<int> > sg;

	// train image
	// index by i j
	std::vector<std::vector<int> > ti;
	
	// hard data  x y attr
	// no hard data is allowed! 
	// attention to the judgement of null!
	std::vector<std::vector<int> > hd;

	//store a list of nodes, which sort by the distance to the center
	// used for calculate the distance 
	std::vector<std::vector<int> > distNodeList;

	// parameters
	DsParameters paras;

	
	DS_2d() {

	}

	bool init();

	void simulation();
	

private:
	void getRandomSimulationPath(std::vector<int>& simulationPath, const std::vector<int>& sgDims, const unsigned int & seed);
	void getUnilateralSimulationPath(std::vector<int>& simulationPath, const std::vector<int>& sgDims);
//	void getMySimulationPath(std::vector<int>& simulationPath, const std::vector<int>& sgDims);

	void calNeiborVect(int sgx, int sgy, int searchRadius, int maxPtNum, const std::vector<int>& sgDims, const std::vector<std::vector<int>>& sg, std::vector<std::vector<int>>& simVects);
	void calTiShift(int & xshiftmin, int & xshiftmax, int & yshiftmin, int & yshiftmax, std::vector<std::vector<int>> simVects, std::vector<int> tiDims);
	void calCategoricalDist(int & distSquare, const int & tiX, const int & tiY, const std::vector<std::vector<int>>& ti, const std::vector<std::vector<int>>& simVects, const std::vector<std::vector<int>>& distNodeList);
	void calContinuousDist(int & distSquare, const int & tiX, const int & tiY, const std::vector<std::vector<int>>& ti, const std::vector<std::vector<int>>& simVects);
	void distNodeListInit();



};

