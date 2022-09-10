Future Work
===========

A ToDo list with ideas to improve this tool
-------------------------------------------

### Documentation

* Fix the formatting (insert `<p>`)
* Properly document parameters and return values of functions

### Functionality

* Support parsing automata and grammars
* Support for generating non-linear grammars

### Other

* Provide an environment for benchmarking that limits the time and memory consumption

### Code

* Add missing move constructors etc.
* Use smart pointers for memory management
* Delete the solvers and all associated information in the `benchmark` method, just keep the winner (as map from Nonterminals to `bool`)
* Store boxes that have already been constructed to avoid wasting memory
* Minimize clauses and formulas
* Sanity check: Check that whenever we are dealing with Letters, they come from the expected alphabet
* Better parameter parsing
    * create classes with callback handlers to avoid code duplication
    * and keep everything in one place
