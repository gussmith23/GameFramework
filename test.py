from Game import Game
from State import State
from Update import Update
from random import randint
from operator import setitem


fields = {
	'number_to_guess' : randint(0, 10), 
	'guessed_num' 		: -1,
	'victory'					: False,
	'current_state' 	: "init"
}

def main():
	
	update_init_print = Update(say_string = "Welcome to Guess a Number!")
	update_init_nextstate = Update(field = 'current_state', value = 'choose')
	state_init = State([update_init_print, update_init_nextstate])
	
	update_guessed_num = Update(get_field = 'guessed_num', get_prompt = "Guess a number: ", get_type = int)
	update_victory = Update(field = 'victory', value_expression = lambda _fields: _fields['number_to_guess'] == _fields['guessed_num'])
	update_guess_branch = Update(branch_state_true = 'high', branch_state_false = 'low',
																branch_expression = lambda _fields: _fields['guessed_num'] > _fields['number_to_guess'])
	state_choose = State([update_guessed_num, update_victory, update_guess_branch])
	
	update_high_say = Update(say_string = "Too high!")
	update_high_nextstate = Update(field = 'current_state', value = 'choose')
	state_high = State([update_high_say, update_high_nextstate])
	
	update_low_say = Update(say_string = "Too low!")
	update_low_nextstate = Update(field = 'current_state', value = 'choose')
	state_low = State([update_low_say, update_low_nextstate])

	
	game = Game(fields, {"init":state_init, "choose":state_choose,
												"low" : state_low, "high" : state_high})
	
	while(not fields['victory']):
		game.step()
		
	print("You won!")
		
if __name__ == "__main__":
	main()