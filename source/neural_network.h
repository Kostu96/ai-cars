#pragma once
#include "neuron.h"

class Neural_network
{
public:
	Neural_network(int inputs_count, int layers_count, int* layers_neurons_count);
	void activate_network();
	double *getOutputs();
	void setWeights(double ***weights);
	void setInputs(double *inputs);
private:
	Neuron** layers;
	int layers_count;
	int *layers_neurons_count;
	int outputs_count;
	double *outputs;
};