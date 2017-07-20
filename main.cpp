// stl
#include <map>
#include <array>
#include <random>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

// boost
#include <boost/thread.hpp>

// c libraries
#include <assert.h>
#include <string.h>


// local



using std::array;
using std::vector;

using std::string;

using std::cin;
using std::cerr;
using std::cout;
using std::endl;
using std::ofstream;
using std::ios_base;


// Побитовая структура для хранения состояние клетки
struct Cell {
	bool is_live = false;

	bool operator== (const Cell& x) const {
		return is_live == x.is_live;
	}
};

struct Point {
	int x;
	int y;

	Point(int x, int y)
		: x(x)
		, y(y)
	{
	}
};

std::ostream& operator<< (std::ostream& os, const Cell& c) {
  return os << c.is_live;
}

typedef vector< vector<Cell> > Matrix;

namespace {
	// Получение координат соседей точки (окрестность мура 1 порядка)
	std::vector<Point> neighbors(const Matrix& m, int x, int y) {
		std::vector<Point> nb;

		int x0 = (x-1 >= 0) ? (x-1) : 0;
		int y0 = (y-1 >= 0) ? (y-1) : 0;

		int xN = (x+1 < m.size()) ? (x+1) : m.size()-1;
		int yN = (y+1 < m.size()) ? (y+1) : m.size()-1;

		for( int _x = x0; _x <= xN; ++_x ){
			for( int _y = y0; _y <= yN; ++_y ){
				if( _x == x && _y == y ){
					continue;
				}
				nb.push_back(Point(_x,_y));
			}
		}
		return nb;
	}

  // Вывести на экран игровое поле
	void print_world(const Matrix& m) {
		auto& file = cout;

		// print Header
		file << "┌─";
		for( int i = 0; i < m[0].size()-1; ++i ){
			file << "──";
		}
		file << "┐\n";
//		file << "│ " << string(m[0].size()*2, ' ') << "│\n";

		// print Frame
		for( int i = 0; i < m.size(); i++ ){
			file << "│";
			for( int j = 0; j < m[i].size()-1; j++ ){
				file << (m[i][j].is_live ? '*' : ' ');
				file << ' ';
			}
			file << (m[i][m[i].size()-1].is_live ? '*' : ' ');
			file << "│\n";
		}

		// print Footer
		file << "└─";
		for( int i = 0; i < m[0].size()-1; ++i ){
			file << "──";
		}
		file << "┘\n";
	}

}

/*
namespace my {

  template <typename T, size_t SIZE>
  void print(const Row<T,SIZE>& r) {
    for( size_t i = 0; i < SIZE; ++i ){
      cout << r[i] << " ";
    }
  }
  template <typename T, size_t ROW, size_t COL>
  void print(const Matrix<T,ROW,COL>& m) {
    for( size_t i = 0; i < ROW; ++i ){
      print(m[i]);
      cout << "\n";
    }
  }

  template <size_t ROW, size_t COL>
  void init_world(Matrix<Cell,ROW,COL>& w) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for( int i = 0; i < w.row; i++ ){
      for( int j = 0; j < w.row; j++ ){
        w[i][j].is_live = dis(gen);
      }
    }
  }

  // Количество живых клеток на игровом поле
  template <size_t ROW, size_t COL>
  int get_live_count(const Matrix<Cell,ROW,COL>& w) {
    int count = 0;
    for( size_t i = 0; i < w.row; i++ ){
      for( size_t j = 0; j < w.col; j++ ){
        if (w[i][j].is_live) {
          count++;
        }
      }
    }
    return count;
  }

  // Количество живых соседей у клетки с координатами x, y
  template <size_t ROW, size_t COL>
  unsigned int count_live_neighbors(const Matrix<Cell,ROW,COL>& w, int x, int y) {
    int count = 0;
    auto nb = w.neighbors(x, y);

    for(int i = 0; i < nb.size(); i++) {
      int _x = nb[i].first;
      int _y = nb[i].second;

      if (w[_x][_y].is_live) {
        count++;
      }
    }

    return count;
  }

  // Сгенерировать следующее поколение игрового мира
  template <size_t ROW, size_t COL>
  Matrix<Cell,ROW,COL> next_generation(const Matrix<Cell,ROW,COL>& prev_world) {
    Matrix<Cell,ROW,COL> w;

    for( auto i = 0; i < prev_world.row; i++ ){
      for( auto j = 0; j < prev_world.col; j++ ){
        const Cell& p = prev_world[i][j];
        const int live_nb = count_live_neighbors(prev_world, i, j);

        if( !p.is_live ){
          if (live_nb == 3) {
            w[i][j].is_live = true;
          }
        }
        else {
          if (live_nb == 2 || live_nb == 3) {
            w[i][j].is_live = true;
          }
        }
      }
    }

    return w;
  }

} // namespace
*/


int main() {
	Matrix m;
	m.resize(11);
	for( auto& r : m ){
		r.resize(11);
	}

	auto ng = neighbors(m, 0, 0);
	{
		auto t = neighbors(m, 0, m[0].size()-1);
		ng.insert(ng.end(), t.begin(),t.end());
	}
	{
		auto t = neighbors(m, m.size()-1, 0);
		ng.insert(ng.end(), t.begin(),t.end());
	}
	{
		auto t = neighbors(m, m.size()-1, m[0].size()-1);
		ng.insert(ng.end(), t.begin(),t.end());
	}
	{
		auto t = neighbors(m, (m.size()-1)/2, (m[0].size()-1)/2);
		ng.insert(ng.end(), t.begin(),t.end());
	}

	for( auto& n : ng ){
		m[n.x][n.y].is_live = true;
	}

	print_world(m);
//  my::Matrix<Cell,__WORLD_WIDTH__,__WORLD_HEIGHT__> world;
//  my::Matrix<Cell, world.row, world.col> prev_world;
//
//  my::init_world(world);
////  world[2][2].is_live = true;
////  world[3][2].is_live = true;
////  world[4][2].is_live = true;
//
//	int live_points = -1;
//	bool is_optimal = false;
//
//	do {
//		print_world(world);
//		prev_world = world;
//    world = my::next_generation(prev_world);
//
//		is_optimal = world == prev_world;
//		live_points = my::get_live_count(world);
//
//		if (is_optimal) {
//			cout << "Optimal configuration detected" << endl;
//		}
//
//		if (live_points == 0) {
//			cout << "All points died" << endl;
//		}
//		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
//	} while (live_points != 0 && !is_optimal);

	cout << "DONE" << endl;

  return 0;
}
