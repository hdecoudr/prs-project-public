/*!
 * \ingroup util_group
 * \file maputil.c
 * \brief Tools for manipulating map archives.
 * 
 * Implementation of the functions declared in the \ref 
 * maputil.h header.
 *
 * \author H.Decoudras
 * \version 1
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "maputil.h"
#include "error.h"
#include "cmdlineobjectproperties.h"


/*!
 * \brief The validate_marc_header() function
 *        validates the header of a map archive.
 *
 * This function exits the program if the header
 * of a map archive is not valid.
 *
 * \param fd Opened map archive.
 *
 * \note The file cursor is advanced by `sizeof(unsigned int)`.
 */
static void validate_marc_header(int fd);

/*!
 * \brief The validate_mapf_header() function
 *        validates the header of a map.
 *
 * This function exits the program if the header
 * of a map is not valid.
 *
 * \param fd Opened map archive.
 *
 * \note The file cursor is advanced by `sizeof(unsigned int)`.
 */
static void validate_mapf_header(int fd);

/*!
 * \brief The validate_object_properties_header() function
 *        validates the header of the properties
 *        of a tile.
 *
 * This function exits the program if the header
 * of a map is not valid.
 *
 * \param fd Opened map archive.
 *
 * \note The file cursor is advanced by `sizeof(unsigned int)`.
 */
static void validate_object_properties_header(int fd);

/*!
 * \brief The seek_mapf_header() function moves the 
 *        file cursor to the begining of the first map.
 *
 * \param fd Opened map archive.
 */
static void seek_mapf_header(int fd);

/*!
 * \brief The backup_archive() function makes
 *        a backup of a map archive.
 *
 * The date of the day is appended to the
 * file name of the backup map archive under
 * the form `<filename>-DD-MM-YYYY-hh:mm:ss` where:
 *
 *  - `DD` corresponds to the current day;
 *  - `MM` corresponds to the current month;
 *  - `YYYY` corresponds to the current year;
 *  - `hh` corresponds to the current hour;
 *  - `mm` corresponds to the current minute;
 *  - `ss` corresponds to the current second.
 *
 *  This function exits the program if it fails 
 *  to create a backup of the map archive.
 *
 * \param filename Map archive to backup.
 *
 * \return The path (file name included) of
 *         the backup map archive.
 */
static char* backup_archive(const char* filename);


/*!
 * \brief The remove_archive() function removes
 *        a map archive from the disk.
 *
 * \param filename Map archive to remove.
 */
static void remove_archive(const char* filename);


/*************************************************************
 *************************************************************
 *
 * Allocate map object properties.
 *
 *************************************************************/
MapObjectProperties* map_object_properties_new(
    const char* path, unsigned int frames, 
    enum enum_solidity solidity, enum enum_destructible destructible,
    enum enum_collectible collectible, enum enum_generator generator
)
{
    MapObjectProperties* p = 
        (MapObjectProperties*)malloc(sizeof(MapObjectProperties));
    exit_on_error(p == NULL);
    p->path = (char*)malloc((strlen(path) + 1) * sizeof(char));
    exit_on_error(p->path == NULL);
    strcpy(p->path, path);
    p->frames = frames;
    switch (solidity)
    {
        case solidity_arg_solid:
        {
            p->solidity = MAP_OBJECT_SOLID;
            break;
        }

        case solidity_arg_semi_solid:
        {
            p->solidity = MAP_OBJECT_SEMI_SOLID;
            break;
        }

        case solidity_arg_air:
        {
            p->solidity = MAP_OBJECT_AIR;
            break;
        }

        default:
        {
            p->solidity = -1;
            break;
        }
    }

    switch (destructible)
    {
        case destructible_arg_destructible:
        {
            p->destructible = MAP_OBJECT_DESTRUCTIBLE;
            break;
        }

        case destructible_arg_not_destructible:
        {
            p->destructible = 0;
            break;
        }

        default:
        {
            p->destructible = -1;
            break;
        }
    }

    switch (collectible)
    {
        case collectible_arg_collectible:
        {
            p->collectible = MAP_OBJECT_COLLECTIBLE;
            break;
        }

        case collectible_arg_not_collectible:
        {
            p->collectible = 0;
            break;
        }

        default:
        {
            p->collectible = -1;
            break;
        }
    }

    switch (generator)
    {
        case generator_arg_generator:
        {
            p->generator = MAP_OBJECT_GENERATOR;
            break;
        }

        case generator_arg_not_generator:
        {
            p->generator = 0;
            break;
        }

        default:
        {
            p->generator = -1;
            break;
        }
    }

    return p;
}

