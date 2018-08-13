# Knight General
## Turn-based strategy for Ludum Dare 42. Theme: Running out of space

## Description

Knight General is inspired by classic turn-based strategy games. The aim of the game is to control the whole map (so that the enemy 'runs out of space'). To do so, a player can move their units into enemy territory, thus increasing their possessions. Each player can buy more units, all of which can fight the enemies and collect resources. Units are bought with coins, which each player collects by:
- gathering resources
- receiving a payout at the beginning of each turn, which depends on the size of owned land

### Features

- two powerful sides: knights and skeletons
- each side has three different units
- difficult AI opponent
- several types of resources (gold, stone, timber, trees) which can be collected and sold to receive coins

## Dependencies

- SFML (tested with v2.4.2)

## Building from source

Install dependencies on Ubuntu:

`sudo apt install libsfml-dev`

```
1. Clone the repo
2. mkdir build && cp -r assets/ build/ && cd build
3. cmake ..
4. make -j
5. ./knight-general
```

## Assets

### Graphics

- https://opengameart.org/content/medieval-rts-120 by Kenney (public domain)

- https://opengameart.org/content/lpc-medieval-fantasy-character-sprites by wulax (CC-BY-SA 3.0 GPL 3.0)

- https://opengameart.org/content/ui-pack-rpg-extension by Kenney (public domain)

- https://opengameart.org/content/crosshair-pack-200%C3%97 by Kenney (public domain)

### Music

- https://opengameart.org/content/adventure-begins by bart (GPL 3.0**

### Fonts

- https://www.dafont.com/caviar-dreams.font by Lauren Thompson (100% free)

## Version info

`./knight-general --version`

## Screenshots

13/08/2018 20:42 BST - almost final version
![](almost_final_version.png "Almost final version")

12/08/2018 20:27 BST - basic gui
![](basic_gui.png "Basic GUI")

12/08/2018 11:50 BST - two units
![](two_units.png "Two units")

11/08/2018 18:05 BST - unit on a map
![](unit_on_a_map.png "Unit on a map")

11/08/2018 14:56 BST - basic map
![](basic_map.png "Basic map")



