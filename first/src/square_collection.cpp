
#include "square_collection.h"

SquareCollection::SquareCollection(int screen_width, int screen_height, int square_length, float* data, std::vector<float*> target_list,  int thread_count) {
	x_squares = screen_width / square_length;
	y_squares = screen_height / square_length;
	int square_array_size = (x_squares * y_squares);
	int data_array_size = square_array_size * 3;
	average_squares_per_thread = square_array_size / thread_count;

	extra_thread_squares = square_array_size % thread_count;

	int square_index_start=0;
	int square_index_end = 0;

	square_threads.reserve(thread_count);

	for (int i = 0; i < thread_count; i++) {
		int real_thread_size= average_squares_per_thread;
		if (i < extra_thread_squares) {
			real_thread_size++;
		}
		square_index_end = square_index_start+real_thread_size;
		square_threads.emplace_back(square_index_start, square_index_end, x_squares, y_squares, data, target_list);
		square_index_start= square_index_end;
	}
	add_links();

}


SquareThread::SquareThread(int square_index_start, int square_index_end, int x_squares, int y_squares, float* data, std::vector<float*> target_list)//end exclusive
{

	int min_y = square_index_start / x_squares;
	int max_y = (square_index_end-1) /x_squares;
	int starting_x = square_index_start % x_squares;
	int ending_x= (square_index_end-1) % x_squares;
	squares_in_thread  = square_index_end - square_index_start;
	
	squares.reserve(squares_in_thread);

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

			bool boundary=false;
			if (x == x_squares - 1 || y == y_squares - 1 || x==0 || y==0)
				boundary = true;
			squares.emplace_back(x, y, data[(data_index)], data[(data_index + 1)], data[(data_index + 2)], target_list, gen_rand, boundary, data_index);
		}
	}

}

void SquareCollection::add_link(SquareArmy& square1, SquareArmy& square2) {
	square1.make_link(square2);
	square2.make_link(square1);

}


void SquareCollection::add_links() {
	for (int y = 0; y < y_squares; y++) {
		for (int x = 0; x < x_squares; x++) {
			if (y > 0) {
				add_link(get_square_army(x, y), get_square_army(x, y - 1));
				if (x > 0) {
					add_link(get_square_army(x, y), get_square_army(x - 1, y - 1));
				}
				if (x < (x_squares - 1)) {
					add_link(get_square_army(x, y), get_square_army(x + 1, y - 1));

				}
			}
			if (x > 0) {
				add_link(get_square_army(x, y), get_square_army(x - 1, y));
			}
		}
	}
}

void SquareCollection::update_links() {
	std::vector<std::future<void>> promise_list;
	for (auto it = square_threads.begin(); it != square_threads.end(); ++it) {
		promise_list.push_back(std::async(std::launch::async, &SquareThread::update_links, &(*it)));
	}
	for (auto it = promise_list.begin(); it != promise_list.end(); ++it) {
		it->wait();
	}
	promise_list.clear();
}

void SquareCollection::update_squares() {
	std::vector<std::future<void>> promise_list;
	for (auto it = square_threads.begin(); it != square_threads.end(); ++it) {
		promise_list.push_back(std::async(std::launch::async, &SquareThread::update_squares, &(*it)));
	}
	for (auto it = promise_list.begin(); it != promise_list.end(); ++it) {
		it->wait();
	}
	promise_list.clear();
}



void SquareCollection::update() {
	update_links();
	update_squares();
}



void SquareThread::update_links() {
	for (auto it = squares.begin(); it != squares.end(); ++it) {
		it->update_links_wave();
	}
}

void SquareThread::update_squares() {
	for (auto it = squares.begin(); it != squares.end(); ++it) {
		it->update_squares_wave();
	}
}

SquareArmy& SquareCollection::get_square_army(int x, int y) {
	return get_square_army((y * x_squares) + x);
}

SquareArmy& SquareCollection::get_square_army(int index) {
	int thread = 0;
	while (true) {
		int thread_squares = average_squares_per_thread;
		if (thread < extra_thread_squares) {
			thread_squares++;
		}
		if (index < thread_squares) {
			break;
		}
		else {
			thread++;
			index -= thread_squares;
		}

	}
	return square_threads[thread].get_square_army(index);
}

SquareArmy& SquareThread::get_square_army(int local_index) {
	return squares[local_index];
}




/*

SquareThread::SquareThread(const SquareThread& old_obj){
	GenRand* gen_rand = new GenRand();
	squares = old_obj.squares;
	for (auto it = squares.begin(); it != squares.end(); ++it) {
		(*it)->update_gen_rand(gen_rand);
	}
}

SquareThread& SquareThread::operator = (const SquareThread& old_obj) {
	GenRand* gen_rand = new GenRand();
	squares = old_obj.squares;
	for (auto it = squares.begin(); it != squares.end(); ++it) {
		(*it)->update_gen_rand(gen_rand);
	}
	return *this;
}

SquareThread::~SquareThread() {
	delete gen_rand;
}

*/

SquareThread::SquareThread(const SquareThread&) {
	std::cout << "never copy" << std::endl;
}



