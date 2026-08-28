#define SCREEN_PORT 1
#define OPERAND_MASK 31

#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 32

/// @brief Processes a GPU command
/// @param input The input command
/// @return The output of the GPU
int GPU_p26(const int input);

/// @brief A visual representation of the current state of the screen
/// @param result A string that is used to store the visual representation of the screen, which should be at least SCREEN_HEIGHT * (SCREEN_WIDTH * 2) characters long
/// @return A pointer to the modified string result, which contains a visual representation of the current state of the screen
char *GPU_getScreen(char result[SCREEN_HEIGHT * (SCREEN_WIDTH * 2)]);