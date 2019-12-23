#ifndef SQUARE_COLLECTION
#define SQUARE_COLLECTION


#include "square_army.h"
#include "gen_random.h"
#include <vector>
#include <future>
#include <iostream>
#include <thread>

class SquareThread {
public:
	SquareThread(int square_index_start, int square_index_end, int x_squares, float* data);
	std::vector<SquareArmy> squares;
	//GenRand *gen_rand=new GenRand();
	GenRand* gen_rand = nullptr;
	void update();

	SquareArmy& get_square_army(int local_index);
	
	SquareThread(const SquareThread& old_obj);
	SquareThread& operator = (const SquareThread& old_obj);
	~SquareThread();
	
};



class SquareCollection {
public:
	SquareCollection(int screen_width, int screen_height, int square_length, float* data, int thread_count);
	int x_squares=0;
	int y_squares=0;
	int squares_per_thread=0;
	std::vector<SquareThread> square_threads;
	void update();

	SquareArmy& get_square_army(int index);
	SquareArmy& get_square_army(int x, int y);  // use y*x_squares + x 
};

#endif