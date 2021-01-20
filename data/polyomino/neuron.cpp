/**
 * 盤面へのポリオミノの置き方に対応するNeuronクラスについて記述
 * @author Kazuki Takabatake
 * @date 2020/12/20
 */
#include "neuron.hpp"

using namespace std;

void Neuron::Write(ofstream& ofs){
	ofs << piece_number << "," << size << "," << num_edges << endl;
	for(const auto& vv : board){
		for(const auto& v : vv){
			ofs << v;
		}
		ofs << endl;
	}
	ofs << endl;
}

/**
 * 二つのポリオミノの置き方を重ねた際に重なる辺の数を計算
 * @param i 一つ目のニューロン
 * @param j 二つ目のニューロン
 * @return 重なる辺の数
 */
int countOverlapEdge(const Neuron& i, const Neuron& j){
	if(i.getPieceNumber() == j.getPieceNumber()){
		return 0;
	}

	int count=0;

	vector<vector<int>> bi(i.board);
	vector<vector<int>> bj(j.board);

	for(int x=0; x<bi.size(); ++x){
		for(int y=0; y<bi[0].size(); ++y){
			bi[x][y] += 2 * bj[x][y];
			if(bi[x][y] == 3){
				return 0;
			}
		}
	}

	for(int x=0; x<bi.size(); ++x){
		for(int y=0; y<bi[0].size(); ++y){
			if(bi[x][y] == 0 || bi[x][y] == 2){
				continue;
			}

			if(x-1 >= 0 && bi[x-1][y] == 2){
				++count;
			}
			if(x+1 < bi.size() && bi[x+1][y] == 2){
				++count;
			}
			if(y-1 >= 0 && bi[x][y-1] == 2){
				++count;
			}
			if(y+1 < bi[0].size() && bi[x][y+1] == 2){
				++count;
			}
		}
	}

	return count;
}

/** 
 * 二つのポリオミノの置き方を重ねた際に重なるマスの数を計算
 * @param i 一つ目のニューロン
 * @param j 二つ目のニューロン
 * @return 重なるマスの数
 */
int calcOverlap(const Neuron& i, const Neuron& j){
	vector<vector<int>> sum_board = i + j;

	int overlap = 0;
	for(uint32_t x=0; x<sum_board.size(); ++x){
		for(uint32_t y=0; y<sum_board[0].size(); ++y){
			if(sum_board[x][y] == 2){
				++overlap;
			}
		}
	}

	return overlap;
}
