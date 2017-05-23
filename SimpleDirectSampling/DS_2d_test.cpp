#include "DS_2d.h"


int main() {

	std::string tiFilename = "./data/ti.txt";
	std::string hdFilename = "./data/hd.txt";

	DS_2d ds_2d;
	ds_2d.paras.tiFilename = tiFilename;
	ds_2d.paras.hdFilename = hdFilename;
	ds_2d.init();
	ds_2d.simulation();

	return 0;
}