# README

## What is this repository for?

This repository features the PRS project.

## What is this directory for?

This directory features the `maputil` utility program.

## Quick summary

The `maputil` utility program is capable of modifying map archives.

This utility program allows to:
  
 - Get the width of a map;
 - Get the height of a map;
 - Get the number of tiles used in a map;
 - Set the width of a map;
 - Set the height of a map;
 - Replace the tiles of a map;
 - Remove unused tiles.

## Prerequisites

Install the following prerequisites:

 - [GNU GCC](https://gcc.gnu.org/);
 - [GNU GDB](https://www.sourceware.org/gdb/);
 - [GNU Make](https://www.gnu.org/software/make/).
 
```
sudo apt-get install gcc gdb make
```

Install the following prerequisites in order to modify 
the `doxygen` documentation:

 - [Doxygen](https://doxygen.nl/);
 - [Graphviz](https://graphviz.org/).

```
sudo apt-get install doxygen graphviz
```

## How do I get setup?

### Build the program

Run the following command:

```
make
```

### Run the utility program

Run the following command:

```
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

### Examples of uses

Examples of uses are listed below:

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

### Consult the documentation of the project

Navigate to the `doc` directory located at the root of this project and 
open the `index.html` file.

