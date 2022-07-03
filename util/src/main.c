/*!
 * \ingroup util_group
 * \file main.c
 * \brief The `maputil` program allows to manipulates
 *        map archive.
 *
 * This program manipulates map archive generated with
 * the help of the `game` executable. It performs the
 * following operations:
 *
 *  - Displays the width of a map;
 *  - Displays the height of a map;
 *  - Displays the number of tiles of a map;
 *  - Sets the width of a map;
 *  - Sets the height of a map;
 *  - Replaces the tiles of a map;
 *  - Removes unused tiles from a map.
 *
 * Modifying the width of a map will alter its right side. 
 * A larger width expands the map from the right side and
 * adds empty and transparent tiles, while a smaller width
 * removes a part of the right side of the map causing a 
 * loss of tiles. 
 *
 * Modifying the height of a map will alter its top side.
 * A larger height expands the map from the top side and
 * adds empty and transparent tiles, while a smaller height
 * removes a part of the top side of the map causing a loss
 * of tiles.
 *
 * All operations that allow to modify a map result in 
 * the creation of a copy of the map archive under the 
 * form `<filename>-DD-MM-YYYY-hh:mm:ss` where:
 *
 *  - `DD` corresponds to the current day;
 *  - `MM` corresponds to the current month;
 *  - `YYYY` corresponds to the current year;
 *  - `hh` corresponds to the current hour;
 *  - `mm` corresponds to the current minute;
 *  - `ss` corresponds to the current second.
 *
 * The examples below desmonstrate how to use this
 * program:
 *
 *  - Gets the width of a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -w
 * ```
 *
 *  - Gets the height of a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -h
 * ```
 *
 *  - Gets the number of tiles of a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -o
 * ```
 *
 *  - Gets all the information listed above:
 *
 * ```
 * ./maputil -f ../maps/saved.map -i
 * ```
 *
 *  - Sets the width of a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -W 40
 * ```
 *
 *  - Sets the height of a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -H 16
 * ```
 *
 *  - Sets the objects of a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -O "-p images/question.png 17 -s solid -d not-destructible -c not-collectible -g generator"
 * ```
 *
 *  - Removes unused tile from a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -p
 * ```
 *
 * See the table below for a complete overview of the 
 * program options:
 *
 * | Long options     | Short options | Mandatory           | Parameters | Description                                                  |
 * |:----------------:|:-------------:|:-------------------:|:-----------|:-------------------------------------------------------------|
 * | `--help`         | `None`        | `No`                | `None`     | Displays the usage of the program.                           |
 * | `--version`      | `-V`          | `No`                | `None`     | Displays the version of the program.                         |
 * | `--file`         | `-f`          | `Yes`               | `String`   | Map archive.                                                 |
 * | `--getwidth`     | `-w`          | `No`                | `None`     | Gets the width of a map.                                     |
 * | `--getheight`    | `-h`          | `No`                | `None`     | Gets the height of a map.                                    |
 * | `--getobjetcs`   | `-o`          | `No`                | `None`     | Gets the number of tiles of a map.                           |
 * | `--getinfo`      | `-i`          | `No`                | `None`     | Gets the width, the height and the number of tiles of a map. |
 * | `--setwidth`     | `-W`          | `No`                | `Integer`  | Sets the width of a map.                                     |
 * | `--setheight`    | `-H`          | `No`                | `Integer`  | sets the height of a map.                                    |
 * | `--setobjects`   | `-O`          | See the table below | `Sring`    | Replaces the tile of a map.                                  |
 * | `--pruneobjects` | `-p`          | `No`                | `None`     | Remove unused tiles from a map.                              |
 *
 * The `--setobjects` option accepts a string where the following parameters are madatory:
 *
 * | Long options     | Short options | Mandatory           | Parameters                                   | Description                       |
 * |:----------------:|:-------------:|:-------------------:|:---------------------------------------------|:----------------------------------| 
 * | `--path`         | `-p`          | `Yes`               | `String`                                     | The path to a tile.               |
 * | `--frames`       | `-f`          | `Yes`               | `Integer`                                    | The number of frames of the tile. |
 * | `--solidity`     | `-s`          | `Yes`               | `Enumeration {solid, semi-solid, air}`       | Solidity property of the tile.    |
 * | `--destructible` | `-d`          | `Yes`               | `Enumeration {destructile, not-destrucible}` | Destructible property of the tile.|
 * | `--collectible`  | `-c`          | `Yes`               | `Enumeration {collectible, not-collectible}` | Collectible property of the tile. |
 * | `--generator`    | `-g`          | `Yes`               | `Enumeration {generator, not-generator}`     | Generator property of the tile.   |                    
 *
 * \author H.Decoudras
 * \version 1
 */

