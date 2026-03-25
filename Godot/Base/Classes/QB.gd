class_name CompilerQB extends Node

##All characters that occur before an operand that specify what type of operand it is.
const OPERAND_PREFIX:PackedStringArray = ["$","#"]

const VAR_TYPES:PackedStringArray = ["int", "char", "bool", "arr", "str", "var"]

##All characters that should be interpreted as numbers.
const NUMBERS:PackedStringArray = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]
const OP_1:PackedStringArray = ["*","/"]
const OP_2:PackedStringArray = [">>","<<"]
const OP_3:PackedStringArray = ["|","&","^"]
const OP_4:PackedStringArray = ["+","-"]

const QB_OPERATORS:PackedStringArray = ["+","-",">>","<<","|","&","^"]
const ASM_OPERATORS:PackedStringArray = ["ADD","SUB","RSH","LSH","OR","AND","XOR"]

##Converts an infix expression (as a [String]) to a postfix expression (as a [PackedStringArray]). [br]
##An infix expression contains an operator surround by operands such as [code] 2 * 5 + 3 [/code]. [br]
##A postfix expression has the operator after the operands such as [code] 2 5 * 3 + [/code].
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
			while stack.size() && OP_1.has(stack[stack.size() - 1]):
				result.append(stack[stack.size() - 1])
				stack.remove_at(stack.size() - 1)
			stack.append(args[0])
		if OP_3.has(args[0]):
			while stack.size() && (OP_1.has(stack[stack.size() - 1]) || OP_2.has(stack[stack.size() - 1])):
				result.append(stack[stack.size() - 1])
				stack.remove_at(stack.size() - 1)
			stack.append(args[0])
		if OP_4.has(args[0]):
			while stack.size() && (OP_1.has(stack[stack.size() - 1]) || OP_2.has(stack[stack.size() - 1]) || OP_3.has(stack[stack.size() - 1])):
				result.append(stack[stack.size() - 1])
				stack.remove_at(stack.size() - 1)
			stack.append(args[0])
		if args[0] == "=":
			while stack.size() && (OP_1.has(stack[stack.size() - 1]) || OP_2.has(stack[stack.size() - 1]) || OP_3.has(stack[stack.size() - 1]) || OP_4.has(stack[stack.size() - 1])):
				result.append(stack[stack.size() - 1])
				stack.remove_at(stack.size() - 1)
			stack.append("=")
		if args[0] == "(":
			stack.append("(")
		if args[0] == ")":
			while stack.size() && stack[stack.size() - 1] != "(":
				result.append(stack[stack.size() - 1])
				stack.remove_at(stack.size() - 1)
			if stack.size():
				stack.remove_at(stack.size() - 1)
		args.remove_at(0)
	while stack.size():
		result.append(stack[stack.size() - 1])
		stack.remove_at(stack.size() - 1)
	return result

static func toAssembly(postfix:PackedStringArray) -> String:
	var result:String = ""
	var stack:PackedStringArray = []
	for i in postfix:
		if OPERAND_PREFIX.has(i.substr(0,1)) || NUMBERS.has(i.substr(0,1)):
			stack.append(i)
		elif QB_OPERATORS.has(i):
			result += ASM_OPERATORS[QB_OPERATORS.find(i)] + stack[stack.size() - 1] + ";\n"
			stack.remove_at(stack.size() - 1)
	return result

func preproc(code:String):
	var postproc:Dictionary
	var undefined:PackedByteArray
	var result:String
	#Clear all definitions
	code = code.replace(";", "\n")
	code = code.replace("*", " * ")
	code = code.replace("/", " / ")
	code = code.replace("+", " + ")
	code = code.replace("-", " - ")
	code = code.replace("(", " ( ")
	code = code.replace(")", " ) ")
	code = code.replace("[", " [ ")
	code = code.replace("]", " ] ")
	code = code.replace("<", " < ")
	code = code.replace(">", " > ")
	code = code.replace("!", " ! ")
	code = code.replace("^", " ^ ")
	code = code.replace("&", " & ")
	code = code.replace("|", " | ")
	code = code.replace("& &", "&&")
	code = code.replace("| |", "||")
	code = code.replace("< <", "<<")
	code = code.replace("> >", ">>")
	postproc = {}
	undefined.resize(256)
	result = ""
	for i in 256:
		undefined[i] = i
	#Define Variables
	for i in code.split("\n",false):
		if i.contains(" ") && VAR_TYPES.has(i.split(" ")[0]) && undefined.size():
			postproc[i.get_slice(" ", 1)] = undefined[0]
			undefined.remove_at(0)
	for line in code.split("\n", false):
		for arg in line.split(" ", false):
			if postproc.keys().has(arg):
				result += postproc[arg]
			else:
				result += arg
			result += " "
		result += "\n"
	result = result.replace(" \n","\n")
