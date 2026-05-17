struct Port {
    int input;
    int output;

    int (*func)(int);
};

/// @brief Determines the output of a port based on its input
/// @param port The port to handle
/// @param input The input value for the port
void handlePort(struct Port port, int input) {
	port.input = input;
    if (port.func) {
		port.output = port.func(port.input);
	}
}