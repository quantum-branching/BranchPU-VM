class_name CompilerQB2 extends RefCounted

const REGISTER_COUNT:int = 256

var freeRegisters:Array[int] = []
var usedRegisters:Array[int] = []
var variableStack:Array[String] = []

func _init() -> void:
	freeRegisters.resize(REGISTER_COUNT)
	for i in REGISTER_COUNT:
		freeRegisters[i] = i

## Allocates a register to [param variable]
func allocv(variable:String):
	var x:int = freeRegisters.pop_back()
	usedRegisters.push_back(x)
	variableStack.push_back(variable)
	return x

## Frees the last [param n] variables
func freev(n:int) -> void:
	for i in n:
		freeRegisters.push_back(usedRegisters.pop_back())
		variableStack.pop_back()

func tokenize(code:String):
	var tokens:Array[Token] = []
	while(code.length):
		if code.begins_with("\""):
			pass
		code = code.substr(1, -1)

class Token:
	var type:int;
	var content:String;
	
	
