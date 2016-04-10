from Game import Game
from State import State
from Update import *
from random import randint
from operator import setitem


fields = {
	'number_to_guess' : randint(0, 10), 
	'guessed_num' 		: -1,
	'victory'					: False,
	'current_state' 	: "init"
}

def main():
	
	update_init_print = Update_say("Welcome to Guess a Number!")
	update_init_nextstate = Update_set(field = 'current_state', value = 'choose')
	state_init = State([update_init_print, update_init_nextstate])
	
	update_guessed_num = Update_get('guessed_num', int, "Guess a number: ")
	update_victory = Update_set(field = 'victory', value_expression = lambda _fields: _fields['number_to_guess'] == _fields['guessed_num'])
	update_guess_branch = Update(branch_state_true = 'high', branch_state_false = 'low',
																branch_expression = lambda _fields: _fields['guessed_num'] > _fields['number_to_guess'])
	state_choose = State([update_guessed_num, update_victory, update_guess_branch])
	
	update_high_say = Update_say("Too high!")
	update_high_nextstate = Update_set(field = 'current_state', value = 'choose')
	state_high = State([update_high_say, update_high_nextstate])
	
	update_low_say = Update_say("Too low!")
	update_low_nextstate = Update_set(field = 'current_state', value = 'choose')
	state_low = State([update_low_say, update_low_nextstate])

	
	game = Game(fields, {"init":state_init, "choose":state_choose,
												"low" : state_low, "high" : state_high})
	
	while(not fields['victory']):
		game.step()
		
	print("You won!")
		
if __name__ == "__main__":
	main()