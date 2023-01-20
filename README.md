# LaserChessEngine
Hi! This program is an engine designed to help you find the best move in any position of ThinkFun's Laser Chess. I have no affiliation with ThinkFun.

## Tech Info
* **IDE** - Visual Studio 2019 (Recommended if you want to clone and work on this repo)
* **Language** - C++14
* **Projects**:
    * **LaserChessEngine** - The library containing all the functionality of the engine
    * **LaserChessEngineExec** - The interface that allows you to use the engine
    * **LaserChessEngineTest** - Test cases I wrote to help ensure the engine works properly (Google Test)

## Board Setup
[This repo](https://github.com/kishannareshpal/laserchess) explains all the rules and notations used, though my engine has some differences:
* The setup position string allows up to 3 '+' characters after any piece, however some pieces may have less than 4 available orientations. In those cases, the '+' may cycle back to the first orientation.
* Piece orientations work as follows:
    * **King** has 1 orientation because all sides are the same
    * **Laser** has 2 orientations: The first one points to the *corresponding side's left*, the second points *away from the player*.
    * **Defender** has 4 orientations. The first one is the defender facing forward (from blue's perspective).
    * **Deflector** has 4 orientations. The first one is where the mirror faces up-right.
    * **Switch** has 2 orientations. The first one is the mirrors facing up-right and down-left.

## Setup Instructions
1. Clone the repo into Visual Studio.
2. Open the solution.
3. Right-click Solution 'LaserChessEngine' > Properties > Startup Project > Set "Single startup project" to LaserChessEngineExec.
4. Right-click LaserChessEngine project > Properties > General > Set "Configuration Type" to static library (.lib).
5. LaserChessEngineExec & LaserChessEngineTest should be "Application (.exe)"
6. Right-click on any project > Configuration Manager > Set "Active solution configuration" to "Release" (performance reasons) and "Active solution platform" to "x86".

## How to Use
1. Supply arguments to LaserChessEngineExec through "Properties" > "Debugging" > Command Arguments.
	* The first argument is a string representing the current position of the board. The program will verify that this is a valid position for a standard
	Laser Chess board.
	* The second argument is the current player's turn. It should be "BLUE" or "RED".
2. Debug > Start Without Debugging.
3. Choose whether you want to input your own move (PLAY), or let the engine suggest a move (AI).
4. Follow the instructions for either option.
5. You can also run the tests through the test explorer in VS.

## Extras
* For performance reasons, the engine will sometimes limit the search. The depth you supply is simply the maximum evaluation depth. I would suggest using a depth of 3 when there are a lot of pieces on the board, or you 'll be waiting quite a while for each move.
* There are some features I decided not to implement, though it'd be cool if someone did:
    * Draw conditions
    * Making <code>findBestMove</code> have a time parameter as well as a depth parameter 
    * Checkmate In X moves indicator from <code>findBestMove</code>
    * Any advanced search pruning methods that don't involve outright ignoring moves or capping search depth
    * More advanced position evaluations. I was worried that making the evaluation function more complex would add to the search runtime
* Feel free to make pull requests if you have any improvements you'd like to make.
