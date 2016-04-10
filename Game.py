class Game:
	def __init__(self, fields, states):
		self.fields = fields
		self.states = states
		
	def step(self):
		# Currently, programmers are required to use "next_state" variable in fields declaration
		self.states[self.fields['next_state']].step(self.fields)
