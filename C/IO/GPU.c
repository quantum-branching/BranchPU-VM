#define PORT 1
#define OPERAND_MASK 31

#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 32

/// Represents the draw line on the GPU
/// This is the y coordinate of all preceding draw calls
int GPU_drawLineY = 0;

int screen[SCREEN_HEIGHT][SCREEN_WIDTH] = {0};

int GPU_p26(const int input) {
    int opcode = input >> 5;

    switch (opcode) {
        case 2:
            // Draws a pixel on the screen at input & 31 (x coordinate) and GPU_drawLineY (y coordinate)
            screen[input & OPERAND_MASK][GPU_drawLineY] = 1;
            break;
        case 3:
            // Erases a pixel on the screen at input & 31 (x coordinate) and GPU_drawLineY (y coordinate)
            screen[input & OPERAND_MASK][GPU_drawLineY] = 0;
            break;
        case 4:
            // Changes the GPU_drawLineY for the next draw call
            GPU_drawLineY = input & OPERAND_MASK;
            break;
        default:
            break;
    }

    return PORT;
}

int getPixel(const int x, const int y) {
    return screen[y][x];
}

char *getScreen(char result[SCREEN_HEIGHT * (SCREEN_WIDTH * 2)]) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            result[y * (SCREEN_WIDTH * 2) + 2 * x] = screen[y][x] ? '#' : ' ';
            result[y * (SCREEN_WIDTH * 2) + 2 * x + 1] = ' ';
        }
        result[y * (SCREEN_WIDTH * 2) + SCREEN_WIDTH * 2 - 1] = '\n';
    }
    return result;
}