#include "neuron.h"
#include <cmath>

Neuron::Neuron()
{
}

void Neuron::setInputsCount(int inputs_count)
{
	this->inputs_count = inputs_count;
}

int Neuron::getInputsCount()
{
	return inputs_count;
}

void Neuron::setInputs(double * inputs)
{
	this->inputs = inputs;
}

void Neuron::setWeights(double * weights)
{
	this->weights = weights;
}

double Neuron::getOutput()
{
	return output;
}

void Neuron::activate()
{
	double sum = 0;
	for (int i = 0; i < this->inputs_count; i++)
	{
		sum += inputs[i] * weights[i];
	}
	output = activationFunction(sum);
}

double Neuron::activationFunction(double t)
{
	double result = 1 / (1 + exp(-t));
	return result;
}