/*************************************************************
 *************************************************************
 *
 * Delete map properties.
 *
 *************************************************************/
void map_object_properties_delete(MapObjectProperties* properties)
{
    free(properties->path);
    free(properties);
}

/*************************************************************
 *************************************************************
 *
 * Get map width.
 *
 *************************************************************/
unsigned int get_map_width(const char* filename)
{
    /* Open the file in read only mode */

    int fd = open(filename, O_RDONLY);
    exit_on_error(fd < 0);

    /* Validate MARC header */

    validate_marc_header(fd);

    /* Go to the MAPF file */
    
    seek_mapf_header(fd);

    /* Validate MAPF header */

    validate_mapf_header(fd);

    /* Get the width */

    unsigned int width;
    ssize_t rw_result = read(fd, &width, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    int result = close(fd);
    exit_on_error(result < 0);
    
    return width;
}

/*************************************************************
 *************************************************************
 *
 * Get map height.
 *
 *************************************************************/
unsigned int get_map_height(const char* filename)
{
    /* Open the file in read only mode */

    int fd = open(filename, O_RDONLY);
    exit_on_error(fd < 0);

    /* Validate MARC header */

    validate_marc_header(fd);

    /* Go to the MAPF file */
    
    seek_mapf_header(fd);

    /* Validate MAPF header */

    validate_mapf_header(fd);

    /* Get the height */

    off_t seek_result = lseek(fd, 0x4, SEEK_CUR);
    exit_on_error(seek_result < 0);

    unsigned int height;
    ssize_t rw_result = read(fd, &height, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    int result = close(fd);
    exit_on_error(result < 0);

    return height;
}

/*************************************************************
 *************************************************************
 *
 * Get number of tiles.
 *
 *************************************************************/
unsigned int get_map_objects_count(const char* filename)
{
    /* Open the file in read only mode */

    int fd = open(filename, O_RDONLY);
    exit_on_error(fd < 0);

    /* Validate MARC header */

    validate_marc_header(fd);

    /* Get number of tiles */

    off_t seek_result = lseek(fd, 0x4, SEEK_SET);
    exit_on_error(seek_result < 0);

    unsigned int count;
    ssize_t rw_result = read(fd, &count, sizeof(unsigned));
    exit_on_error(rw_result < 0);

    int result = close(fd);
    exit_on_error(result < 0);

    return count;
}

/*************************************************************
 *************************************************************
 *
 * Get map width, map height and number of tiles.
 *
 *************************************************************/
void get_map_info(const char* filename, MapInfo* info)
{
    /* Open the file in read only mode */

    int fd = open(filename, O_RDONLY);
    exit_on_error(fd < 0);

    /* Validate MARC header */

    validate_marc_header(fd);

    /* Get number of tiles */

    off_t seek_result = lseek(fd, 0x4, SEEK_SET);
    exit_on_error(seek_result < 0);

    ssize_t rw_result = read(
        fd, 
        &info->map_objects_count, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    /* Go to the MAPF file */

    seek_mapf_header(fd);

    /* Validate MAPF header */
    
    validate_mapf_header(fd);

    /* Get the width */

    rw_result = read(fd, &info->map_width, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    /* Get the height */

    rw_result = read(fd, &info->map_height, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    int result = close(fd);
    exit_on_error(result < 0);
}

/*************************************************************
 *************************************************************
 *
 * Set map width.
 *
 *************************************************************/
void set_map_width(const char* filename, unsigned int map_width)
{
    /* Create a backup */

    char* backup_filename = backup_archive(filename);

    /* Open the backup file in read only mode */

    int fd_backup = open(backup_filename, O_RDONLY);
    exit_on_error(fd_backup < 0); 

    /* Validate MARC header */

    validate_marc_header(fd_backup); 

    /* Go to the MAPF file */

    seek_mapf_header(fd_backup);

    /* Validate MAPF header */

    validate_mapf_header(fd_backup);

    /* Get old width */

    unsigned int backup_map_width;
    ssize_t rw_result = read(
        fd_backup, 
        &backup_map_width, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    if (map_width == backup_map_width)
    {
        int result = close(fd_backup);
        exit_on_error(result < 0);
        remove_archive(backup_filename);
        free(backup_filename);
        return;
    }

    free(backup_filename);

    /* Get map height */

    unsigned int backup_map_height;
    rw_result = read(
        fd_backup, 
        &backup_map_height, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    /* Go to map data */

    off_t seek_result = lseek(fd_backup, 0x4, SEEK_CUR);
    exit_on_error(seek_result < 0);

    /* Open the file in read and write mode */

    int fd_new = open(filename, O_RDWR, 0666);
    exit_on_error(fd_new < 0);

    /* Go to the MAPF file */

    seek_mapf_header(fd_new);

    /* Update map width */

    seek_result = lseek(fd_new, 0x4, SEEK_CUR);
    exit_on_error(seek_result < 0);

    rw_result = write(fd_new, &map_width, sizeof(unsigned int));
    exit_on_error(rw_result < 0); 

    /* Update map size */

    seek_result = lseek(fd_new, 0x4, SEEK_CUR);
    exit_on_error(seek_result < 0);

    unsigned int map_size = map_width * backup_map_height;
    rw_result = write(fd_new, &map_size, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    char map_data;
    if(map_width > backup_map_width)
    {
        /* Expand */

        char map_data_none = (char)MAP_OBJECT_NONE;
        unsigned int padding = map_width - backup_map_width;
        for (unsigned int y = 0; y < backup_map_height; ++y)
        {
            for (unsigned int x = 0; x < backup_map_width; ++x)
            {
                rw_result = read(fd_backup, &map_data, sizeof(char));
                exit_on_error(rw_result < 0);

                rw_result = write(fd_new, &map_data, sizeof(char));
                exit_on_error(rw_result < 0);
            }
            
            for (unsigned int x = 0; x < padding; ++x)
            {
                rw_result = write(
                    fd_new, 
                    &map_data_none, 
                    sizeof(char)
                );
                exit_on_error(rw_result < 0);
            }
        }
    }
    else
    {
        /* Shrink */
        
        for (unsigned int y = 0; y < backup_map_height; ++y)
        {
            for (unsigned int x = 0; x < map_width; ++x)
            {
                rw_result = read(fd_backup, &map_data, sizeof(char));
                exit_on_error(rw_result < 0);

                rw_result = write(fd_new, &map_data, sizeof(char));
                exit_on_error(rw_result < 0);
            }

            seek_result = lseek(
                fd_backup, 
                backup_map_width - map_width, 
                SEEK_CUR
            );
            exit_on_error(seek_result < 0);
        }

        seek_result = lseek(fd_new, 0, SEEK_CUR);
        exit_on_error(seek_result < 0);

        int result = ftruncate(fd_new, seek_result);
        exit_on_error(result < 0);
    }

    int result = close(fd_backup);
    exit_on_error(result < 0);

    result = close(fd_new);
    exit_on_error(result < 0);
}

/*************************************************************
 *************************************************************
 *
 * Set map height.
 *
 *************************************************************/
void set_map_height(const char* filename, unsigned int map_height)
{
    /* Create a backup */

    char* backup_filename = backup_archive(filename);

    /* Open the backup file in read only mode */

    int fd_backup = open(backup_filename, O_RDONLY);
    exit_on_error(fd_backup < 0); 

    /* Validate MARC header */

    validate_marc_header(fd_backup); 

    /* Go to the MAPF file */

    seek_mapf_header(fd_backup);

    /* Validate MAPF header */

    validate_mapf_header(fd_backup);

    /* Get old width */

    unsigned int backup_map_width;
    ssize_t rw_result = read(
        fd_backup, 
        &backup_map_width, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    /* Get map height */

    unsigned int backup_map_height;
    rw_result = read(
        fd_backup, 
        &backup_map_height, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    if (map_height == backup_map_height)
    {
        int result = close(fd_backup);
        exit_on_error(result < 0);
        remove_archive(backup_filename);
        free(backup_filename);
        return;
    }

    free(backup_filename);

    /* Go to map data */

    off_t seek_result = lseek(fd_backup, 0x4, SEEK_CUR);
    exit_on_error(seek_result < 0);

    /* Open the file in read and write mode */

    int fd_new = open(filename, O_RDWR, 0666);
    exit_on_error(fd_new < 0);

    /* Go to the MAPF file */

    seek_mapf_header(fd_new);

    /* Update map height */

    seek_result = lseek(fd_new, 0x8, SEEK_CUR);
    exit_on_error(seek_result < 0);

    rw_result = write(fd_new, &map_height, sizeof(unsigned int));
    exit_on_error(rw_result < 0); 

    /* Update map size */

    unsigned int map_size = backup_map_width * map_height;
    rw_result = write(fd_new, &map_size, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    char map_data;
    if(map_height > backup_map_height)
    {
        /* Expand */

        char map_data_none = (char)MAP_OBJECT_NONE;
        unsigned int padding = map_height - backup_map_height;
        for (unsigned int y = 0; y < padding; ++y)
        {
            for (unsigned int x = 0; x < backup_map_width; ++x)
            {
                rw_result = write(
                    fd_new, 
                    &map_data_none, 
                    sizeof(char)
                );
                exit_on_error(rw_result < 0);
            }
        }

        for (unsigned int y = 0; y < backup_map_height; ++y)
        {
            for (unsigned int x = 0; x < backup_map_width; ++x)
            {
                rw_result = read(fd_backup, &map_data, sizeof(char));
                exit_on_error(rw_result < 0);

                rw_result = write(fd_new, &map_data, sizeof(char));
                exit_on_error(rw_result < 0);
            }            
        }
    }
    else
    {
        /* Shrink */
       
        seek_result = lseek(
            fd_backup, 
            (backup_map_height - map_height) * backup_map_width, 
            SEEK_CUR
        );
        exit_on_error(seek_result < 0);

        for (unsigned int y = 0; y < map_height; ++y)
        {
            for (unsigned int x = 0; x < backup_map_width; ++x)
            {
                rw_result = read(fd_backup, &map_data, sizeof(char));
                exit_on_error(rw_result < 0);

                rw_result = write(fd_new, &map_data, sizeof(char));
                exit_on_error(rw_result < 0);
            }
        }

        seek_result = lseek(fd_new, 0, SEEK_CUR);
        exit_on_error(seek_result < 0);

        int result = ftruncate(fd_new, seek_result);
        exit_on_error(result < 0);
    }

    int result = close(fd_backup);
    exit_on_error(result < 0);

    result = close(fd_new);
    exit_on_error(result < 0);
}

/*************************************************************
 *************************************************************
 *
 * Set map objects.
 *
 *************************************************************/
void set_map_objects(
    const char* filename, MapObjectProperties** properties,
    unsigned int properties_count
)
{
    char* backup_filename = backup_archive(filename);

    /* Open the backup file in read only mode */

    int fd_backup = open(backup_filename, O_RDONLY);
    exit_on_error(fd_backup < 0);

    /* Validate MARC header */

    validate_marc_header(fd_backup); 

    /* Get number of tiles */

    unsigned int tiles_count;
    ssize_t rw_result = read(
        fd_backup, 
        &tiles_count, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    if (tiles_count > properties_count)
    {
        int result = close(fd_backup);
        exit_on_error(result < 0);
        remove_archive(backup_filename);
        free(backup_filename);
        return;
    }

    free(backup_filename);

    /* Go to MAPF */

    seek_mapf_header(fd_backup);

    /* Validate MAPF header */

    validate_mapf_header(fd_backup);

    unsigned int map_width;
    rw_result = read(
        fd_backup, 
        &map_width, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    /* Get map height */

    unsigned int map_height;
    rw_result = read(
        fd_backup, 
        &map_height, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    off_t seek_result = lseek(fd_backup, -0xc, SEEK_CUR);
    exit_on_error(seek_result < 0);

    /* Open the file in read and write mode */

    int fd_new = open(filename, O_RDWR, 0666);
    exit_on_error(fd_new < 0);

    /* Write tile count */

    seek_result = lseek(fd_new, 0x4, SEEK_SET);
    exit_on_error(seek_result < 0);

    rw_result = write(
        fd_new, 
        &properties_count, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    /* Write tile paths */

    char tile_paths[0x40] = {0};
    seek_result = lseek(fd_new, 0x8, SEEK_CUR);
    exit_on_error(seek_result < 0);

    for (unsigned int i = 0; i < properties_count; ++i)
    {
        strcpy(tile_paths, properties[i]->path);
        rw_result = write(fd_new, tile_paths, 0x40 * sizeof(char));
        memset(tile_paths, 0, 0x40 * sizeof(char));
    }

    /* Update tile properties offset */

    seek_result = lseek(fd_new, 0, SEEK_CUR);
    exit_on_error(seek_result < 0);

    unsigned int object_properties_offset = (unsigned int)seek_result;
    seek_result = lseek(fd_new, 0x8, SEEK_SET);
    exit_on_error(seek_result < 0);

    rw_result = write(
        fd_new, 
        &object_properties_offset, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    seek_result = lseek(fd_new, object_properties_offset, SEEK_SET);
    exit_on_error(seek_result < 0);

    /* Write tile properties */

    unsigned int tile_properties[0x8] = {0};
    tile_properties[0] = OBJECT_PROPERTIES_HEADER;
    for (unsigned int i = 0; i < properties_count; ++i)
    {
        tile_properties[0x1] = properties[i]->frames;
        tile_properties[0x2] = properties[i]->solidity;
        tile_properties[0x3] = properties[i]->destructible;
        tile_properties[0x4] = properties[i]->collectible;
        tile_properties[0x5] = properties[i]->generator;
        rw_result = write(
            fd_new, 
            tile_properties, 
            0x8 * sizeof(unsigned int)
        );
        exit_on_error(rw_result < 0);
    }

    /* Update map offset */

    seek_result = lseek(fd_new, 0, SEEK_CUR);
    exit_on_error(seek_result < 0);

    unsigned int map_offset = (unsigned int)seek_result;
    seek_result = lseek(fd_new, 0xc, SEEK_SET);
    exit_on_error(seek_result < 0);

    rw_result = write(fd_new, &map_offset, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    seek_result = lseek(fd_new, map_offset, SEEK_SET);
    exit_on_error(seek_result < 0);

    /* MAPF header */
    
    char mapf_header[0xf];
    rw_result = read(fd_backup, mapf_header, 0xf * sizeof(char));
    exit_on_error(rw_result < 0);

    rw_result = write(fd_new, mapf_header, 0xf * sizeof(char));
    exit_on_error(rw_result < 0);

    /* Copy map */

    char map_data;
    for (unsigned int y = 0; y < map_height; ++y)
    {
        for (unsigned int x = 0; x < map_width; ++x)
        {
            rw_result = read(fd_backup, &map_data, sizeof(char));
            exit_on_error(rw_result < 0);

            rw_result = write(fd_new, &map_data, sizeof(char));
            exit_on_error(rw_result < 0);
        }
    }

    int result = close(fd_backup);
    exit_on_error(result < 0);

    result = close(fd_new);
    exit_on_error(result < 0);
}

/*************************************************************
 *************************************************************
 *
 * Prune objects.
 *
 *************************************************************/
void prune_objects(const char* filename)
{
    /* Create a backup */

    char* backup_filename = backup_archive(filename);

    /* Open the backup file in read only mode */

    int fd_backup = open(backup_filename, O_RDONLY);
    exit_on_error(fd_backup < 0);
    free(backup_filename);

    /* Validate MARC header */

    validate_marc_header(fd_backup);

    /* Get number of tiles */

    unsigned int tiles_count;
    ssize_t rw_result = read(
        fd_backup, 
        &tiles_count, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);    
    
    int used_tiles[tiles_count];
    memset(used_tiles, 0, tiles_count * sizeof(int));

    /* Go to the MAPF file */

    seek_mapf_header(fd_backup);

    /* Validate MAPF header */

    validate_mapf_header(fd_backup);

    /* Get old width */

    unsigned int map_width;
    rw_result = read(
        fd_backup, 
        &map_width, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    /* Get map height */

    unsigned int map_height;
    rw_result = read(
        fd_backup, 
        &map_height, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

   /* Go to map data */

    off_t seek_result = lseek(fd_backup, 0x4, SEEK_CUR);
    exit_on_error(seek_result < 0);

    /* Count objects */

    char map_data;
    for (unsigned int y = 0; y < map_height; ++y)
    {
        for (unsigned int x = 0; x < map_width; ++x)
        {
            rw_result = read(fd_backup, &map_data, sizeof(char));
            exit_on_error(rw_result < 0);
            
            if (map_data != (char)MAP_OBJECT_NONE)
            {
                used_tiles[(int)map_data]++;
            }
        }
    }

    /* New tiles count */

    unsigned int new_tiles_count = 0;
    for (unsigned int i = 0; i < tiles_count; ++i)
    {
        if (used_tiles[i])
        {
            ++new_tiles_count;
        }
    }

    /* Go back to tile paths */

    seek_result = lseek(fd_backup, 0x10, SEEK_SET);
    exit_on_error(seek_result < 0);

    /* Open the file in read and write mode */

    int fd_new = open(filename, O_RDWR, 0666);
    exit_on_error(fd_new < 0);

    /* Write tiles count */

    seek_result = lseek(fd_new, 0x4, SEEK_SET);
    exit_on_error(fd_new < 0);

    rw_result = write(fd_new, &new_tiles_count, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    /* Go to tile paths */

    seek_result = lseek(fd_new, 0x8, SEEK_CUR);
    exit_on_error(seek_result < 0);

    /* Remove unused paths */

    char tile_paths[0x40];
    for (unsigned int i = 0; i < tiles_count; ++i)
    {
        if (used_tiles[i])
        {
            rw_result = read(
                fd_backup, 
                tile_paths, 
                0x40 * sizeof(char)
            );
            exit_on_error(rw_result < 0);

            rw_result = write(
                fd_new, 
                tile_paths, 
                0x40 * sizeof(char)
            );
            exit_on_error(rw_result < 0);
        }
        else
        {
            seek_result = lseek(fd_backup, 0x40, SEEK_CUR);
            exit_on_error(seek_result < 0);
        }
    }

    /* Update tile properties offset */

    seek_result = lseek(fd_new, 0, SEEK_CUR);
    exit_on_error(seek_result < 0);

    unsigned int object_properties_offset = (unsigned int)seek_result;
    seek_result = lseek(fd_new, 0x8, SEEK_SET);
    exit_on_error(seek_result < 0);

    rw_result = write(
        fd_new, 
        &object_properties_offset, 
        sizeof(unsigned int)
    );
    exit_on_error(rw_result < 0);

    seek_result = lseek(fd_new, object_properties_offset, SEEK_SET);
    exit_on_error(seek_result < 0);

    /* Remove unused objects */

    char object_properties[0x20];
    for (unsigned int i = 0; i < tiles_count; ++i)
    {
        validate_object_properties_header(fd_backup);

        seek_result = lseek(fd_backup, -0x4, SEEK_CUR);
        exit_on_error(seek_result < 0);

        if (used_tiles[i])
        {
            rw_result = read(
                fd_backup, 
                object_properties, 
                0x20 * sizeof(char)
            );
            exit_on_error(rw_result < 0);

            rw_result = write(
                fd_new, 
                object_properties, 
                0x20 * sizeof(char)
            );
            exit_on_error(rw_result < 0);
        }
        else
        {
            seek_result = lseek(fd_backup, 0x20, SEEK_CUR);
            exit_on_error(seek_result < 0);
        }
    }

    /* Update map offset */

    seek_result = lseek(fd_new, 0, SEEK_CUR);
    exit_on_error(seek_result < 0);

    unsigned int map_offset = (unsigned int)seek_result;
    seek_result = lseek(fd_new, 0xc, SEEK_SET);
    exit_on_error(seek_result < 0);

    rw_result = write(fd_new, &map_offset, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    seek_result = lseek(fd_new, map_offset, SEEK_SET);
    exit_on_error(seek_result < 0);

    /* MAPF header */
    
    char mapf_header[0xf];
    rw_result = read(fd_backup, mapf_header, 0xf * sizeof(char));
    exit_on_error(rw_result < 0);

    rw_result = write(fd_new, mapf_header, 0xf * sizeof(char));
    exit_on_error(rw_result < 0);

    /* Copy map */

    char diff = (char)(tiles_count - new_tiles_count);
    for (unsigned int y = 0; y < map_height; ++y)
    {
        for (unsigned int x = 0; x < map_width; ++x)
        {
            rw_result = read(fd_backup, &map_data, sizeof(char));
            exit_on_error(rw_result < 0);
            
            if (map_data != (char)MAP_OBJECT_NONE &&
                map_data > new_tiles_count)
            {
                map_data -= diff;
            }

            rw_result = write(fd_new, &map_data, sizeof(char));
            exit_on_error(rw_result < 0);
        }
    }

    seek_result = lseek(fd_new, 0, SEEK_CUR);
    exit_on_error(seek_result < 0);

    int result = ftruncate(fd_new, seek_result);
    exit_on_error(result < 0);

    result = close(fd_backup);
    exit_on_error(result < 0);

    result = close(fd_new);
    exit_on_error(result < 0);
}


/*************************************************************
 *************************************************************
 *
 * Validate MARC.
 *
 *************************************************************/
void validate_marc_header(int fd)
{
    unsigned int header;
    ssize_t rw_result = read(fd, &header, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    if (header != MARC_HEADER)
    {
        fprintf(
            stderr,
            "MARC header [%x] does not match!\n",
            header
        );

        exit(EXIT_FAILURE);
    }
}

/*************************************************************
 *************************************************************
 *
 * Validate MAPF.
 *
 *************************************************************/
void validate_mapf_header(int fd)
{
    unsigned int header;
    ssize_t rw_result = read(fd, &header, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    if (header != MAPF_HEADER)
    {
        fprintf(
            stderr,
            "MARF header [%x] does not match!\n",
            header
        );

        exit(EXIT_FAILURE);
    }
}

/*************************************************************
 *************************************************************
 *
 * Validate object header.
 *
 *************************************************************/
void validate_object_properties_header(int fd)
{
    unsigned int header;
    ssize_t rw_result = read(fd, &header, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    if (header != OBJECT_PROPERTIES_HEADER)
    {
        fprintf(
            stderr,
            "Object header [%x] does not match!\n",
            header
        );

        exit(EXIT_FAILURE);
    }
}

/*************************************************************
 *************************************************************
 *
 * Go to MAPF.
 *
 *************************************************************/
void seek_mapf_header(int fd)
{
    off_t seek_result = lseek(fd, 0xc, SEEK_SET);
    exit_on_error(seek_result < 0);

    unsigned int map_offset;
    ssize_t rw_result = read(fd, &map_offset, sizeof(unsigned int));
    exit_on_error(rw_result < 0);

    seek_result = lseek(fd, map_offset, SEEK_SET);
    exit_on_error(seek_result < 0);
}

/*************************************************************
 *************************************************************
 *
 * Backup archive.
 *
 *************************************************************/
char* backup_archive(const char* filename)
{
    char time_str_buffer[256] = {0};
    time_t now = time(NULL);
    struct tm* local_time = localtime(&now);
    strftime(
        time_str_buffer, 
        sizeof(time_str_buffer), 
        "-%d-%m-%Y-%H:%M:%S",
        local_time
    );
    const char* backup_suffix = ".backup";
    char* backup_filename = (char*)malloc(
        (strlen(filename) + 
         strlen(time_str_buffer) + 
         strlen(backup_suffix)
         + 1) *
        sizeof(char)
    );
    exit_on_error(backup_filename == NULL);

    strcpy(backup_filename, filename);
    strcat(backup_filename, time_str_buffer);
    strcat(backup_filename, backup_suffix);

    pid_t child_pid = fork();
    if (!child_pid)
    {
        int fd_null = open("/dev/null", O_WRONLY, 0600);
        exit_on_error(fd_null < 0);

        int result = dup2(fd_null, STDERR_FILENO);
        exit_on_error(result < 0);

        result = close(fd_null);
        exit_on_error(result < 0);

        result = execlp(
            "cp",
            "cp", 
            filename,
            backup_filename, 
            NULL
        );
        exit(EXIT_FAILURE);
    }

    int status;
    int result = wait(&status);
    exit_on_error(result < 0);
    
    if (WIFEXITED(status))
    {
        if (WEXITSTATUS(status))
        {
            fprintf(
                stderr, 
                "Failed to backup %s\n!", 
                filename
            );
            exit(EXIT_FAILURE);
        }    
    }
    else
    {
        fprintf(
            stderr, 
            "Failed to backup %s\n!", 
            filename
        );
        exit(EXIT_FAILURE);
    }
    
    return backup_filename;
}

/*************************************************************
 *************************************************************
 *
 * Remove archive.
 *
 *************************************************************/
void remove_archive(const char* filename)
{
    pid_t child_pid = fork();
    if (!child_pid)
    {
        int fd_null = open("/dev/null", O_WRONLY, 0600);
        exit_on_error(fd_null < 0);

        int result = dup2(fd_null, STDERR_FILENO);
        exit_on_error(result < 0);

        result = close(fd_null);
        exit_on_error(result < 0);

        result = execlp(
            "rm",
            "rm", 
            filename, 
            NULL
        );
        exit(EXIT_FAILURE);
    }

    int status;
    int result = wait(&status);
    exit_on_error(result < 0);
    
    if (WIFEXITED(status))
    {
        if (WEXITSTATUS(status))
        {
            fprintf(
                stderr, 
                "Failed to remove %s\n!", 
                filename
            );
            exit(EXIT_FAILURE);
        }    
    }
    else
    {
        fprintf(
            stderr, 
            "Failed to backup %s\n!", 
            filename
        );
        exit(EXIT_FAILURE);
    }
}

