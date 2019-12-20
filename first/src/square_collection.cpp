
#include "square_collection.h"

SquareCollection::SquareCollection(int screen_width, int screen_height, int square_length, float* data, int thread_count){
	int x_squares = screen_width / square_length;
	int y_squares = screen_height / square_length;
	int data_array_size = (x_squares * y_squares) * 3;
	int square_array_size = data_array_size / 3;
	

	int squares_per_thread = square_array_size / thread_count;
	int extra_squares = square_array_size % thread_count;

	int square_index_start=0;
	int square_index_end = 0;
	for (int i = 0; i < thread_count; i++) {
		int real_thread_size= squares_per_thread;
		if (i < extra_squares) {
			real_thread_size++;
		}
		square_index_end = square_index_start+real_thread_size;
		//if (square_index_end > square_array_size) {
		//	__debugbreak();
		//}
		//std::cout << "make thread from("<<square_index_start<<" to "<< square_index_end<<") with squares per thread "<< squares_per_thread <<std::endl;
		//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		square_threads.push_back(SquareThread(square_index_start, square_index_end, x_squares, data)); //end exclusive
		//std::cout << "after thread make" << std::endl;
		square_index_start= square_index_end;
	}
}


SquareThread::SquareThread(int square_index_start, int square_index_end, int x_squares, float* data)//end exclusive
{
	//std::cout <<"thread rand:"<< gen_rand->getStandardUniform_100() << std::endl;;
	int min_y = square_index_start / x_squares;
	int max_y = (square_index_end-1) /x_squares;
	int starting_x = square_index_start % x_squares;
	int ending_x= (square_index_end-1) % x_squares;  

	int data_index=0;
	int square_index=0;
	for (int y = min_y; y <= max_y; y++) {
		int min_x = 0;
		int max_x = x_squares-1;
		if (y == min_y)
			min_x = starting_x;
		if (y == max_y)
			max_x = ending_x;
		for (int x = min_x; x <= max_x; x++) {
			square_index = (y * x_squares) + x;
			data_index = square_index * 3;
			//if (data_index + 2 => 18000) {
				//__debugbreak();
			//}
			SquareArmy temp_square_army(x, y, data[(data_index)], data[(data_index + 1)], data[(data_index + 2)], *gen_rand);
			//squares.push_back(SquareArmy(x, y, data[(data_index)], data[(data_index + 1)], data[(data_index + 2)], gen_rand));
			squares.push_back(temp_square_army);
		}
	}
	//std::cout << "data_index:" << data_index << std::endl;

}

void SquareCollection::update() {
	std::vector<std::future<void>> promise_list;
	for (auto it = square_threads.begin(); it != square_threads.end(); ++it) {
		promise_list.push_back(std::async(std::launch::async, &SquareThread::update, &(*it)));
	}
	for (auto it = promise_list.begin(); it != promise_list.end(); ++it) {
		it->wait();
	}
	promise_list.clear();
}

void SquareThread::update() {
	for (auto it = squares.begin(); it != squares.end(); ++it) {
		it->update();
	}
}

/*
SquareThread::SquareThread(const SquareThread& old_obj) {
	squares = old_obj.squares;
	GenRand* gen_rand = new GenRand();
}

SquareThread& SquareThread::operator = (const SquareThread& old_obj) {
	squares = old_obj.squares;
	GenRand* gen_rand = new GenRand();
}

SquareThread::~SquareThread() {
	delete gen_rand;
}
*/