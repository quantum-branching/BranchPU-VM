class_name CPU extends RefCounted

##Maximum programCounter value
const MAX_PC:int = 2047

##Maximum value for any byte
const MAX_BYTE:int = 255

##The size of the memory
const MEMORY_SIZE:int = 256

##The current line that the program is on
var programCounter:int = 0

var stack:PackedInt32Array = []

##An intermediate container for values being calculated
##Almost all calculations change this value
var accumulator:int = 0:
	set(x):
		accumulator = x & 0xFF

var memory:PackedByteArray = []

var condFlag:bool = false

const CMP_NOT:int = 4
const CMP_EQ:int = 2
const CMP_GT:int = 1

var portInputs:PackedByteArray = []
var portOutputs:PackedByteArray = [] 

const PORTS:int = 8

func _init() -> void:
	memory.resize(MEMORY_SIZE)
	portInputs.resize(PORTS)
	portOutputs.resize(PORTS)
	memory.fill(0)
	portInputs.fill(0)
	portOutputs.fill(0)

##Jumps to line3 * 256 + line11
func jmp(line3:int, line11:int) -> void:
	programCounter = ((line3 << 8) + line11 - 1) % (MAX_PC + 1)

#region Register Primary

##Increments the accumulator by the specified register
func add(register:int) -> void:
	accumulator += memory[register]

##Decrements the accumulator by the specified register
func sub(register:int) -> void:
	accumulator -= memory[register]

##Shift all bits in the accumulator to the left by the amount in the specified register
func lsh(register:int) -> void:
	accumulator <<= memory[register]

##Shift all bits in the accumulator to the right by the amount in the specified register
func rsh(register:int) -> void:
	accumulator >>= memory[register]

##Applies a bitwise AND between the accumulator and the specified register 
func And(register:int) -> void:
	accumulator &= memory[register]

##Applies a bitwise OR between the accumulator and the specified register
func Or(register:int) -> void:
	accumulator |= memory[register]

##Aplies a XOR AND between the accumulator and the specified register
func Xor(register:int) -> void:
	accumulator ^= memory[register]

##Sets the Accumulator to the specified register
func lda(register:int) -> void:
	accumulator = memory[register]

##Sets the specified register to the value in the accumulator
func sta(register:int) -> void:
	memory[register] = accumulator

func cmp(flags:int, register:int) -> void:
	condFlag = ((flags & CMP_EQ) && (memory[register] == accumulator)) || ((flags & CMP_GT) && (memory[register] > accumulator))
	if flags & CMP_NOT:
		condFlag = !condFlag

#endregion

#region Immediates
##Increments the accumulator by the specified register
func addi(immediate:int) -> void:
	accumulator += immediate

##Decrements the accumulator by the specified register
func subi(immediate:int) -> void:
	accumulator -= immediate

##Shift all bits in the accumulator to the left by the amount in the specified register
func lshi(immediate:int) -> void:
	accumulator <<= immediate

##Shift all bits in the accumulator to the right by the amount in the specified register
func rshi(immediate:int) -> void:
	accumulator >>= immediate

##Applies a bitwise AND between the accumulator and the specified register 
func andi(immediate:int) -> void:
	accumulator &= immediate

##Applies a bitwise OR between the accumulator and the specified register
func ori(immediate:int) -> void:
	accumulator |= immediate

##Aplies a XOR AND between the accumulator and the specified register
func xori(immediate:int) -> void:
	accumulator ^= immediate

##Sets the accumulator an immediate value
func ldi(immediate:int) -> void:
	accumulator = immediate

func icp(flags:int, immediate:int) -> void:
	condFlag = ((flags & CMP_EQ) && (immediate == accumulator)) || ((flags & CMP_GT) && (immediate > accumulator))
	if flags & CMP_NOT:
		condFlag = !condFlag

#endregion

#region Immediate Only

##Jumps to a specific line if the compare flag is set to true [br]
##See [method cmp]
func cnd(line3:int, line11:int) -> void:
	if condFlag:
		programCounter = ((line3 << 8) + line11 - 1) % (MAX_PC + 1)

##Pushes the current line to the stack and jumps to a line
##Jumps to line3 * 256 + line11
##See [method jmp]
func psh(line3:int, line11:int) -> void:
	stack.append(programCounter)
	programCounter = ((line3 << 8) + line11 - 1) % (MAX_PC + 1)

##Sets the current line to the line it was on before the last [method psh] call
##Pops the last line off the call stack
func pop() -> void:
	programCounter = stack[stack.size() - 1]
	stack.remove_at(stack.size() - 1)

#endregion

#region Ports
func rpa(port:int) -> void:
	accumulator = portOutputs[port]

func rpr(port:int, register:int) -> void:
	memory[register] = portOutputs[port]

func wpa(port:int) -> void:
	portInputs[port] = accumulator

func wpr(port:int, register:int) -> void:
	portInputs[port] = memory[register]
#endregion

#region Shortcuts/Legacy
func cst(immediate:int) -> void:
	memory[0] = immediate
#endregion
