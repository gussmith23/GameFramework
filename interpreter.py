from Game import Game
from State import State
from Update import *

from operator import setitem
from random import randint
from math import *

import json

def create_update(_elem):
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

def create_state(_updates):
	updates = [ create_update(update) for update in updates ]
	return State(updates)

# Takes json dict of rough format
def create_game(_states):
	states = {}
	for name,updates in _states.items():
		statess[name] = create_state(updates)
	return Game(fields, states)

# Function takes json input and runs completed game
def run_game(_input):
	#json read stuff
	game = create_game(input)
	while(True):
		game.step()
	print("Donzo")
