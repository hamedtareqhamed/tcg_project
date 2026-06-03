#ifndef BATTLE_ARENA_HPP
#define BATTLE_ARENA_HPP

#include <GL/glut.h>
#include <cmath>

namespace battleArena {

    class MyVirtualWorld {
    private:
        bool battleArenaActive;
        float energyPulseTimer;

        // Interactive state machine tracking variables
        bool isBoxOpen;
        float damageFlashTimer;
        bool skill1Active;
        float skill1Timer;
        bool skill2Active;
        float skill2Timer;

        // Structured internal segment drawers
        void drawBattlegroundField();
        void drawStadiumSeatingStructure();
        void drawStadiumJumbotronScreen();
        void drawEnergyCoreEffects();
        void drawInteractiveProps();
        void drawSkillVFX();

        // Private primitive builders for rendering standard voxel shapes
        void drawCube(float x, float y, float z, float sx, float sy, float sz, float r, float g, float b);

    public:
        MyVirtualWorld();
        void init();
        void draw();
        void tickTime();

        // Event hooks triggered via CGLabmain keyboard matrix
        void toggleBattlePhase();
        void castBlastoiseSkill();
        void castArmSwingSkill();
        void triggerBoxOpen();
        void damageBlastoise();
    };

} // namespace battleArena

#endif // BATTLE_ARENA_HPP
