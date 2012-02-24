#ifndef _CUNIT_H_
    #define _CUNIT_H_

enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    LEFTUP,
    LEFTDOWN,
    RIGHTUP,
    RIGHTDOWN
};

class CUnit {
        SDL_Surface *Body_Tileset;
        SDL_Surface *fogofwar;
    public:
        static std::vector<CUnit> UnitList;
        int posx;
        int posy;
        int facing;
        int maxhealth;
        int health;
        //И еще больше статов
        CUnit();
        bool OnLoad(const char *);
        void OnRender(SDL_Surface *Surf_Display);
        void OnCleanup();
        
        bool CanMove(int direction);
        void Move(int direction);
        void DrawFog(SDL_Surface *Surf_Display);
};

#endif