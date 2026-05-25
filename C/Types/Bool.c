#include "Bool.h"

int flag(int value, int boolean, int bit) {
    value = value & ~(1 << bit) | (boolean << bit);
}