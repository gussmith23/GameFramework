import json

def create_update(elem):
	if elem['type'] == 'say':
		return Update_say(elem['say'])
	if elem['type'] == 'get':
		#return Update_get(elem['get_field'], elem['get_type'], elem['get_prompt'] if elem['get_prompt'] else None)
        # Hardcoding int for now
        # http://stackoverflow.com/questions/11775460/lexical-cast-from-string-to-type
        return Update_get(elem['get_field'], int, elem['get_prompt'] if elem['get_prompt'] else None)
	if elem['type'] == 'set':
        f = lambda __: elem['value']
		return Update_set(field = elem['field'], value_expression = f)
    if elem['type'] == 'cond':
        pass

def create_state(updates):
    pass

def create_game(list):
    pass

def run_game(input):
    #json read stuff
    # call create_game
    #while(True):
    #    game.step
    pass
