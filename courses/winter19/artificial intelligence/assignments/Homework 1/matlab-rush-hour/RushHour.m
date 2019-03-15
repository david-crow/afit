% <p>Title: Rush Hour</p>
% <p>Description: </p>
% <p>Copyright: Copyright (c) 2005</p>
% <p>Company: AFIT</p>
% @author not attributable
% @version 1.0

% The main function for the RushHour solver. 
% Loads the data file, and calls importRushBoard to generate the data
% structures. Then calls the search that you write on each board and
% outputs the solution returned.
function RushHour()

  BOARD_SIZE = 6;

  % open board data file
  [inputFile,message] = fopen('Data.txt','r');
  if inputFile == -1
    disp(message);
    return;
  end

  %get number of boards from first line
  numBoards = fscanf(inputFile,'%d');
  %used for output
  boardCounter = 0;

  %main prog loop
  while (numBoards > 0)
    %increment board counter
    boardCounter = boardCounter+1;

    %nested loop to parse file into an array
    for i = 1:BOARD_SIZE
      for j = 1:BOARD_SIZE
         x = fscanf(inputFile,'%c',2);
         newBoard(i,j) = {x}; % newBoard is a cell array with paired characters
      end
      x = fscanf(inputFile,'%c',2);
    end

    %decrement boards counter
    numBoards = numBoards - 1;

    %delete space between board if needed
    if (numBoards > 0)
      %space between boards
      x = fscanf(inputFile,'%c',2);
    end
    
    %create new workspace
    [piecelist,theBoard] = importRushBoard(newBoard,BOARD_SIZE);

    tic; % get start time
    % HERE: on the following line you need to call your search, sending it 
    % the initial node.
 
    %find moves to get out
    result = struct('direction',{},'piece_index',{},'spaces',{});
    result_index = 1;
    disp('Searching....');
    [result,result_index,nodes_visited] = search(result,result_index,piecelist,theBoard);
 
    t = toc; % get end time
    
    disp(sprintf('\n\nBoard: %d %f seconds',boardCounter,t));
    disp(sprintf('Nodes Visited: %d\n',nodes_visited));

   % Print the move list
   if result_index == 1
     disp('No Path found!');
   else
     for i = 1:(result_index-1)
       disp(sprintf('Move %s: %s %d spaces',piecelist(result(i).piece_index).name, result(i).direction, result(i).spaces));
     end
     disp(sprintf('\n\n'));
   end
  end %end loop

  %close board file
  fclose(inputFile);
return;
        

    