#include "maputil.h"
#include "error.h"
#include "cmdline.h"
#include "cmdlineobjectproperties.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*!
 *\brief Main entry point of the program.
 *
 * This program manipulates map archive generated with
 * the help of the `game` executable. It performs the
 * following operations:
 *
 *  - Displays the width of a map;
 *  - Displays the height of a map;
 *  - Displays the number of tiles of a map;
 *  - Sets the width of a map;
 *  - Sets the height of a map;
 *  - Replaces the tiles of a map;
 *  - Removes unused tiles from a map.
 *
 * Modifying the width of a map will alter its right side. 
 * A larger width expands the map from the right side and
 * adds empty and transparent tiles, while a smaller width
 * removes a part of the right side of the map causing a 
 * loss of tiles. 
 *
 * Modifying the height of a map will alter its top side.
 * A larger height expands the map from the top side and
 * adds empty and transparent tiles, while a smaller height
 * removes a part of the top side of the map causing a loss
 * of tiles.
 *
 * All operations that allow to modify a map result in 
 * the creation of a copy of the map archive under the 
 * form `<filename>-DD-MM-YYYY-hh:mm:ss` where:
 *
 *  - `DD` corresponds to the current day;
 *  - `MM` corresponds to the current month;
 *  - `YYYY` corresponds to the current year;
 *  - `hh` corresponds to the current hour;
 *  - `mm` corresponds to the current minute;
 *  - `ss` corresponds to the current second.
 *
 * The examples below desmonstrate how to use this
 * program:
 *
 *  - Gets the width of a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -w
 * ```
 *
 *  - Gets the height of a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -h
 * ```
 *
 *  - Gets the number of tiles of a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -o
 * ```
 *
 *  - Gets all the information listed above:
 *
 * ```
 * ./maputil -f ../maps/saved.map -i
 * ```
 *
 *  - Sets the width of a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -W 40
 * ```
 *
 *  - Sets the height of a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -H 16
 * ```
 *
 *  - Sets the objects of a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -O "-p images/question.png 17 -s solid -d not-destructible -c not-collectible -g generator"
 * ```
 *
 *  - Removes unused tile from a map:
 *
 * ```
 * ./maputil -f ../maps/saved.map -p
 * ```
 *
 * See the table below for a complete overview of the 
 * program options:
 *
 * | Long options     | Short options | Mandatory           | Parameters | Description                                                  |
 * |:----------------:|:-------------:|:-------------------:|:-----------|:-------------------------------------------------------------|
 * | `--help`         | `None`        | `No`                | `None`     | Displays the usage of the program.                           |
 * | `--version`      | `-V`          | `No`                | `None`     | Displays the version of the program.                         |
 * | `--file`         | `-f`          | `Yes`               | `String`   | Map archive.                                                 |
 * | `--getwidth`     | `-w`          | `No`                | `None`     | Gets the width of a map.                                     |
 * | `--getheight`    | `-h`          | `No`                | `None`     | Gets the height of a map.                                    |
 * | `--getobjetcs`   | `-o`          | `No`                | `None`     | Gets the number of tiles of a map.                           |
 * | `--getinfo`      | `-i`          | `No`                | `None`     | Gets the width, the height and the number of tiles of a map. |
 * | `--setwidth`     | `-W`          | `No`                | `Integer`  | Sets the width of a map.                                     |
 * | `--setheight`    | `-H`          | `No`                | `Integer`  | sets the height of a map.                                    |
 * | `--setobjects`   | `-O`          | See the table below | `Sring`    | Replaces the tile of a map.                                  |
 * | `--pruneobjects` | `-p`          | `No`                | `None`     | Remove unused tiles from a map.                              |
 *
 * The `--setobjects` option accepts a string where the following parameters are madatory:
 *
 * | Long options     | Short options | Mandatory           | Parameters                                   | Description                       |
 * |:----------------:|:-------------:|:-------------------:|:---------------------------------------------|:----------------------------------| 
 * | `--path`         | `-p`          | `Yes`               | `String`                                     | The path to a tile.               |
 * | `--frames`       | `-f`          | `Yes`               | `Integer`                                    | The number of frames of the tile. |
 * | `--solidity`     | `-s`          | `Yes`               | `Enumeration {solid, semi-solid, air}`       | Solidity property of the tile.    |
 * | `--destructible` | `-d`          | `Yes`               | `Enumeration {destructile, not-destrucible}` | Destructible property of the tile.|
 * | `--collectible`  | `-c`          | `Yes`               | `Enumeration {collectible, not-collectible}` | Collectible property of the tile. |
 * | `--generator`    | `-g`          | `Yes`               | `Enumeration {generator, not-generator}`     | Generator property of the tile.   |
 *
 * \param argc Number of arguments of the program.
 * \param argv Arguments of the program.
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of error
 */ 
