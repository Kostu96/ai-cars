#include "neural_network.h"
#include <cstdlib>
#include <ctime>

Neural_network::Neural_network(int inputs_count, int layers_count, int * layers_neurons_count)
{
	this->layers_count = layers_count;
	this->layers_neurons_count = layers_neurons_count;
	layers = new Neuron*[layers_count];
	layers[0] = new Neuron[layers_neurons_count[0]];
	for (int j = 0; j < layers_neurons_count[0]; j++)
	{
		layers[0][j].setInputsCount(inputs_count);
	}

	for (int i = 1; i < layers_count; i++)
	{
		layers[i] = new Neuron[layers_neurons_count[i]];
		for (int j = 0; j < layers_neurons_count[i]; j++)
		{
			layers[i][j].setInputsCount(layers_neurons_count[i-1]);
		}
	}

	srand(time(NULL));
}

void Neural_network::setInputs(double *inputs)
{
	for (int j = 0; j < layers_neurons_count[0]; j++)
	{
		layers[0][j].setInputs(inputs);
	}
}

void Neural_network::setWeights(double *** weights)
{
	for (int i = 0; i < layers_count; i++)
	{
		for (int j = 0; j < layers_neurons_count[i]; j++)
		{
			layers[i][j].setWeights(weights[i][j]);
		}
	}
}

void::Neural_network::setRandomWeights()
{
	weights = new double**[layers_count];
	
	for (int i = 0; i < layers_count; i++)
	{
		weights[i] = new double*[layers_neurons_count[i]];

		int layer_i_inputs_count = layers[i][0].getInputsCount();
		for (int j = 0; j < layers_neurons_count[i]; j++)
		{
			weights[i][j] = new double[layer_i_inputs_count];
			for (int k = 0; k < layer_i_inputs_count; k++)
			{
				weights[i][j][k] = ((double) rand() / RAND_MAX - 0.5) * 0.35;
			}
			layers[i][j].setWeights(weights[i][j]);
		}

	}

}

void Neural_network::activate_network()
{
	for (int j = 0; j < layers_neurons_count[0]; j++)
	{
		layers[0][j].activate();
	}

	for (int i = 1; i < layers_count; i++)
	{
		double *layer_inputs = new double[layers_neurons_count[i - 1]];
		for (int j = 0; j < layers_neurons_count[i-1]; j++)
		{
			layer_inputs[j] = layers[i-1][j].getOutput();
		}

		for (int j = 0; j < layers_neurons_count[i]; j++)
		{
			layers[i][j].setInputs(layer_inputs);
			layers[i][j].activate();
		}
	}
	
	double* network_outputs = new double[layers_neurons_count[layers_count - 1]];

	for (int j = 0; j < layers_neurons_count[layers_count-1]; j++)
	{
		network_outputs[j] = layers[layers_count - 1][j].getOutput();
	}

	this->outputs = network_outputs;
}

double * Neural_network::getOutputs()
{
	return outputs;
}



