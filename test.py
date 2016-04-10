from Game import Game
from State import State
from Update import Update
import operator

fields = {
	'number_to_guess' : 5, 
	'guessed_num' 		: 0,
	'victory'					: False,
	'current_state' 	: "choose"
}

def main():
	update_guessed_num = Update(get_guess)
	update_victory = Update(lambda _fields: operator.setitem(_fields, 'victory', _fields['number_to_guess'] == _fields['guessed_num']))
	state_guess_number = State([update_guessed_num, update_victory])
	
	game = Game(fields, {"choose":state_guess_number})
	
	while(not fields['victory']):
		game.step()
		
	print("You won!")

def get_guess(_fields):
	try:
		guess = int(input('Guess a number: '))
		_fields['guessed_num'] = guess
		return True
	except ValueError:
		return False
		
if __name__ == "__main__":
	main()