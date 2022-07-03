/*!
 * \ingroup game_group
 * \file map.h
 * \brief Declaration of functions related to the
 *        management of maps.
 *
 * \author Provided
 * \author Modified by H.Decoudras
 * \version 2
 */

#ifndef MAP_IS_DEF
#define MAP_IS_DEF


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
#define MAP_OBJECT_NONE -1


/*!
 * \brief Minimum number of tiles on the `y` axis
 *        of the map.
 */
#define MIN_HEIGHT 12

/*!
 * \brief Maximum number of tiles on the `y` axis
 *        of the map.
 */
#define MAX_HEIGHT 20

/*!
 * \brief Minimum number of tiles on the `x` axis
 *        of the map.
 */
#define MIN_WIDTH  16

/*!
 * \brief Maximum number of tiles on the `x` axis
 *        of the map.
 */
#define MAX_WIDTH  1024

/*!
 * \brief Air property of a tile.
 *
 * Such a tile can be crossed in all directions.
 */
#define MAP_OBJECT_AIR 0

/*!
 * \brief Semi-solid property of a tile.
 *
 * Such a tile can only be crossed from below.
 */
#define MAP_OBJECT_SEMI_SOLID 1

/*!
 * \brief Solid property of a tile.
 *
 * Such a tile cannot be crossed.
 */
#define MAP_OBJECT_SOLID 2

/*!
 * \brief Destructible property of a tile.
 *
 * Such a tile can be destroyed.
 */
#define MAP_OBJECT_DESTRUCTIBLE 4

/*!
 * \brief Collectible property of a tile.
 *
 * Such a tile is a collectible item.
 */
#define MAP_OBJECT_COLLECTIBLE 8

/*!
 * \brief Generator property of a tile.
 *
 * Such an object triggers an action.
 */
#define MAP_OBJECT_GENERATOR 16

/*!
 * \brief The map_init() function initialize a map.
 *
 * \param filename Map archive.
 */
void map_init(char* filename);


/*!
 * \brief The map_width() function gets the number
 *        of tiles on the `x` axis of the map.
 *
 * \return The number of tiles on the `x` axis of the map.
 */
unsigned int map_width(void);

/*!
 * \brief The map_height() function gets the number
 *        of tiles on the `y` axis of the map.
 *
 * \return The number of tiles on the `y` axis.
 */
unsigned int map_height(void);

/*!
 * \brief The map_objects() function gets the number
 *        of tiles used in a map.
 *
 * \return The number of tiles used in a map.
 */
unsigned int map_objects(void);


/*! 
 * \brief The map_get() function gets the type
 *        of a tile located at the designated 
 *        coordinates on the  map.
 *
 * The coordinates of a map are represented in terms 
 * of tiles and not in terms of pixels.
 *
 * The type of a tile can be a combination (a logical or)
 * of the following values:
 *  - \ref MAP_OBJECT_NONE;
 *  - \ref MAP_OBJECT_AIR;
 *  - \ref MAP_OBJECT_SEMI_SOLID;
 *  - \ref MAP_OBJECT_SOLID;
 *  - \ref MAP_OBJECT_DESTRUCTIBLE;
 *  - \ref MAP_OBJECT_COLLECTIBLE;
 *  - \ref MAP_OBJECT_GENERATOR.
 *
 * \param x Coordinate on the `x` axis.
 * \param y Coordinate on the `y` axis.
 *
 * \return The type of the a tile.
 *
 * \see MAP_OBJECT_NONE
 * \see MAP_OBJECT_AIR
 * \see MAP_OBJECT_SEMI_SOLID
 * \see MAP_OBJECT_SOLID
 * \see MAP_OBJECT_DESTRUCTIBLE
 * \see MAP_OBJECT_COLLECTIBLE
 * \see MAP_OBJECT_GENERATOR
 * \see map_set()
 */
int map_get(int x, int y);

