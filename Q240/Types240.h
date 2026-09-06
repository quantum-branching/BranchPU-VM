#include <ctype.h>

#define TRUE 1
#define FALSE 0

#define push(stack, ptr, src) (stack)[(ptr)++] = src
#define pop(stack, ptr, dest) dest = (stack)[--(ptr)]

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
typedef ssize_t isize;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef size_t usize;