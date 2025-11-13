extends Node

signal mode_changed(model_name)

var current_mode: int = 0
var model_names = ["deepseek-chat", "deepseek-reasoner"]

func toggle_mode():
	current_mode = 1 - current_mode  # Toggle between 0 and 1
	mode_changed.emit(get_current_model_name())
	save_mode()
	return get_current_model_name()

func get_current_model_name() -> String:
	return model_names[current_mode]

func get_current_mode_display_name() -> String:
	return "Chat" if current_mode == 0 else "DeepThink"

func save_mode():
	var config = ConfigFile.new()
	config.set_value("settings", "selected_mode", current_mode)
	config.save("user://config.cfg")

func load_mode() -> int:
	var config = ConfigFile.new()
	var error = config.load("user://config.cfg")
	if error == OK:
		return config.get_value("settings", "selected_mode", 0)
	return 0

func initialize():
	current_mode = load_mode()