/*!
 * \brief The map_set() function sets the type
 *        of a tile located at the designated
 *        location on the map.
 *
 * The coordinates of a map are represented in terms 
 * of tiles and not in terms of pixels.
 *
 * The type of a tile can be a combination (a logical or)
 * of the following values:
 *  - \ref MAP_OBJECT_NONE;
 *  - \ref MAP_OBJECT_AIR;
 *  - \ref MAP_OBJECT_SEMI_SOLID;
 *  - \ref MAP_OBJECT_SOLID;
 *  - \ref MAP_OBJECT_DESTRUCTIBLE;
 *  - \ref MAP_OBJECT_COLLECTIBLE;
 *  - \ref MAP_OBJECT_GENERATOR.
 *
 * \param x Coordinate on the `x` axis.
 * \param y Coordinate on the `y` axis.
 * \param object Type of a tile.
 *
 * \see MAP_OBJECT_NONE
 * \see MAP_OBJECT_AIR
 * \see MAP_OBJECT_SEMI_SOLID
 * \see MAP_OBJECT_SOLID
 * \see MAP_OBJECT_DESTRUCTIBLE
 * \see MAP_OBJECT_COLLECTIBLE
 * \see MAP_OBJECT_GENERATOR
 * \see map_get()
 */
void map_set(int x, int y, int object);

void map_set_used(int x, int y);
int map_is_used(int x, int y);


/*!
 * \brief The map_get_name() functions gets the
 *        path of a tile designated by its index.
 *
 * Each tile of a map is referenced by its index. 
 * For instance, the tile with the index `0` is 
 * "images/ground" in the following snippet of code:
 *
 * ```{.c}
 * map_object_begin(3);
 * map_object_add("images/ground.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/wall.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
 * map_object_end();
 * ```
 *
 * \param obj Index of a tile.
 *
 * \return The path of a tile.
 */
char* map_get_name(int obj);

/*!
 * \brief The map_get_frames() function gets the 
 *        number of frames of a tile designated
 *        by its index.
 *
 * Each tile of a map is referenced by its index. 
 * For instance, the tile with the index `0` is 
 * "images/ground" in the following snippet of code:
 *
 * ```{.c}
 * map_object_begin(3);
 * map_object_add("images/ground.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/wall.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
 * map_object_end();
 * ```
 * 
 * \param obj Index of a tile.
 *
 * \return The number of frames of a tile.
 */
unsigned int map_get_frames(int obj);

/*!
 * \brief The map_get_solidity() function gets the
 *        solidy property of a tile designated by
 *        its index.
 *
 * Each tile of a map is referenced by its index. 
 * For instance, the tile with the index `0` is 
 * "images/ground" in the following snippet of code:
 *
 * ```{.c}
 * map_object_begin(3);
 * map_object_add("images/ground.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/wall.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
 * map_object_end();
 * ```
 *
 * \param obj Index of a tile.
 *
 * \return This function can return the following values:
 *          - \ref MAP_OBJECT_AIR;
 *          - \ref MAP_OBJECT_SOLID;
 *          - \ref MAP_OBJECT_SEMI_SOLID.
 *
 * \see MAP_OBJECT_AIR
 * \see MAP_OBJECT_SOLID
 * \see MAP_OBJECT_SEMI_SOLID
 */
int map_get_solidity(int obj);

/*!
 * \brief The map_is_destructible() function determines
 *        if a tile designated by its index is destructible 
 *        or not.
 *
 * Each tile of a map is referenced by its index. 
 * For instance, the tile with the index `0` is 
 * "images/ground" in the following snippet of code:
 *
 * ```{.c}
 * map_object_begin(3);
 * map_object_add("images/ground.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/wall.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
 * map_object_end();
 * ```
 *
 * \param obj Index of a tile.
 *
 * \return This function can return the following values:
 *          - \p **0** if the tile is not destructible;
 *          - \p **1** if the tile is destructible.
 */
int map_is_destructible(int obj);

