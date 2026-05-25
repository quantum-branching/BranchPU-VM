#define TRUE 1
#define FALSE 0

#define invert(x) x ^= TRUE
#define bool(x) (!!x)

/// @brief Sets one of the bits to the boolean value
/// @param value Initial value
/// @param boolean Value of the flag
/// @param bit Position of the flag
/// @return The new value with the flag changed
int flag(int value, int boolean, int bit);