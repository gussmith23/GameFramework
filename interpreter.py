from Game import Game
from State import State
from Update import *

from operator import setitem
from random import randint
from math import *

import json
import subprocess
import sys

# Initializes the fields dictionary global for the game object
def create_fields(_input):
	init = _input['init']
	fields = {}
	for update in init:
		add_initial_field_from_update(update,fields)
	return fields
		
# parses an update to see if there's an initial
# value to be set, and if there is, add it to fields.
# if the update isn't a "set" or "update_list" then it just returns.
# if it's a set, it adds to fields. if it's a list,
# it recurses.
def add_initial_field_from_update(update, fields):

	# straightforward case.
	if update['type'] == "set":
		fields[update['field']] = eval(update['value']) # note this assumes values are constants
		
	# recurse to find sets contained in lists.
	elif update['type'] == "update_list":
		for u in update['list']:
			add_initial_field_from_update(u, fields)

def create_update(elem):
	if elem['type'] == 'say':
		return Update_say(elem['say_string'])
	if elem['type'] == 'get':
		#return Update_get(elem['get_field'], elem['get_type'], elem['get_prompt'] if elem['get_prompt'] else None)
		# Hardcoding int for now
		# http://stackoverflow.com/questions/11775460/lexical-cast-from-string-to-type
		try:
			return Update_get(elem['get_field'], int, elem['get_prompt'])
		except KeyError:
			return Update_get(elem['get_field'], int)
	if elem['type'] == 'set':
		f = lambda _fields: eval(elem['value'], _fields)
		return Update_set(field = elem['field'], value_expression = f)
	if elem['type'] == 'cond':
		return parse_cond_update(elem)
	if elem['type'] == 'update_list':
		return Update_list([create_update(i) for i in elem['list']])
	if elem['type'] == 'finish':
		return Update_finish()

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
		cond_list_element['condition'] = if_or_else_dict['condition']
		cond_list_element['updates'] = [create_update(i) for i in if_or_else_dict['updates']]
		
		cond_list.append(cond_list_element)
		
	return Update_cond(cond_list)

# Creates all updates
# Returns the created state with all updates added
def create_state(_updates):
	updates = [ create_update(update) for update in _updates ]
	return State(updates)

# Takes json dict of rough format:
#	{ "init" : [u1, u2, ...], ... }
def create_game(_states, fields):
	#states = {}
	#for name,updates in _states.items():
	#	states[name] = create_state(updates)
	states = {name:create_state(updates) for name,updates in _states.items()}
	return Game(fields, states)

# Main function: just run the game 
def run(filename):
	with open(filename) as file:
		data = file.read().replace('\n','')
	dj = json.loads(data)
	#json read stuff
	#fields = create_fields(_input)
	fields = create_fields(dj)
	# debug
	#print(fields)
	#game = create_game(_input, fields)
	game = create_game(dj, fields)
	# not ideal solution
	while( fields['next_state'] != "finished" ):
		game.step()
	print(" done with the loop ")

def runWithFilename(filename):
	with open(filename) as file:
		data = file.read()
	runFromString(data)

# Input is a string containing the contents of the file.	
def runFromString(_input):
	completed = subprocess.check_output(["parser/build/parser"], universal_newlines = True, input = _input)
	parsed = json.loads(completed)
	fields = create_fields(parsed)
	game = create_game(parsed, fields)
	# not ideal solution
	while( fields['next_state'] != "finished" ):
		game.step()
	print("Game over. Play again?")

if __name__ == '__main__':
	runWithFilename(sys.argv[1])
