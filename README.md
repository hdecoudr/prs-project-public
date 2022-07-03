# README

## What is this repository for?

This repository features the PRS project.

## Quick summary

The goal of this project is to develop some basic mechanisms to be
used in the development of a small 2D platform game. 

The project is be divided into several distinct parts. The first 
one is about the implementation of a saving and loading mechanism 
for the map used in the game. The second one aims to develop a small 
program, `maputi`, that can modify files containing a map. The last 
one consists in implementing an event manager that will allow the 
game to plan the moments at which an event need to be to triggered. 

This project requires the use of `Linux` system calls as much as
possible.

The main game logic is provided in the form of the static library
`libgame.a` which also contains the entry point of the program.
No more information is supplied about this file.

## Prequisites

Install the following prerequisites:

 - [GNU GCC](https://gcc.gnu.org/);
 - [GNU GDB](https://www.sourceware.org/gdb/);
 - [GNU Make](https://www.gnu.org/software/make/);
 - [SDL2](https://www.libsdl.org/);
 - [SDL2 Image](https://www.libsdl.org/projects/SDL_image/);
 - [SDL2 Mixer](https://www.libsdl.org/projects/SDL_mixer/).

```
sudo apt-get install gcc gdb make libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev
```

Additionally, install the following prerequisites in order to
reverse engineer the `libgame.a`:

 - [IDA Pro](https://www.hex-rays.com/ida-pro/) (not free);
 - [Ghidra](https://github.com/NationalSecurityAgency/ghidra).

Using `objdump` is also a possibility.

Next, install the following prerequisites in order to modify
the `maputil` parsers:

 - [Gengetopt](https://www.gnu.org/software/gengetopt/gengetopt.html).

```
sudo apt-get install gengetopt
```

Finally, install the following prerequisites in order to modify 
the `doxygen` documentation:

 - [Doxygen](https://doxygen.nl/);
 - [Graphviz](https://graphviz.org/).

```
sudo apt-get install doxygen graphviz
```

## How do I get setup?

### Build the project

#### Build the game

Run the following command:

```
make
```

#### Build the utility program

Run the following command:

```
cd util/ && make
```

### Build the documentation of the exercises

Navigate to the [doxygen](./doxygen) directory and invoke `doxygen`:

```
cd doxygen && doxygen doxygen.conf
```

### Run the project

#### Run the game

Run the game with the following command in order to use a predefined
map:

```
./game
```

Use the `-l` option to load a saved map:

```
./game -l maps/saved.map
```

See the table below to know how to use the game:

| Key      | Descrption                        |
|:--------:|:----------------------------------|
| `UP`     | Jump                              |
| `LEFT`   | Move the character to the left    |
| `RIGHT`  | Move the character to the right   |
| `DOWN`   | Plant a bomb                      |
| `SPACE`  | Launch a missile                  |
| `t`      | Place a mine                      |
| `e`      | Enter/Leave the level editor mode |
| `s`      | Save the current map              |
| `ESCAPE` | Leave the game                    |

The following input are applicable to the level editor mode:

| Key      | Descrption                       |
|:--------:|:---------------------------------|
| `UP`     | Move a tile up                   |
| `LEFT`   | Move a tile to the left          |
| `RIGHT`  | Move a tile to the right         |
| `DOWN`   | Move a tile down                 |
| `SPACE`  | Place a tile                     |
| `TAB`    | Select a tile                    |
| `ESCAPE` | Leave the level editor mode      |
| `e`      | Leave the level editor mode      |

Saved maps are located in the `maps` directory.

#### Run the utility program

Run the following commands:

```
cd util/
./maputil [OPTIONS...]
```

See the table below for a complete overview of the 
program options:

| Long options     | Short options | Mandatory           | Parameters | Description                                                  | 
|:----------------:|:-------------:|:-------------------:|:-----------|:-------------------------------------------------------------|
| `--help`         | `None`        | `No`                | `None`     | Displays the usage of the program.                           |
| `--version`      | `-V`          | `No`                | `None`     | Displays the version of the program.                         |
| `--file`         | `-f`          | `Yes`               | `String`   | Map archive.                                                 |
| `--getwidth`     | `-w`          | `No`                | `None`     | Gets the width of a map.                                     |
| `--getheight`    | `-h`          | `No`                | `None`     | Gets the height of a map.                                    |
| `--getobjetcs`   | `-o`          | `No`                | `None`     | Gets the number of tiles of a map.                           |
| `--getinfo`      | `-i`          | `No`                | `None`     | Gets the width, the height and the number of tiles of a map. |
| `--setwidth`     | `-W`          | `No`                | `Integer`  | Sets the width of a map.                                     |
| `--setheight`    | `-H`          | `No`                | `Integer`  | sets the height of a map.                                    |
| `--setobjects`   | `-O`          | See the table below | `Sring`    | Replaces the tile of a map.                                  |
| `--pruneobjects` | `-p`          | `No`                | `None`     | Remove unused tiles from a map.                              |

The `--setobjects` option accepts a string where the following parameters are madatory:

| Long options     | Short options | Mandatory           | Parameters                                   | Description                       | 
|:----------------:|:-------------:|:-------------------:|:---------------------------------------------|:----------------------------------| 
| `--path`         | `-p`          | `Yes`               | `String`                                     | The path to a tile.               |
| `--frames`       | `-f`          | `Yes`               | `Integer`                                    | The number of frames of the tile. |
| `--solidity`     | `-s`          | `Yes`               | `Enumeration {solid, semi-solid, air}`       | Solidity property of the tile.    |
| `--destructible` | `-d`          | `Yes`               | `Enumeration {destructile, not-destrucible}` | Destructible property of the tile.|
| `--collectible`  | `-c`          | `Yes`               | `Enumeration {collectible, not-collectible}` | Collectible property of the tile. |
| `--generator`    | `-g`          | `Yes`               | `Enumeration {generator, not-generator}`     | Generator property of the tile.   |

 - Gets the width of a map:
  
```
./maputil -f ../maps/saved.map -w
```

 - Gets the height of a map:

```
./maputil -f ../maps/saved.map -h
```

 - Gets the number of tiles of a map:
 
```
./maputil -f ../maps/saved.map -o
```

 - Gets all the information listed above:

```
./maputil -f ../maps/saved.map -i
```

 - Sets the width of a map:

```
./maputil -f ../maps/saved.map -W 40
```

 - Sets the height of a map:

```
./maputil -f ../maps/saved.map -H 16
```

 - Sets the objects of a map:

```
./maputil -f ../maps/saved.map -O "-p images/marble.png 1 -s solid -d not-destructible -c not-collectible -g not-generator" -O "-p images/marble.png 1 -s solid -d not-destructible -c not-collectible -g not-generator" -O "-p images/marble.png 1 -s solid -d not-destructible -c not-collectible -g not-generator"
```

 - Removes unused tile from a map:

```
./maputil -f ../maps/saved.map -p
```

#### Consult the documentation of the project

Navigate to the `doc` directory located at the root of this project and 
open the `index.html` file.

