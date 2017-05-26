#include "DS_2d.h"
#include "myIO.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include <direct.h>// for create a folder
#include <ctime>
#include <random>




bool DS_2d::init() {
	if (false == myIO::readTIFromTxtFile(paras.tiFilename, ti))
		return false;
	if (false == myIO::readHardData(paras.hdFilename, hd))
		return false;


	tiDims.push_back(ti[0].size());
	tiDims.push_back(ti.size());
	
	sgDims.push_back(paras.sgDimX);
	sgDims.push_back(paras.sgDimY);
	sg.resize(sgDims[1]);
	for (int i = 0; i < sgDims[1]; i++) {
		sg[i].resize(sgDims[0], UNSIMULATIED_VALUE);
	}

	distNodeListInit();

	return true;
}




void DS_2d::simulation() {
	
	// asign hard data to simulation grid
	for (auto ii = hd.begin(); ii != hd.end(); ii++) {
		sg[(*ii)[1]][(*ii)[0]] = (*ii)[2];
	}

	// generate random path of simulation grid
	std::vector<int> simulationPath;

	if (paras.isRandomPath == true)
		getRandomSimulationPath(simulationPath, sgDims, paras.randSeed);
	else
		getUnilateralSimulationPath(simulationPath, sgDims);
	
	// relative vector of simulated points around path point
	// index by i/j/k
	std::vector<std::vector<int> > simVects;
	
	// for each random node in simulation grid
	int sgx, sgy;
	int dist;

	float progressCnt = 0;
	int simulatedSkipCut = 0;
	std::vector<int> dist_list;
	for (auto index = simulationPath.begin(); index != simulationPath.end(); index++) {
		sgy = *index / sgDims[0];
		sgx = *index - sgy * sgDims[0];

		if (sg[sgy][sgx] != UNSIMULATIED_VALUE)
			continue;

		// TODO : for debug
		progressCnt++;
		if (int(progressCnt) % 100 == 0) {
			std::cout << "simulation progress : " << progressCnt / sgDims[0] / sgDims[1] << std::endl;
		}
		
		// get simulated points in search radius
		calNeiborVect(sgx, sgy, paras.searchRadius, paras.maxPtNum, sgDims, sg, simVects);
		
		// TODO : unconditional simulation is not supported now
		// it can be supported by alert there
		// and not enough point is find may not be calculate by modifying there
		if (simVects.size() == 0) {
			continue;
		}
		
		// calculate the TI range according to the simVects
		int xshiftmin, xshiftmax, yshiftmin, yshiftmax;
		calTiShift(xshiftmin, xshiftmax, yshiftmin, yshiftmax, simVects, tiDims);

		std::vector<int> tiPath;
		for (int j = yshiftmin; j < yshiftmax; j++) {
			for (int k = xshiftmin; k < xshiftmax; k++) {
				tiPath.push_back(j*tiDims[0] + k);
			}
		}
		std::random_shuffle(tiPath.begin(), tiPath.end());
		
		// random search TI
		int shortest;
		int shortestX;
		int shortestY;
		int distSquare; // distance square
		int tiX, tiY;
		//int vecNearX, vecNearY;
		shortest = INT_MAX;
		for (int ii = 0; ii < tiPath.size(); ii++) {
			tiY = tiPath[ii] / tiDims[0];
			tiX = tiPath[ii] % tiDims[0];
			
			if (paras.isContinuousData)
				calContinuousDist(distSquare, tiX, tiY, ti, simVects);
			else
				calCategoricalDist(distSquare, tiX, tiY, ti, simVects, distNodeList);

			if (distSquare < shortest) {
				shortestX = tiX;
				shortestY = tiY;
				shortest = distSquare;
			}
			if (shortest < paras.tolerateDist*paras.tolerateDist) {
				break;
			}
		}


		//if (shortest < 1000){//TODO maybe a parameter, skip the far distance node
			sg[sgy][sgx] = ti[shortestY][shortestX];
			dist_list.push_back(shortest);
		//}

		// for visual debug
		bool isVisualDebug = false;
		if (isVisualDebug ) {
		
			// need to create the folder first
			//_mkdir("./python");
			//_mkdir("./python/debug");

			std::string visualDebugPath = "./visualDebug";
			_mkdir(visualDebugPath.c_str());

			std::vector<int> sgPnt = std::vector<int>{ sgx, sgy, paras.searchRadius};
			myIO::save2txt(visualDebugPath + "/sgPnt.txt", std::vector<std::vector<int> >{sgPnt});
			myIO::save2txt(visualDebugPath + "/sg.txt", sg);
			myIO::save2txt(visualDebugPath + "/ti.txt", ti);
			simVects.insert(simVects.begin(), std::vector<int>{shortestX, shortestY, 0});
			// WARN : the first line is the (x y) of TI coordinate
			// the vectors of other line is the (deltaY deltaX attribute)
			myIO::save2txt(visualDebugPath + "/pattern.txt", simVects);

			system(("python " + visualDebugPath + "/visualDebug.py " + visualDebugPath).c_str());
		}
	}

	int countNotSimulated = 0;

	for (int j = 0; j < sgDims[1]; j++) {
		for (int k = 0; k < sgDims[0]; k++) {
			if (sg[j][k] == UNSIMULATIED_VALUE) {
				countNotSimulated++;
			}
		}
	}

	std::cout << "countNotSimulated" << countNotSimulated << std::endl;

	std::string resultPath = "./result";
	_mkdir(resultPath.c_str());

	myIO::writeToASCIIFile(resultPath + "/result_sg.txt", sg);
	//myIO::writeToVTKFile("result_small_sg1.vtk", sg);
	myIO::writeToASCIIFile(resultPath + "/distSquares.txt", std::vector<std::vector<int> >{dist_list});
	
	bool isVisualResult = true;
	if ( isVisualResult) {
		//_mkdir("./python");
		//_mkdir("./python/result");

		myIO::save2txt(resultPath + "/sg.txt", sg);
		myIO::save2txt(resultPath + "/ti.txt", ti);
		myIO::save2txt(resultPath + "/hd.txt", hd);
		system(("python " + resultPath + "/visualResult.py " + resultPath).c_str());
	}


}


