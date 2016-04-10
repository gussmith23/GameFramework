class Game:
	fields = {}
	states = {}
	current_state = 0
	
	def __init__(self, fields, states, initial_state):
		# DEFINE FIELDS
		# SET INITIAL FIELDS
		self.fields = fields
		# DEFINE STATES
		# SET INITIAL STATE
		self.current_state = self.states[initial_state]
						
	def step(self):
		self.current_state = self.current_state.step()