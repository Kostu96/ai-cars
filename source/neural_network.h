#pragma once
#include "neuron.h"

class Neural_network
{
public:
	Neural_network(int inputs_count, int layers_count, int* layers_neurons_count);
	~Neural_network();
	void activate_network();
	double *getOutputs();
	void setRandomWeights();
	void setWeights(double ***weights);
	double ***getWeights();
	void crossoverWeights(double ***p1, double ***p2);
	void setInputs(double *inputs);
	void mutate();
	static bool isSeeded();
	static void seedRand();
private:
	Neuron** layers;
	int layers_count;
	int *layers_neurons_count;
	double ***weights;
	int outputs_count;
	double *outputs;
	static bool rand_seeded;
	float neuron_mutation_chance = 0.7;
};