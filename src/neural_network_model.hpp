#ifndef NEURAL_NETWORK_MODEL_HPP
#define NEURAL_NETWORK_MODEL_HPP

#include <vector>
#include <string>
#include <fstream>
#include <random>

class NeuralNetworkModel {
public:

	typedef struct {
		std::string weights_file;
		std::string biases_file;
		std::string output_file;

		bool synchronize;
		bool inner_potential;
		uint32_t seed;
		uint32_t generations;
		uint32_t time_constant;
		float delta_t;
		float base_potential;
	}Parameters;

	typedef struct {
		uint32_t neuron_id;
		float weight;
	}Weight;

	NeuralNetworkModel() {};
	NeuralNetworkModel(const Parameters& parameters);
	virtual ~NeuralNetworkModel() {};

	virtual void simulate() {};

protected:
	float func(const float input);
	float inverseFunc(const float input);

	void output();
	void outputU();
	bool calcEnergyNQueen();

	std::vector<float> potentials;
	std::vector<float> outputs;
	std::vector<float> outputs_old;
	std::vector<float> biases;
	std::vector<std::vector<Weight>> weights;

	std::mt19937 mt;
	std::uniform_int_distribution<> rand_int;

	Parameters parameters;
	uint32_t num_neurons;
	float reciprocal_time_constant;
	float reciprocal_base_potential;

private:
	void readWeights();
	void readBiases();

	std::ofstream ofs;
};

#endif
