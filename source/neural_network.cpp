#include "neural_network.h"
#include <cstdlib>
#include <ctime>

bool Neural_network::rand_seeded = false;

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

	if (Neural_network::isSeeded() == false)
		Neural_network::seedRand();
	
}

void Neural_network::setInputs(double *inputs)
{
	for (int j = 0; j < layers_neurons_count[0]; j++)
	{
		layers[0][j].setInputs(inputs);
	}
}

void Neural_network::mutate()
{
	for (int i = 0; i < layers_count; i++)
	{
		for (int j = 0; j < layers_neurons_count[i]; j++)
		{
			if ((float)rand() / RAND_MAX < 0.5)
			{
				int neuronWeightsCount = layers[i][j].getInputsCount() + 1;
				int k = rand() % neuronWeightsCount;
				double mod = (double)rand() / RAND_MAX * 3 - 1.5;
				//for (int k = 0; k < neuronWeightsCount; k++)
				{
					layers[i][j].mutate(k, mod);
				}
				
				
			}
			
		}
	}
}

bool Neural_network::isSeeded()
{
	return Neural_network::rand_seeded;
}

void Neural_network::seedRand()
{
	srand(time(NULL));
	Neural_network::rand_seeded = true;
}

void Neural_network::setWeights(double *** weights)
{
	this->weights = new double**[layers_count];

	for (int i = 0; i < layers_count; i++)
	{
		this->weights[i] = new double*[layers_neurons_count[i]];

		int layer_i_inputs_count = layers[i][0].getInputsCount();
		for (int j = 0; j < layers_neurons_count[i]; j++)
		{
			this->weights[i][j] = new double[layer_i_inputs_count + 1]; //+1 for bias weight
			for (int k = 0; k < layer_i_inputs_count + 1; k++)
			{
				this->weights[i][j][k] = weights[i][j][k];
			}
			layers[i][j].setWeights(weights[i][j]);
		}

	}

	/*this->weights = weights;
	for (int i = 0; i < layers_count; i++)
	{
		for (int j = 0; j < layers_neurons_count[i]; j++)
		{
			layers[i][j].setWeights(weights[i][j]);
		}
	}*/
}

double *** Neural_network::getWeights()
{
	return weights;
}

void Neural_network::crossoverWeights(double *** p1, double *** p2)
{
	weights = new double**[layers_count];

	for (int i = 0; i < layers_count; i++)
	{
		weights[i] = new double*[layers_neurons_count[i]];

		int layer_i_weights_count = layers[i][0].getInputsCount() + 1; //+1 for bias weight
		for (int j = 0; j < layers_neurons_count[i]; j++)
		{
			weights[i][j] = new double[layer_i_weights_count]; 

			//randomize crossover point
			int crossover_point = rand() % (layer_i_weights_count);

			for (int k = 0; k < crossover_point; k++)
			{
				weights[i][j][k] = p1[i][j][k];
			}

			for (int k = crossover_point; k < layer_i_weights_count; k++)
			{
				weights[i][j][k] = p2[i][j][k];
			}

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
			weights[i][j] = new double[layer_i_inputs_count+1]; //+1 for bias weight
			for (int k = 0; k < layer_i_inputs_count+1; k++)
			{
				weights[i][j][k] = ((double) rand() / RAND_MAX - 0.5) * 5;
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



