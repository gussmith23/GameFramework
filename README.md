# Game Framework
### A generalized language for coding (simple) text-based games

Programmatic creation of games requires a decent amount of work, so we created a unique, writable, and readable language to create games in.
In most languages, game code requires many defined objects and inter-relationships.
With RuleBook, the programmatic creation of a game is as simple as writing the rulebook for the game.
The game is defined by different paragraphs in the book, each of which is defined by a set of instructions.

They can be quite simply defined:

##### Output text:
    Say "Hello World".

##### Get user input to set a variable:
    Get variable_name.

##### Set a variable value:
    Set variable_name.

##### Assignment uses "is":
    variable is value.

Notice every statement in RuleBook so far ends with a period `.`.

##### If/Else Conditionals:
    if condition:
      statement1,
      statement2.
    else:
      statement3,
      statement4.
  
The if/else lines need a `:` to define; the last statement in each block ends with `.`; all other statements end with `,`.
  
##### Conditional Form:
    value1 is value2
    value1 < value2
    value1 > value2

##### Defining a paragraph:
    state choose:
    
Paragraphs begin with the keyword `state` and end with a `:`.

##### Every rulebook has two obligitory paragraphs:
    state init:
      define all your variables here
    state finished:
      this is the state your game goes to when done
      
##### Transitions between states are formalized by setting the required `next_state` at the end of each state:
    next_state is "finished".

Some background:
Text-based games can be represented by state machines by a low-footprint Python interpreter,
so it is our Python interpreter and C++ language parser that does the heavy lifting.

##### Instructions for building and compiling:
    $ directory/you/want/GameFramework
    $ git clone https://github.com/gussmith23/GameFramework/
    $ cd GameFramework/parser
    $ mkdir build
    $ cd build
    $ cmake .. -DCMAKE_BUILD_TYPE=Release
    $ make
    $ cd ../..
    $ python3 interpreter.py GuessNumber.txt

#### After the initial compilation, you can focus on writing simple, readable code.
#### Happy Gaming!