/*!
 * \brief The map_is_collectible() function determines
 *        if a tile designated by its index is collectible 
 *        or not.
 *
 * Each tile of a map is referenced by its index. 
 * For instance, the tile with the index `0` is 
 * "images/ground" in the following snippet of code:
 *
 * ```{.c}
 * map_object_begin(3);
 * map_object_add("images/ground.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/wall.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
 * map_object_end();
 * ```
 *
 * \param obj Index of a tile.
 *
 * \return This function can return the following values:
 *          - \p **0** if the tile is not collectible;
 *          - \p **1**` if the tile is collectible.
 */
int map_is_collectible(int obj);

int map_collectible(void);

/*!
 * \brief The map_is_floor() function determines
 *        if a tile located at the designated coordinates
 *        on the map is considered to be a ground or not.
 *
 * The coordinates of a map are represented in terms 
 * of tiles and not in terms of pixels.
 *
 * \param x Coordinate on the `x` axis.
 * \param y Coordinate on the `y` axis.
 *
 * \return This function can return the following values:
 *          - \p **0** if the tile is not collectible;
 *          - \p **1** if the tile is collectible.
 */
int map_is_floor(int x, int y);

/*!
 * \brief The map_is_floor() function determines
 *        if a tile located at the designated coordinates
 *        on the map is considered to be a tough or not.
 *
 * The coordinates of a map are represented in terms 
 * of tiles and not in terms of pixels.
 *
 * \param x Coordinate on the `x` axis.
 * \param y Coordinate on the `y` axis.
 *
 * \return This function can return the following values:
 *          - \p **0** if the tile is not tough;
 *          - \p **1** if the tile is tough.
 */
int map_is_tough(int x, int y);

/*!
 * \brief The map_is_generator() function determines
 *        if a tile designated by its index is a generator
 *        or not.
 *
 * Each tile of a map is referenced by its index. 
 * For instance, the tile with the index `0` is 
 * "images/ground" in the following snippet of code:
 *
 * ```{.c}
 * map_object_begin(3);
 * map_object_add("images/ground.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/wall.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
 * map_object_end();
 * ```
 *
 * \param obj Index of a tile.
 *
 * \return This function can return the following values:
 *          - \p **0** if the tile is not a generator;
 *          - \p **1** if the tile is a generator.
 */
int map_is_generator(int obj);


/*!
 * \brief The map_mine_add() function adds a mine
 *        located at the designated coordinates
 *        on the map.
 *
 * The coordinates of a map are represented in terms 
 * of tiles and not in terms of pixels.
 *
 * \param x Coordinate on the `x` axis.
 * \param y Coordinate on the `y` axis.
 */
void map_mine_add(int x, int y);

/*!
 * \brief The map_mine_clear() function removes a mine
 *        located at the designated coordinates
 *        on the map.
 *
 * The coordinates of a map are represented in terms 
 * of tiles and not in terms of pixels.
 *
 * \param x Coordinate on the `x` axis.
 * \param y Coordinate on the `y` axis.
 */
void map_mine_clear(int x, int y);

/*!
 * \brief The map_is_mine() function determines
 *        if a tile lacated at the designated coordinates
 *        on the map is a mine or not.
 *
 * The coordinates of a map are represented in terms 
 * of tiles and not in terms of pixels.
 *
 * Each tile of a map is referenced by its index. 
 * For instance, the tile with the index `0` is 
 * "images/ground" in the following snippet of code:
 *
 * ```{.c}
 * map_object_begin(3);
 * map_object_add("images/ground.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/wall.png", 1, MAP_OBJECT_SOLID);
 * map_object_add("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
 * map_object_end();
 * ```
 *
 * \param obj Index of a tile.
 *
 * \return This function can return the following values:
 *          - \p **0** if the tile is a not mine;
 *          - \p **1**` if the tile is a mine.
 */
int map_is_mine(int x, int y);


/*!
 * \brief The map_object_begin() function allows to
 *        register the tiles used in the map.
 *
 * A call to this function must be followed by
 * a call to the map_object_end() function after
 * the registration of the tiles used in the map.
 *
 * \param nb_objects Number of tiles on the map.
 *
 * \see map_object_add()
 * \see map_object_end()
 */
