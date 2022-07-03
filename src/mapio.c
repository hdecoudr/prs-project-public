/*!
 * \ingroup game_group
 * \file mapio.c
 * \brief Implementation of functions for saving and
 *        loading map archives.
 *
 * Implementation of the functions declared in the \ref 
 * map.c header.
 *
 * \author H.Decoudras
 * \version 1
 */

#include "map.h"
#include "error.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#ifdef PADAWAN


/*************************************************************
 *************************************************************
 *
 * Allocate a map object.
 *
 *************************************************************/
void map_new(unsigned width, unsigned height)
{
    map_allocate(width, height);

    /* Ground */

    for (int x = 0; x < width; ++x)
    {
        map_set(x, height - 1, 0);          // Ground.
    }

    /* Walls */

    for (int y = 0; y < height - 1; ++y) 
    {
        map_set(0, y, 1);                  
        map_set(width - 1, y, 1);
    }

    map_object_begin(6);

    map_object_add("images/ground.png", 1, MAP_OBJECT_SOLID);
    map_object_add("images/wall.png", 1, MAP_OBJECT_SOLID);
    map_object_add("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
    map_object_add(
        "images/marble.png", 
        1, 
        MAP_OBJECT_SOLID | MAP_OBJECT_DESTRUCTIBLE
    );
    map_object_add("images/flower.png", 1, MAP_OBJECT_AIR);
    map_object_add(
        "images/coin.png", 
        20, 
        MAP_OBJECT_AIR | MAP_OBJECT_COLLECTIBLE
    );
    
    map_object_end();
}

/*************************************************************
 *************************************************************
 *
 * Save map.
 *
 *************************************************************/
void map_save(char* filename)
{                                 
    /* Output file */

    int fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    exit_on_error(fd_out < 0);

    /* Keeps track of write offset */

    ssize_t current_offset = 0;
    
    /* MARC archive */
    
    unsigned int arch_header = MARC_HEADER; 
    ssize_t rw_result = write(
        fd_out, 
        &arch_header, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);
    current_offset += rw_result;

    /* Number of tiles */

    unsigned int tile_count = map_objects(); 
    rw_result = write(fd_out, &tile_count, sizeof(unsigned int));
    exit_on_error(rw_result < 0);
    current_offset += rw_result;

    /* 
        Tile attributes offset 

        Need to be written later 
     */
    
    unsigned int tile_attributes_offset = 0;
    rw_result = write(
        fd_out, 
        &tile_attributes_offset, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);
    current_offset += rw_result;

    /* 
        Tile map offset

        Need to be written later 
     */

    unsigned int tile_map_offset = 0;
    rw_result = write(fd_out, &tile_map_offset, sizeof(unsigned int));
    exit_on_error(rw_result < 0);
    current_offset += rw_result;

    /* Enumeration of tile paths */
    
    char tile_path[0x40] = {0};
    for (unsigned int i = 0; i < tile_count; ++i)
    {
        /* Path of tile */
        
        strcpy(tile_path, map_get_name(i));
        rw_result = write(fd_out, tile_path, 0x40 * sizeof(char));
        exit_on_error(rw_result < 0);
        current_offset += rw_result;
    }
    
    /* Writes tiles attributes offset */
    
    off_t seek_result = lseek(fd_out, 0x8, SEEK_SET);
    exit_on_error(seek_result < 0);
    
    tile_attributes_offset = (unsigned int)current_offset;
    rw_result = write(
        fd_out, 
        &tile_attributes_offset, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    seek_result = lseek(fd_out, 0, SEEK_END);
    exit_on_error(seek_result < 0);

    /* Tile attributes */
    
    unsigned int tile_attributes[0x8] = {0};
    tile_attributes[0x0] = OBJECT_PROPERTIES_HEADER;
    for (unsigned int i = 0; i < tile_count; ++i)
    {
        tile_attributes[0x1] = map_get_frames(i);
        tile_attributes[0x2] = map_get_solidity(i);
        tile_attributes[0x3] = 
            (map_is_destructible(i) ? MAP_OBJECT_DESTRUCTIBLE : 0);
        tile_attributes[0x4] = 
            (map_is_collectible(i) ? MAP_OBJECT_COLLECTIBLE : 0);
        tile_attributes[0x5] = 
            (map_is_generator(i) ? MAP_OBJECT_GENERATOR : 0);
        rw_result = write(
            fd_out, 
            tile_attributes, 
            0x8 * sizeof(unsigned int)
        );
        exit_on_error(rw_result < 0);
        current_offset += rw_result;
    }

    /* Writes first map offset */
    
    seek_result = lseek(fd_out, 0xc, SEEK_SET);
    exit_on_error(seek_result < 0);

    tile_map_offset = (unsigned int)current_offset;
    rw_result = write(fd_out, &tile_map_offset, sizeof(unsigned int));

    seek_result = lseek(fd_out, 0, SEEK_END);
    exit_on_error(seek_result < 0);

    /* MAPF map */

    unsigned int map_header = MAPF_HEADER;
    rw_result = write(fd_out, &map_header, sizeof(unsigned int));
    exit_on_error(rw_result < 0);
    current_offset += rw_result;

    unsigned int map_w = map_width();
    rw_result = write(fd_out, &map_w, sizeof(unsigned int));
    exit_on_error(rw_result < 0);
    current_offset += rw_result;

    unsigned int map_h = map_height();
    rw_result = write(fd_out, &map_h, sizeof(unsigned int));
    exit_on_error(rw_result < 0);
    current_offset += rw_result;

    unsigned int map_size = map_w * map_h;
    rw_result = write(fd_out, &map_size, sizeof(unsigned int));
    exit_on_error(rw_result < 0);
    current_offset += rw_result;

    /* Map */
    
    char map_data;
    for (unsigned int j = 0; j < map_h; ++j)
    {
        for (unsigned int i = 0; i < map_w; ++i)
        {
            map_data = (char)map_get(i, j);
            rw_result = write(fd_out, &map_data, sizeof(char));
            exit_on_error(rw_result < 0);
            current_offset += rw_result;
        }
    }
 
    /* Padding if needed */

    ssize_t remainder = current_offset % 0x10;
    if (remainder)
    {
        unsigned int zero = 0;
        remainder = 0x10 - remainder;
        while (remainder)
        {
            rw_result = write(fd_out, &zero, sizeof(unsigned int));
            exit_on_error(rw_result < 0);
            current_offset += rw_result;
            --remainder;
        }
    }

    close(fd_out);

    fprintf(
        stderr, 
        "Map successfully saved!\n"
        "[%lx] bytes written!\n",
        current_offset
    );
}

/*************************************************************
 *************************************************************
 *
 * Load map.
 *
 *************************************************************/
void map_load(char* filename)
{
    /* Input file */
    
    int fd_in = open(filename, O_RDONLY);
    exit_on_error(fd_in < 0);

    /* MARC archive */
 
    unsigned int header;
    ssize_t rw_result = read(fd_in, &header, sizeof(unsigned int));
    exit_on_error(rw_result < 0);
    if (header != MARC_HEADER)
    {
        fprintf(
            stderr, 
            "MARC header [%x] does not match at offset [0]!\n", 
            header
        );
        exit_on_error(1);
    } 

    /* Number of tiles */

    unsigned int tile_count;
    rw_result = read(fd_in, &tile_count, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    /* Tile attributes offset */
    
    unsigned int tile_attributes_offset;
    rw_result = read(
        fd_in, 
        &tile_attributes_offset, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    /* Map offset */

    unsigned int tile_map_offset;
    rw_result = read(fd_in, &tile_map_offset, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    off_t seek_result = lseek(fd_in, tile_map_offset, SEEK_SET);
    exit_on_error(seek_result < 0);

    /* MAPF map */
    
    seek_result = lseek(fd_in, 0, SEEK_CUR);
    exit_on_error(seek_result < 0);
    
    rw_result = read(fd_in, &header, sizeof(unsigned int));
    exit_on_error(rw_result < 0);
    if (header != MAPF_HEADER)
    {
        fprintf(
            stderr, 
            "MAPF header [%x] does not match at offset [%lx]!\n", 
            header, 
            seek_result
        );
        exit_on_error(1);
    }

    /* Map width */

    unsigned int map_w;
    rw_result = read(fd_in, &map_w, sizeof(unsigned int));  
    exit_on_error(rw_result < 0);

    /* Map Height */

    unsigned int map_h;
    rw_result = read(fd_in, &map_h, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    /* Skip map size */

    seek_result = lseek(fd_in, 0x4, SEEK_CUR);
    exit_on_error(seek_result < 0);

    /* Allocate map */

    map_allocate(map_w, map_h);

    char map_data;
    for (unsigned int y = 0; y < map_h; ++y)
    {
        for (unsigned int x = 0; x < map_w; ++x)
        {
            rw_result = read(fd_in, &map_data, sizeof(char));
            exit_on_error(rw_result < 0);
            map_set(x, y, (int)map_data);  
        }
    }

    /* Tile paths */

    seek_result = lseek(fd_in, 0x10, SEEK_SET);
    exit_on_error(seek_result < 0);

    char tile_path[tile_count][0x40];
    for (unsigned i = 0; i < tile_count; ++i)
    {
        rw_result = read(fd_in, &tile_path[i][0], 0x40 * sizeof(char));
        exit_on_error(rw_result < 0);
    }

    /* Tile attributes */

    map_object_begin(tile_count);

    unsigned int tile_attributes[5];
    for (unsigned int i = 0; i < tile_count; ++i)
    {
        /* Attribute flag */

        rw_result = read(fd_in, &header, sizeof(unsigned int));
        exit_on_error(rw_result < 0);
        if (header != OBJECT_PROPERTIES_HEADER)
        {
            seek_result = lseek(fd_in, 0, SEEK_CUR);
            exit_on_error(seek_result < 0);
            
            seek_result -= 0x4;
            fprintf(
                stderr, 
                "Tile properties flag [%x] "
                "does not match at offset [%lx]!\n", 
                header,
                seek_result
            );
            exit_on_error(1);
        }

        for (int j = 0; j < 5; ++j)
        {
            rw_result = read(
                fd_in, 
                &tile_attributes[j], 
                sizeof(unsigned int)
            );
            exit_on_error(rw_result < 0);
        }

        map_object_add(
            &tile_path[i][0x0], 
            tile_attributes[0x0], 
            tile_attributes[0x1] | 
            tile_attributes[0x2] | 
            tile_attributes[0x3] | 
            tile_attributes[0x4]
        );
        
        /* Skip padding */

        seek_result = lseek(fd_in, 0x8, SEEK_CUR);
        exit_on_error(seek_result < 0);
    }

    map_object_end();

    close(fd_in);
}

#endif

