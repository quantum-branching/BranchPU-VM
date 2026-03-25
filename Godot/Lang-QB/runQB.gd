extends TextureButton

var running:bool = false
var compiler:Assembly
var code:String = ""
var time:float = 1.0
var time2:float = 1.0
var postproc:Dictionary
var undefined:PackedByteArray

func _ready() -> void:
	for i in OS.get_cmdline_args():
		if i.ends_with(".qb"):
			%Edit.text = FileAccess.get_file_as_string(i)
			code = %Edit.text
	%Edit.syntax_highlighter = AssemblyHighligher.new(%Edit)
	compiler = Assembly.new(code)

func _pressed() -> void:
	#%Edit.text = CompilerQB.toAssembly(CompilerQB.toPostFix(%Edit.text))
	%Edit.text = ", ".join(CompilerQB.toPostFix(%Edit.text))
	return
	running = !running
	if running:
		code = %Edit.text
		compiler = Assembly.new(code)
	else:
		time = 1.0
		%Edit.text = code
		compiler = Assembly.new(code)

func _process(delta: float) -> void:
	if running:
		compiler.step()
		
		time += delta
		
		if time > 0.1:
			if Input.is_key_pressed(KEY_ALT):
				if %Edit.text != compiler.code:
					%Edit.text = compiler.code
			else:
				%Edit.text = compiler.to_string()
			time = 0.0
	else:
		time2 += delta
		if time2 > 1.0:
			compiler = Assembly.new(%Edit.text)
			time2 = 0.0
	for relative:String in compiler.labels.keys():
		if relative.begins_with("."):
			%Edit.add_code_completion_option(CodeEdit.KIND_CONSTANT, relative, relative.replace(".", ""), AssemblyHighligher.RELATIVE)
		else:
			%Edit.add_code_completion_option(CodeEdit.KIND_CONSTANT, relative, relative.replace(".", ""), AssemblyHighligher.PRE_ARG)
	for op in AssemblyHighligher.OPERATIONS:
		%Edit.add_code_completion_option(CodeEdit.KIND_FUNCTION, op, op, AssemblyHighligher.OP)
	%Edit.update_code_completion_options(true)
