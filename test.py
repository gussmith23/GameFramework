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
	
	update_init_print = Update(lambda _fields: print("Welcome to Guess a Number!"))
	update_init_nextstate = Update(field = 'current_state', value = 'choose')
	state_init = State([update_init_print, update_init_nextstate])

	update_guessed_num = Update(field = 'guessed_num', value_expression = lambda _fields: int(input('Guess a number: ')))
	update_victory = Update(field = 'victory', value_expression = lambda _fields: _fields['number_to_guess'] == _fields['guessed_num'])
	state_choose = State([update_guessed_num, update_victory])
	
	game = Game(fields, {"init":state_init, "choose":state_choose})
	
	while(not fields['victory']):
		game.step()
		
	print("You won!")
		
if __name__ == "__main__":
	main()