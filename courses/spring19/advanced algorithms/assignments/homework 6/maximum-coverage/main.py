from data import problems
from copy import deepcopy
from itertools import combinations

# change this to try a new problem instance
# instance = problems[0]
#
# size = 0
# drones = 0
# battery = 0


class MaxCoverage:
    def __init__(self, instance):
        self.size = instance["size"]
        self.drones = instance["drones"]
        self.battery = instance["battery"]

    # pretty-print a 2D world
    @staticmethod
    def show_world(world):
        for row in world:
            for i in row:
                print("x" if i == 1 else "-", end="\t")
            print()
        print()

    # print all path worlds side-by-side
    def show_paths(self, paths):
        path_worlds = []

        # build the path worlds
        for path in paths:
            pw = self.init_world()

            for i in range(len(path)):
                self.cover(pw, path[i][0], path[i][1], i + 1)

            path_worlds.append(pw)

        # print the i-th row of every path world (with separators)
        for i in range(self.size):
            for pw in path_worlds:
                for j in pw[i]:
                    print(j, end="\t")
                if pw != path_worlds[-1]:
                    print("|", end="\t")
            print()

        print()

    # pretty print a path world
    def show_path(self, path):
        path_world = self.init_world()

        # build the path world
        for i in range(len(path)):
            self.cover(path_world, path[i][0], path[i][1], i + 1)

        self.show_world(path_world)

    # build a size * size world of all zeroes
    def init_world(self):
        world = []
        for i in range(self.size):
            world.append([])
            for j in range(self.size):
                world[i].append(0)

        return world

    # give every drone an empty path
    def init_paths(self):
        paths = []
        for i in range(self.drones):
            paths.append([])

        return paths

    # return the world value of (x, y)
    def at(self, world, x, y):
        return world[self.size - y - 1][x]

    # give (x, y) a value (usually 1)
    def cover(self, world, x, y, value=1):
        world[self.size - y - 1][x] = value

    # returns the total number of covered squares
    def total_covered(self, world):
        total = 0

        for i in range(self.size):
            for j in range(self.size):
                total += self.at(world, i, j)

        return total

    # returns the total number of covered edges
    def edges_covered(self, world):
        total = 0

        # count the edges
        for i in range(self.size):
            total += self.at(world, i, 0)
            total += self.at(world, 0, i)
            total += self.at(world, i, self.size - 1)
            total += self.at(world, self.size - 1, i)

        # remove duplicates (corners)
        for i in (0, self.size - 1):
            for j in (0, self.size - 1):
                total -= self.at(world, i, j)

        return total

    # returns a set of all edges
    def edges(self):
        edge_set = []

        # add all non-corner edges to edge_set
        for i in range(self.size - 2):
            edge_set.append((i + 1, 0))
            edge_set.append((0, i + 1))
            edge_set.append((i + 1, self.size - 1))
            edge_set.append((self.size - 1, i + 1))

        # add all corners to edge_set
        for i in (0, self.size - 1):
            for j in (0, self.size - 1):
                edge_set.append((i, j))

        return edge_set

    # returns the (up to) eight squares adjacent to (x, y)
    def adjacent(self, x, y):
        possible_x = [x]
        possible_y = [y]

        neighbors = []

        # checks whether we're on an edge
        if x < self.size - 1:
            possible_x.append(x + 1)
        if x > 0:
            possible_x.append(x - 1)
        if y < self.size - 1:
            possible_y.append(y + 1)
        if y > 0:
            possible_y.append(y - 1)

        for i in possible_x:
            for j in possible_y:
                neighbors.append((i, j))

        # the current square is not a neighbor of the current square
        neighbors.remove((x, y))
        return neighbors

    # returns the edge squares adjacent to (x, y)
    def adjacent_edges(self, x, y):
        neighbors = self.adjacent(x, y)
        edge_set = deepcopy(neighbors)

        # if it's not an edge cell, remove it
        for n in neighbors:
            if (n[0], n[1]) not in self.edges():
                edge_set.remove(n)

        return edge_set

    # estimate the value of a given world
    def value(self, world, paths, show_details=False):
        # we want to cover as many squares as possible
        total_ratio = self.total_covered(world) / float(self.size * self.size)

        # we should leave the edges open for later exit
        edge_ratio = 1 - (self.edges_covered(world) / float(4 * self.size - 4))

        # count the number of repeat visits to any given square
        same_repeats = 0
        other_repeats = 0
        for path in paths:
            # find every visited square for all other paths
            all_locations = []

            for p in paths:
                if p is not path:
                    all_locations += p

            # if the current path visits one of those squares, +1 repeat
            for p in path:
                if p in all_locations:
                    other_repeats += 1

            for i in range(len(path)):
                for j in range(i + 1, len(path)):
                    if path[i] == path[j]:
                        same_repeats += 1

        # these are double counted in the loop above
        other_repeats = int(other_repeats / 2.0)

        # apply weights to the various metrics
        value = 0.90 * total_ratio + 0.10 * edge_ratio - 0.25 * other_repeats - 0.25 * same_repeats

        if show_details:
            print("Total ratio   :", total_ratio)
            print("Edge ratio    :", edge_ratio)
            print("Other repeats :", other_repeats)
            print("Same repeats  :", same_repeats)
            print("State value   :", value)

        return value

    # best-first search
    def a_star(self, w, p):
        frontier = []
        explored = []

        # add every starting configuration to the frontier
        configurations = combinations(self.edges(), self.drones)
        for config in configurations:
            new_world = deepcopy(w)
            new_paths = deepcopy(p)

            # start the drones in their designated spots
            for i in range(self.drones):
                self.cover(new_world, config[i][0], config[i][1])
                new_paths[i].append(config[i])

            frontier.append((self.value(new_world, new_paths), new_world, new_paths))

        # while we still have states to search
        while len(frontier) > 0:
            # pull the next state from the frontier
            frontier.sort(reverse=True)
            _, world, paths = frontier.pop(0)
            explored.append(world)

            # if the world is covered and every drone is on an edge square
            goal_found = True
            if self.total_covered(world) == self.size * self.size:
                for path in paths:
                    current = path[-1]
                    if current[0] != 0 and current[0] != self.size - 1 and current[1] != 0 and current[1] != self.size - 1:
                        goal_found = False

            # otherwise, if every drone has depleted its battery
            else:
                for path in paths:
                    # if at least one drone has remaining battery capacity
                    if len(path) < self.battery:
                        goal_found = False
                        break

            if goal_found:
                return world, paths

            # for every drone...
            for i in range(self.drones):
                # drone's current location
                current = paths[i][-1]

                # find the adjacent nodes (consider strictly edge nodes if the battery is nearly depleted)
                neighbors = None
                if len(paths[i]) == self.battery - 1:
                    neighbors = self.adjacent_edges(current[0], current[1])
                elif len(paths[i]) < self.battery - 1:
                    neighbors = self.adjacent(current[0], current[1])

                # if the drone can move
                if neighbors is not None:
                    # move it to every possible adjacent cell; add this world to the frontier
                    for neighbor in neighbors:
                        new_world = deepcopy(world)
                        self.cover(new_world, neighbor[0], neighbor[1])

                        new_paths = deepcopy(paths)
                        new_paths[i].append(neighbor)

                        frontier.append((self.value(new_world, new_paths) - 1, new_world, new_paths))


def main():
    for i, problem in enumerate(problems):
        print("PROBLEM INSTANCE {}:\t".format(i + 1))
        for key in problem:
            print("\t{} -\t{}".format(problem[key], key))

        print()
        solver = MaxCoverage(problem)
        world, paths = solver.a_star(solver.init_world(), solver.init_paths())

        print("World\n-----")
        solver.show_world(world)

        print("Paths\n-----")
        solver.show_paths(paths)

        print("Value\n-----")
        solver.value(world, paths, show_details=True)

        if i != len(problems) - 1:
            print("\n")


main()