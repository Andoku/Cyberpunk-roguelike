#ifndef _CTILE_H_
    #define _CTILE_H_
 
enum {
    TILE_TYPE_NONE = 0,
    TILE_TYPE_NORMAL = 1,
    TILE_TYPE_BLOCK = 2
};
 
class CTile {
    public:
        int TileID;
        int TypeID;
        CTile();
};

#endif
