#include "neural_network_model.hpp"

#include "discrete_neuron.hpp"
#include "utils.hpp"

#include <iostream>
#include <fstream>

using namespace std;

namespace{
  vector<int> readBias(string bias_file){
    ifstream ifs(bias_file);
    if(ifs.fail()){
      cout << "[ERROR] file open error: " << bias_file << endl;
      exit(1);
    }

    vector<int> bias;
    string line;
    while(getline(ifs, line)){
      bias.emplace_back(stoi(line));
    }

    return bias;
  }
}

NeuralNetworkModel::NeuralNetworkModel(bool _sync, int num_neurons, std::string weights_file, std::string bias_file):
    sync(_sync){

  vector<int> bias = readBias(bias_file);
  if(num_neurons != bias.size()){
    cout << "[ERROR] wrong bias size, num of neurons: " << num_neurons << ", bias size: " << bias.size() << endl;
    exit(1); 
  }

  for(int i=0; i<num_neurons; ++i){
    neurons.emplace_back(std::make_shared<DiscreteNeuron>(bias[i]));
  }

  //TODO
  weights.resize(num_neurons);
  ifstream ifs(weights_file);

  string line;
  while(getline(ifs, line)){
    vector<string> split_line = utils::split(line, ',');
    Weight w = {stoi(split_line[0]), stoi(split_line[2])};
    weights[stoi(split_line[1])].emplace_back(w);
  }

  for(int i=0; i<num_neurons; ++i){
    cout << i << ", num weights: " << weights[i].size() << endl;
  }
}

void NeuralNetworkModel::update(){
  
}