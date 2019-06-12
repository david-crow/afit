## What is this?

Performance comparison of three Bron–Kerbosch algorithm implementations that find all maximal independent sets in a graph. 

## Implementations

* **Ver1:** Naive Bron–Kerbosch algorithm
* **Ver2:** Ver1 with pivot
* **Ver3:** Ver2 with degeneracy ordering

## Run

    python mis.py

## Result

Stdout result for the sample data

    ## Naive Bron–Kerbosch algorithm
    18 recursive calls
    0: [1, 5]
    1: [1, 6]
    2: [1, 7]
    3: [2, 6]
    4: [2, 7]
    5: [3, 6]
    6: [3, 7]
    7: [4, 5]
    8: [4, 6]
    9: [4, 7]

    ## Bron–Kerbosch algorithm with pivot
    15 recursive calls
    0: [1, 5]
    1: [1, 6]
    2: [1, 7]
    3: [2, 6]
    4: [2, 7]
    5: [3, 6]
    6: [3, 7]
    7: [4, 5]
    8: [4, 6]
    9: [4, 7]

    ## Bron–Kerbosch algorithm with pivot and degeneracy ordering
    18 recursive calls
    0: [2, 6]
    1: [2, 7]
    2: [3, 6]
    3: [3, 7]
    4: [5, 1]
    5: [5, 4]
    6: [1, 6]
    7: [1, 7]
    8: [6, 4]
    9: [4, 7]

## Time Complexity

Worst-case time-complexity analysis is [here](http://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm#Worst-case_analysis).

## License

[BSD License](http://opensource.org/licenses/BSD-3-Clause)
