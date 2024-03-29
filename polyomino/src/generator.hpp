#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <string>
#include <vector>

class Generator {
public:
  struct Parameter {
    int board_size;
    std::string piece_sizes;
    std::string output_path;
  };

  Generator(Parameter p) : parameter(p){};
  Generator(){};
  ~Generator(){};

  void run();
  void writeData();

private:
  Parameter parameter;

  std::vector<std::pair<int, int>> board;
  std::vector<std::vector<std::pair<int, int>>> pieces;
};

#endif
