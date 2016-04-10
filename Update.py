from operator import setitem

class Update:
	
	def __init__(self, update = None, field = None, value_expression = None):
		if update != None:
			self.update = update
		elif field is not None and value_expression is not None:
			self.update = lambda _fields: setitem(_fields, field, value_expression())
		else:
			self.update = lambda _fields: None
		
	def execute(self, fields):
		self.update(fields)