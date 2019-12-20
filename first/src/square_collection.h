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
	GenRand *gen_rand=new GenRand();
	void update();

	//SquareThread(const SquareThread& old_obj);
	//SquareThread& operator = (const SquareThread& old_obj);
	//~SquareThread();
};



class SquareCollection {
public:
	SquareCollection(int screen_width, int screen_height, int square_length, float* data, int thread_count);
		
	std::vector<SquareThread> square_threads;
	void update();
};

#endif