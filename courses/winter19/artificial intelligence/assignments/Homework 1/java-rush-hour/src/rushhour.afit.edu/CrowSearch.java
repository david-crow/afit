package rushhour.afit.edu;
import java.util.Vector;
import java.util.PriorityQueue;
import java.util.Comparator;

public class CrowSearch implements Search {
  private Vector<Board> explored;
  private PriorityQueue<Board> frontier;

  CrowSearch(Board b) {
    explored = new Vector<>(10);
    frontier = new PriorityQueue<>(10, new Comparator<>() {
      public int compare(Board a, Board b) {
        return Integer.compare(a.estimate, b.estimate);
      }
    });

    frontier.add(new Board(b));
    frontier.peek().estimate();
  }

  // A* Graph Search
  @Override
  public Move findMoves() {
    while (frontier.size() > 0) {
      Board current = frontier.poll();
      explored.add(current);

      Move temp = current.genMoves();

      // for each possible move in this state
      while (temp != null) {
        Board new_board = new Board(current);
        new_board.makeMove(temp);

        if (new_board.isGoal())
          return new_board.move_list;

        new_board.estimate();

        if (!explored.contains(new_board)) {
          if (!frontier.contains(new_board)) {
            // we've never seen this state, so add it to the frontier
            frontier.add(new_board);
          } else {
            for (Board b : frontier) {
              // if this state is already in the frontier, update the estimate when necessary
              if (b.equals(new_board) && b.estimate > new_board.estimate) {
                b.estimate = new_board.estimate;
              }
            }
          }
        }

        temp = temp.next;
      }
    }

    return null;
  }

  @Override
  public long nodeCount() {
    return explored.size();
  }
}
