class Game:
	def __init__(self, fields, states):
		self.fields = fields
		self.states = states
		
	def step(self):
		self.states[self.fields['current_state']].step(self.fields)