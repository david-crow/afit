% This function searches the piecelist for the piece which matches by name
% @param name: the name of the piece being looked for
% @param piecelist: the list of pieces
% @return index: the piecelist array index for the found piece (else -1)
function [index] = findPiece(name,piecelist)
  index = -1;
  [a,b] = size(piecelist);
  for i=1:b
    if strcmp(piecelist(i).name, name) == 1
      index = i;
      return;
    end
  end
return;