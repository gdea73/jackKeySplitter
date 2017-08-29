#ifndef JACK_H
#define JACK_H

#define EVENT_SIZE 3

#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>

#include <jack/jack.h>
#include <jack/midiport.h>

class Client {
	public:
		Client();
		~Client();
		
		int initialize(std::vector<size_t> ranges);

		static int staticProcess(jack_nframes_t nframes, void *arg);

	private:
		// std::vector<MidiEvent> events;
		int lookupTable[128];

		int process(jack_nframes_t nframes);

		jack_client_t* client;
		jack_port_t* inPort;
		std::vector<jack_port_t *> outPorts;
		std::vector<void *> outPortBufs;
};

#endif
