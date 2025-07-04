
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

`>` `<` `==` compares two values and return a 0 if false and 1 if true. 

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

Euclidean data sequence generator 
`a = {5,7}` where first is hits and second is length

Reference to another var, this will evaluate at the current global step when running, if its a sequence.
If not it will just refer to the value assigned to that variable.
`c = [64, 64, 64]`
`a = c`

if you want to reference a specific value in a data sequence you can access the index, starting at 0
`c = [64, 65, 70]`
`a = c[0]`  value is here 64
`x = c[1]`  value is here 65

Be aware when using `|` `^` `&` `< > == ` these will always return a boolean.
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
- track

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





