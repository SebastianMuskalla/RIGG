RIGG
====

Tool for solving Regular Inclusion Grammar Games
------------------------------------------------

RIGG is a tool for solving regular inclusion grammar games, a certain type of board games with perfect information.
These are games on a game arena defined by a context-free grammar and inclusion in the regular language of a finite automaton as the winning condition.
This is the type of game described in the publication "Summaries for context-free games" (see [Publications](#Publications) below), and the algorithm implemented by RIGG to solve such games also stems from that publication.

### Contents

* [Branches](#Branches)
* [License](#License)
* [Warning](#Warning)
* [Project structure](#Project-structure)
* [Build instructions](#Build-instructions)
* [Usage](#Usage)
* [A note on the computational complexity](#A-note-on-the-computational-complexity)
* [Publications](#Publications)
* [Theoretical background](#Theoretical-background)

### Branches

We provide two branches:

* **main** is a modern cleaned-up version of the code intended to be compiled using GCC. It does **not** contain the code that was used for the evaluation of the tool in our publication.
* **benchmarking** is a legacy variant of the code that was used for creating the evaluation featured in our 2016 [publication](#Publications). It contains the benchmarking code that relies on Windows API functions and is intended to be compiled using GCC 4.8.3 under MinGW 64 on Windows.

### License

Copyright 2016-2022 Sebastian Muskalla

RIGG is free and open-source software, licensed under the GPLv3 (GNU General Public License), see [LICENSE](LICENSE).

### Warning

This tool is an experimental prototype implementation of our algorithm that I created in the weeks before submitting our 2016 paper.
While the code is functional, it certainly leaves a lot of room for improvement (documentation, code quality, code style, efficiency).
While I recently took the time to refactor and rewrite large parts of the code in order to make it more readable and usable, there is still a substantial amount of work to do.

### Project structure

* [CMakeLists.txt](CMakeLists.txt) - cmake file used for building the code
* **src** - `C++` source code
  * [main.cpp](src/main.cpp) - file containing the main method
  * [Help.h](src/Help.h) and [Help.cpp](src/Help.cpp) - helper class for printing the help
  * [Examples.h](src/Examples.h) and [Examples.cpp](src/Examples.cpp) - helper class for printing two handcrafted examples
  * [Benchmarking.h](src/Benchmarking.h) and [Benchmarking.cpp](src/Benchmarking.cpp) - class for benchmarking randomly generated instances
  * `common/` - classes that are shared between the algorithms
  * `summaries/` - classes implementing our summary-based algorithm
  * `saturation/` - classes implementing SaturationSolver's saturation-based algorithm
  * `randomgen/` - classes implementing the random generator for grammars and automata
  * `logging/` - classes for logging to `STDOUT`
* [Evaluation.md](Evaluation.md) - a markdown version of the experimental evaluation of the tool that was created using the *benchmark* branch
* [LICENSE](LICENSE) - the GPLv3 license
* [README.md](README.md) - this README file

### Build instructions

1. Create a `build` folder
2. Create a `Makefile` using `cmake`
3. Compile the code using `make` (invokes GCC)

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

Building/compiling has been tested using `cmake` version 3.18.4, GNU `make` 4.3, GCC version 10.2.1, under Debian 11.4 running in WSL2 on Windows 10 21H2.\
The code is using the C++17 standard.

### Usage

RIGG provides a command-line interface that supports the following arguments.

* `-h` (or `--help`): Show a list of the available arguments
* `-e` (or `--examples)`: Execute two handcrafted examples with extensive debug output, including one example featured in our publication.

If neither of these parameters is provided, RIGG will behave as follows:

* Create a randomly-generated automaton using the Tabakov-Vardi model
* Create a randomly-generated linear game grammar using a modified Tabakov-Vardi model\
    (with rules of the shape `X -> aYb`)
* Solve the instance of the inclusion game specified by this automaton and grammar using
    * the summary-based algorithm from our publication
    * a saturation-based procedure
* RIGG will log the times needed for solving the instances
* (plus sanity check that both algorithms yield the same result)

The details of the random generation (and some other features) can be specified using the following arguments:

* `--log <LOGLEVEL>`
    * `--log NORMAL` (**default**): Show some output when benchmarking
    * `-q` (or `--quiet` or `--log ERROR`): Show reduced output when benchmarking
    * `-v` (or `--verbose` or `--log INFO`): Show extended output when benchmarking,\
        e.g. the generated grammar and the automaton
    * `-vv` (or `--vv` or `--veryverbose` or `--log DEBUG`): Show debug output when benchmarking,\
        e.g. detailed information about the intermediary steps of applying the algorithm.

        Note that the console output using `--log INFO` or `--log DEBUG` may influence the time measurements.

* `-a <ALGO>` (or `--algorithm <ALGO>`): which algorithm should be used to solve the instance?
    * `-a BOTH` (**default**): use both algorithm, compare the times and the results
    * `-a SUMMARIES`: just us the summaries-based algorithm
    * `-a SATURATION`: just us the saturation-based algorithm

* `-r <INT>` (or `--seed <INT>`): specify a seed for the pseudo-random number generator ([Mersenne Twister 19937](https://cplusplus.com/reference/random/mt19937/)).
    * If no seed is provided, the tool will use [std::random_device](https://en.cppreference.com/w/cpp/numeric/random/random_device) to generate a seed. The generated seed will be printed so it can be reused.
    * The following guarantee should hold: If the same binary of RIGG on the same machine is called with the same seed and the same parameters for the random generation of the objects, then it will generate the same game instance.
* `-l <INT>` (or `--letters <INT>`), **default value 5**: number of (terminal) letters in the shared alphabet of the automaton and the grammar
* `-s <INT>` (or `--states <INT>`), **default value 5**: number of control states of the automaton
* `-n <INT>` (or `--nonterminals <INT>`), **default value 5**: number of nonterminal symbols of the grammar per player (the total number of nonterminals will be twice the specified number)
    * Alternatively, the number of nonterminals can be set per player using the arguments `--nonterminalsuniversal <INT>` and `--nonterminalsexistential <INT>`.
* `--automatondensity <DOUBLE>`, **default value 0.8**: the density of the transition relation of the automaton, i.e. the rate of transitions per state letter\
    (e.g. a rate of 1 means that on average, each state will have one outgoing transition per letter).
* `--automatonfinals <DOUBLE>`, **default value 0.2**: the rate of final states of the automaton\
    (e.g. a rate of 0.2 means that 20% of all control states will be final).
* `--grammardensity <DOUBLE>`, **default value 0.75**: the density of the production rules of the grammar, i.e. the rate of rules per pair of nonterminals\
    (e.g. a rate of 0.75 means that the number of rules will be 0.75 times the total number of non-terminals squared).
* `--grammarfinals <DOUBLE>`, **default value 0.85**: the rate of rules of the grammar that will **not** have nonterminal on their right-hand side\
    (e.g. a rate of 0.85 will mean that 85% of the rules have no nonterminal, while 15% do)
* `--grammarleft <DOUBLE>`, **default value 0.85**: the rate of rules of the grammar that will have a terminal on the left-hand side of the potential occurrence of the nonterminal
* `--grammarright <DOUBLE>`, **default value 0.85**: the rate of rules of the grammar that will have a terminal on the right-hand side of the potential occurrence of the nonterminal


Calling RIGG with no arguments (`./RIGG`) is equivalent to the following call

```
./RIGG                             \
    --log NORMAL                   \
    --algorithm BOTH               \
    --letters 5                    \
    --states 5                     \
    --nonterminalsuniversal 5      \
    --nonterminalsexistential 5    \
    --automatondensity 0.8         \
    --automatonfinals 0.2          \
    --grammardensity 0.75          \
    --grammarfinals 0.85           \
    --grammarleft 0.85             \
    --grammarright 0.85
```

### A note on the computational complexity

**Do not be confused** if RIGG **fails** to solve some instances, even if the size of the grammar and the automaton specified by the arguments seem to be *small*.

RIGG may not terminate within acceptable time, or it may try to allocate more memory than available.\
It is sensible to use limit the resources (time and/or memory) of RIGG to avoid unexpected behavior.

This is not a problem of our implementation (even though it certainly could be more efficient).\
Both algorithms implemented in RIGG are optimal with respect to their asymptotic worst-case time complexity.\
Solving regular inclusion grammar games simply is a very difficult task.

#### Details

In our publication, we have shown that the computational complexity of the problem is 2EXPTIME.\
Roughly speaking, this means that solving an instance (automaton plus grammar) that can be encoded in `n` bits may take up to `2^(2^n)` steps, no matter which algorithm is used.\
Note that this number is already insanely large for small values of `n`, e.g. for `n = 10`, `2^(2^10)` is a decimal number with 309 digits.

Both algorithms implemented in RIGG are optimal in the sense that they take at most doubly exponential time.
It is also known that this doubly exponential time complexity cannot be avoided in some cases.

Typically (in the average case), we expect RIGG to solve instances much faster than the doubly exponential worst-case time complexity.\
However, complexity theory tells us that this lower bound cannot be avoided, i.e. for some instances, any algorithm that solves regular inclusion grammar games will require a doubly exponential number of steps.

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
  T. SaturationSolver\
  In: ICALP 2022, volume 2380 of LNCS, pages 704–715

  We compare our algorithm to an approach to solving inclusion games that uses a method by SaturationSolver. To this end, our tool contains an implementation of SaturationSolver's algorithm.

* *Experimental evaluation of classical automata constructions*\
  D. Tabakov and M. Y. Vardi\
  In: LPAR 2005, volume 3835 of LNCS, pages 396–411

  Our tool implements a generator for random automata using the Tabakov-Vardi model. We have also implemented a generalization of the Tabakov-Vardi model to linear context-free grammars.

* *Compiler design: Analysis and transformation*
  H. Seidl, R. Wilhelm, and S. Hack.
  Springer, 2012

  This book provides a description for the worklist-based implementation of Kleene iteration - often called chaotic iteration - on which our summary-based algorithm relies

### Theoretical background

We provide some theoretical background for inclusion games.\
For more details, we refer to the [publication](#Publications).

#### Instances

We consider regular inclusion grammar game (short: games).
An instance of a game consists of
- a context-free grammar with an ownership partitioning of the nonterminals into the nonterminals owned by the Existential resp. the Universal Player,
- a finite automaton over the terminals specifying the regular target language,
- an initial sentential form.

The tool provides a random generator for instances that is based on the Tabakov-Vardi model for automata and an extension thereof for linear context-free grammars.

#### Playing the game

A play of the game starts from the initial sentential form.
Each move of the play consists of the owner of the left-most nonterminal picking a replacement rule.
This replacement rule is applied, we obtain a new nonterminal and the game goes on.
This process repeats until we obtain a word, a sentential form without nonterminals, or ad infinitum.

The Universal Player wins the play if the play goes on forever or if it ends in a word that is contained in the regular target language.\
The Existential Player wins if the play ends after finitely many steps in a word that is not in the regular target language.

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

### SaturationSolver-based solver

The second solver uses an algorithm by T. SaturationSolver for solving games on pushdown graphs with a regular winning condition.

In order to be able to use this algorithm for solving regular inclusion grammar game, we first need to transform the game instance.
This involves determinizing and minimizing the automaton for the winning condition.
Then, we can apply SaturationSolver's algorithm.

SaturationSolver's algorithm starts with a description of the target language in the form of a so-called P-AFA.
It then *saturates* it by adding transitions.
When this process has been applied exhaustively, we end up with a description of the winning regions from which the winner of the game can be read off.
