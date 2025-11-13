extends Control

@onready var chat_display: TextEdit = $VBoxContainer/ChatDisplay
@onready var message_input: TextEdit = $VBoxContainer/InputContainer/MessageInput
@onready var send_button: Button = $VBoxContainer/InputContainer/SendButton
@onready var back_button: Button = $VBoxContainer/Header/BackButton
@onready var mode_toggle: CheckButton = $VBoxContainer/Header/ModeToggle

var mode_manager: Node
var api_manager: Node

func _ready():
	# Create managers
	mode_manager = preload("res://scripts/ModeManager.gd").new()
	add_child(mode_manager)
	mode_manager.initialize()
	
	api_manager = preload("res://scripts/APIManager.gd").new()
	add_child(api_manager)
	api_manager.initialize()
	
	# Connect signals
	send_button.connect("pressed", _on_send_button_pressed)
	back_button.connect("pressed", _on_back_button_pressed)
	mode_toggle.connect("toggled", _on_mode_toggled)
	mode_manager.connect("mode_changed", _on_mode_changed)
	api_manager.connect("request_completed", _on_api_request_completed)
	
	# Set initial mode
	update_mode_display()
	
	chat_display.text = "Welcome to DeepSeek Chat!\n\nCurrent mode: " + mode_manager.get_current_mode_display_name() + "\n\nType your message below.\n\n"

func _on_send_button_pressed():
	var message = message_input.text.strip_edges()
	if message.is_empty():
		return
	
	# Add user message to chat
	chat_display.text += "You: " + message + "\n\n"
	message_input.text = ""
	send_button.disabled = true
	
	# Show loading message
	chat_display.text += "DeepSeek: Thinking...\n\n"
	
	# Send to DeepSeek API
	api_manager.send_request(message, mode_manager.get_current_model_name())

func _on_back_button_pressed():
	get_tree().change_scene_to_file("res://scenes/MainMenu.tscn")

func _on_mode_toggled(button_pressed: bool):
	var new_mode_name = mode_manager.toggle_mode()
	update_mode_display()

func _on_mode_changed(model_name: String):
	chat_display.text += "\n[Mode changed to: " + mode_manager.get_current_mode_display_name() + "]\n\n"

func _on_api_request_completed(response_text: String, error: String):
	send_button.disabled = false
	
	# Remove the "Thinking..." message
	var text = chat_display.text
	var last_thinking = text.rfind("Thinking...")
	if last_thinking != -1:
		chat_display.text = text.substr(0, last_thinking)
	
	if error:
		chat_display.text += "Error: " + error + "\n\n"
	else:
		chat_display.text += "DeepSeek: " + response_text + "\n\n"
	
	# Scroll to bottom
	chat_display.scroll_vertical = chat_display.get_line_count()

func update_mode_display():
	mode_toggle.button_pressed = (mode_manager.current_mode == 1)
	mode_toggle.text = "DeepThink: " + ("ON" if mode_manager.current_mode == 1 else "OFF")
