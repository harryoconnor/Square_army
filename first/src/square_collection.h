#ifndef SQUARE_COLLECTION
#define SQUARE_COLLECTION


#include "square_army.h"
#include "gen_random.h"
#include <vector>
#include <future>
#include <iostream>
#include <thread>
#include <memory>

class SquareThread {
public:
	SquareThread(const SquareThread&);
public:
	SquareThread(int square_index_start, int square_index_end, int x_squares, int y_squares, float* data, std::vector<float*> target_list);
	//std::vector<SquareArmy> squares;
	std::vector<SquareArmy> squares;
	//GenRand *gen_rand=new GenRand();
	GenRand gen_rand;
	// void update();
	void update_links();
	void update_squares();
	int squares_in_thread = 0;

	SquareArmy& get_square_army(int local_index);
	
	//SquareThread(const SquareThread& old_obj);
	//SquareThread& operator = (const SquareThread& old_obj);
	//~SquareThread();
	
};



class SquareCollection {
private:
	SquareCollection(const SquareCollection&);
public:
	SquareCollection(int screen_width, int screen_height, int square_length, float* data, std::vector<float*> target_list, int thread_count);
	int x_squares=0;
	int y_squares=0;
	int average_squares_per_thread=0;
	int extra_thread_squares = 0;
	std::vector<SquareThread> square_threads;
	void update();
	void update_links();
	void update_squares();

	SquareArmy& get_square_army(int index);
	SquareArmy& get_square_army(int x, int y);  // use y*x_squares + x 

	void add_links();

	void add_link(SquareArmy& square1, SquareArmy& square2);
};

#endif