# RIGG - Regular Inclusion Grammar Games

This tool provides two algorithms to solve Regular Inclusion Grammar Games.

## Instances

An instance of a regular inclusion grammar game consists of
- a context-free grammar with an ownership partitioning of the non-terminals
- a finite automaton over the terminals specifying the regular goal language.
- an initial sentential formulas

An instance is a YES-instance, if the first player (called refuter) can enforce
the derivation of a word from the given initial sential form not accepted by
the automaton in finitely many steps.

Otherwise, the second player (called prover) can enforce either the derivation
of a word in the language of the automaton, or an infinite derivation process.

The tool provides random generators for automata (according to the Tabakov-Vardi
model) and grammars.

## Solver 1

The first solver is based on the paper

Antichains for Inclusion Games
by Lukáš Holík, Roland Meyer, Sebastian Muskalla

It creates a system of equations that has one equation for each non-terminal
if the grammar.
The different rules are connected using conjunction (for rules of refuter)
or disjunction (for rules of prover).

Then a Kleene-iteration is used to find the least solution to this system
of equations over the domain of positive Boolean formulas
over the transition monoid of finite automaton.

Deciding whether refuter can win the game from a given initial sentential
form then amounts to
- computing a formula for the given sentential form by composing
    the formulas for its letters and 
- evaluating this formula under the "rejecting" assignmentthat evaluates
    boxes to true that represent a rejecting behavior of the automaton

## Solver 2

The second solver is based on the paper

Symbolic Strategy Synthesis for Games on Pushdown Graphs
by Thierry Cachat

Cachat considers games on pushdown systems. His instances are specified by
- a pushdown system P with an ownership partitioning of the control states
- a P-AFA specifying the goal configurations
    -> a P-AFA is an alternating finite automaton over the stack alphabet of P
            with one initial state for each control state of P

His algorithm satures the given P-AFA so that it recognizes the whole winning
region, i.e. the set of configurations from which the first player can enforce
a run in which a configuration accepted by the AFA occurs.

One can convert a grammar game into a pushdown game by creating a pushdown
system that uses
- its control states to simulate the state of the finite automaton after
    reading the terminal prefix of the current sentential form
- its stack to simulate the sentential form starting from the first
    non-terminal
    
The finite automaton has to be determinized before creating the pushdown system.
Otherwise, in each step, the active player would be allowed to resolve the non-
determinism in the automaton. This would change the semantics of the game.

## How to build

This is a C++-project, originally developed for MinGW 64 under Windows, GCC 4.8.3.
A CMake project file is contained.

Note that the classes used for benchmarking use Windows system functions and will only work with MinGW under Windows.
The "linux"-branch contains a project file that does not contain the benchmarking-classes.
