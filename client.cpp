#include "client.hpp"

using namespace std;

Client::Client() {
	if ((client = jack_client_open("KeySplitter", JackNullOption, NULL)) == 0) {
		std::cout << "Could not connect to JACK server.\n";
	}
}

int Client::initialize(std::vector<size_t> ranges) {
	std::cout << "Initializing JACK client; " << ranges.size() << " ranges\n" << std::flush;
	
	for (size_t k = 0; k < ranges.size(); k++) {
		std::cout << "ranges[" << k << "]: " << ranges[k] << '\n';
	}

	if (ranges.size() < 2 || ranges[0] != 0) {
		std::cout << "Malformed vector of input ranges.\n";
		return 1;
	}

	// ensure output port vector is large enough
	outPorts.resize(ranges.size());
	outPortBufs.resize(ranges.size());
	size_t i, j = 0;

	for (i = 1; i < ranges.size() + 1; i++) {
	// ranges[0] is expected to be 0 (lowest note bound)
		std::ostringstream outPortName;
		outPortName << "midi_out_" << (i - 1);
		outPorts[i - 1] = jack_port_register(client, outPortName.str().c_str(),
		                  JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
		// final output range is [ranges[ranges.size() - 1], 127] (inclusive)
		size_t upperBound = (i < ranges.size()) ? ranges[i] : 127;
		if (upperBound <= ranges[i - 1]) {
			std::cout << "Split boundaries must be passed in ascending order.\n";
			return 1;
		}
		if (upperBound > 127) {
			std::cout << "Upper boundaries may not exceed 127.\n";
			return 1;
		}
		while (j <= upperBound) {
			lookupTable[j] = i - 1;
			j++;
		}
		std::cout << "Configured MIDI output for input range [" << ranges[i - 1] + 1
		          << ", " << upperBound << "].\n" << std::flush;
	}

	inPort = jack_port_register(client, "midi_in", JACK_DEFAULT_MIDI_TYPE,
	         JackPortIsInput, 0);
	jack_set_process_callback(client, staticProcess, static_cast<void *>(this));

	if (jack_activate(client)) {
		std::cout << "Could not activate JACK client.\n";
		return 1;
	}
	std::cout << "Activated JACK client, registered JACK ports and callback.\n"
	          << std::flush;
	return 0;
}

int Client::staticProcess(jack_nframes_t nframes, void *arg) {
	return static_cast<Client *>(arg)->process(nframes);
}

int Client::process(jack_nframes_t nframes) {
#ifdef DEBUG
	std::cout << "process() " << nframes << "\n" << std::flush;
#endif

	if (nframes == 0) {
		return 0;
	}

	jack_midi_event_t event;
	size_t i;

	void *inPortBuf = jack_port_get_buffer(inPort, nframes);

	// clear all output buffers
#ifdef DEBUG
	std::cout << "Clearing output buffers..." << std::flush;
#endif
	for (i = 0; i < outPorts.size(); i++) {
		outPortBufs[i] = jack_port_get_buffer(outPorts[i], nframes);
		jack_midi_clear_buffer(outPortBufs[i]);
	}

#ifdef DEBUG
	std::cout << "Passing MIDI events..." << std::flush;
#endif
	jack_nframes_t event_count = jack_midi_get_event_count(inPortBuf);
	for (i = 0; i < event_count; i++) {
		jack_midi_event_get(&event, inPortBuf, i);
		// depending on the note value, write to an output range port
		void *eventBuf = jack_midi_event_reserve(outPortBufs[lookupTable[event.buffer[1]]],
		                        event.time, EVENT_SIZE);
		if (eventBuf == NULL) {
			std::cout << "Failed to write event to outPort "
			          << lookupTable[event.buffer[1]] << '\n' << std::flush;
		} else {
			memcpy(eventBuf, event.buffer, EVENT_SIZE);
		}
	}
	return 0;
}

Client::~Client() {
	std::cout << "Closing JACK client.";
	jack_deactivate(client);
	jack_client_close(client);
	std::cout << "Closed JACK client.";
}
