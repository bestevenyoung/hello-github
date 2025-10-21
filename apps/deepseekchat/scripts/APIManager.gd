extends Node

signal request_completed(response_text, error)

var api_key: String = ""

func initialize():
	var config = ConfigFile.new()
	var error = config.load("user://config.cfg")
	if error == OK:
		api_key = config.get_value("api", "key", "")

func send_request(message: String, model: String):
	if api_key.is_empty():
		request_completed.emit("", "No API key configured")
		return
	
	var http_request = HTTPRequest.new()
	add_child(http_request)
	http_request.connect("request_completed", _on_request_completed.bind(http_request))
	
	var headers = [
		"Content-Type: application/json",
		"Authorization: Bearer " + api_key
	]
	
	var body = JSON.stringify({
		"model": model,
		"messages": [
			{"role": "user", "content": message}
		],
		"stream": false
	})
	
	var error = http_request.request("https://api.deepseek.com/chat/completions", headers, HTTPClient.METHOD_POST, body)
	if error != OK:
		request_completed.emit("", "Failed to send request")

func _on_request_completed(_result, response_code, _headers, body, http_request):
	http_request.queue_free()
	
	if response_code != 200:
		request_completed.emit("", "API request failed (Code: " + str(response_code) + ")")
		return
	
	var json = JSON.new()
	var parse_result = json.parse(body.get_string_from_utf8())
	
	if parse_result != OK:
		request_completed.emit("", "Failed to parse response")
		return
	
	var response_data = json.get_data()
	
	if response_data.has("choices") and response_data["choices"].size() > 0:
		var assistant_message = response_data["choices"][0]["message"]["content"]
		request_completed.emit(assistant_message, "")
	else:
		request_completed.emit("", "No response from API")
