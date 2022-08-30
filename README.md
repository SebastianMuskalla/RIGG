RIGG
====

Tool for solving Regular Inclusion Grammar Games
------------------------------------------------

RIGG is a tool for solving regular inclusion grammar games, a certain type of board games with perfect information.
These are games on a game arena defined by a context-free grammar and inclusion in the regular language of a finite automaton as the winning condition.
This is the type of game described in the publication "Summaries for context-free games" (see [Publications](#Publications) below), and the algorithm implemented by RIGG to solve such games also stems from that publication.

### Contents

* [Branches](#Branches)
* [License](#LIcense)
* [Project structure](#Project-structure)
* [Build instructions](#Build-instructions)
* [Usage](#Usage)
* [Publications](#Publications)
* [Theoretical background](#Theoretical-background)

### Branches

We provide two branches:

* **main** is a modern cleaned-up version of the code intended to be compiled using GCC. It does **not** contain the code used for benchmarking.
* **benchmarking** is a legacy variant of the code that was used for creating the evaluation featured in our 2016 [publication](#Publications). It contains the benchmarking code that relies on Windows API functions and is intended to be compiled using GCC 4.8.3 under MinGW 64 on Windows.

### License

Copyright 2016-2022 Sebastian Muskalla

GPLv3 License, see [LICENSE](LICENSE)

### Project structure

* **src** - C++ source code
  * [main.cpp](src/main.cpp) - file containing the main method
  * [examples.cpp](src/examples.cpp) - some handcrafted examples
  * *common* - classes that are shared between the algorithms
  * *summaries* - classes implementing our summary-based algorithm
  * *saturation* - classes implementing Cachat's saturation-based algorithm
  * *randomgen* - classes implementing the random generator for grammars and automata
* [CMakeLists.txt](CMakeLists.txt) - cmake file (can also be used as CLion project file)
* [Evaluation.md](Evaluation.md) - a markdown version of the experimental evaluation of the tool that was created using the *benchmark* branch
* [LICENSE](LICENSE) - the GPLv3 license
* [README.md](README.md) - this README file

### Build instructions

This is a C++-project, originally developed for MinGW 64 under Windows, GCC 4.8.3.
A CMake project file is contained.

Note that the classes used for benchmarking use Windows system functions and will only work with MinGW under Windows.
The "linux"-branch contains a project file that does not contain the benchmarking-classes.

### Usage


### Publications

This tool was written in conjunction with our FSTTCS 2016 publication

**Summaries for context-free games**\
L. Holík, R. Meyer, and S. Muskalla\
In: FSTTCS 2016, volume 65 of LIPIcs, pages 41:1–41:16\
<https://muskalla.saarland/publications#hmm16>

This tool provides a prototype implementation of the algorithm for solving inclusion games described in the above paper.\
The main purpose of the tool was to evaluate the performance of the algorithm compared to other algorithms that can be used to solve such games.

The tool also implements algorithms described in the following publications:

* *Symbolic strategy synthesis for games on pushdown graphs*\
  T. Cachat\
  In: ICALP 2022, volume 2380 of LNCS, pages 704–715

  We compare our algorithm to an approach to solving inclusion games that uses a method by Cachat. To this end, our tool contains an implementation of Cachat's algorithm.

* *Experimental evaluation of classical automata constructions*\
  D. Tabakov and M. Y. Vardi\
  In: LPAR 2005, volume 3835 of LNCS, pages 396–411

  Our tool implements a generator for random automata using the Tabakov-Vardi model. We have also implemented a generalization of the Tabakov-Vardi model to linear context-free grammars.

* *Compiler design: Analysis and transformation*
  H. Seidl, R. Wilhelm, and S. Hack.
  Springer, 2012

  This book provides a description for the worklist-based implementation of Kleene iteration - often called chaotic iteration - on which our summary-based algorihm relies

### Theoretical background

We provide some theoretical background for inclusion games.\
For more details, we refer to the [publication](#Publications).

#### Instances

We consider regular inclusion grammar game (short: games).
An instance of a game consists of
- a context-free grammar with an ownership partitioning of the nonterminals into the nonterminals owned by the existential resp. the universal player,
- a finite automaton over the terminals specifying the regular target language,
- an initial sentential form.

The tool provides a random generator for instances that is based on the Tabakov-Vardi model for automata and an extension thereof for linear context-free grammars.

#### Playing the game

A play of the game starts from the initial sentential form.
Each move of the play consists of the owner of the left-most nonterminal picking a replacement rule.
This replacement rule is applied, we obtain a new nonterminal and the game goes on.
This process repeats until we obtain a word, a sentential form without nonterminals, or ad infinitum.

The universal player wins the play if the play goes on forever or if it ends in a word that is contained in the regular target language.\
The existential player wins if the play ends after finitely many steps in a word that is not in the regular target language.

The computational problem solved by the tool is, given an instance, deciding the winner, i.e. which player can enforce reaching her winning condition.

The challenge is that the plays potentially go on forever and that the game arena on which the game is played is potentially infinite.
Hence, we have to compute information about an infinite game given its finite syntactical description.

In order to do so, the tool implements two solver.


#### Summary-based solver

The summary-based solver is based on the algorithm described in our [publication](#Publications).

Given a game instance, it proceeds as follows.
It transforms the context-free grammar into a system of equations with one equation per nonterminal of the grammar.
It then solves this system over the domain of positive Boolean formulas over the transition monoid of the automaton.
To this end, it uses chaotic iteration (a worklist-based implementation of Kleene iteration) to computes its least solution.
The least solution provides for each nonterminal a *summary* of its effect on the automaton, hence, we call this approach summary-based.

Using this least solution, one can compute a formula associated to the initial sentential form from which the winner can be read off.

### Saturation-based solver

The second solver uses an algorithm by T. Cachat for solving games on pushdown graphs with a regular winning condition.

In order to be able to use this algorithm for solving regular inclusion grammar game, we first need to transform the game instance.
This involves determinizing and minimizing the automaton for the winning condition.
Then, we can apply Cachat's algorithm.

Cachat's algorithm starts with a description of the target language in the form of a so-called P-AFA.
It then *saturates* it by adding transitions.
When this process has been applied exhaustively, we end up with a description of the winning regions from which the winner of the game can be read off.
