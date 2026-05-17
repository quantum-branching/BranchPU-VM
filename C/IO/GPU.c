#define PORT 1
#define OPERAND_MASK 31

/// Represents the draw line on the GPU
/// This is the y coordinate of all preceding draw calls
int GPU_drawLineY = 0;

int screen[32][32] = {0};

int GPU_p26(int input) {
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

int getPixel(int x, int y) {
    return screen[y][x];
}