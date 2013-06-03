#ifndef CALIBRATIONPARAMETERS_H
#define CALIBRATIONPARAMETERS_H

struct calibrationParameters {
	int DFTDimention;
	int overlay;
	int threshold;
	bool farawayZone;
	bool manuallyFindCycles;
	int bandHigh;
	int bandLow;
	double soundSpeed;
};

#endif
