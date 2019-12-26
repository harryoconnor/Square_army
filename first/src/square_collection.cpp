
#include "square_collection.h"

SquareCollection::SquareCollection(int screen_width, int screen_height, int square_length, float* data, int thread_count){
	x_squares = screen_width / square_length;
	y_squares = screen_height / square_length;
	int square_array_size = (x_squares * y_squares);
	int data_array_size = square_array_size * 3;
	squares_per_thread = square_array_size / thread_count;

	int squares_per_thread = square_array_size / thread_count;
	int extra_squares = square_array_size % thread_count;

	int square_index_start=0;
	int square_index_end = 0;

	square_threads.reserve(thread_count);

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
		//square_threads.push_back(SquareThread(square_index_start, square_index_end, x_squares, data)); //end exclusive
		square_threads.emplace_back(square_index_start, square_index_end, x_squares, data);
		//std::cout << "after thread make" << std::endl;
		square_index_start= square_index_end;
	}
	add_links();

}


SquareThread::SquareThread(int square_index_start, int square_index_end, int x_squares, float* data)//end exclusive
{
	//std::cout <<"thread rand:"<< gen_rand->getStandardUniform_100() << std::endl;;
	//gen_rand = new GenRand();
	int min_y = square_index_start / x_squares;
	int max_y = (square_index_end-1) /x_squares;
	int starting_x = square_index_start % x_squares;
	int ending_x= (square_index_end-1) % x_squares;
	int squares_per_thread = square_index_end - square_index_start;
	
	squares.reserve(squares_per_thread);

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
			//SquareArmy temp_square_army(x, y, data[(data_index)], data[(data_index + 1)], data[(data_index + 2)], gen_rand);
			//std::shared_ptr<SquareArmy> temp_square_army = std::make_shared<SquareArmy>(x, y, data[(data_index)], data[(data_index + 1)], data[(data_index + 2)], gen_rand);
			//squares.push_back(SquareArmy(x, y, data[(data_index)], data[(data_index + 1)], data[(data_index + 2)], gen_rand));
			//squares.push_back(temp_square_army);
			squares.emplace_back(x, y, data[(data_index)], data[(data_index + 1)], data[(data_index + 2)], gen_rand);
		}
	}
	//std::cout << "data_index:" << data_index << std::endl;
}

void SquareCollection::add_link(SquareArmy& square1, SquareArmy& square2) {
	Link& link_1_to_2 = square1.make_link(square2);
	Link& link_2_to_1 = square2.make_link(square1);
	link_1_to_2.other_link = &link_2_to_1;
	link_2_to_1.other_link = &link_1_to_2;
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

/*
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
*/

void SquareCollection::update() {
	update_links();
	update_squares();
}

/*
void SquareThread::update() {
	for (auto it = squares.begin(); it != squares.end(); ++it) {
		(*it)->update();
	}
}
*/

void SquareThread::update_links() {
	for (auto it = squares.begin(); it != squares.end(); ++it) {
		it->update_links();
	}
}

void SquareThread::update_squares() {
	for (auto it = squares.begin(); it != squares.end(); ++it) {
		it->update_squares();
	}
}

SquareArmy& SquareCollection::get_square_army(int x, int y) {
	return get_square_army((y * x_squares) + x);
}

SquareArmy& SquareCollection::get_square_army(int index) {
	int thread = index / squares_per_thread;
	int local_index= index % squares_per_thread;
	return square_threads[thread].get_square_army(local_index);
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



