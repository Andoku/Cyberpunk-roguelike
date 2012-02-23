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
    public:
        int posx;
        int posy;
        int facing;
        //И еще больше статов
        CUnit();
};

CUnit::CUnit() {
    posx = 0;
    posy = 0;
    facing = UP;
}
