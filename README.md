# constraints_solver

The problem solved by this library is defined as follows: 

- Some variables are given, and each has a set of possible values it can take. 
- In addition to that, some pairs of variables are given, and variables in the same pair must be different.

Given these constraints, the algorithm finds a solution, or reports that there is none.
This is a general problem that can have different applications.
In the examples, the algorithm is used to solve some sudokus.

## Variable\<t\>

Class definining a single variable over a domain of possible values of type t.

## ConstraintsSolver

This class, given variables and constraints, finds a solution. 
The Variable objects are modified by the class, and when a solution is found, these will contain the solution.
Iterative deepening is used to find the solution efficiently.
