#include "key_value_sort.h"

int main(int argc, char* argv[]) {
	char* input_file;

	if (argc < 2) {
		cerr << "Usage:" << endl << argv[0] << " $input_file" << endl;
		exit(-1);
	}
	else
		input_file = argv[1];

	long count = scatter_input(input_file);
	gather_output(count);

	return 0;
}
