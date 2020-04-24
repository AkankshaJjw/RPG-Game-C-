#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map;
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    // TODO: Fix me!
    int ans2=(X)*(map.h)+Y;
    unsigned ans=(unsigned)(ans2); //h is height of map 
    return ans;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    // TODO: Fix me!
    return key%(BUCKET_NUM);
}

void maps_init()
{
    // TODO: Implement!    
    // Initialize hash table
    // Set width & height
    map.items = createHashTable(map_hash, BUCKET_NUM);
    map.w = WIDTH;
    map.h = HEIGHT;
}

Map* get_active_map()
{
    // There's only one map
    return &map;
}

Map* set_active_map(int m)
{
    active_map = m;
    return &map;
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    Map *map = get_active_map();
    return map->w;
}

int map_height()
{
    Map *map = get_active_map();
    return map->h;
}

int map_area()
{
    Map *map = get_active_map();
    return (map->h * map->w);
}

MapItem* get_north(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x,y-1); //north is y-1
    return (MapItem*) getItem(map->items,key);
}

MapItem* get_south(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x,y+1); //south is y+1
    return (MapItem*) getItem(map->items,key);
}

MapItem* get_east(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x+1,y); //east is x+1
    return (MapItem*) getItem(map->items,key);
}

MapItem* get_west(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x-1,y);//west is x-1
    return (MapItem*) getItem(map->items,key);
}

MapItem* get_here(int x, int y)
{
     Map *map = get_active_map();
    int key = XY_KEY(x,y);
    return (MapItem*) getItem(map->items,key);
}


void map_erase(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x,y);
    MapItem* checkItem = (MapItem*) getItem(map->items,key);
    if(checkItem->type == candies || checkItem ->type == reindeer || checkItem -> type == gifts) deleteItem(map->items,key);
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_wall2(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++) {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall2;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_elf(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = elf;
    w1->draw = draw_elf;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_santa(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = santa;
    w1->draw = draw_santa;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_candies(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = candies;
    w1->draw = draw_candies;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}



void add_rud(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = reindeer;
    w1->draw = draw_rud;
    w1->walkable = false;
    w1->data = (char*)"rud";
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_pran(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = reindeer;
    w1->draw = draw_pran;
    w1->walkable = false;
    w1->data = (char*)"pran";
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_gifts(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = gifts;
    w1->draw = draw_gifts;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}