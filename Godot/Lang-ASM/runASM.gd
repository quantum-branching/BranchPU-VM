extends TextureButton

var running:bool = false
var asmCompiler:Assembly
var code:String = ""
var time:float = 1.0
var time2:float = 1.0

func _ready() -> void:
	for i in OS.get_cmdline_args():
		if i.ends_with(".bpu"):
			%Edit.text = FileAccess.get_file_as_string(i)
			code = %Edit.text
	%Edit.syntax_highlighter = AssemblyHighligher.new(%Edit)
	asmCompiler = Assembly.new(code)

func _pressed() -> void:
	running = !running
	if running:
		code = %Edit.text
		asmCompiler = Assembly.new(code)
	else:
		time = 1.0
		%Edit.text = code
		asmCompiler = Assembly.new(code)

func handlePortsP26():
	asmCompiler.cpu.portOutputs[1] = $"../Port1".output
	$"../Port1".input = asmCompiler.cpu.portInputs[1]

func _process(delta: float) -> void:
	if running:
		asmCompiler.step()
		handlePortsP26()
		time += delta
		
		if time > 0.2:
			if Input.is_key_pressed(KEY_ALT):
				if %Edit.text != asmCompiler.code:
					%Edit.text = asmCompiler.code
			else:
				%Edit.text = asmCompiler.to_string()
			time = 0.0
	else:
		time2 += delta
		if time2 > 1.0:
			asmCompiler = Assembly.new(%Edit.text)
			time2 = 0.0
		for relative:String in asmCompiler.labels.keys():
			if relative.begins_with("."):
				%Edit.add_code_completion_option(CodeEdit.KIND_CONSTANT, relative, relative.replace(".", ""), AssemblyHighligher.RELATIVE)
			else:
				%Edit.add_code_completion_option(CodeEdit.KIND_CONSTANT, relative, relative.replace(".", ""), AssemblyHighligher.PRE_ARG)
		for op in AssemblyHighligher.OPERATIONS:
			%Edit.add_code_completion_option(CodeEdit.KIND_FUNCTION, op, op, AssemblyHighligher.OP)
		%Edit.add_code_completion_option(CodeEdit.KIND_CONSTANT, "#removeBlank", "removeBlank", AssemblyHighligher.PREPROCESSOR)
		var dir:DirAccess = DirAccess.open("res://Lang-ASM/Packages/")
		for i in dir.get_files():
			%Edit.add_code_completion_option(CodeEdit.KIND_CONSTANT, "#import " + i.replace(".bpu", ""), "import " + i.replace(".bpu", ""), AssemblyHighligher.PREPROCESSOR)
		%Edit.update_code_completion_options(true)
