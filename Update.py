class Update:
	
	def __init__(self, update):
		self.update = update
		
	def execute(self, fields):
		self.update(fields)