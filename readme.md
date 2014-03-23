## Game of life

Simple implementation of Conways's »Game of Life«, written in C++ and using QT5.

#### Rules

1. Any live cell with fewer than two live neighbours dies, as if caused by under-population.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overcrowding.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

#### Features

- start, pause and clear game
- randomly populate board with alive cells
- adjustable ratio of alive cells for population
- adjustable speed of iterations
- statistics (number of iterations and cells alive)
- create patterns by drawing on board
- basic zooming of board

#### Todo

- zoom doesn't work in fullscreen mode
- custom board size
- resizable window/board frame
- toggle wrap
- refactor game logic
- save history to forward/backward a few iterations
- prefab patterns
- save/load boards
