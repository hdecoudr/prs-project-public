/*!
 * \ingroup util_group
 * \file maputil.h
 * \brief Tools for manipulating map archives.
 *
 * \author H.Decoudras
 * \version 1
 */

#ifndef DEF_MAPUTIL_H
#define DEF_MAPUTIL_H

#include "cmdlineobjectproperties.h"


/*!
 * \brief MARC header signature. 
 */
#define MARC_HEADER 0x4352414d

/*!
 * \brief MAPF header signature
 */
#define MAPF_HEADER 0x4650414d

/*!
 * \brief Tile properties header signature.
 */
#define OBJECT_PROPERTIES_HEADER 0x00000010

/*!
 * \brief None object map data.
 */
#define MAP_OBJECT_NONE 0xff

/*!
 * \brief Air property of a tile.
 *
 * Such a tile can be crossed in all directions.
 */
#define MAP_OBJECT_AIR 0x00000000

/*!
 * \brief Semi-solid property of a tile.
 *
 * Such a tile can only be crossed from below.
 */
#define MAP_OBJECT_SEMI_SOLID 0x00000001

/*!
 * \brief Solid property of a tile.
 *
 * Such a tile cannot be crossed.
 */
#define MAP_OBJECT_SOLID 0x00000002

/*!
 * \brief Destructible property of a tile.
 *
 * Such a tile can be destroyed.
 */
#define MAP_OBJECT_DESTRUCTIBLE 0x00000004

/*!
 * \brief Collectible property of a tile.
 *
 * Such a tile is a collectible item.
 */
#define MAP_OBJECT_COLLECTIBLE 0x00000008

/*!
 * \brief Generator property of a tile.
 *
 * Such an object triggers an action.
 */
#define MAP_OBJECT_GENERATOR 0x00000010

/*!
 * \struct map_info
 * \brief The \ref map_info structure contains 
 *        information about a map.
 *
 * The following information are represented:
 *
 *  - The width of a map, which corresponds to 
 *    the number of tiles on the `x` axis;
 *  - The height of a map, which corresponds to 
 *    the number of tiles on the `y` axis;
 *  - The number of tiles of a map.
 */
struct map_info
{
    /*!
     * \brief Width of a map.
     */
    unsigned int map_width;
    
    /*!
     * \brief Height of a map.
     */
    unsigned int map_height;
    
    /*!
     * \brief Number of tiles of a map.
     */
    unsigned int map_objects_count;
};


/*!
 * \brief Type definition of the \ref map_info structure.
 *
 * \see map_info
 */
typedef struct map_info MapInfo;


/*!
 * \struct map_object_properties
 * \brief The \ref map_object_properties structure
 *        represents the properties of a tile.
 *
 * \see MAP_OBJECT_AIR
 * \see MAP_OBJECT_SEMI_SOLID
 * \see MAP_OBJECT_SOLID
 * \see MAP_OBJECT_DESTRUCTIBLE
 * \see MAP_OBJECT_COLLECTIBLE
 * \see MAP_OBJECT_GENERATOR
 * \see map_object_properties_new()
 * \see map_object_properties_delete()
 */
struct map_object_properties
{
    /*!
     * \brief Path of a tile.
     */
    char* path;
    
    /*!
     * \brief Number of frames of a tile.
     */
    unsigned int frames;
    
    /*!
     * \brief Solidity property of a tile.
     */
    unsigned int solidity;
    
    /*!
     * \brief Destructible property of a tile.
     */
    unsigned int destructible;
    
    /*!
     * \brief Collectible property of a tile.
     */
    unsigned int collectible;
    
    /*!
     * \brief Generator property of a tile.
     */
    unsigned int generator;
};


/*!
 * \brief Type definition of the \ref map_info structure.
 *
 * \see map_object_properties
 */
typedef struct map_object_properties MapObjectProperties;


/*!
 * \brief The map_object_properties_new() function allocates
 *        a \ref MapObjectProperties structure.
 *
 * This function exits the program if the allocation fails.
 *
 * \param path Tile path.
 * \param frames The number of frames of a tile.
 * \param solidity The solidity property of the tile.
 * \param destructible The destructible property of the tile.
 * \param collectible The collectible property of the tile.
 * \param generator The generator property of the tile.
 *
 * \return An allocated \ref MapObjectProperties structure.
 *
 * \see enum_solidity
 * \see enum_destructible
 * \see enum_collectible
 * \see enum_generator
 * \see MAP_OBJECT_AIR
 * \see MAP_OBJECT_SEMI_SOLID
 * \see MAP_OBJECT_SOLID
 * \see MAP_OBJECT_DESTRUCTIBLE
 * \see MAP_OBJECT_COLLECTIBLE
 * \see MAP_OBJECT_GENERATOR
 * \see MapObjectProperties
 * \see map_object_properties_delete()
 */
