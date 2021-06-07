#pragma once

class Neuron 
{
public:
	Neuron();
	void setInputsCount(int inputs_count);
	int getInputsCount();
	void setInputs(double *inputs);
	void setWeights(double *weights);
	void activate();
	void mutate(int weight_index, double weight_modification);
	double getOutput();
private:
	double activationFunction(double t);

	int inputs_count;
	double *inputs;
	double *weights;
	double output;
};