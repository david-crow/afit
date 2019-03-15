
% Structure Definitions
%
% piece (used as piecelist)
%   name: 2 character string
%   move_direction: 'NORTH_SOUTH' or 'EAST_WEST'
%   x: integer
%   y: integer 
%   dx: integer (x size)
%   dy: integer (y size)
%
% move (used as movelist)
%   direction: 'NORTH','SOUTH','EAST', or 'WEST' (direction moved)
%   piece_index: integer (index of the piece moved in the piecelist)
%   spaces: integer (number of spaces moved)

% result
%   piece_index: index to piece array (i.e. which piece moves)
%   direction: the direction the piece is moved
%   spaces: the number of spaces the piece is moved.

% This is where your search function goes
% call genMoves to get the movelist, makeMove to change the state, and 
% reverseMoves to undo the makeMove state change.
% The function can be recursive so I set the inputs and outputs to be 
% similar - also a MATLAB quirk requires the structure to be passed in 
% prior to being passed out (result)
% @param result: the result structure (defined above - constructed in
% RushHour)
% @param result_index: the current number of moves on the result list
% @param picelist: the list of pieces (defined above)
% @param theBoard: the matrix containing the board layout information
% @return result: the result structure (defined above)
% @return result_index: the number of moves on the result list (result)
% @return nodes_visited: a count of the number of nodes the algorithm
% visited
function [result, result_index, nodes_visited] = search(result,result_index,piecelist,theBoard)
  nodes_visited = 0;
 
return;