void map_object_begin(unsigned int nb_objects);

/*!
 * \brief The map_object_add() function registers a tile
 *        used in the map.
 *
 * A tile is characterized by a path an image, the number
 * of frames represented in the image and the following
 * properties:
 *
 *  - \ref MAP_OBJECT_NONE;
 *  - \ref MAP_OBJECT_AIR;
 *  - \ref MAP_OBJECT_SEMI_SOLID;
 *  - \ref MAP_OBJECT_SOLID;
 *  - \ref MAP_OBJECT_DESTRUCTIBLE;
 *  - \ref MAP_OBJECT_COLLECTIBLE;
 *  - \ref MAP_OBJECT_GENERATOR.
 *
 * The type of a tile can be a combination (a logical or)
 * of the above values.
 *
 * A call to this function must be preceded by a call
 * to the map_object_begin() function.
 *
 * \param png_file Path to the tile.
 * \param frames Number of frames of the tile.
 * \param obj_type Properties of the tile.
 *
 * \see MAP_OBJECT_NONE
 * \see MAP_OBJECT_AIR
 * \see MAP_OBJECT_SEMI_SOLID
 * \see MAP_OBJECT_SOLID
 * \see MAP_OBJECT_DESTRUCTIBLE
 * \see MAP_OBJECT_COLLECTIBLE
 * \see MAP_OBJECT_GENERATOR
 * \see map_object_begin()
 */
void map_object_add(
    char* png_file, unsigned int frames, unsigned int obj_type
);

/*!
 * \brief The map_object_end() function marks the end 
 *        of the registration of the tiles in the map.
 *
 * \see map_object_begin()
 */
void map_object_end(void);


/*!
 * \brief The map_allocate() function allocates a map.
 *
 * \param w Number of tiles on the `x` axis.
 * \param h Number of tiles on the `y` axis.
 */
void map_allocate(int w, int h);

/*!
 * \brief The map_new() function creates a sample map.
 *
 * \param width Number of tiles on the `x` axis.
 * \param height Number of tiles on the `y` axis.
 */
void map_new(unsigned int width, unsigned int height);

/*!
 * \brief The map_save() function saves the map.
 *
 * The maps are saved as a draft map archive.
 * For now, a map is made up of the following elements:
 *
 * <table>
 *  <caption>Header of a map archive</caption>
 *  <tr>
 *      <th>Description</th>
 *      <th>Size</th>
 *  </tr>
 *  <tr>
 *      <td>Signature of the archive header (see \ref MARC_HEADER)</td>
 *      <td rowspan="4">4 bytes</td>
 *  </tr>
 *  <tr>
 *      <td>Number of tiles referenced in the map</td>
 *  </tr>
 *  <tr>
 *      <td>Offset of the properties of the tiles</td>
 *  </tr>
 *  <tr>
 *      <td>Offset of the first map (only one for now)</td>
 *  </tr>
 * </table>
 *
 * <table>
 *  <caption>Tile paths</caption>
 *  <tr>
 *      <th>Description</th>
 *      <th>Size</th>
 *  </tr>
 *  <tr>
 *      <td>Path of a tile ressource</td>
 *      <td>16 bytes</td>
 *  </tr>
 * </table>
 *
 * <table>
 *  <caption>Tile properties</caption>
 *  <tr>
 *      <th>Description</th>
 *      <th>Size</th>
 *  </tr>
 *  <tr>
 *      <td>
 *          Signature of the properties of a tile 
 *          (see \ref OBJECT_PROPERTIES_HEADER)
 *      </td>
 *      <td rowspan="6">4 bytes</td>
 *  </tr>
 *  <tr>
 *      <td>Number of frames of the tile</td>
 *  </tr>
 *  <tr>
 *      <td>Solidity property of the tile</td>
 *  </tr>
 *  <tr>
 *      <td>Destructible property of the tile</td>
 *  </tr>
 *  <tr>
 *      <td>Collectible property of the tile</td>
 *  </tr>
 *  <tr>
 *      <td>Generator property of a tile</td>
 *  </tr>
 *  <tr>
 *      <td>Padding</td>
 *      <td>8 bytes</td>
 *  </tr>
 * </table>
 *
 * <table>
 *  <tr>
 *      <th>Description</th>
 *      <th>Size</th>
 *  </tr>
 *  <tr>
 *      <td>Signature of a map header (\ref MAPF_HEADER)</td>
 *      <td rowspan="4">4 bytes</td>
 *  </tr>
 *  <tr>
 *      <td>Number of tile on the \p x axis of the map</td>
 *  </tr>
 *  <tr>
 *      <td>Number of tile on the \p y axis of the map</td>
 *  </tr>
 *  <tr>
 *      <td>Number of tiles of the map</td>
 *  </tr>
 *  <tr>
 *      <td>Map data</td>
 *      <td>1 byte</td>
 *  </tr>
 * </table> 
 *
 * \param filename Map archive.
 *
 * \see map_get()
 * \see map_load()
 */
