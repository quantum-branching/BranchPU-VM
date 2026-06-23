#include "Port.h"

inline void port_handlePort(struct Port port, const int input) {
	port.input = input;
    if (port.update) {
		port.output = port.update(port.input);
	}
}