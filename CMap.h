#ifndef _CMAP_H_
    #define _CMAP_H_

class CMap {
    
    public:
        static CMap MapControl;
        int LeftEdgePositionX, LeftEdgePositionY; //РАССТРЭЛЯТЬ!
        int mapsizex, mapsizey;
        std::vector< std::vector<CTile> > Map;
        
    private:
        SDL_Surface *Surf_Tileset;
        std::vector<CTile> TileList;
        void DrawTiles(SDL_Surface *Surf_Display);

    public:
        CMap();
        bool OnLoad(const char *File);
        void OnRender(SDL_Surface *Surf_Display, int HeroX, int HeroY);
        void OnCleanup();
};

#endif