int myrandom(int i) { return std::rand() % i; }
void DS_2d::getRandomSimulationPath(std::vector<int> &simulationPath, const std::vector<int>& sgDims, const unsigned int & seed) {
	for (int ii = 0; ii < sgDims[0] * sgDims[1]; ii++)
		simulationPath.push_back(ii);

	//	std::random_shuffle(simulationPath.begin(), simulationPath.end(), std::default_random_engine(unsigned(std::time(0))));
	//std::srand(unsigned(std::time(NULL)));
	std::srand(seed);
	std::random_shuffle(simulationPath.begin(), simulationPath.end(), myrandom);
}

void DS_2d::getUnilateralSimulationPath(std::vector<int> &simulationPath, const std::vector<int>& sgDims) {
	for (int j = 0; j < sgDims[1]; j++) {
		for (int i = 0; i < sgDims[0]; i++) {
			simulationPath.push_back(i*sgDims[1] + j);
		}
	}
}


//// for test, not use
//void DS_2d::getMySimulationPath(std::vector<int> &simulationPath, const std::vector<int>& sgDims) {
//
//	for (int j = 0; j < sgDims[1]; j+=4) {
//		for (int i = 0; i < sgDims[0]; i++) {
//			simulationPath.push_back(i*sgDims[1] + j);
//		}
//	}
//	for (int j = 2; j < sgDims[1]; j += 4) {
//		for (int i = 0; i < sgDims[0]; i++) {
//			simulationPath.push_back(i*sgDims[1] + j);
//		}
//	}
//	for (int j = 1; j < sgDims[1]; j += 4) {
//		for (int i = 0; i < sgDims[0]; i++) {
//			simulationPath.push_back(i*sgDims[1] + j);
//		}
//	}
//	for (int j = 3; j < sgDims[1]; j += 4) {
//		for (int i = 0; i < sgDims[0]; i++) {
//			simulationPath.push_back(i*sgDims[1] + j);
//		}
//	}
//
//}

/**
* calculate the neighbor vector of the center point ([sgx],[sgy]) in the range on [searchRadius]
* if the number of points beyond [maxPtNum], get the nearest [maxPtNum]
* note: now the neighbor is a rectanger but not a circle
*
* used in DS_2d::simulation()
*/
void DS_2d::calNeiborVect(int sgx, int sgy, int searchRadius, int maxPtNum, 
	const std::vector<int>& sgDims, const std::vector<std::vector<int>>& sg, std::vector<std::vector<int>>& simVects)
{

	// the loop consuming most of the time
	// TODO : there is not a circle but a rect
	// TODO : can be packed to a function
	simVects.clear();
	int searchXmin = sgx - searchRadius;
	int searchYmin = sgy - searchRadius;
	int searchXmax = sgx + searchRadius;
	int searchYmax = sgy + searchRadius;
	if (searchXmin < 0)
		searchXmin = 0;
	if (searchYmin < 0)
		searchYmin = 0;
	if (searchXmax >= sgDims[0])
		searchXmax = sgDims[0];
	if (searchYmax >= sgDims[1])
		searchYmax = sgDims[1];

	int dist;
#pragma omp parallel for
	for (int j = searchYmin; j < searchYmax; j++) {
		for (int k = searchXmin; k < searchXmax; k++) {
			if (sg[j][k] != UNSIMULATIED_VALUE && sg[j][k] != SIMULATED_VALUE) {
				//dist = (i - sgz)*(i - sgz) + (j - sgy)*(j - sgy) + (k - sgx)*(k - sgx);

				dist = (j - sgy)*(j - sgy) + (k - sgx)*(k - sgx);
				if (dist <= searchRadius*searchRadius)
					simVects.push_back(std::vector<int>{j - sgy, k - sgx, int(sg[j][k]), dist}); 
			}
		}
	}

	// get nearest maxPtNum points if points ...
	if (simVects.size() > maxPtNum) {
		//std::random_shuffle(simPts.begin(), simPts.end());
		std::sort(simVects.begin(), simVects.end(), [](const std::vector<int> &a, const std::vector<int> & b) -> bool
		{
			return a[3] < b[3];
		});
		simVects.resize(maxPtNum);
	}
}


