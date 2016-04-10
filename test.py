from Game import Game
from State import State
from Update import Update

fields = {
	'number_to_guess' : 5, 
	'guessed_num' 		: 0,
	'victory'					: False,
	'current_state' 	: "choose"
}

def main():
	update_guessed_num = Update(get_guess)

def get_guess(_fields):
	try:
		guess = int(raw_input('Guess a number:'))
		_fields['guessed_num'] = guess
		return True
	except ValueError:
		return False
		
if __name__ == "__main__":
	main()