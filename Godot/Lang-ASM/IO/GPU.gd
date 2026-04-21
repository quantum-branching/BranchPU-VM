extends TileMapLayer
var input:int = 0
var output:int = 0

var dly:int = 0

const PORT:int = 1

func _process(_delta: float) -> void:
	pass #output = p26(input)

func p26(op:int) -> int:
	if !(op >> 5):
		return PORT
	if (op >> 5) == 1:
		#NOTE: Need to implement shader feature
		return PORT
	if (op >> 5) == 2:
		set_cell(Vector2i(op & 31, dly), 8, Vector2i(0, 0))
		return PORT
	if (op >> 5) == 3:
		set_cell(Vector2i(op & 31, dly), 8, Vector2i(1, 0))
		return PORT
	if (op >> 5) == 4:
		dly = op & 31
		return PORT
	return PORT

func reset():
	input = 0
	for x in 32:
		for y in 32:
			set_cell(Vector2i(x, y), 8, Vector2i(1, 0))
