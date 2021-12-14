# City Final Assigment

## Ben Boyle 117501 - CS 4350 Fall 2021

## Building and Running

Build the project using CMake and run the `city` executable.

Occaisionally segfaults on initialisation; keep trying and it will start.

## Keyboard Controls

| Key(s) | Function                                                      |
|--------|:--------------------------------------------------------------|
| r      | generate new city and switch to it                            |
| space  | cycle through generated cities                                |
| wasd   | first person camera controls                                  |
| mouse  | look around with first person camera                          |
| scroll | change camera FOV                                             |
| shift  | 'sprint' (temporarily increase camera movement speed and FOV) |
| `      | toggle debug menus                                            |
| ctrl   | use camera while debug menus are open                         |

## Debug Menu

Things to try in the debug menu:

* Change the variables that dictate how the city grid is generated so when you press 'r' new cities will have those properties

* Select an individual building and change its dimensions and position

* Change the 'downtown' point so future cities will have its talles buildings concentrated somewhere other than (0,0, 0).

* Change the Citadel scale and animation function parameters
