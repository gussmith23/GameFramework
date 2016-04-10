class State:
	
	def __init__(self, updates):
		self.updates = updates
	
	def step(self, fields):
		for update in self.updates:
			update.execute(fields)