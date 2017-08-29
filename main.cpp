/**
	jackKeySplitter: a utility for the JACK audio framework which demultiplexes
	a MIDI instrument into two or more independent output ports.

	author: George Albertson
	creation date: 8/28/2017
	license: MIT
*/

#include <iostream>
#include <unistd.h>
#include "client.hpp"

int main(int argc, char **argv) {
	std::cout << "Starting jackKeySplitter...\n";
	std::vector<size_t> ranges = std::vector<size_t>();
	ranges.push_back(0);
	for (int i = 1; i < argc; i++) {
		size_t r;
		sscanf(argv[i], "%zu", &r);
		ranges.push_back(r);
	}
	// initialize JACK client with desired input ranges
	Client j;
	if (j.initialize(ranges)) {
		std::cout << "Initialization of JACK client failed.\n";
		exit(1);
	}

	// sleep until interrupt, then die
	pause();
	std::cout << "Exiting.\n";
	return 0;
}
