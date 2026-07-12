# QB Language Reference Sheet

---

## 1. Symbols

| Symbol | Meaning |
|--------|--------|
| `//`   | Starts a comment (line) |
| `#`    | Starts a preprocessor function |
| `\n`   | Ends any statement |
| `;`    | Ends any statement |
| `{`    | Starts a subroutine / function block |
| `}`    | Ends a subroutine / function block |
| `true` | Boolean `true` (1) |
| `false`| Boolean `false` (0) |
| `[`    | Starts a fixed-size array |
| `]`    | Ends a fixed-size array |
| `'`    | Starts and ends a character |
| `"`    | Starts and ends a string |
| `.`    | Member or element access |
| `-`    | Subtraction |
| `+`    | Addition |
| `*`    | Multiplication |
| `/`    | Division |
| `<<`   | Left bit shift |
| `>>`   | Right bit shift |
| `&`    | Bitwise AND |
| `^`    | Bitwise XOR |
| `\|`    | Bitwise OR |
| `!`    | Logical NOT / bitwise inversion |

---

## 2. Definitions / Keywords

| Keyword | Purpose |
|---------|--------|
| `func`  | Defines a function or subroutine |
| `reg`   | Parameter can be global |
| `int`   | Defines an 8-bit integer |
| `char`  | Integer interpreted as ASCII character |
| `bool`  | Defines a boolean |
| `arr`   | Defines an array of integers |
| `str`   | Array interpreted as a string |
| `var`   | Defines a variable from an initialized value (type fixed) |

---

## 3. Casting Rules

| Cast | Meaning |
|------|---------|
| `func` -> `[type]` | Calls the function and returns the accumulator cast to `[type]` |
| `int` -> `char` | Returns `int.to_ascii()` |
| `int` -> `bool` | Returns `1` if `int > 0`, else `0` |
| `char` -> `int` | Returns `char.from_ascii()` |
| `char` -> `bool` | Returns `1` if `char.from_ascii() > 0`, else `0` |
| `bool` -> `char` | Returns `0.to_ascii()` if false, `1.to_ascii()` if true |

---

## 4. Order of Operations

| Priority | Operations | Notes |
|----------|------------|-------|
| 0 | `(`,`)`| Can slow down references if parenthesis affect operation order |
| 1 | `*`,`/`| Depends on software libraries |
| 2 | `>>`,`<<`| |
| 3 | `\|`,`&`,`^` | Use for boolean manipulation as well (with `and true`) |
| 4 | `+`,`-` | |

Executes lower priority operations before higher priority operations. Prioritizes operations in parenthesis over other operations.

---

## 5. Notes on Behavior

- All scalar types are **8-bit integers at runtime**.  
- **Arrays and dictionaries cannot be cast as a whole**, only elements can be accessed via `.` notation.
  - *This means that* `[idx]` *does not access array elements.*
- **Strings** are arrays of ASCII values; **characters** are single ASCII integers.  
- **Boolean logic**: any non-zero integer is `true`, `0` is `false`.  
- **Operations** support arithmetic, bitwise, and logical operators.  

---

## 6. Example Usage

```C
// Define variables
int x = 10
bool flag = true
char letter = 'A'

// Arrays
arr numbers = [1,2,3]
str message = 'Hello'

// Function definition
int fib(reg int n) {
  int first = 0
	int temp = 0
	int second = 1

	while(n != 1) {
    n -= 1
		temp = second
		second += first
		first = temp
  }
	
	return second
}

// Recursive function definition
int factorial(int n) {
	if(n == 0) {
		return 1  
	}

	return n * factorial(n - 1)
}

// Bitwise operations
int a = 5
int b = 3
int c = a & b   // AND
int d = a | b   // OR
int e = a ^ b   // XOR
int f = a << 1  // Left shift
int g = b >> 1  // Right shift
int h = !a      // NOT
```
