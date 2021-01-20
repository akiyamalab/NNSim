/**
 * メイン関数。ポリオミノパズルの情報ファイルを入力としてNN用の結合重み・バイアス値情報等を出力 
 * @author Kazuki Takabatake
 * @date 2020/12/20
 */

#include "neuron.hpp"
#include "piece.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>

using namespace std;

float A,B,C,D,gamma;

// バイアス値。beforeがシミュレーション開始時、afterが終了時の値
typedef struct{
	float before_bias;
	float after_bias;
} Bias;

// 結合重み。
typedef struct{
	uint32_t neuron_id;
	float before_weight;
	float after_weight;
} Weight;

/**
 * 文字列分割関数
 * @param str 分割前文字列
 * @param delim 分割文字
 * @return 分割された文字列のvector
 */
std::vector<std::string> split(const std::string &str, const char delim){
	std::vector<std::string> elems;
	std::string item;
	for(char ch: str){
		if(ch == delim){
			elems.push_back(item);
			item.clear();
		}else{
			item += ch;
		}
	}

	elems.push_back(item);
	return elems;
}

/**
 * 入力ファイルから、盤面と各ピース（ポリオミノ）の情報を読み込む
 * @param ifs 入力ファイルストリーム
 * @param board 盤面
 * @param pieces 全ピース
 * @param rotation 回転フラグ
 * @param inversion 反転フラグ
 */
void Read(ifstream& ifs, vector<vector<int>>& board, vector<Piece>& pieces, bool rotation, bool inversion){
	string line;

	getline(ifs, line);
	vector<string> splited_line = split(line, ',');

	int num_pieces = stoi(splited_line[0]);
	cout << "num pieces: " << num_pieces << endl;
	int boardx = stoi(splited_line[1]);
	int boardy = stoi(splited_line[2]);
	board.resize(boardx, vector<int>(boardy));
	cout << "board size: " << boardx << " by " << boardy << endl;

	for(int i=0; i<num_pieces; ++i){
		getline(ifs, line);
		getline(ifs, line);
		vector<string> piece_size = split(line, ',');
		int piecex = stoi(piece_size[0]);
		int piecey = stoi(piece_size[1]);
		vector<vector<int>> default_piece_state(piecex, vector<int>(piecey));

		for(int j=0; j<piecex; ++j){

			getline(ifs, line);
			vector<string> piece_column = split(line, ',');

			for(int k=0; k<piecey; ++k){
				default_piece_state[j][k] = stoi(piece_column[k]);
			}
		}

		Piece piece(default_piece_state, i, rotation, inversion);
		pieces.emplace_back(piece);

	}
}

/**
 * 盤面、ピース、位置からニューロンになるか判定し、盤面にピースを置く
 * @param board 盤面
 * @param piece ピース
 * @param x ピースを置くx座標
 * @param y ピースを置くy座標
 * @return ピースがおける場合はtrue,置けない場合はfalse
 */
bool generateNeuronBoard(vector<vector<int>>& board, vector<vector<int>>& piece, int x, int y){
	for(int i=0; i<piece.size(); ++i){
		for(int j=0; j<piece[0].size(); ++j){

			if(piece[i][j] == 1){
				if(x+i >= board.size() || y+j >= board[0].size()){
					return false;
				}else{
					board[x+i][y+j] = 1;
				}
			}

		}
	}
	return true;
}

/**
 * 指定座標が盤面上で、かつピースが存在するか判定
 * @param board 盤面
 * @param x 指定するx座標
 * @param y 指定するy座標
 * @return ピースが存在する座標であればtrue,そうでなければfalse
 */
bool isEmpty(vector<vector<int>>& board, int x, int y){
	if(x < 0 || x >= board.size() || y < 0 || y >= board[0].size()){

	}else if(board[x][y] == 0){
		board[x][y] = 1;
		return true;
	}
	return false;
}

/**
 * 盤面にバブル（指定数以下のピースや壁に囲まれた空間）が存在するかどうか判定
 * @param board 盤面
 * @param but_bubble_size 空間がこの値以下であればバブルと判定
 * @return バブルが存在する場合はtrue,存在しない場合はfalse
 */
bool checkBubble(const vector<vector<int>>& board, const int cut_bubble_size){
	vector<vector<int>> check_board(board);

	for(int x=0; x<check_board.size(); ++x){
		for(int y=0; y<check_board[0].size(); ++y){
			if(check_board[x][y] == 0){

				vector<pair<int, int>> cells;
				cells.emplace_back(make_pair(x,y));
				check_board[x][y] = 1;

				for(uint32_t i=0; i<cells.size(); ++i){
					int current_x = cells[i].first;
					int current_y = cells[i].second;

					if(isEmpty(check_board, current_x + 1, current_y)){
						cells.emplace_back(make_pair(current_x + 1, current_y));
					}
					if(isEmpty(check_board, current_x - 1, current_y)){
						cells.emplace_back(make_pair(current_x - 1, current_y));
					}
					if(isEmpty(check_board, current_x, current_y + 1)){
						cells.emplace_back(make_pair(current_x, current_y + 1));
					}
					if(isEmpty(check_board, current_x, current_y - 1)){
						cells.emplace_back(make_pair(current_x, current_y - 1));
					}
				}

				if(cells.size() <= cut_bubble_size){
					return true;
				}

			}
		}
	}
	return false;
}

