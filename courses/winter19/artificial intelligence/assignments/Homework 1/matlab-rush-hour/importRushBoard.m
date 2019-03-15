% This function takes the raw data from the flat file and turns it into the
% data structures used for searching
% @param newBoard: [pointer to the board object for reading in
% @param BOARD_SIZE: the size of the board
% @return [piecelist, theBoard]: the piecelist array and the loaded problem
function [piecelist,theBoard] = importRushBoard(newBoard, BOARD_SIZE)
  piece_count = 0;
  for x=1:BOARD_SIZE
    for y=1:BOARD_SIZE
      row = y;
      col = x;
      string = char(newBoard(x,y));
      % Check to make sure that there is a piece
      if string(1) == '.'
          theBoard(col,row) = -1;
          continue;
      end
      % Check to make sure we haven't already created this piece
      if piece_count > 0
        index = findPiece(string,piecelist);
        if index > 0
          theBoard(col,row) = index;
          continue;
        end
      end
      % Add the new piece   
      piece_count = piece_count+1;
      % This is a new 1x3 (Truck)
      if string(1) >= 'O' && string(1) <= 'R'
        % Which direction is this object facing?
        if  (x + 1) <= BOARD_SIZE
          string2 = char(newBoard(x+1,y));
          if string2(1) == string(1) && string2(2) == string(2)
            %A Piece name, move_direction, location (x,y), size (dx,dy)
            piecelist(piece_count)= struct('name',string,'move_direction','NORTH_SOUTH','x',col,'y',row,'dx',3,'dy',1);
          end
        end
        if y+1 <= BOARD_SIZE
          string2 = char(newBoard(x,y+1));
          if string2(1) == string(1) && string2(2) == string(2)
            %A Piece name, move_direction, location (x,y), size (dx,dy)
            piecelist(piece_count)= struct('name',string,'move_direction','EAST_WEST','x',col,'y',row,'dx',1,'dy',3);
          end
        end
        theBoard(col,row) = piece_count;
      end
      % This is a new 1x2 (Car)
      if string(1) >= 'A' && string(1) <= 'K'
        % Which direction is this object facing?
        if  (x + 1) <= BOARD_SIZE
          string2 = char(newBoard(x+1,y));
          if string2(1) == string(1) && string2(2) == string(2)
            %A Piece name, move_direction, location (x,y), size (dx,dy)
            piecelist(piece_count)= struct('name',string,'move_direction','NORTH_SOUTH','x',col,'y',row,'dx',2,'dy',1);
          end
        end
        if y+1 <= BOARD_SIZE
          string2 = char(newBoard(x,y+1));
          if string2(1) == string(1) && string2(2) == string(2)
            %A Piece name, move_direction, location (x,y), size (dx,dy)
            piecelist(piece_count)= struct('name',string,'move_direction','EAST_WEST','x',col,'y',row,'dx',1,'dy',2);
          end
        end
        theBoard(col,row) = piece_count;
      end
      % This is the Car
      if string(1) >= 'X'
        %A Piece name, move_direction, location (x,y), size (dx,dy)
        piecelist(piece_count)= struct('name',string,'move_direction','EAST_WEST','x',col,'y',row,'dx',1,'dy',2);
        theBoard(col,row) = piece_count;
      end
    end
  end
return;