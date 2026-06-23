/// @brief Represents a port that contains an input, output, and a function to determine the output based on the input
struct Port {
    int input;
    int output;

    int (*update)(const int);
};

/// @brief Determines the output of a port based on its input and updates the port's output field
/// @param port The port to handle (will be modified)
/// @param input The input value for the port
inline void port_handlePort(struct Port port, const int input);