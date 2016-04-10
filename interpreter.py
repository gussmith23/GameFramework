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
		f = lambda _fields: eval(elem['value'], _fields)
		return Update_set(field = elem['field'], value_expression = f)
	if elem['type'] == 'cond':
		return parse_cond_update(elem)
	if elem['type'] == 'update_list':
		return Update_list([create_update(i) for i in elem['list']])
	if elem['type'] == 'finish':
		return Update_finish()

# Creates all updates
# Returns the created state with all updates added
def create_state(_updates):
	updates = [ create_update(update) for update in _updates ]
	return State(updates)

# Takes json dict of rough format:
#	{ "init" : [u1, u2, ...], ... }
def create_game(_states):
	#states = {}
	#for name,updates in _states.items():
	#	states[name] = create_state(updates)
	states = {name:create_state(updates) for name,updates in _states.items()}
	return Game(fields, states)

# Function 
def run_game(_input):
	#json read stuff
	game = create_game(input)

	# not ideal solution
	while( fields['state'] != "Finish" ):
		game.step()
	print("Donzo")


# takes cond update json
# returns conditional Update.
def parse_cond_update(elem):
	#we need to make: 
	# cond_list = [
	#  {'type':'if', 'condition':some lambda, 'updates' : list of updates},
	#		{'type':'else', 'updates'}]
	cond_list = []
	
	for if_or_else_dict in elem['cond_list']:
		cond_list_element = {}
		cond_list_element['type'] = if_or_else_dict['type']
		cond_list_element['condition'] = lambda _fields: exec(if_or_else_dict['condition'], _fields)
		cond_list_element['updates'] = [create_update(i) for i in if_or_else_dict['updates']]
		
		cond_list.append(cond_list_element)
		
	return Update_cond(cond_list)
