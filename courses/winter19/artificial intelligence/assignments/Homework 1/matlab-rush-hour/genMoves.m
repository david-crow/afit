% Generate the list of possible moves for the current search node
% @param piecelist: the list of pieces
% @param theBoard: the current board representation
% @return movelist: the list of moves
function [movelist] = genMoves(piecelist,theBoard)

  BOARD_EXIT_X = 3; % constant for exit placement
  BOARD_EXIT_Y = 6; % constant for exit placement
  move_count = 1;

  % For each piece on the board
  [a,piece_count] = size(piecelist);
  BOARD_SIZE = size(theBoard);
  for i = 1:piece_count
    % If the piece can move North and South (Up/Down)
    if strcmp(piecelist(i).move_direction,'NORTH_SOUTH') == 1
      for j = 1:BOARD_SIZE
        % Can we move North 'j' squares?
        % First do a bounds check
        if (piecelist(i).x - j) < 1
          break;
        end
        % Then check for colliding pieces
        if theBoard((piecelist(i).x - j),piecelist(i).y) == -1
          % Add this move to movelist
          movelist(move_count) = struct('direction','NORTH','piece_index',i,'spaces',j);
          move_count = move_count + 1;
        else
            break;
        end
     end
     for j = 1:BOARD_SIZE
        % Can we move South 'j' squares?
        % First do a bounds check
        if (piecelist(i).x + piecelist(i).dx + j - 1) > BOARD_SIZE
          break;
        end
        % Then check for colliding pieces
       if theBoard((piecelist(i).x + piecelist(i).dx + j -1),piecelist(i).y) == -1
          % Add this move to movelist
          movelist(move_count) = struct('direction','SOUTH','piece_index',i,'spaces',j);
          move_count = move_count + 1;
        else
            break;
        end
      end
    end
    % If the piece can move East and West (Left/Right)
    if strcmp(piecelist(i).move_direction,'EAST_WEST') == 1
      for j = 1:BOARD_SIZE
        % Can we move East 'j' squares?
        % First do a bounds check
        if (piecelist(i).y + piecelist(i).dy + j - 1) > BOARD_SIZE
          break;
        end
        % Then check for colliding pieces
       if theBoard(piecelist(i).x,(piecelist(i).y + piecelist(i).dy + j -1)) == -1
          % Add this move to movelist
          movelist(move_count) = struct('direction','EAST','piece_index',i,'spaces',j);
          move_count = move_count + 1;
        else
            break;
       end
      end
%       if strcmp(piecelist(i).name,'X0') == 1 && (piecelist(i).y + piecelist(i).dy) < BOARD_EXIT_Y && piecelist(i).x == BOARD_EXIT_X
%         if theBoard(piecelist(i).x,(piecelist(i).y + piecelist(i).dy + j - 1)) == -1 && theBoard((piecelist(i).x + piecelist(i).dx - 1),(piecelist(i).y + piecelist(i).dy + j - 1)) == -1
%           movelist(move_count) = struct('direction','EAST','piece_index',i,'spaces',j);
%           move_count = move_count + 1;
%         end
%       end
      for j = 1:BOARD_SIZE
        % Can we move West 'j' squares?
        % First do a bounds check
        if (piecelist(i).y - j) < 1
          break;
        end
        % Then check for colliding pieces
        if theBoard(piecelist(i).x,(piecelist(i).y - j)) == -1
          movelist(move_count) = struct('direction','WEST','piece_index',i,'spaces',j);
          move_count = move_count + 1;
        else
           break;
        end
      end
    end
  end
% Return list of possible moves from this node
return;