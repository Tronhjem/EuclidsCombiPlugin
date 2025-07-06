# ORchestra

Project is still WIP.

This is a sequencer that generates and combines sequences using euclidean algorithms or manual input. 
It uses logic operations like `&`, `^`, and `|` to combine sequences into tracks that trigger specific notes. 
Each part loops and is evaluated with the specified operator, allowing sequences of different lengths 
to phase and create complex rhythmic patterns. Where it gets really powerful is when combining sequences 
of different legnths and hear how the operations creates interesting evolving patterns.

See more in the prototype repo.
____

Starting work on the plugin version of the prototype Euclids Combinator.
Prototype can be found here: <https://github.com/Tronhjem/EuclidsCombinator>

## Tests

Tests are provided with a seperate jucer and project for a console app that imports select code and runs
the tests with the juce framework unit testing code.

## Syntax and rules for code

It's the program is evaluated from top to bottom.
You will have to declare anything first that you will use later.

Usually this means sequences are created first, then tracks, the sequences triggers to tracks then outputs.

- New line is a new instruction, it's looking for `\n`
- All white space is ignored in a line
- Use `//` for comments until the end of the line

### Data sequences

Data is designed to be used anywhere in any way, it is always just the number.
For triggers, the first argument of `note` or `cc` data, they will evaluate if it's above 0 and be a boolean for trigger.
For cc's and notes and velocity, values will be sent directly as defined.

### Operators

`+` `-` `*` `/` operates on numbers as you would expect them to in normal math.
Normal math precedence is implemented, and parentheses can create precedence just like normal programming math. All math operators takes precedence over logical and comparisons

`|` `^` `&` can be used to evaluate logic operations. This will always use the number as a trigger, meaning check if it's above 0 or not, and combine two numbers and return a 0 or a 1.

`>` `<` `>=` `<=` `==`  compares two values and return a 0 if false and 1 if true.

### Variables

the `=` operator declares a variable.
`a =` declares a variable named a
Variables can be any length but have to start with an alpha numeric character or `_` and can't start with a number.

Following can be assigned to variable:

Regular value
`a = 64`

Any expression:
`a = 64 + 2`
`c = a | 1`
`z = a * (2+4)`

Data sequence
`a = [127, 0, 64]`

Reference to another var, this will evaluate at the current global step when running, if its a sequence.
If not it will just refer to the value assigned to that variable.
`c = [64, 64, 64]`
`a = c`

if you want to reference a specific value in a data sequence you can access the index, starting at 0
`c = [64, 65, 70]`
`a = c[0]`  value is here 64
`x = c[1]`  value is here 65

Be aware when using `|` `^` `&` `< > ==` these will always return a boolean.
`a = [64,63]`
`c = a[0] > 0` Here the value is 1

`z = [1,0]`
`x = [0,1]`
`y = z[0] & x[0]` Here the value is 0, as z is 1 at index 0 but x is 0 at index 0 and a AND operation evaluates to 0.

### Reserved keywords

Following words are reserved and can't be used as variable names.

- note
- cc
- print
- test
- ran
- euc

### Tracks

A track will collect a trigger, data and a midi channel.
Tracks are not variables and  the keywords `note` or `cc` following `(...)` will define them.
They need exactly 4 inputs. These can also be expressions or variables.
The trigger argument will always check if the value is greater than 0 and consider it true or trigger if it is.

For `note` this will be like this:
`note(trigger, note, velocity, channel)`

`cc` is like this:
`cc(trigger, controlNumber, controlValue, channel)`

Example of variables for trigger and note data sequence into a note track

```cpp
a = [1,0,1,0]
b = [64,64,65]

note(a, b, 100, 1) 
// a is the trigger, 
// b is the note track
// a fixed velocity of 100 and sending to channel 1
```

### Built in functions

#### Euclidean sequence

We can assign a euclidean data sequence to an identifier by generating it with the built in function `euc(hits, length)` where we supply the number of hits equally divided on the length of the sequence. It can only be used to be assigned to a variable not as a parameter else where. Also not it only generate 0's and 1's and is meant for being used as a trigger track.
example

```cpp
// euclidean sequence with 4 hits divided on 8 triggers.
a = euc(4,8)
// Note track where a the euclidean sequnence is used as trigger.
note(a, 64, 100, 1)
```

#### Random

with the function `ran(low,high)` you can generate a random value. First parameter specifies the lower valuer of the random generation and the second, the upper, both included. Meaning `ran(0,4)` will generate a random value between 0 and 4 both included.
Random is evaluated at every tick and will give a new random value every time, not just at compile time.
Below is an example of how a random value is used for random velocity

```cpp
vel = ran(50,100)
note(1, 64, vel, 1)
```
