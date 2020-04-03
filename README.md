# Learning C++ by implementing calculators

This tiny repository is purely meant for learning C++ the practical way.
It may help if you do know some basics from at least one language already,
but it should be fun doing each level.

## Levels

1. Ask for two numbers, and print the sum of both
2. Ask for two numbers, ask for operation, and evaluate the result
3. Same as above but with proper error handling.

4. Ask for a string input and evaluate the result linearly without operator precedence
5. Same as above but split parsing, syntax checking, and evaluation

6. Ask for an expression, and evaluate its result using a stack
7. Ask for an expression, and evaluate its result using an expression tree

## Other fancy levels

* support complex numbers (by using your own custom number type!)
* support constants (such as E, Pi)
* pre-fold constant expressions in your tree
* simplify expression tree
* support custom functions
* support custom series (such as custom taylor series, sin/cos)
* Add optimization passes, such as:
  * E1 - E2 = 0
  * E1 + E1 = 2 * E1
  * (E1 + E2) * (E1 + E2) = (E1 + E2)^2
  * be creative
* Let the parser generate custom instructions (bytecode stream) that is then interpreted by a VM to
  evaluate the result.
