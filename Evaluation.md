RIGG
====

Experimental evaluation
-----------------------

We present the experimental evaluation of our tool.

Note that the data that we present was created using the code in the **benchmark** branch of the algorithm.

For the experimental evaluation, we have generated random instances of regular inclusion grammar games and solved them using a set of three algorithms.

### Instances

We generate an automaton according to the Tabakov-Vardi model and a linear context-free grammar (a grammar with rules of the shape `X -> aYb`).
For both automaton and grammar, we generate examples (i.e. automata and grammars with a lot of transitions relative to the number of states / nonterminals).
The specific parameters for the Tabakov-Vardi model can be seen in the source code.

### Algorithms

To each instance, we apply the following algorithms.
The algorithms are described in more detail in the *Theoretical background* section of [README.md](README.md) resp. our publication.

1. A summary-based solver using a naive implementation of Kleene iteration\
   (i.e. one that updates the information about every non-terminal in every iteration)
2. A summary-based solver using chaotic iteration\
   (i.e. a worklist-based implementation of Kleene iteration that tracks dependencies and updates information on-demand)
3. A saturation-based solver using Cachat's algorithm


### Experiments

For each set of parameters (see below) we have generated a set of 50 randomly generated instances and applied all three algorithms.

If an algorithm could not solve an instance within 10 seconds, we count that as a timeout.
Otherwise, we track the time needed by that algorithm.

The data was obtained in 2016 using a machine running Windows with an Intel i7-6700K running at 4 GHz and 16 GB of RAM.

A triple `x/y/z` in the first column specifies the parameters of the random generation for the instances benchmarked in that row.
* `x` is the number of states of the randomly generated automaton
* `y` is the number of terminals shared between the automaton and the grammar
* `z` is the number of nonterminals of the grammar per player (so the total number of nonterminals is `2z`)

The `avg. time` column (for each of the algorithms) is the average time (in miliseconds) the algorithm needed to solve the instances **that could be solved within a 10 second interval**.\
The `timeout` is the percentage of instances that could not be solved within a 10 second interval. (`0` means no timeouts, `100` means no instance could be solved).

| Parameters | Summaries (naive) |           | Summaries (worklist) |           | Saturation  |           |
|-----------:|------------------:|----------:|---------------------:|----------:|------------:|----------:|
|            |       `avg. time` | `timeout` |          `avg. time` | `timeout` | `avg. time` | `timeout` |
| ` 5/ 5/ 5` |           `65.2`  |      `2%` |                `0.8` |      `0%` |      `94.7` |      `0%` |
| ` 5/ 5/10` |            `5.4`  |      `4%` |                `7.4` |      `0%` |     `701.7` |      `0%` |
| ` 5/10/ 5` |           `13.9`  |      `0%` |                `0.3` |      `0%` |     `375.7` |      `0%` |
| ` 5/ 5/15` |            `6.0`  |      `0%` |                `1.1` |      `0%` |    `1618.6` |      `0%` |
| ` 5/10/10` |           `32.0`  |      `2%` |              `122.1` |      `0%` |    `2214.4` |      `0%` |
| ` 5/15/ 5` |           `44.5`  |      `0%` |                `0.2` |      `0%` |     `620.7` |      `0%` |
| ` 5/ 5/20` |            `3.4`  |      `0%` |                `1.4` |      `0%` |    `3434.6` |      `4%` |
| ` 5/10/15` |          `217.7`  |      `0%` |                `7.4` |      `0%` |    `5263.0` |     `16%` |
| `10/ 5/ 5` |            `8.8`  |      `2%` |                `0.6` |      `0%` |    `2737.8` |      `2%` |
| `10/ 5/10` |            `9.0`  |      `6%` |               `69.8` |      `0%` |    `6484.9` |     `66%` |
| `15/ 5/ 5` |           `30.7`  |      `0%` |                `0.2` |      `0%` |    `5442.4` |     `52%` |
| `10/10/ 5` |            `9.7`  |      `0%` |                `0.2` |      `0%` |    `7702.1` |     `92%` |
| `10/15/15` |          `252.3`  |      `0%` |                `1.9` |      `0%` |       `n/a` |    `100%` |
| `10/15/20` |           `12.9`  |      `0%` |                `1.8` |      `0%` |       `n/a` |    `100%` |

### Evaluation

* Unsurprisingly, the worklist-based implementation of our summary-based algorithm is superior to the one using naive Kleene iteration.

  In some rows (e.g `5/10/10`) it seems that the naive algorithm is superior because it has a better average running time. However, note that the naive algorithm had timeouts while the worklist-based one did not. The average time only takes solved instances into account.

* Both summary-based algorithms are vastly superior to the saturation-based algorithm.

  For the parameters `10/15/15` and `10/15/20`, the saturation-based algorithm was not able to solve a single instance within a 10 second time interval, while the summary-based algorithms solved the same 50 instances without problems.

  The performance of the summary-based algorithm using a worklist was always faster by at least one order of magnitude (factor 10) and often faster by two orders of magnitude (factor 100)
