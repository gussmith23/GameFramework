from operator import setitem

class Update:
	
	def __init__(self, update = None, field = None, value = None, 
							value_expression = None, say_string = None, say_args = None,
							get_field = None, get_prompt = None, get_type = None,
							branch_state_true = None, branch_state_false = None, branch_expression = None):
		if update != None:
			self.update = update
		elif field is not None and value_expression is not None:
			self.update = lambda _fields: setitem(_fields, field, value_expression(_fields))
		elif field is not None and value is not None:
			self.update = lambda _fields: setitem(_fields, field, value)
		elif say_string is not None:
			# if there's formatting:
			if say_args is not None:
				self.update = lambda _fields: print(say_string.format(*say_args))
			else: 
				self.update = lambda _fields: print(say_string)
		elif get_field is not None:
			# can't do anything if there's no type specified.
			if get_type is None:
				self.update = lambda _fields: print("Error: no get_type specified")
			elif get_prompt is not None:
				self.update = lambda _fields: setitem(_fields, get_field, get_type(input(get_prompt)))
			else:
				self.update = lambda _fields: setitem(_fields, get_field, get_type(input()))
		elif branch_expression is not None:
			self.update = lambda _fields: setitem(_fields, 'current_state', 
					branch_state_true if branch_expression(_fields) else branch_state_false)
		else:
			self.update = lambda _fields: None
		
	def execute(self, fields):
		self.update(fields)


class Update_set():
	def __init__(self, update = None, field = None, value = None, value_expression = None):
		if update != None:
			self.update = update
		elif field is not None and value_expression is not None:
			self.update = lambda _fields: setitem(_fields, field, value_expression(_fields))
		elif field is not None and value is not None:
			self.update = lambda _fields: setitem(_fields, field, value)

	def execute(self, _fields):
		self.update(fields)

class Update_say():
	def __init__(self, say_string, say_args = None):
		if say_args is not None:
			self.update = lambda _fields: print(say_string.format(*say_args))
		else: 
			self.update = lambda _fields: print(say_string)
			
	def execute(self, fields):
		self.update(fields)

class Update_get():
	def __init__(self, get_field, get_type, get_prompt = None ):
		if get_prompt is not None:
			self.update = lambda _fields: setitem(_fields, get_field, get_type(input(get_prompt)))
		else:
			self.update = lambda _fields: setitem(_fields, get_field, get_type(input()))
			
	def execute(self, fields):
		self.update(fields)
