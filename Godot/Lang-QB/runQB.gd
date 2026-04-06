extends TextureButton

var running:bool = false
var asmCompiler:Assembly
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
	#TODO: %Edit.syntax_highlighter = QBHighligher.new(%Edit)
	asmCompiler = CompilerQB.compile(code)

func _pressed() -> void:
	running = !running
	if running:
		code = %Edit.text
		asmCompiler = CompilerQB.compile(%Edit.text)
	else:
		time = 1.0
		%Edit.text = code
		asmCompiler = CompilerQB.compile(%Edit.text)

func _process(delta: float) -> void:
	if running:
		asmCompiler.step()
		time += delta
		
		if time > 0.1:
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