void map_save(char* filename);
    
/*!
 * \brief The map_load() function loads a map.
 *
 * For now, a map is made up of the following elements:
 *
 * <table>
 *  <caption>Header of a map archive</caption>
 *  <tr>
 *      <th>Description</th>
 *      <th>Size</th>
 *  </tr>
 *  <tr>
 *      <td>Signature of the archive header (see \ref MARC_HEADER)</td>
 *      <td rowspan="4">4 bytes</td>
 *  </tr>
 *  <tr>
 *      <td>Number of tiles referenced in the map</td>
 *  </tr>
 *  <tr>
 *      <td>Offset of the properties of the tiles</td>
 *  </tr>
 *  <tr>
 *      <td>Offset of the first map (only one for now)</td>
 *  </tr>
 * </table>
 *
 * <table>
 *  <caption>Tile paths</caption>
 *  <tr>
 *      <th>Description</th>
 *      <th>Size</th>
 *  </tr>
 *  <tr>
 *      <td>Path of a tile ressource</td>
 *      <td>16 bytes</td>
 *  </tr>
 * </table>
 *
 * <table>
 *  <caption>Tile properties</caption>
 *  <tr>
 *      <th>Description</th>
 *      <th>Size</th>
 *  </tr>
 *  <tr>
 *      <td>
 *          Signature of the properties of a tile 
 *          (see \ref OBJECT_PROPERTIES_HEADER)
 *      </td>
 *      <td rowspan="6">4 bytes</td>
 *  </tr>
 *  <tr>
 *      <td>Number of frames of the tile</td>
 *  </tr>
 *  <tr>
 *      <td>Solidity property of the tile</td>
 *  </tr>
 *  <tr>
 *      <td>Destructible property of the tile</td>
 *  </tr>
 *  <tr>
 *      <td>Collectible property of the tile</td>
 *  </tr>
 *  <tr>
 *      <td>Generator property of a tile</td>
 *  </tr>
 *  <tr>
 *      <td>Padding</td>
 *      <td>8 bytes</td>
 *  </tr>
 * </table>
 *
 * <table>
 *  <tr>
 *      <th>Description</th>
 *      <th>Size</th>
 *  </tr>
 *  <tr>
 *      <td>Signature of a map header (\ref MAPF_HEADER)</td>
 *      <td rowspan="4">4 bytes</td>
 *  </tr>
 *  <tr>
 *      <td>Number of tile on the \p x axis of the map</td>
 *  </tr>
 *  <tr>
 *      <td>Number of tile on the \p y axis of the map</td>
 *  </tr>
 *  <tr>
 *      <td>Number of tiles of the map</td>
 *  </tr>
 *  <tr>
 *      <td>Map data</td>
 *      <td>1 byte</td>
 *  </tr>
 * </table>
 *
 * \param filename Map archive.
 *
 * \see map_allocate()
 * \see map_set()
 * \see map_object_begin()
 * \see map_object_add()
 * \see map_object_end()
 * \see map_save()
 */
void map_load(char* filename);


#endif // MAP_IS_DEF