/**
* calculate the shift coordinate in the ti 
* according to the vectors [simVects] and [tiDims]
*/
void DS_2d::calTiShift(int &xshiftmin,int &xshiftmax,
 int &yshiftmin,

 int &yshiftmax,
	std::vector<std::vector<int> > simVects, std::vector<int> tiDims) {
	// calculate the search range in TI
	int tiXmax = INT_MIN;
	int tiYmax = INT_MIN;
	int tiXmin = INT_MAX;
	int tiYmin = INT_MAX;

	// get range of simVects
	// for the range calculation of TI
	for (auto ii = simVects.begin(); ii != simVects.end(); ii++) {
		if ((*ii)[0] > tiYmax)
			tiYmax = (*ii)[0];
		if ((*ii)[0] < tiYmin)
			tiYmin = (*ii)[0];
		if ((*ii)[1] > tiXmax)
			tiXmax = (*ii)[1];
		if ((*ii)[1] < tiXmin)
			tiXmin = (*ii)[1];
	}



	yshiftmax = tiDims[1] - tiYmax;
	xshiftmax = tiDims[0] - tiXmax;
	yshiftmin;
	xshiftmin;
	if (tiYmin >= 0)
		yshiftmin = 0;
	else
		yshiftmin = -tiYmin;
	if (tiXmin >= 0)
		xshiftmin = 0;
	else
		xshiftmin = -tiXmin;

	if (yshiftmax > tiDims[1])
		yshiftmax = tiDims[1];
	if (xshiftmax > tiDims[0])
		xshiftmax = tiDims[0];

}


/**
* calculate the categorical Distance between data event and ti pattern
*/
void DS_2d::calCategoricalDist(int &distSquare, const int &tiX, const int &tiY, const std::vector<std::vector<int> >& ti, const std::vector<std::vector<int> >& simVects, const std::vector<std::vector<int> > &distNodeList) {
	
	int vecX, vecY;
	distSquare = 0;

	// calculate the distance of simVects
	for (auto ii = simVects.begin(); ii != simVects.end(); ii++) {
		// there is different from continuous
		//if (ti[tiY + (*i)[0]][tiX + (*i)[1]] != (*i)[2])
		//	distSquare += 1;		

		vecX = tiX + (*ii)[1];
		vecY = tiY + (*ii)[0];

		// if the distance to the current ti point is far
		if (pow((*ii)[0], 2) + pow((*ii)[1], 2) > 2) { // TODO : maybe a parameter
			auto j = distNodeList.begin();
			for (; j != distNodeList.end(); j++) {
				if (vecY + (*j)[0] < 0 || vecY + (*j)[0] >= ti.size())
					continue;
				if (vecX + (*j)[1] < 0 || vecX + (*j)[1] >= ti[0].size())
					continue;
				if (ti[vecY + (*j)[0]][vecX + (*j)[1]] == (*ii)[2]) {
					distSquare += (*j)[0] * (*j)[0] + (*j)[1] * (*j)[1];
					break;
				}
			}

			if (j == distNodeList.end()) {
				distSquare += 10000; // TODO
			}
		}
		else { // for near point, pattern should be the same
			if (ti[vecY][vecX] != (*ii)[2]) {
				distSquare += 10000;
			}
		}
	}

	if (distSquare >= 10000)
		return;
}

/**
* calculate the continuous Distance between data event and ti pattern
*/
void DS_2d::calContinuousDist(int &distSquare, const int &tiX, const int &tiY, const std::vector<std::vector<int> >& ti, const std::vector<std::vector<int> >& simVects) {

	int vecX, vecY;
	distSquare = 0;
	int dist;
	// calculate the distance of simVects
	for (auto ii = simVects.begin(); ii != simVects.end(); ii++) {

		vecX = tiX + (*ii)[1];
		vecY = tiY + (*ii)[0];
		dist = pow((*ii)[2] - ti[vecY][vecX], 2);

		distSquare += dist;

		//// if the distance to the current ti point is far
		//if (dist > 20) { // TODO : a parameter for every point
		//	distSquare += 10000;
		//	return;
		//}
		//else { // for near point, pattern should be the same
		//	distSquare += dist;
		//}



	}

}

	


// initialize the distNodeList
// distNodeList stores 9 points in order by distance to the center
// TODO : eight may be set to 25
void DS_2d::distNodeListInit() {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			distNodeList.push_back(std::vector<int>{i, j, (i*i + j*j)});
		}
	}
	std::sort(distNodeList.begin(), distNodeList.end(), [](const std::vector<int> &a, const std::vector<int> & b) -> bool
	{
		return a[2] < b[2];
	});

	for (auto i = distNodeList.begin(); i != distNodeList.end(); i++) {
		(*i).resize(2);
	}
}