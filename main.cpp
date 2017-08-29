#include <iostream>
#include <unistd.h>
#include "jack.hpp"

int main(int argc, char **argv) {
	std::cout << "KeySplitter!\n";
	std::vector<size_t> ranges = std::vector<size_t>();
	ranges.push_back(0);
	for (int i = 1; i < argc; i++) {
		size_t r;
		sscanf(argv[i], "%zu", &r);
		ranges.push_back(r);
	}
	// initialize JACK client with desired input ranges
	Jack j;
	if (j.initialize(ranges)) {
		std::cout << "Initialization of JACK client failed.\n";
		exit(1);
	}

	// sleep until interrupt, then die
	pause();
	std::cout << "Exiting.\n";
	return 0;
}