MapObjectProperties* map_object_properties_new(
    const char* path, unsigned int frames, 
    enum enum_solidity solidity, enum enum_destructible destructible,
    enum enum_collectible collectible, enum enum_generator generator
);

/*!
 * \brief The map_object_properties_delete() function 
 *        frees the memory occupied by a 
 *        \ref MapObjectProperties structure.
 *
 * \param properties \ref MapObjectProperties structure to free.
 */
void map_object_properties_delete(MapObjectProperties* properties);


/*!
 * \brief The get_map_width() function gets the 
 *        width of a map.
 *
 * The width of a map corresponds to the number of
 * tiles on the `x` axis.
 *
 * \param filename Map archive.
 *
 * \return The width of a map.
 *
 * \see validate_marc_header()
 * \see seek_mapf_header()
 * \see validate_mapf_header()
 */
unsigned int get_map_width(const char* filename);

/*!
 * \brief The get_map_height() function gets the 
 *        height of a map.
 *
 * The height of a map corresponds to the number of
 * tiles on the `y` axis.
 *
 * \param filename Map archive.
 *
 * \return The height of a map.
 *
 * \see validate_marc_header()
 * \see seek_mapf_header()
 * \see validate_mapf_header() 
 */
unsigned int get_map_height(const char* filename);

/*!
 * \brief The get_map_objects_counts() function gets the 
 *        number of tiles of a map.
 *
 * \param filename Map archive.
 *
 * \return The number of tiles of a map.
 *
 * \see validate_marc_header()
 */
unsigned int get_map_objects_count(const char* filename);

/*!
 * \brief The get_map_info() function gets the width, 
 *        the height and the number of tiles of a map.
 *
 * This function aggregates the same treatments as the
 * get_map_width(), get_map_height() and 
 * get_map_objects_count() functions.
 *
 * \param filename Map archive.
 * \param info Contains the width, the height and the
 *             number of tiles of a map.
 *
 * \see MapInfo
 * \see validate_marc_header()
 * \see seek_mapf_header()
 * \see validate_mapf_header()
 */
void get_map_info(const char* filename, MapInfo* info);

/*!
 * \brief The set_map_width() function sets the width
 *        of a map.
 *
 * If the requested width of the map is greater than the 
 * actual width, then the previous map data are preserved
 * and tiles referencing the \ref MAP_OBJECT_NONE map data 
 * are added to the right side of the map.
 *
 * If the requested width of the map is smaller than the
 * actual width, then the previous map data are removed from
 * the right side of the map. 
 *
 * \param filename Map archive.
 * \param map_width Width of the map.
 * 
 * \see MAP_OBJECT_NONE
 * \see validate_marc_header()
 * \see seek_mapf_header()
 * \see validate_mapf_header()
 * \see backup_archive()
 * \see remove_archive()
 */
void set_map_width(const char* filename, unsigned int map_width);

/*!
 * \brief The set_map_height() function sets the height
 *        of a map.
 *
 * If the requested height of the map is greater than the 
 * actual height, then the previous map data are preserved
 * and tiles referencing the \ref MAP_OBJECT_NONE map data 
 * are added to the top side of the map.
 *
 * If the requested height of the map is smaller than the
 * actual width, then the previous map data are removed from
 * the top side of the map. 
 *
 * \param filename Map archive.
 * \param map_height Height of the map.
 * 
 * \see MAP_OBJECT_NONE
 * \see validate_marc_header()
 * \see seek_mapf_header()
 * \see validate_mapf_header()
 * \see backup_archive()
 * \see remove_archive()
 */
void set_map_height(const char* filename, unsigned int map_height);

/*!
 * \brief The set_map_objects() function replaces the tiles 
 *        of a map.
 *
 * The tiles of a map are replaced only if the provided 
 * \p properties contains at least the same amount of tiles.
 *
 * \param filename Map archive.
 * \param properties Tile properties.
 * \param properties_count Number of tile properties.
 *
 * \see MapObjectProperties
 * \see MAP_OBJECT_NONE
 * \see validate_marc_header()
 * \see seek_mapf_header()
 * \see validate_mapf_header()
 * \see backup_archive()
 * \see remove_archive()
 */
void set_map_objects(
    const char* filename, MapObjectProperties** properties, 
    unsigned int properties_count
);

/*!
 * \brief The prune_object() function removes unused
 *        tiles from a map.
 *
 * \param filename Map archive.
 *
 * \see MAP_OBJECT_NONE
 * \see validate_marc_header()
 * \see seek_mapf_header()
 * \see validate_mapf_header()
 * \see validate_object_properties_header() 
 * \see backup_archive()
 * \see remove_archive()
 */
void prune_objects(const char* filename);

#endif // DEF_MAPUTIL_H