/**
 * ニューロンiとjの間の結合重みを計算
 * @param neurons ニューロン集合
 * @param i ニューロンiの番号
 * @param j ニューロンjの番号
 * @param cut_bubble_size 空間のサイズがこの値以下であればバブルと判定
 * @return 結合重み
 */
Weight calcWeight(const vector<Neuron>& neurons, const uint32_t i, const uint32_t j, const int cut_bubble_size){
	float before_weight = 0, after_weight = 0;

	if(neurons[i].getPieceNumber() == neurons[j].getPieceNumber()){
		before_weight += - A;
		after_weight += - A;
	}

	before_weight += - B * calcOverlap(neurons[i], neurons[j]);
	after_weight += - B * calcOverlap(neurons[i], neurons[j]);

	if(checkBubble(neurons[i] + neurons[j], cut_bubble_size)){
		after_weight += - C ;
		//TODO バブル項を一定にする設定
		before_weight += -C ;
	}

	float ave_num_edges = (float)(neurons[i].getNumEdges() + neurons[j].getNumEdges()) / 2;
	before_weight += D * countOverlapEdge(neurons[i], neurons[j]) / ave_num_edges;

	Weight w = {j, before_weight, after_weight};

	return w;
}

// メイン関数
int main(int argc, char *argv[]){

	if(argc != 11){
		cout << "./polyomino [input_file] [output_file] [cut_bubble_size] [rotation_flag] [inversion_flag] [A] [B] [C] [D] [gamma]" << endl;
		exit(0);
	}

	string input_file = argv[1];
	string output_file = argv[2];
	int cut_bubble_size = stoi(argv[3]); // 空間のサイズがこの値以下であればバブルと判定
	int _rotation = stoi(argv[4]); // 回転を考慮するか
	int _inversion = stoi(argv[5]); // 反転を考慮するか
	A = stof(argv[6]);
	B = stof(argv[7]);
	C = stof(argv[8]);
	D = stof(argv[9]);
	gamma = stof(argv[10]);

	bool rotation, inversion;
	if(_rotation == 1){
		rotation = true;
	}else{
		rotation = false;
	}

	if(_inversion == 1){
		inversion = true;
	}else{
		inversion = false;
	}

	vector<vector<int>> board;
	vector<Piece> pieces;

	ifstream ifs(input_file);

	Read(ifs, board, pieces, rotation, inversion);
	ifs.close();

	int sum_piece_sizes = 0;
	for(const auto& s : pieces){
		sum_piece_sizes += s.getSize();
	}
	if(board.size() * board[0].size() != sum_piece_sizes){
		cout << "[ERROR] board cells: " << board.size() * board[0].size()
				 << ", sum piece sizes: " << sum_piece_sizes << endl;
		exit(0);
	}

	vector<Neuron> neurons;

	for(int i=0; i<pieces.size(); ++i){
		for(const auto& piece_state : pieces[i].getStates()){
			vector<vector<int>> state = piece_state.getState();

			for(int x=0; x<board.size(); ++x){
				for(int y=0; y<board[0].size(); ++y){
					vector<vector<int>> current_board(board);
					bool is_generated = generateNeuronBoard(current_board, state, x, y);
					if(is_generated){

						bool has_bubble = checkBubble(current_board, cut_bubble_size);
						if(!has_bubble){
							Neuron neuron(current_board, i, pieces[i].getSize(), pieces[i].getNumEdges());
							neurons.emplace_back(neuron);
						}
					}
				}
			}

		}
	}

	ofstream ofs_info(output_file + "_info");
	for(auto& n : neurons){
		n.Write(ofs_info);
	}
	ofs_info.close();

	uint32_t neurons_size = neurons.size();

	// biases
	ofstream ofs_biases(output_file + "_biases", ios::binary);
	vector<Bias> biases;

	for(uint32_t i=0; i<neurons_size; ++i){
		float ai = neurons[i].getSize();
		float b1 = (gamma - 0.5) * A + (gamma - 0.5) * B * ai;
		float b2 = 0.5 * A + 0.5 * B * ai;
		Bias b = {b1, b2};
		biases.emplace_back(b);
	}
	ofs_biases.write((char*)&neurons_size, sizeof(uint32_t));
	ofs_biases.write((char*)&biases[0], sizeof(Bias) * neurons_size);
	ofs_biases.close();

	// weights
	ofstream ofs_weights(output_file + "_weights", ios::binary);
	ofs_weights.write((char*)&neurons_size, sizeof(uint32_t));

	for(uint32_t i=0; i<neurons_size; ++i){
		vector<Weight> Ws;

		for(uint32_t j=0; j<neurons_size; ++j){
			if(i == j){
				continue;
			}

			Weight w = calcWeight(neurons, i, j, cut_bubble_size);
			Ws.emplace_back(w);
		}

		uint32_t Ws_size = Ws.size();
		ofs_weights.write((char*)&Ws_size, sizeof(uint32_t));
		ofs_weights.write((char*)&Ws[0], sizeof(Weight) * Ws_size);
	}

	ofs_weights.close();

	return 0;
}
