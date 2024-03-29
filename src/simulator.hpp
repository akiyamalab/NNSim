#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <boost/program_options.hpp>
#include <string>

namespace po = boost::program_options;

class Simulator {
public:

  typedef struct {
    std::string input_path;
    std::string output_path;

    char model;
    bool synchronize;
    bool inner_potential;
    uint32_t seed;
    uint32_t generations;
    uint32_t threads;
    uint32_t time_constant;
    float delta_t;

    // Gaussian Sharpening Parameters
    float T_mf;
    uint32_t time_constant_T_mf;

    // Gaussian Annealing Parameters
    float T_epsilon;
    uint32_t time_constant_T_epsilon;
  } Parameters;

  Simulator(){};
  ~Simulator(){};

  void run(po::variables_map &vm);

private:
  void setParameters(po::variables_map &vm);

  Parameters parameters;
};

#endif
