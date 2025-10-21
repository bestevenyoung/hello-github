extends Control

# We'll find these nodes safely in _ready()
var api_key_input: LineEdit
var save_button: Button
var chat_button: Button
var status_label: Label

func _ready():
	# Find nodes by name instead of using fixed paths
	api_key_input = find_child("APIKeyInput")
	save_button = find_child("SaveAPIKeyButton")
	chat_button = find_child("ChatButton")
	status_label = find_child("StatusLabel")
	
	# Check if all required nodes were found
	if not api_key_input:
		push_error("APIKeyInput node not found! Please check your scene.")
		return
	if not save_button:
		push_error("SaveAPIKeyButton node not found! Please check your scene.")
		return
	if not chat_button:
		push_error("ChatButton node not found! Please check your scene.")
		return
	
	# Check if we have a saved API key
	var saved_api_key = load_api_key()
	
	if saved_api_key and not saved_api_key.is_empty():
		# We have an API key, skip directly to chat
		api_key_input.text = saved_api_key
		chat_button.disabled = false
		show_message("API key found. Redirecting to chat...")
		
		# Wait a moment so user can see the message, then redirect
		await get_tree().create_timer(1.5).timeout
		get_tree().change_scene_to_file("res://scenes/ChatInterface.tscn")
	else:
		# No API key, show normal interface
		chat_button.disabled = true
		show_message("Please enter your DeepSeek API key")
	
	# Connect signals
	save_button.connect("pressed", _on_save_button_pressed)
	chat_button.connect("pressed", _on_chat_button_pressed)

func _on_save_button_pressed():
	var api_key = api_key_input.text.strip_edges()
	if api_key.is_empty():
		show_message("Please enter an API key")
		return
	
	save_api_key(api_key)
	show_message("API Key saved successfully! Redirecting to chat...")
	chat_button.disabled = false
	
	# Redirect to chat after saving
	await get_tree().create_timer(1.5).timeout
	get_tree().change_scene_to_file("res://scenes/ChatInterface.tscn")

func _on_chat_button_pressed():
	var api_key = load_api_key()
	if api_key.is_empty():
		show_message("Please save an API key first")
		return
	
	get_tree().change_scene_to_file("res://scenes/ChatInterface.tscn")

func save_api_key(api_key: String):
	var config = ConfigFile.new()
	config.set_value("api", "key", api_key)
	config.save("user://config.cfg")

func load_api_key() -> String:
	var config = ConfigFile.new()
	var error = config.load("user://config.cfg")
	if error == OK:
		return config.get_value("api", "key", "")
	return ""

func show_message(message: String):
	print(message)
	if status_label:
		status_label.text = message
