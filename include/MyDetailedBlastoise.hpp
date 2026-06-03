#ifndef MYDETAILEDBLASTOISE_HPP
#define MYDETAILEDBLASTOISE_HPP

#include <GL/glut.h>

namespace BlastoiseLab
{

class MyDetailedBlastoise
{
public:
    enum CharacterState
    {
        IDLE,
        SKILL_CASTING,
        ARM_SWING_SKILL
    };

    MyDetailedBlastoise();
    virtual ~MyDetailedBlastoise();

    void draw();
    void tickTime(long time);

    void startSkill();
    void startArmSwingSkill();
    void takeHit();

    CharacterState getState() const;
    float getSkillPower() const;
    float getArmSwingPower() const;

    bool isIdle() const { return state == IDLE; }
    float getHitShake() const { return hitShake; }
    float getIdleBob() const { return idleBob; }

    void drawBody();
    void drawShell();
    void drawHead();
    void drawTail();
    void drawArms();
    void drawLegs();
    void drawTopCannon();
    void drawCannon(float x);

private:
    CharacterState state;

    float idleClock;
    float idleBob;
    float chestPulse;
    float bodyLean;
    float headTurn;
    float headNod;
    float armSwing;
    float legSettle;
    float tailSwing;
    float cannonLift;
    float cannonRecoil;
    float skillTimer;
    float skillPower;
    float hitTimer;
    float hitShake;

    float armSwingTimer;
    float armSwingPower;
    float leftArmExtraSwing;
    float rightArmExtraSwing;
    float bodyTwist;
    int armSwingPhase;

    void drawShadow();
    void drawHydroCannon();
    void drawWaterBeam(float x, float y, float z, float length, float scale);
    void drawWaterRing(float z, float radius, float alpha);

    void drawArmSwingEffect();
    void drawWindTrail(float x, float y, float z, float power);
};

class MyVirtualWorld
{
private:
    MyDetailedBlastoise blastoise;

    bool battlePhase;
    float battleClock;

    bool hasBox;
    bool isOpening;
    float boxTime;
    bool environmentActive;

    void drawEnvironment();
    void drawBlueArena();
    void drawPokeballBox();   // Pokeball style blind box

public:
    MyVirtualWorld();
    void init();
    void draw();
    void tickTime();
    void setEnvironmentActive(bool active);
    bool isEnvironmentActive() const;

    void toggleBattlePhase();
    void castBlastoiseSkill();
    void castArmSwingSkill();
    void damageBlastoise();
    void triggerBoxOpen();
    bool isBattlePhase() const;
    bool hasBlindBox() const { return hasBox; }
};

}
#endif
