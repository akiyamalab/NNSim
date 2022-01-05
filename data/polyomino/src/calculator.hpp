/**
 *
 * @author Kazuki Takabatake
 * @date 2021/5/2
 */

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <string>
#include <vector>

#include "neuron.hpp"
#include "piece.hpp"

struct Bias {
  float before_bias;
  float after_bias;
};

struct BiasDetail {
  float b_A, b_B, b_C, b_D, b_E;
  float a_A, a_B, a_C, a_D, a_E;
};

struct Weight {
  uint32_t neuron_id;
  float before_weight;
  float after_weight;
};

struct WeightDetail {
  uint32_t neuron_id;
  float b_A, b_B, b_C, b_D, b_E, b_F;
  float a_A, a_B, a_C, a_D, a_E, a_F;
};

class Calculator {
public:
  struct Parameter {
    std::string input_file;
    std::string output_file;
    float A, B, C, D, E, F;
    float gamma;
    int cut_bubble_size;
    bool rotation;
    bool inversion;
  };

  Calculator(Parameter &_parameter);
  Calculator(){};
  ~Calculator(){};

  std::vector<BiasDetail> calcBiasDetail();
  std::vector<WeightDetail> calcWeightDetail(const uint32_t neuron_id);

  void writeInfo();
  void writeData();
  void writeDataDetail();

private:
  Parameter parameter;

  std::vector<std::vector<int>> board;
  std::vector<Piece> pieces;
  std::vector<Neuron> neurons;
};

#endif