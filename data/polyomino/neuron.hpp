/**
 * 盤面へのポリオミノの置き方に対応するNeuronクラスについて記述
 * @author Kazuki Takabatake
 * @date 2020/12/20
 */
#ifndef NEURON_HPP
#define NEURON_HPP

#include <vector>
#include <iostream>
#include <fstream>

class Neuron {
public:

	std::vector<std::vector<int>> board; // ポリオミノが一つ置かれた盤面

	Neuron() {};
	~Neuron() {};

	Neuron(std::vector<std::vector<int>>& _board, int _piece_number, int _size, int _num_edges):
		board(_board),piece_number(_piece_number),size(_size),num_edges(_num_edges) {};

	void Write(std::ofstream& ofs);
	inline int getPieceNumber() const { return piece_number; };
	inline int getSize() const { return size; };
	inline int getNumEdges() const { return num_edges; };

	/**
	 * 盤面を足し合わせる
	 * @return 足された盤面
	 */
	std::vector<std::vector<int>> operator + (Neuron neuron) const {
		std::vector<std::vector<int>> sum_board(this->board);
		for(uint32_t i=0; i<this->board.size(); ++i){
			for(uint32_t j=0; j<this->board[0].size(); ++j){
				sum_board[i][j] += neuron.board[i][j];
			}
		}
		return sum_board;
	}

private:

	int piece_number; // ポリオミノのid
	int size; // ポリオミノの大きさ
	int num_edges; // ポリオミノの辺の数
};

int countOverlapEdge(const Neuron& i, const Neuron& j);
int calcOverlap(const Neuron& i, const Neuron& j);

#endif