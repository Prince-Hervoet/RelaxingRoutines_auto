#include <vector>
#include <set>
#include "soroutine.hpp"

class RoutinePool
{
    friend class Soroutine;

private:
    std::vector<Soroutine *> freeRoutines;
    std::set<Soroutine *> activeRoutines;
    int freeSize = 0;
    int activeSize = 0;

public:
    RoutinePool() {}
    ~RoutinePool()
    {
        for (int i = 0; i < freeRoutines.size(); i++)
        {
            delete freeRoutines[i];
        }
        for (auto i = activeRoutines.begin(); i != activeRoutines.end(); i++)
        {
            delete (*i);
        }
        freeRoutines.clear();
        activeRoutines.clear();
    }
    Soroutine *get();
    void giveBack(Soroutine *so);
};