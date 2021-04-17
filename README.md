## Project description.

Implement simple match 3 game.

Reference games Example: Homescapes, Garedenscapes in AppStore, PlayStore.

### Main tasks:

**1.  Game Config. Basic gameplay. Dropping logic. Objectives.** <br/>
**Game Config.** <br/>
Game should have a configuration JSON file providing these config values. <br/>
1. Board row/column size. (min 7, max 10)
2. Moves count.
3. Objectives count (max 3) their colors and values. i.e. 10 red, 12 green, 20 blue.
4. Figures colors count. (min 3, max 5)

**Basic gameplay.** <br/>

***Board generation.*** <br/>
Board should be generated based on config rules (See Game config) <br/>
***Swap logic.*** <br/>
It is possible to swap any two neighbour figures. <br/>
Click on figure1 then clicking on figure2 swaps figure1 and figure2. <br/>
***Matches.*** <br/>
Match is a predefined combination of same color figures. <br/>
In the current task we define a match as a group of more than two figures with same color positioned on same line horizontal or vertical. <br/>
If after swap there is a match, swap is considered as successful and matched figures are removed from board. <br/>
If the swap didn't result a match, the figures are swaping back to their initial cells. <br/>
***Dropping logic.*** <br/>
When matched figures are cleared from board top figures are dropping into freed cells. <br/>
When dropping is finished, the game should check if the dropping resulted the creation of a new match and remove those matched figures from the board. <br/>

**Objectives** <br/>

Every successful match decreases moves count. <br/>
Game has objectives. Objectives are defined in the game config section. <br/>
After successful match, if the collected figures are in the list of objectives, the objectives number should go down by the number of collected figures. <br/>
If all objectives are completed (counter of objective is 0), the game is considered complete and a "Game Complete" popup should be shown. <br/>
If the player is out of moves before completing all game objectives, a "Game Failed" popup should appear. <br/>

**2. Boosters. Radial. Horizontal. Vertical. Affecting (detonating) each other.** <br/>
Boosters are special type of figures that are generated when specific matches are made. <br/> 
***Boosters generation:*** <br/>
*Radial.* <br/>
Radial booster (bomb) is generated when the player forms a square match, consisting of 4 same color figures. <br/>
*Horizontal line bomb.*  <br/>
This booster is generated when the player forms a vertical line match, consisting of 4 same color figures. <br/>
*Vertical line bomb*. <br/>
This booster is generated when the player forms a horizontal line match, consisting of 4 same color figures. <br/>
***Boosters activation:*** <br/>
All boosters are activated when the player taps on them. <br/>
Tap on a bomb is considered a move.  <br/>
Two boosters swapped with each other is not a successful match and booster figures are swapped back to their cells. If a match is not successful, a move is not subtracted. <br/>
***Boosters effect:*** <br/>
*Radial.* <br/>
When tapped on a Radial booster, it clears itself and all adjacent figures up/down left/right and 4 diagonals. <br/>
*Horizontal line bomb.* <br/>
When tapped on, it clears itself and the whole horizontal line it was on. <br/>
*Vertical line bomb.* <br/>
When tapped on, it clears itself and the whole vertical line it was on. <br/>
If the booster effect affects another booster, the affected booster should detonate the same way as if it has been tapped on. <br/>
All figures that have been cleared from any of the boosters effects, and that are in the objectives list, should decrease objective counts. <br/>


### Bonus tasks:

**1. Matching pattern.** <br/>
Instead of simple matching model defined in main task it should be possible to define a list of matching patterns. <br/>
For example pattern could be defined as T like shape. After swap game should search for all defined patterns and clear them if found. <br/>

T like shape: <br/>
```
    ooo
     o
     o
```

**2. Shuffle if there are no moves.** <br/>
The alignment of figures on the board can result a situation that no match can be made. At that point existing board figures should be shuffled. The board should be shuffled as many times, as necessary to create a possibility to match.

### Technical specification. 
Game should be written in C++ language using SFML framework. <br/>
Initial sources can be found in this repository. <br/>
Project can be built using Cmake tool.  <br/>

All required graphics are provided in repository. <br/>

#### Match.

Simple match <br/>
![alt text](https://github.com/Playrix-AM/DevTestGame/blob/master/doc/resources/simple_match.jpg)

Match 4. <br>
![alt text](https://github.com/Playrix-AM/DevTestGame/blob/master/doc/resources/horizontal_4_match_wo_bomb.jpg)

Square match <br/>
![alt text](https://github.com/Playrix-AM/DevTestGame/blob/master/doc/resources/square_match_wo_bomb.jpg)

#### Boosters <br/>

Vertical bomb <br/>
![alt text](https://github.com/Playrix-AM/DevTestGame/blob/master/doc/resources/horizontal_4_match.jpg)

Horizontal bomb <br/>
![alt text](https://github.com/Playrix-AM/DevTestGame/blob/master/doc/resources/vertical_4_match.jpg)

Radial bomb <br/>
![alt text](https://github.com/Playrix-AM/DevTestGame/blob/master/doc/resources/square_match.jpg)


![alt text](https://github.com/Playrix-AM/DevTestGame/blob/master/doc/resources/preview.jpg)
