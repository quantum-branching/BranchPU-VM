class_name Assembly extends RefCounted

var cpu:CPU

var code:String

var removeBlank:bool = false

var labels:Dictionary = {}

var functions:Dictionary = {}

var implications:Dictionary = {}

func _init(newCode:String = "") -> void:
	cpu = CPU.new()
	
	if newCode.containsn("#removeblank"):
		removeBlank = true
		newCode = newCode.replacen("#removeBlank", "")
	newCode = define(implies(import(newCode).replace("\t", "")))
	
	
	var counter:int = 1
	
	for line in newCode.split("\n",!removeBlank):
		if line.contains(";"):
			line = line.get_slice(";", 0)
		if line.begins_with("."):
			labels[line.get_slice(" ", 0)] = String.num_int64(counter)
			if line.contains(" "):
				line = line.split(" ", false, 1)[1]
			else:
				line = ""
		if line || !removeBlank:
			counter += 1
			code += line + "\n"
	
	for i in labels.keys():
		code = code.replace(i, labels[i])
	
	code = code.replace("\\","\n")

func import(newCode:String) -> String:
	var result:String = ""
	for line in newCode.split("\n"):
		if line.begins_with("#import "):
			result += FileAccess.get_file_as_string("res://Lang-ASM/Packages/" + line.get_slice(" ", 1).to_lower() + ".bpu")
		else:
			result += line + "\n"
	return result

func define(newCode:String) -> String:
	var result:String = ""
	for line in newCode.split("\n"):
		if line.begins_with("#define "):
			labels[line.get_slice(" ", 1)] = line.get_slice(" ", 2)
		else:
			result += line + "\n"
	return result

func implies(newCode:String) -> String:
	var result:String = ""
	for line in newCode.split("\n"):
		if line.begins_with("#implies ") && line.contains(" ") && line.split(" ",false, 1)[1].contains(" "):
			implications[line.get_slice(" ", 1) + " " + line.get_slice(" ", 2)] = line.split(" ",false, 3)[3]
		else:
			result += line + "\n"
	return result

func step() -> void:
	if cpu.programCounter > cpu.MAX_PC:
		cpu.programCounter = 0
	parseLine(code.get_slice("\n", cpu.programCounter - 1))
	cpu.programCounter += 1

func parseLine(line:String):
	cpu.accumulator %= 256
	line = line.to_lower()
	#region Classic Operations
	if line.contains("#"):
		if line.begins_with("add "):
			cpu.addi(parseByte(line))
		elif line.begins_with("sub "):
			cpu.subi(parseByte(line))
		elif line.begins_with("lsh "):
			cpu.lshi(parseByte(line))
		elif line.begins_with("rsh "):
			cpu.rshi(parseByte(line))
		elif line.begins_with("and "):
			cpu.andi(parseByte(line))
		elif line.begins_with("or "):
			cpu.ori(parseByte(line))
		elif line.begins_with("xor "):
			cpu.xori(parseByte(line))
		elif line.begins_with("sta "):
			cpu.sta(parseByte(line))
		elif line.begins_with("ldi "):
			cpu.ldi(parseByte(line))
		elif line.begins_with("cnd "):
			cpu.cnd(parseMod(line), parseByte(line))
		elif line.begins_with("psh "):
			cpu.psh(parseMod(line), parseByte(line))
		elif line.begins_with("pop"):
			cpu.pop()
		elif line.begins_with("cst "):
			cpu.cst(parseByte(line))
		elif line.begins_with("cmp "):
			cpu.icp(parseMod(line), parseByte(line))
		elif line.begins_with("icp "):
			cpu.icp(parseMod(line), parseByte(line))
	else:
		if line.begins_with("jmp "):
			cpu.jmp(parseMod(line), parseByte(line))
		elif line.begins_with("add "):
			cpu.add(parseByte(line))
		elif line.begins_with("sub "):
			cpu.sub(parseByte(line))
		elif line.begins_with("lsh "):
			cpu.lsh(parseByte(line))
		elif line.begins_with("rsh "):
			cpu.rsh(parseByte(line))
		elif line.begins_with("and "):
			cpu.And(parseByte(line))
		elif line.begins_with("or "):
			cpu.Or(parseByte(line))
		elif line.begins_with("xor "):
			cpu.Xor(parseByte(line))
		elif line.begins_with("lda "):
			cpu.lda(parseByte(line))
		elif line.begins_with("sta "):
			cpu.sta(parseByte(line))
		elif line.begins_with("ldi "):
			cpu.ldi(parseByte(line))
		elif line.begins_with("cnd "):
			cpu.cnd(parseMod(line), parseByte(line))
		elif line.begins_with("psh "):
			cpu.psh(parseMod(line), parseByte(line))
		elif line.begins_with("pop"):
			cpu.pop()
		elif line.begins_with("cmp "):
			cpu.cmp(parseMod(line), parseByte(line))
		elif line.begins_with("rpa "):
			cpu.rpa(parseMod(line))
		elif line.begins_with("rpr "):
			cpu.rpr(parseMod(line), parseByte(line))
		elif line.begins_with("wpa "):
			cpu.wpa(parseMod(line))
		elif line.begins_with("wpr "):
			cpu.wpr(parseMod(line), parseByte(line))
	#endregion
	#region Compiler Defined Operations
		elif line.begins_with("cst "):
			cpu.cst(parseByte(line))
		elif line.begins_with("nop"):
			cpu.programCounter -= 1
	#endregion

func parseMod(modifier:String) -> int:
	if modifier.contains(" "):
		return modifier.get_slice(" ", 1).replace("#", "").to_int()
	return 0

func parseByte(immediate:String) -> int:
	if immediate.contains(" "):
		return immediate.rsplit(" ", false, 1)[1].replace("#", "").replace("$", "").to_int()
	return 0

func _to_string() -> String:
	var result:String = "Line: " + String.num_int64(cpu.programCounter)
	result += "\nAccumulator: \n\t"
	result += String.num_int64(cpu.accumulator)
	result += "\nMemory: \n"
	for i in cpu.memory:
		result += "\t" + String.num_int64(i) + "\n"
	return result
