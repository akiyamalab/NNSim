/**
 * 六方形タイルのクラスを記述
 * @author Kazuki Takabatake
 * @date 2021/3/2
 */

#ifndef HEX_HPP
#define HEX_HPP

#include <cmath>
#include <iostream>
#include <string>

static constexpr float root3 = 1.732;

typedef struct {
  int point;
  int x;
  int y;
} HexStruct;

class Hex {
public:
  Hex(const int _point, const int _x, const int _y) : point(_point), x(_x), y(_y) {
    if ((x + y) % 2 != 0) std::cerr << "[ERROR] wrong hex " << x << "," << y << std::endl;
  }
  Hex() {}
  ~Hex() {}

  void operator+=(const Hex& other) {
    x += other.getX();
    y += other.getY();
  }

  bool operator==(const Hex& other) const {
    if (x == other.getX() && y == other.getY()) {
      return true;
    }
    return false;
  }

  bool isConnect(const Hex& other) const {
    if (std::abs(x - other.getX()) <= 2 && std::abs(y - other.getY()) <= 1) {
      return true;
    }
    return false;
  }

  void rotate60() {
    // ユークリッド座標に変換
    float yx = root3 * x;
    float yy = 3 * y;

    float yx_rotated = 0.5 * yx - root3 * yy * 0.5;
    float yy_rotated = root3 * yx * 0.5 + 0.5 * yy;

    x = std::round(yx_rotated / root3);
    y = std::round(yy_rotated / 3);
  }

  void inverse() { x = -x; }

  void setPoint(int p) { point = p; }
  int getPoint() const { return point; }
  int getX() const { return x; }
  int getY() const { return y; }

  std::string getStatus() const { return std::to_string(x) + "," + std::to_string(y); }

private:
  int point;
  int x;
  int y;
};

#endif