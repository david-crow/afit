% Reverse the last move made
function [result_index,piecelist,theBoard] = reverseMove(m, result_index, piecelist, theBoard)
  
  % First remove the piece from theBoard in its current location.
  x = piecelist(m.piece_index).x;
  y = piecelist(m.piece_index).y;
  for i = 0:(piecelist(m.piece_index).dx-1)
    for j = 0:(piecelist(m.piece_index).dy-1)
        theBoard(x+i,y+j) = -1;
    end
  end
  
  if strcmp(m.direction,'NORTH') == 1
      piecelist(m.piece_index).x = piecelist(m.piece_index).x + m.spaces;
  end
  if strcmp(m.direction,'SOUTH') == 1
      piecelist(m.piece_index).x = piecelist(m.piece_index).x - m.spaces;
  end
  if strcmp(m.direction,'EAST') == 1
      piecelist(m.piece_index).y = piecelist(m.piece_index).y - m.spaces;
  end
  if strcmp(m.direction,'WEST') == 1
      piecelist(m.piece_index).y = piecelist(m.piece_index).y + m.spaces;
  end
  
  % Now place the piece in theBoard in it's new location.
  x = piecelist(m.piece_index).x;
  y = piecelist(m.piece_index).y;
  for i = 0:(piecelist(m.piece_index).dx-1)
    for j = 0:(piecelist(m.piece_index).dy-1)
        theBoard(x+i,y+j) = m.piece_index;
    end
  end
  
  result_index = result_index - 1;
  % Returning the moves taken (result), the current index into result (result_index), theBoard, piecelist, and the list of moves.
return
