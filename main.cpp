// stl
#include <map>
#include <array>
#include <random>
//#include <thread>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

// boost
#include <boost/thread.hpp>

// c libraries
#include <assert.h>
#include <string.h>




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
struct point {
	bool is_live = false;

  bool operator== (const point& x) const {
    return is_live == x.is_live;
  }
};

std::ostream& operator<< (std::ostream& os, const point& p) {
  return os << p.is_live;
}

enum {
	__WORLD_WIDTH__ = 10,
	__WORLD_HEIGHT__ = 25
};

namespace my {
  template <typename T, size_t SIZE>
  struct Row {
    enum {
      size = SIZE
    };

    array<T,SIZE> row;

    T& operator[] (const size_t index){
      return row[index];
    }
    const T& operator[] (const size_t index) const {
      return row[index];
    }

    bool operator== (const Row<T,SIZE>& r) const {
      return row == r.row;
    }
  };
  template <typename T, size_t ROW, size_t COL>
  struct Matrix {
    enum {
      row = ROW,
      col = COL
    };

    array<Row<T,COL>, ROW> matrix;

    Row<T,COL>& operator[] (const size_t index) {
      return matrix[index];
    }
    const Row<T,COL>& operator[] (const size_t index) const {
      return matrix[index];
    }

    bool operator== (const Matrix<T,ROW,COL>& m) const {
      return matrix == m.matrix;
    }
    // Получение координат соседей точки (окрестность мура 1 порядка)
    std::vector< std::pair<int,int> > neighbors(int x, int y) const {
      std::vector< std::pair<int,int> > nb;

      for( int i = x - 1; i <= x + 1; i++ ){
        for( int j = y - 1; j <= y + 1; j++ ){
          if( i == x && j == y ){
            continue;
          }
          if( i < 0 || j < 0 ){
            continue;
          }
          if( i >= ROW || j >= COL ){
            continue;
          }

          std::pair<int,int> p;
          p.first = i;
          p.second = j;

          nb.push_back(p);
        }
      }
      return nb;
    }
  };

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
  void init_world(Matrix<point,ROW,COL>& w) {
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
  int get_live_count(const Matrix<point,ROW,COL>& w) {
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
  unsigned int count_live_neighbors(const Matrix<point,ROW,COL>& w, int x, int y) {
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
  Matrix<point,ROW,COL> next_generation(const Matrix<point,ROW,COL>& prev_world) {
    Matrix<point,ROW,COL> w;

    for( auto i = 0; i < prev_world.row; i++ ){
      for( auto j = 0; j < prev_world.col; j++ ){
        const point& p = prev_world[i][j];
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

  // Вывести на экран игровое поле
  template <size_t ROW, size_t COL>
  void print_world(const Matrix<point,ROW,COL>& w) {
    //ofstream file("cppstudio.txt", ios_base::trunc);
    //ofstream file("cppstudio.txt", ios_base::app);
    auto& file = cout;
		file << "┌─";
		for( int i = 0; i < w.col; ++i ){
			file << "──";
		}
		file << "─┐\n";
		for( int i = 0; i < w.row; i++ ){
			file << "│ ";
			for( int j = 0; j < w.col; j++ ){
				if( w[i][j].is_live ){
					file << '*';
				}
				else {
					file << ' ';
				}
				file << ' ';
			}
			file << " │\n";
		}
		file << "└─";
		for( int i = 0; i < w.col; ++i ){
			file << "──";
		}
		file << "─┘\n";
  }
} // namespace


int main() {
  my::Matrix<point,__WORLD_WIDTH__,__WORLD_HEIGHT__> world;
  my::Matrix<point, world.row, world.col> prev_world;

  my::init_world(world);
//  world[2][2].is_live = true;
//  world[3][2].is_live = true;
//  world[4][2].is_live = true;

	int live_points = -1;
	bool is_optimal = false;

	do {
		print_world(world);
		prev_world = world;
    world = my::next_generation(prev_world);

		is_optimal = world == prev_world;
		live_points = my::get_live_count(world);

		if (is_optimal) {
			cout << "Optimal configuration detected" << endl;
		}

		if (live_points == 0) {
			cout << "All points died" << endl;
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	} while (live_points != 0 && !is_optimal);

  return 0;
}
