#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <set>
#include <queue>

using namespace std;

void flush_mem(const multiset<pair<string, string> >& Set, const string& file_name);
long scatter_input(const string& input_file_name);
void print_result(const string& key, const multiset<string>& values);
void gather_output(long file_count);

// write memory content into a temporary file.
void flush_mem(const multiset<pair<string, string> >& Set, const string& file_name) {
	ofstream output;
	output.open(file_name, ios::out);
	if(!output.is_open())
	{
		cerr << "Unable to open file " << file_name << "." << endl;
		exit(0);
	}

	for (auto it = Set.begin(); it != Set.end(); it++) {
		output << it->first << " " << it->second << endl;
	}

	output.close();
}

// parse and sort input in memory;
// if input size exceeds memory limit, write current memory contents into temporary files to be merged in gather_output function.
long scatter_input(const string& input_file_name) {
	ifstream input;
	input.open(input_file_name, ios::in);
	if (!input.is_open()) {
		cerr << "Unable to open file " << input_file_name << "." << endl;
		exit(1);
	}

	string line = "";
	long mem_limit = 0;
	if (!input.eof()) {
		getline(input, line);
		mem_limit = stol(line);  // get memory limit from the first line of the input file.
	}

	// use binary search tree based multiset to sort key-value pairs in memory;
	// for n pairs it will take O(nlogn) time to sort.
	multiset<pair<string, string> > Set;
	// file_count is used to keep track of how many temporary files we have opened;
	// cur_mem keeps track of current memory usage.
	long file_count = 0, cur_mem = 0;

	while (!input.eof()) {
		getline(input, line);  // read line by line from the input file.
		int pos = line.find_first_of(" ");  // we assume key and value are separated by one white space.
		string key = line.substr(0, pos);
		if (key == "")  // key shouldn't be empty, in case of corner cases.
			continue;
		string val = line.substr(pos + 1);

		long temp = key.size() + 1 + val.size() + 1;  // memory uasge for this key-value pair.

		// if meet memory limit, we should flush current memory content into a file and clear current memory.
		if (cur_mem + temp > mem_limit) {
			flush_mem(Set, to_string(file_count));  // current file_count is used as current file name.
			file_count++;
			Set.clear();
			cur_mem = 0;
		}
		cur_mem += temp;  // update current memory usage.

		Set.insert({key, val});
	}

	// we write whatever left in the memory into the last file.
	flush_mem(Set, to_string(file_count));
	file_count++;

	input.close();

	return file_count;
}

// print one key and all its values in sorted order to stdout.
void print_result(const string& key, const multiset<string>& values) {
	cout << key;
	for (auto it = values.begin(); it != values.end(); it++) {
		cout << " " << *it;
	}
	cout << endl;
}

void gather_output(long file_count) {
	// an array of file pointers to temporary files created by scatter_input function.
	vector<ifstream> in(file_count);
	for (int i = 0; i < file_count; i++) {
		in[i].open(to_string(i), ios::in);
		if(!in[i].is_open())
		{
			cerr << "Unable to open file " << to_string(i) << "." << endl;
			exit(2 + i);
		}
	}

	// a min_heap to help merge and sort keys from all files (one key per file);
	// each element in the min_heap is a pair containing a key and a file index.
	priority_queue<pair<string, int>, vector<pair<string, int> >, greater<pair<string, int> > > min_heap;
	// inititalize min_heap by pushing the first key-file_index pair from each file into min_heap.
	for (int i = 0; i < file_count; i++) {
		if (!in[i].eof()) {
			string key = "";
			in[i] >> key;
			if (key == "")  // key shouldn't be empty, in case of corner cases.
				continue;
			min_heap.push({key, i});
		}
	}

	// process one key and all its values, output them to the screen;
	// min_heap's size is always smaller or equal to number of files (file_count);
	// we further use a binary search tree based multiset to store and sort values associated with the current key;
	// we only load current key's values into memory (based on question statement it will fit in memory);
	// for n key-value pairs it will take O(nlogn) time to process.
	multiset<string> values;
	string cur_key = min_heap.top().first;
	while (!min_heap.empty()) {
		auto cur = min_heap.top();
		min_heap.pop();
		// print current key and values;
		// clear values and set cur_key to the next key.
		if (cur.first != cur_key) {
			print_result(cur_key, values);
			values.clear();
			cur_key = cur.first;
		}
		string val = "";
		in[cur.second] >> val;  // read value.
		values.insert(val);  // store value in values to sort.

		// update current file pointer and push the pair into min_heap.
		if (!in[cur.second].eof()) {
			string key = "";
			in[cur.second] >> key;
			if (key == "")  // key shouldn't be empty, in case of corner cases.
				continue;
			min_heap.push({key, cur.second});
		}
	}
	print_result(cur_key, values);  // print the last key and its values.

	for (int i = 0; i < file_count; i++) {
		in[i].close();
		const char* file_name = to_string(i).c_str();
		if (remove(file_name) != 0)  // remove temporary files.
			cerr << "Error deleting file " << to_string(i) << "." << endl;
	}
}
