class_name CompilerGB extends Node

const OPERAND_PREFIX:PackedStringArray = ["$","#"]

const NUMBERS:PackedStringArray = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]
const OP_1:PackedStringArray = ["*","/"]
const OP_2:PackedStringArray = [">>","<<"]
const OP_3:PackedStringArray = ["|","&","^"]
const OP_4:PackedStringArray = ["+","-"]

static func toPostFix(line:String) -> PackedStringArray:
	var result:PackedStringArray = []
	var stack:PackedStringArray = []
	var args:PackedStringArray = line.split(" ",false)
	while args:
		if NUMBERS.has(args[0].substr(0,1)) || OPERAND_PREFIX.has(args[0].substr(0,1)):
			result.append(args[0])
		if OP_1.has(args[0]):
			stack.append(args[0])
		if OP_2.has(args[0]):
			while OP_1.has(stack[stack.size() - 1]):
				result.append(stack[stack.size() - 1])
				stack.remove_at(stack.size() - 1)
			stack.append(args[0])
		if OP_3.has(args[0]):
			while OP_1.has(stack[stack.size() - 1]) || OP_2.has(stack[stack.size() - 1]):
				result.append(stack[stack.size() - 1])
				stack.remove_at(stack.size() - 1)
		if OP_4.has(args[0]):
			while OP_1.has(stack[stack.size() - 1]) || OP_2.has(stack[stack.size() - 1]) || OP_3.has(stack[stack.size() - 1]):
				result.append(stack[stack.size() - 1])
				stack.remove_at(stack.size() - 1)
			stack.append(args[0])
		if args[0] == "=":
			while OP_1.has(stack[stack.size() - 1]) || OP_2.has(stack[stack.size() - 1]) || OP_3.has(stack[stack.size() - 1]) || OP_4.has(stack[stack.size() - 1]):
				result.append(stack[stack.size() - 1])
				stack.remove_at(stack.size() - 1)
		args.remove_at(0)
	return result