int main(int argc, char** argv)
{
    struct gengetopt_args_info args_info;
    if (cmdline_parser(argc, argv, &args_info)) 
    {
        exit(EXIT_FAILURE);
    } 
  
    if (args_info.getwidth_given)
    {
        fprintf(
            stdout, 
            "Map width        : [%6u]\n", 
            get_map_width(args_info.file_arg)
        );
    }
    
    if (args_info.getheight_given)
    {
        fprintf(
            stdout, 
            "Map height       : [%6u]\n", 
            get_map_height(args_info.file_arg)
        );
    }

    if (args_info.getobjects_given)
    {
        fprintf(
            stdout, 
            "Number of objects: [%6u]\n", 
            get_map_objects_count(args_info.file_arg)
        );

    }

    if (args_info.getinfo_given)
    {
        MapInfo info;
        get_map_info(args_info.file_arg, &info);
        fprintf(
            stdout, 
            "Map width        : [%6u]\n"
            "Map height       : [%6u]\n"
            "Number of objects: [%6u]\n",
            info.map_width,
            info.map_height,
            info.map_objects_count
        );            
    }

    if (args_info.setwidth_given)
    {
        set_map_width(
            args_info.file_arg, 
            (unsigned int)args_info.setwidth_arg
        );
    }

    if (args_info.setheight_given)
    {
        set_map_height(
            args_info.file_arg,
            (unsigned int)args_info.setheight_arg
        );
    }

    if (args_info.setobjects_given)
    {
        MapObjectProperties* properties_array[
            args_info.setobjects_given
        ];
        struct gengetopt_args_info_object_properties
                args_info_object_properties;
        for (unsigned int i = 0, res; 
             i < args_info.setobjects_given; 
             ++i)
        {
            res = cmdline_parser_object_properties_string(
                args_info.setobjects_arg[i],
                &args_info_object_properties,
                argv[0]
            );
            
            if (res)
            {
                exit(EXIT_FAILURE);
            }
            
            properties_array[i] = map_object_properties_new(
                args_info_object_properties.path_arg,
                (unsigned int)args_info_object_properties.frames_arg,
                args_info_object_properties.solidity_arg,
                args_info_object_properties.destructible_arg,
                args_info_object_properties.collectible_arg,
                args_info_object_properties.generator_arg
            );

            cmdline_parser_object_properties_free(
                &args_info_object_properties
            );       
        }

        set_map_objects(
            args_info.file_arg, 
            properties_array, 
            (unsigned int)args_info.setobjects_given
        );

        for (int i = 0; i < args_info.setobjects_given; ++i)
        {
            map_object_properties_delete(properties_array[i]);
        }
    }

    if (args_info.pruneobjects_given)
    {
        prune_objects(args_info.file_arg);
    }

    cmdline_parser_free(&args_info);

    return EXIT_SUCCESS;
}

