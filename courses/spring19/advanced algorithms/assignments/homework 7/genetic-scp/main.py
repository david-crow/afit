from data import problems
from random import randint, uniform
from timeit import default_timer as timer


# return the vertices in a problem instance
def vertex_set(p):
    vertices = []
    for i in range(len(p)):
        vertices.append(i)

    return vertices


# return the edges in a problem instance
def edge_set(p):
    edges = []

    for i, neighbors in enumerate(p):
        for n in neighbors:
            edges.append((i, n))

    return edges


# return a full covering set and an empty covering set
def init_parents(p):
    p1 = ""
    p2 = ""

    # build the two strings
    for i in range(len(p)):
        p1 += "0"
        p2 += "1"

    return p1, p2


# return the set covered by some individual
def covered(string, edges):
    c = []

    # if s is in the covering set
    for i, s in enumerate(string):
        if s == "1":
            c.append(i)

            # check for every edge leaving s
            for e in edges:
                if e[0] == i:
                    c.append(e[1])

    # we only want unique values
    return list(set(c))


# return the covering set represented by some individual
def covering_set(solution):
    c = []
    for i, s in enumerate(solution):
        if s == "1":
            c.append(i)

    return c


# return the size of a covering set
def size(s):
    set_size = 0

    for i in range(len(s)):
        set_size += int(s[i])

    return set_size


# select two parents from a population
def select_parents(population, vertices, edges):
    # primary goal: cover the set
    # secondary goal: minimize the size of the covering set
    choices = {p: len(covered(p, edges)) + (len(vertices) ** -1) * (len(vertices) - size(p)) for p in population}

    p1 = None

    maximum = sum(choices.values())
    pick = uniform(0, maximum)
    current = 0

    # pick a random element in the population by weight
    for key, value in choices.items():
        current += value
        if current > pick:
            p1 = key
            break

    # ensure p1 ≠ p2
    del choices[p1]
    p2 = None

    maximum = sum(choices.values())
    pick = uniform(0, maximum)
    current = 0

    # pick a second random element in the population by weight
    for key, value in choices.items():
        current += value
        if current > pick:
            p2 = key
            break

    return p1, p2


# randomly 1-fold two parents
def generate_successor(p1, p2):
    r = randint(1, len(p1) - 1)
    return p1[:r] + p2[:-r]


# build a population
def genetic(p1, p2, vertices, edges):
    population = [p1, p2]

    # ... of an arbitrary size
    for i in range(len(vertices) ** 2):
        new_p1, new_p2 = select_parents(population, vertices, edges)
        population.append(generate_successor(new_p1, new_p2))

    # filter out duplicate values
    population = list(set(population))

    # sort the population by covered set size and then by covering set size
    return sorted(population, key=lambda e: (len(vertices) - len(covered(e, edges)), size(e)))[0]


def main():
    problem = problems[3]

    vertices = vertex_set(problem)
    edges = edge_set(problem)
    p1, p2 = init_parents(problem)

    start = timer()
    solution = genetic(p1, p2, vertices, edges)
    end = timer()

    print(
        "Covering set :", covering_set(solution),
        "\nSet size     :", len(covering_set(solution)),
        "\nCovered      :", len(covered(solution, edges)),
        "\nExec. time   :", end - start
    )


main()
