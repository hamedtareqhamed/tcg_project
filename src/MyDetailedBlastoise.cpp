// ============================================================
// MyDetailedBlastoise.cpp
// Detailed voxel Blastoise built with legacy OpenGL / GLUT.
// ============================================================

#include "MyDetailedBlastoise.hpp"

#include <cmath>
#include <cstdlib>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

namespace
{
    const float PI = 3.14159265f;

    // Pokeball colors
    const float POKEBALL_RED_R = 0.95f;
    const float POKEBALL_RED_G = 0.22f;
    const float POKEBALL_RED_B = 0.18f;

    const float POKEBALL_WHITE_R = 0.95f;
    const float POKEBALL_WHITE_G = 0.95f;
    const float POKEBALL_WHITE_B = 0.95f;

    const float POKEBALL_BLACK_R = 0.12f;
    const float POKEBALL_BLACK_G = 0.12f;
    const float POKEBALL_BLACK_B = 0.14f;

    const float POKEBALL_BUTTON_R = 0.08f;
    const float POKEBALL_BUTTON_G = 0.12f;
    const float POKEBALL_BUTTON_B = 0.55f;

    const float POKEBALL_BUTTON_GLOW_R = 0.35f;
    const float POKEBALL_BUTTON_GLOW_G = 0.55f;
    const float POKEBALL_BUTTON_GLOW_B = 1.0f;

    // Blastoise body colors
    const float BODY_R = 0.36f;
    const float BODY_G = 0.62f;
    const float BODY_B = 0.82f;

    const float BODY_DARK_R = 0.24f;
    const float BODY_DARK_G = 0.45f;
    const float BODY_DARK_B = 0.66f;

    const float BODY_LIGHT_R = 0.48f;
    const float BODY_LIGHT_G = 0.72f;
    const float BODY_LIGHT_B = 0.90f;

    const float CREAM_R = 0.86f;
    const float CREAM_G = 0.80f;
    const float CREAM_B = 0.61f;

    const float CREAM_DARK_R = 0.62f;
    const float CREAM_DARK_G = 0.55f;
    const float CREAM_DARK_B = 0.38f;

    const float SHELL_R = 0.43f;
    const float SHELL_G = 0.29f;
    const float SHELL_B = 0.18f;

    const float SHELL_DARK_R = 0.25f;
    const float SHELL_DARK_G = 0.16f;
    const float SHELL_DARK_B = 0.10f;

    const float RIM_R = 0.90f;
    const float RIM_G = 0.92f;
    const float RIM_B = 0.88f;

    const float STEEL_R = 0.58f;
    const float STEEL_G = 0.61f;
    const float STEEL_B = 0.64f;

    const float STEEL_DARK_R = 0.38f;
    const float STEEL_DARK_G = 0.40f;
    const float STEEL_DARK_B = 0.43f;

    const float WHITE_R = 0.98f;
    const float WHITE_G = 0.98f;
    const float WHITE_B = 0.94f;

    const float BLACK_R = 0.02f;
    const float BLACK_G = 0.025f;
    const float BLACK_B = 0.03f;

    const float EYE_BLUE_R = 0.08f;
    const float EYE_BLUE_G = 0.42f;
    const float EYE_BLUE_B = 0.78f;

    float clamp01(float value)
    {
        if (value < 0.0f) return 0.0f;
        if (value > 1.0f) return 1.0f;
        return value;
    }

    void beginLitColor(float r, float g, float b)
    {
        glColor3f(r, g, b);
    }

    void drawUnitCube()
    {
        glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);

        glEnd();
    }

    void drawBlock(float sx, float sy, float sz, float r, float g, float b)
    {
        glPushMatrix();
        beginLitColor(r, g, b);
        glScalef(sx, sy, sz);
        drawUnitCube();
        glPopMatrix();
    }

    void drawBlockAt(float x, float y, float z, float sx, float sy, float sz, float r, float g, float b)
    {
        glPushMatrix();
        glTranslatef(x, y, z);
        drawBlock(sx, sy, sz, r, g, b);
        glPopMatrix();
    }

    void drawVoxelOval(float y, float z, float sx, float sy, float sz, float r, float g, float b)
    {
        drawBlockAt(0.0f, y, z, sx, sy, sz, r, g, b);
        drawBlockAt(-sx * 0.47f, y + sy * 0.02f, z, sx * 0.28f, sy * 0.74f, sz * 0.92f, r, g, b);
        drawBlockAt(sx * 0.47f, y + sy * 0.02f, z, sx * 0.28f, sy * 0.74f, sz * 0.92f, r, g, b);
        drawBlockAt(0.0f, y + sy * 0.46f, z, sx * 0.76f, sy * 0.22f, sz * 0.90f, r, g, b);
        drawBlockAt(0.0f, y - sy * 0.46f, z, sx * 0.80f, sy * 0.24f, sz * 0.92f, r, g, b);
    }

    void drawClaw(float x, float y, float z, float sx, float sy, float sz)
    {
        drawBlockAt(x, y, z, sx, sy, sz, WHITE_R, WHITE_G, WHITE_B);
        drawBlockAt(x, y, z + sz * 0.48f, sx * 0.68f, sy * 0.72f, sz * 0.38f, WHITE_R, WHITE_G, WHITE_B);
    }
}

// ============================================================
// Constructor
// ============================================================
BlastoiseLab::MyDetailedBlastoise::MyDetailedBlastoise()
    : state(IDLE),
      idleClock(0.0f),
      idleBob(0.0f),
      chestPulse(0.0f),
      bodyLean(0.0f),
      headTurn(0.0f),
      headNod(0.0f),
      armSwing(0.0f),
      legSettle(0.0f),
      tailSwing(0.0f),
      cannonLift(0.0f),
      cannonRecoil(0.0f),
      skillTimer(0.0f),
      skillPower(0.0f),
      hitTimer(0.0f),
      hitShake(0.0f),
      armSwingTimer(0.0f),
      armSwingPower(0.0f),
      leftArmExtraSwing(0.0f),
      rightArmExtraSwing(0.0f),
      bodyTwist(0.0f),
      armSwingPhase(0)
{
}

BlastoiseLab::MyDetailedBlastoise::~MyDetailedBlastoise()
{
}

// ============================================================
// Animation update
// ============================================================
void BlastoiseLab::MyDetailedBlastoise::tickTime(long time)
{
    float dt = static_cast<float>(time);
    idleClock += 0.055f * dt;

    idleBob = std::sin(idleClock * 1.65f) * 0.06f;
    chestPulse = std::sin(idleClock * 1.65f) * 0.045f;
    bodyLean = std::sin(idleClock * 0.92f) * 1.3f;
    headTurn = std::sin(idleClock * 0.72f) * 2.0f;
    headNod = std::sin(idleClock * 1.45f) * 1.2f;

    if (state != ARM_SWING_SKILL)
    {
        armSwing = std::sin(idleClock * 1.15f) * 3.2f;
        leftArmExtraSwing = 0.0f;
        rightArmExtraSwing = 0.0f;
        bodyTwist = 0.0f;
    }
    else
    {
        armSwing = std::sin(idleClock * 1.15f) * 3.2f;
        leftArmExtraSwing = 0.0f;
        rightArmExtraSwing = 0.0f;
        bodyTwist = 0.0f;
    }

    legSettle = std::sin(idleClock * 1.20f) * 0.035f;
    tailSwing = std::sin(idleClock * 1.55f) * 5.5f;
    cannonLift = std::sin(idleClock * 1.00f) * 1.0f;
    cannonRecoil = 0.0f;
    skillPower = 0.0f;

    if (hitTimer > 0.0f)
    {
        hitTimer += 0.055f * dt;
        float fade = 1.0f - clamp01(hitTimer / 0.62f);
        hitShake = std::sin(hitTimer * 35.0f) * 0.22f * fade;
        bodyLean += 4.0f * fade;
        headTurn -= 6.0f * fade;
        if (hitTimer > 0.62f)
        {
            hitTimer = 0.0f;
            hitShake = 0.0f;
        }
    }

    // Water cannon skill
    if (state == SKILL_CASTING)
    {
        skillTimer += 0.045f * dt;
        float charge = clamp01(skillTimer / 0.42f);
        float fire = clamp01((skillTimer - 0.30f) / 0.58f);
        float fade = 1.0f - clamp01((skillTimer - 1.05f) / 0.36f);

        skillPower = clamp01(0.70f * charge + 0.55f * fire) * fade;
        bodyLean = -3.2f * charge;
        headTurn = 0.0f;
        headNod = -2.0f * charge;
        armSwing = -8.0f * charge;
        cannonLift = -13.0f * charge;
        cannonRecoil = std::sin(fire * PI) * 0.58f;

        if (skillTimer > 1.42f)
        {
            state = IDLE;
            skillTimer = 0.0f;
            skillPower = 0.0f;
            cannonRecoil = 0.0f;
        }
    }

    // Arm swing skill
    if (state == ARM_SWING_SKILL)
    {
        armSwingTimer += 0.04f * dt;

        if (armSwingTimer < 0.25f)
        {
            armSwingPhase = 1;
            float t = armSwingTimer / 0.25f;
            armSwingPower = t;
            leftArmExtraSwing = -90.0f * t;
            rightArmExtraSwing = -25.0f * t;
            bodyTwist = -30.0f * t;
            bodyLean = -10.0f * t;
        }
        else if (armSwingTimer < 0.42f)
        {
            armSwingPhase = 2;
            float t = (armSwingTimer - 0.25f) / 0.17f;
            leftArmExtraSwing = -90.0f + t * 210.0f;
            rightArmExtraSwing = -25.0f + t * 60.0f;
            bodyTwist = -30.0f + t * 70.0f;
            bodyLean = -10.0f + t * 18.0f;
            armSwingPower = 1.0f;
            hitShake = sin(armSwingTimer * 120.0f) * 0.08f;
        }
        else if (armSwingTimer < 0.85f)
        {
            armSwingPhase = 3;
            float t = (armSwingTimer - 0.42f) / 0.43f;
            leftArmExtraSwing = 120.0f * (1.0f - t);
            rightArmExtraSwing = 35.0f * (1.0f - t);
            bodyTwist = 40.0f * (1.0f - t);
            bodyLean = 8.0f * (1.0f - t);
            armSwingPower = 1.0f - t;
        }
        else
        {
            state = IDLE;
            armSwingTimer = 0.0f;
            armSwingPower = 0.0f;
            armSwingPhase = 0;
            leftArmExtraSwing = 0.0f;
            rightArmExtraSwing = 0.0f;
            bodyTwist = 0.0f;
        }
    }
}

// ============================================================
// Skill triggers
// ============================================================
void BlastoiseLab::MyDetailedBlastoise::startSkill()
{
    state = SKILL_CASTING;
    skillTimer = 0.0f;
    hitTimer = 0.0f;
    hitShake = 0.0f;
    armSwingPower = 0.0f;
}

void BlastoiseLab::MyDetailedBlastoise::startArmSwingSkill()
{
    state = ARM_SWING_SKILL;
    armSwingTimer = 0.0f;
    armSwingPower = 0.0f;
    hitTimer = 0.0f;
    hitShake = 0.0f;
    skillPower = 0.0f;
}

void BlastoiseLab::MyDetailedBlastoise::takeHit()
{
    if (state == IDLE)
    {
        hitTimer = 0.01f;
    }
}

BlastoiseLab::MyDetailedBlastoise::CharacterState
BlastoiseLab::MyDetailedBlastoise::getState() const
{
    return state;
}

float BlastoiseLab::MyDetailedBlastoise::getSkillPower() const
{
    return skillPower;
}

float BlastoiseLab::MyDetailedBlastoise::getArmSwingPower() const
{
    return armSwingPower;
}

// ============================================================
// Body part drawing functions
// ============================================================
void BlastoiseLab::MyDetailedBlastoise::drawBody()
{
    glPushMatrix();
    glTranslatef(0.0f, 4.30f, 0.58f);
    glRotatef(bodyLean + bodyTwist * 0.3f, 1.0f, 0.0f, 0.0f);
    glRotatef(bodyTwist * 0.2f, 0.0f, 1.0f, 0.0f);
    glScalef(1.0f + chestPulse * 0.45f, 1.0f, 1.0f);

    drawVoxelOval(0.10f, 0.0f, 4.36f, 5.58f, 3.72f, BODY_R, BODY_G, BODY_B);
    drawBlockAt(0.0f, 0.12f, 2.12f, 3.36f, 4.72f, 0.42f, CREAM_R, CREAM_G, CREAM_B);
    drawBlockAt(0.0f, 1.78f, 2.36f, 3.05f, 0.13f, 0.26f, CREAM_DARK_R, CREAM_DARK_G, CREAM_DARK_B);
    drawBlockAt(0.0f, 0.58f, 2.40f, 3.26f, 0.12f, 0.28f, CREAM_DARK_R, CREAM_DARK_G, CREAM_DARK_B);
    drawBlockAt(0.0f, -0.62f, 2.38f, 3.08f, 0.12f, 0.28f, CREAM_DARK_R, CREAM_DARK_G, CREAM_DARK_B);
    drawBlockAt(-1.78f, 0.0f, 2.28f, 0.22f, 3.78f, 0.30f, CREAM_DARK_R, CREAM_DARK_G, CREAM_DARK_B);
    drawBlockAt(1.78f, 0.0f, 2.28f, 0.22f, 3.78f, 0.30f, CREAM_DARK_R, CREAM_DARK_G, CREAM_DARK_B);
    drawBlockAt(0.0f, -2.12f, 1.65f, 2.68f, 0.64f, 1.10f, CREAM_R, CREAM_G, CREAM_B);

    glPopMatrix();
}

void BlastoiseLab::MyDetailedBlastoise::drawShell()
{
    glPushMatrix();
    glTranslatef(0.0f, 4.40f, -1.56f);
    glRotatef(bodyLean * 0.65f + bodyTwist * 0.1f, 1.0f, 0.0f, 0.0f);

    drawVoxelOval(0.38f, -0.76f, 5.92f, 5.94f, 2.92f, SHELL_R, SHELL_G, SHELL_B);
    drawBlockAt(0.0f, 0.40f, 0.82f, 5.16f, 5.18f, 0.40f, RIM_R, RIM_G, RIM_B);
    drawBlockAt(-2.78f, 0.25f, -0.26f, 0.46f, 4.62f, 2.25f, RIM_R, RIM_G, RIM_B);
    drawBlockAt(2.78f, 0.25f, -0.26f, 0.46f, 4.62f, 2.25f, RIM_R, RIM_G, RIM_B);
    drawBlockAt(0.0f, 3.08f, -0.36f, 4.12f, 0.50f, 2.16f, RIM_R, RIM_G, RIM_B);

    drawBlockAt(0.0f, 1.86f, -2.62f, 4.18f, 0.22f, 0.40f, SHELL_DARK_R, SHELL_DARK_G, SHELL_DARK_B);
    drawBlockAt(0.0f, 0.44f, -2.88f, 4.88f, 0.22f, 0.42f, SHELL_DARK_R, SHELL_DARK_G, SHELL_DARK_B);
    drawBlockAt(0.0f, -1.02f, -2.62f, 4.14f, 0.22f, 0.40f, SHELL_DARK_R, SHELL_DARK_G, SHELL_DARK_B);
    drawBlockAt(-1.38f, 0.42f, -2.96f, 0.22f, 3.44f, 0.40f, SHELL_DARK_R, SHELL_DARK_G, SHELL_DARK_B);
    drawBlockAt(1.38f, 0.42f, -2.96f, 0.22f, 3.44f, 0.40f, SHELL_DARK_R, SHELL_DARK_G, SHELL_DARK_B);

    glPopMatrix();
}

void BlastoiseLab::MyDetailedBlastoise::drawHead()
{
    glPushMatrix();
    glTranslatef(0.0f, 8.18f, 2.56f);
    glRotatef(headTurn, 0.0f, 1.0f, 0.0f);
    glRotatef(headNod, 1.0f, 0.0f, 0.0f);

    drawBlockAt(0.0f, 0.18f, 0.0f, 3.86f, 2.38f, 2.70f, BODY_R, BODY_G, BODY_B);
    drawBlockAt(0.0f, 0.88f, 0.20f, 3.40f, 0.86f, 2.54f, BODY_LIGHT_R, BODY_LIGHT_G, BODY_LIGHT_B);
    drawBlockAt(-2.04f, 0.08f, 0.05f, 0.56f, 1.62f, 2.20f, BODY_DARK_R, BODY_DARK_G, BODY_DARK_B);
    drawBlockAt(2.04f, 0.08f, 0.05f, 0.56f, 1.62f, 2.20f, BODY_DARK_R, BODY_DARK_G, BODY_DARK_B);

    drawBlockAt(0.0f, -0.54f, 1.55f, 3.18f, 0.98f, 1.28f, BODY_R, BODY_G, BODY_B);
    drawBlockAt(0.0f, -1.15f, 1.38f, 2.96f, 0.58f, 1.08f, CREAM_R, CREAM_G, CREAM_B);
    drawBlockAt(0.0f, -0.76f, 2.26f, 2.48f, 0.22f, 0.24f, BODY_DARK_R, BODY_DARK_G, BODY_DARK_B);
    drawBlockAt(-0.52f, -0.35f, 2.25f, 0.22f, 0.17f, 0.09f, BLACK_R, BLACK_G, BLACK_B);
    drawBlockAt(0.52f, -0.35f, 2.25f, 0.22f, 0.17f, 0.09f, BLACK_R, BLACK_G, BLACK_B);

    drawBlockAt(-1.52f, 1.64f, -0.10f, 0.60f, 1.48f, 0.84f, BODY_DARK_R, BODY_DARK_G, BODY_DARK_B);
    drawBlockAt(-1.72f, 2.18f, -0.08f, 0.42f, 0.74f, 0.62f, BODY_LIGHT_R, BODY_LIGHT_G, BODY_LIGHT_B);
    drawBlockAt(1.52f, 1.64f, -0.10f, 0.60f, 1.48f, 0.84f, BODY_DARK_R, BODY_DARK_G, BODY_DARK_B);
    drawBlockAt(1.72f, 2.18f, -0.08f, 0.42f, 0.74f, 0.62f, BODY_LIGHT_R, BODY_LIGHT_G, BODY_LIGHT_B);

    drawBlockAt(-0.94f, 0.22f, 1.68f, 0.80f, 0.58f, 0.10f, WHITE_R, WHITE_G, WHITE_B);
    drawBlockAt(0.94f, 0.22f, 1.68f, 0.80f, 0.58f, 0.10f, WHITE_R, WHITE_G, WHITE_B);
    drawBlockAt(-0.80f, 0.14f, 1.75f, 0.33f, 0.50f, 0.08f, EYE_BLUE_R, EYE_BLUE_G, EYE_BLUE_B);
    drawBlockAt(0.80f, 0.14f, 1.75f, 0.33f, 0.50f, 0.08f, EYE_BLUE_R, EYE_BLUE_G, EYE_BLUE_B);
    drawBlockAt(-0.70f, 0.09f, 1.82f, 0.13f, 0.25f, 0.06f, BLACK_R, BLACK_G, BLACK_B);
    drawBlockAt(0.70f, 0.09f, 1.82f, 0.13f, 0.25f, 0.06f, BLACK_R, BLACK_G, BLACK_B);
    drawBlockAt(-0.94f, 0.62f, 1.80f, 0.92f, 0.16f, 0.08f, BODY_DARK_R, BODY_DARK_G, BODY_DARK_B);
    drawBlockAt(0.94f, 0.62f, 1.80f, 0.92f, 0.16f, 0.08f, BODY_DARK_R, BODY_DARK_G, BODY_DARK_B);

    glPopMatrix();
}

void BlastoiseLab::MyDetailedBlastoise::drawCannon(float x)
{
    glPushMatrix();
    glTranslatef(x, 6.92f, -2.08f);
    glRotatef(x < 0.0f ? -11.0f : 11.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(-22.0f + cannonLift, 1.0f, 0.0f, 0.0f);

    drawBlockAt(0.0f, 0.0f, 0.0f, 1.62f, 1.48f, 1.72f, SHELL_R, SHELL_G, SHELL_B);
    drawBlockAt(0.0f, -0.22f, 0.58f, 1.86f, 0.62f, 1.28f, SHELL_DARK_R, SHELL_DARK_G, SHELL_DARK_B);
    drawBlockAt(0.0f, 0.38f, 1.00f, 1.40f, 0.52f, 1.18f, SHELL_R, SHELL_G, SHELL_B);

    drawBlockAt(0.0f, 0.0f, 2.05f - cannonRecoil, 1.10f, 1.10f, 2.58f, STEEL_R, STEEL_G, STEEL_B);
    drawBlockAt(0.0f, 0.0f, 1.12f - cannonRecoil, 1.34f, 1.34f, 0.42f, STEEL_DARK_R, STEEL_DARK_G, STEEL_DARK_B);
    drawBlockAt(0.0f, 0.0f, 2.16f - cannonRecoil, 1.22f, 1.22f, 0.20f, STEEL_DARK_R, STEEL_DARK_G, STEEL_DARK_B);
    drawBlockAt(0.0f, 0.0f, 3.20f - cannonRecoil, 1.32f, 1.32f, 0.24f, WHITE_R, WHITE_G, WHITE_B);
    drawBlockAt(0.0f, 0.0f, 3.35f - cannonRecoil, 0.80f, 0.80f, 0.10f, BLACK_R, BLACK_G, BLACK_B);

    for (int i = 0; i < 3; ++i)
    {
        float px = -0.36f + static_cast<float>(i) * 0.36f;
        drawBlockAt(px, 0.54f, 2.76f - cannonRecoil, 0.09f, 0.11f, 0.08f, BLACK_R, BLACK_G, BLACK_B);
    }

    glPopMatrix();
}

void BlastoiseLab::MyDetailedBlastoise::drawTopCannon()
{
    glPushMatrix();
    glTranslatef(0.0f, 7.38f, -2.42f);
    glRotatef(-11.0f + cannonLift, 1.0f, 0.0f, 0.0f);

    drawBlockAt(0.0f, 0.0f, 0.0f, 2.08f, 1.55f, 1.96f, SHELL_R, SHELL_G, SHELL_B);
    drawBlockAt(-0.70f, -0.28f, 0.35f, 0.62f, 0.72f, 1.70f, SHELL_DARK_R, SHELL_DARK_G, SHELL_DARK_B);
    drawBlockAt(0.70f, -0.28f, 0.35f, 0.62f, 0.72f, 1.70f, SHELL_DARK_R, SHELL_DARK_G, SHELL_DARK_B);
    drawBlockAt(0.0f, 0.42f, 0.74f, 1.62f, 0.44f, 1.38f, SHELL_R, SHELL_G, SHELL_B);

    drawBlockAt(0.0f, 0.0f, 2.58f - cannonRecoil, 1.26f, 1.26f, 3.05f, STEEL_R, STEEL_G, STEEL_B);
    drawBlockAt(0.0f, 0.0f, 1.38f - cannonRecoil, 1.54f, 1.54f, 0.48f, STEEL_DARK_R, STEEL_DARK_G, STEEL_DARK_B);
    drawBlockAt(0.0f, 0.0f, 2.55f - cannonRecoil, 1.44f, 1.44f, 0.22f, STEEL_DARK_R, STEEL_DARK_G, STEEL_DARK_B);
    drawBlockAt(0.0f, 0.0f, 3.95f - cannonRecoil, 1.52f, 1.52f, 0.28f, WHITE_R, WHITE_G, WHITE_B);
    drawBlockAt(0.0f, 0.0f, 4.10f - cannonRecoil, 0.92f, 0.92f, 0.12f, BLACK_R, BLACK_G, BLACK_B);

    glPopMatrix();
}

void BlastoiseLab::MyDetailedBlastoise::drawArms()
{
    for (int side = -1; side <= 1; side += 2)
    {
        float s = static_cast<float>(side);
        glPushMatrix();
        glTranslatef(s * 3.25f, 5.66f, 1.38f);
        glRotatef(s * 5.0f, 0.0f, 0.0f, 1.0f);

        float armSwingAngle = 0.0f;
        if (state == ARM_SWING_SKILL)
        {
            if (side < 0)
            {
                armSwingAngle = -15.0f + armSwing + leftArmExtraSwing;
            }
            else
            {
                armSwingAngle = 10.0f - armSwing + rightArmExtraSwing;
            }
        }
        else
        {
            armSwingAngle = (side < 0) ? armSwing : -armSwing;
        }
        glRotatef(armSwingAngle, 1.0f, 0.0f, 0.0f);

        drawVoxelOval(-0.55f, 0.0f, 1.90f, 1.82f, 1.66f, BODY_R, BODY_G, BODY_B);
        drawBlockAt(s * 0.22f, -1.54f, 0.16f, 1.54f, 1.72f, 1.48f, BODY_LIGHT_R, BODY_LIGHT_G, BODY_LIGHT_B);
        drawBlockAt(s * 0.24f, -2.40f, 0.30f, 1.70f, 0.20f, 1.50f, BODY_DARK_R, BODY_DARK_G, BODY_DARK_B);
        drawBlockAt(s * 0.34f, -2.96f, 0.42f, 1.42f, 1.10f, 1.26f, BODY_R, BODY_G, BODY_B);
        drawBlockAt(s * 0.48f, -3.68f, 0.80f, 1.40f, 0.62f, 1.16f, BODY_DARK_R, BODY_DARK_G, BODY_DARK_B);

        for (int i = 0; i < 4; ++i)
        {
            float offset = -0.54f + static_cast<float>(i) * 0.36f;
            drawClaw(s * (0.48f + offset), -3.78f, 1.48f, 0.24f, 0.28f, 0.58f);
        }

        glPopMatrix();
    }
}

void BlastoiseLab::MyDetailedBlastoise::drawLegs()
{
    for (int side = -1; side <= 1; side += 2)
    {
        float s = static_cast<float>(side);
        glPushMatrix();
        glTranslatef(s * 1.44f, 2.34f + legSettle, 0.66f);

        drawVoxelOval(0.10f, -0.06f, 1.86f, 2.46f, 1.74f, BODY_R, BODY_G, BODY_B);
        drawBlockAt(s * 0.10f, -1.50f, 0.20f, 1.58f, 1.68f, 1.54f, BODY_DARK_R, BODY_DARK_G, BODY_DARK_B);
        drawBlockAt(s * 0.14f, -2.40f, 0.70f, 1.64f, 0.72f, 1.82f, BODY_R, BODY_G, BODY_B);

        for (int i = 0; i < 4; ++i)
        {
            float offset = -0.66f + static_cast<float>(i) * 0.44f;
            drawClaw(offset, -2.55f, 1.84f, 0.30f, 0.30f, 0.68f);
        }

        glPopMatrix();
    }
}

void BlastoiseLab::MyDetailedBlastoise::drawTail()
{
    glPushMatrix();
    glTranslatef(0.0f, 2.40f, -3.46f);
    glRotatef(tailSwing, 0.0f, 1.0f, 0.0f);
    drawBlockAt(0.0f, 0.0f, -0.70f, 0.86f, 0.86f, 1.52f, BODY_R, BODY_G, BODY_B);
    drawBlockAt(0.0f, -0.08f, -1.60f, 0.58f, 0.58f, 0.82f, BODY_LIGHT_R, BODY_LIGHT_G, BODY_LIGHT_B);
    glPopMatrix();
}

void BlastoiseLab::MyDetailedBlastoise::drawShadow()
{
    GLboolean lightingIsOn;
    glGetBooleanv(GL_LIGHTING, &lightingIsOn);

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.05f, 0.06f, 0.07f, 0.35f);  // Darker shadow

    // Draw shadow ellipse at the bottom of the model
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, -3.2f, 0.0f);  // Shadow centered at feet level
    for (int i = 0; i <= 48; ++i)
    {
        float angle = (2.0f * PI * static_cast<float>(i)) / 48.0f;
        glVertex3f(std::cos(angle) * 5.2f, -3.2f, std::sin(angle) * 4.0f);
    }
    glEnd();

    glDisable(GL_BLEND);

    if (lightingIsOn == GL_TRUE)
    {
        glEnable(GL_LIGHTING);
    }
}

// ============================================================
// Effect drawing functions
// ============================================================
void BlastoiseLab::MyDetailedBlastoise::drawWaterBeam(float x, float y, float z, float length, float scale)
{
    drawBlockAt(x, y, z + length * 0.50f, scale, scale, length, 0.22f, 0.68f, 1.0f);
    drawBlockAt(x, y, z + length * 0.20f, scale * 1.34f, scale * 1.34f, scale * 0.24f, WHITE_R, WHITE_G, WHITE_B);
    drawBlockAt(x, y, z + length, scale * 1.55f, scale * 1.55f, scale * 0.62f, WHITE_R, WHITE_G, WHITE_B);
}

void BlastoiseLab::MyDetailedBlastoise::drawWaterRing(float z, float radius, float alpha)
{
    glColor4f(0.32f, 0.76f, 1.0f, alpha);
    float block = 0.18f + radius * 0.035f;
    drawBlockAt(radius, 8.05f, z, block, block, block, 0.32f, 0.76f, 1.0f);
    drawBlockAt(-radius, 8.05f, z, block, block, block, 0.32f, 0.76f, 1.0f);
    drawBlockAt(0.0f, 8.05f + radius * 0.34f, z, block, block, block, 0.32f, 0.76f, 1.0f);
    drawBlockAt(0.0f, 8.05f - radius * 0.34f, z, block, block, block, 0.32f, 0.76f, 1.0f);
}

void BlastoiseLab::MyDetailedBlastoise::drawHydroCannon()
{
    if (skillPower <= 0.01f)
    {
        return;
    }

    GLboolean lightingIsOn;
    glGetBooleanv(GL_LIGHTING, &lightingIsOn);

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float beamLength = 6.0f + skillPower * 12.0f;
    float beamScale = 0.32f + skillPower * 0.48f;

    glColor4f(0.24f, 0.70f, 1.0f, 0.62f * skillPower);
    drawWaterBeam(-4.04f, 8.02f, 1.02f, beamLength, beamScale);
    drawWaterBeam(4.04f, 8.02f, 1.02f, beamLength, beamScale);

    drawWaterRing(6.2f + skillPower * 3.8f, 1.0f + skillPower * 1.2f, 0.35f * skillPower);
    drawWaterRing(5.0f + skillPower * 3.0f, 1.4f + skillPower * 1.0f, 0.28f * skillPower);
    drawWaterRing(3.8f + skillPower * 2.5f, 1.7f + skillPower * 0.8f, 0.20f * skillPower);

    glDisable(GL_BLEND);

    if (lightingIsOn == GL_TRUE)
    {
        glEnable(GL_LIGHTING);
    }
}

void BlastoiseLab::MyDetailedBlastoise::drawWindTrail(float x, float y, float z, float power)
{
    int numTrails = static_cast<int>(4 + power * 8);
    for (int i = 0; i < numTrails; ++i)
    {
        float t = static_cast<float>(i) / static_cast<float>(numTrails);
        float angle = t * PI * 2.0f;
        float radius = 0.6f + power * 0.8f;
        float alpha = (1.0f - t) * 0.5f * power;

        glColor4f(0.7f, 0.85f, 1.0f, alpha);
        drawBlockAt(x + std::cos(angle) * radius,
                    y + std::sin(angle) * radius,
                    z - t * 1.8f * power,
                    0.12f + power * 0.15f,
                    0.12f + power * 0.15f,
                    0.12f + power * 0.15f,
                    0.7f, 0.85f, 1.0f);
    }
}

void BlastoiseLab::MyDetailedBlastoise::drawArmSwingEffect()
{
    if (armSwingPower <= 0.01f)
    {
        return;
    }

    GLboolean lightingIsOn;
    glGetBooleanv(GL_LIGHTING, &lightingIsOn);

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float effectPower = armSwingPower * 1.2f;
    float pulse = std::sin(armSwingTimer * 30.0f);

    glColor4f(0.9f, 0.95f, 1.0f, 0.45f * effectPower);
    drawWindTrail(-2.8f, 6.5f, 2.2f, effectPower);

    for(int i = 0; i < 8; i++)
    {
        float offset = i * 0.12f;
        glColor4f(0.8f, 0.95f, 1.0f, (0.35f - offset) * effectPower);
        drawBlockAt(-2.8f, 6.5f, 2.0f - offset * 4.0f, 0.35f, 0.35f, 1.2f, 0.8f, 0.95f, 1.0f);
    }

    drawWindTrail(2.8f, 6.5f, 2.2f, effectPower);

    float shockRadius = 1.2f + effectPower * 2.5f;
    glColor4f(0.85f, 0.9f, 1.0f, 0.28f * effectPower * (0.5f + 0.5f * pulse));
    drawBlockAt(0.0f, 6.2f, 2.0f, shockRadius, shockRadius * 0.3f, shockRadius * 0.8f, 0.85f, 0.9f, 1.0f);

    for (int i = 0; i < 12; ++i)
    {
        float angle = (armSwingTimer * 50.0f + static_cast<float>(i) * 30.0f) * PI / 180.0f;
        float dist = 2.5f + effectPower * 2.0f;
        float alpha = 0.3f * effectPower * (0.7f + 0.3f * std::sin(angle * 8.0f));
        glColor4f(0.8f, 0.88f, 1.0f, alpha);
        drawBlockAt(std::cos(angle) * dist, 6.0f + std::sin(angle) * dist * 0.5f, 1.5f,
                    0.08f, 0.08f, 0.5f + effectPower * 1.0f, 0.8f, 0.88f, 1.0f);
    }

    glDisable(GL_BLEND);

    if (lightingIsOn == GL_TRUE)
    {
        glEnable(GL_LIGHTING);
    }

    if (armSwingPhase == 2)
    {
        float r = 1.0f + effectPower * 4.0f;
        drawWaterRing(2.5f, r, 0.5f * effectPower);
    }
}

void BlastoiseLab::MyDetailedBlastoise::draw()
{
    GLboolean lightingWasOn;
    glGetBooleanv(GL_LIGHTING, &lightingWasOn);

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat noSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, noSpecular);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);

    glPushMatrix();

    // Scale up Blastoise
    glScalef(2.5f, 2.5f, 2.5f);

    // Position adjustment - feet at Y = 0 (after scaling)
    // The legs bottom is around Y = -2.5f in local coordinates
    // After scaling by 2.5, feet are at -6.25, so we need to offset up
    glTranslatef(hitShake, idleBob, 0.0f);

    drawShadow();
    drawShell();
    drawBody();
    drawHead();
    drawCannon(-3.35f);
    drawCannon(3.35f);
    drawArms();
    drawLegs();
    drawTail();
    drawHydroCannon();
    drawArmSwingEffect();

    glPopMatrix();

    // glDisable(GL_COLOR_MATERIAL);
    // glDisable(GL_NORMALIZE);

    if (lightingWasOn == GL_TRUE)
    {
        glEnable(GL_LIGHTING);
    }
    else
    {
        glDisable(GL_LIGHTING);
    }
}

// ============================================================
// MyVirtualWorld implementation
// ============================================================

// Constructor
BlastoiseLab::MyVirtualWorld::MyVirtualWorld()
    : battlePhase(false),
      battleClock(0.0f),
      hasBox(true),
      isOpening(false),
      boxTime(0.0f)
{
}

// Initialization
void BlastoiseLab::MyVirtualWorld::init()
{
    battlePhase = false;
    battleClock = 0.0f;
    hasBox = true;
    isOpening = false;
    boxTime = 0.0f;
}

void BlastoiseLab::MyVirtualWorld::drawBlueArena()
{
    GLboolean lightingIsOn;
    glGetBooleanv(GL_LIGHTING, &lightingIsOn);

    glDisable(GL_LIGHTING);

    int gridSize = 12;
    float blockSize = 5.5f;

    for (int x = -gridSize; x <= gridSize; x++)
    {
        for (int z = -gridSize; z <= gridSize; z++)
        {
            // Slight wave for visual effect
            float wave = sin(x * 1.0f + battleClock * 1.5f) * cos(z * 1.0f) * 0.15f;
            float baseHeight = 1.2f + wave;  // Thicker ground

            glPushMatrix();
            glTranslatef(x * blockSize, -baseHeight / 2.0f, z * blockSize);
            glScalef(blockSize * 0.95f, baseHeight, blockSize * 0.95f);

            // Color based on position and wave
            float blueIntensity = 0.5f + 0.2f * sin(x * 0.8f + z * 0.8f);
            if (wave > 0.08f)
                glColor3f(0.25f, 0.60f, 0.90f);
            else if (wave < -0.08f)
                glColor3f(0.10f, 0.35f, 0.70f);
            else
                glColor3f(0.18f + blueIntensity * 0.1f,
                          0.50f + blueIntensity * 0.1f,
                          0.82f + blueIntensity * 0.08f);

            glutSolidCube(1.0);
            glPopMatrix();

            // Reflective sparkles
            if ((x + z) % 4 == 0 && (abs(x) % 2 == 0 || abs(z) % 2 == 0))
            {
                glPushMatrix();
                float sparkle = sin(battleClock * 10.0f + x * 3.0f + z * 3.0f);
                if (sparkle > 0.4f)
                {
                    glTranslatef(x * blockSize, 0.5f + wave * 0.5f, z * blockSize);
                    glScalef(0.28f, 0.12f, 0.28f);
                    glColor3f(0.9f, 0.95f, 1.0f);
                    glutSolidCube(1.0);
                }
                glPopMatrix();
            }
        }
    }

    if (lightingIsOn == GL_TRUE)
    {
        glEnable(GL_LIGHTING);
    }
}

void BlastoiseLab::MyVirtualWorld::drawEnvironment()
{
    glPushMatrix();

    // Ground plane fixed at Y = -8.0f (bottom of ground blocks at -8.0 - 1.2 = -9.2)
    glTranslatef(0.0f, -8.0f, 0.0f);

    drawBlueArena();

    // Floating particles
    int numDroplets = 100;
    float arenaSize = 65.0f;

    GLboolean lightingIsOn;
    glGetBooleanv(GL_LIGHTING, &lightingIsOn);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < numDroplets; i++)
    {
        float startX = sin(i * 13.4f) * arenaSize * 0.55f;
        float startZ = cos(i * 9.7f) * arenaSize * 0.55f;
        float speed = 4.0f + (i % 8) * 1.2f;

        float riseY = fmod(battleClock * speed + (i * 2.8f), 30.0f);
        float swayX = sin(riseY * 0.25f + i) * 2.2f;
        float swayZ = cos(riseY * 0.2f + i) * 1.8f;

        float size = (1.0f - (riseY / 30.0f)) * (0.3f + (i % 4) * 0.2f);
        size *= (0.7f + 0.5f * sin(battleClock * 15.0f + i));

        glPushMatrix();
        glTranslatef(startX + swayX, riseY + 1.0f, startZ + swayZ);
        glRotatef(battleClock * 80.0f + i * 45.0f, 1.0f, 1.0f, 0.5f);
        glScalef(size, size, size);

        float alpha = 0.5f * (1.0f - riseY / 30.0f);
        if (i % 4 == 0)
            glColor4f(0.7f, 0.85f, 0.98f, alpha);
        else if (i % 2 == 0)
            glColor4f(0.4f, 0.7f, 0.92f, alpha);
        else
            glColor4f(0.25f, 0.55f, 0.85f, alpha);

        glutSolidCube(1.0);
        glPopMatrix();
    }

    glDisable(GL_BLEND);

    if (lightingIsOn == GL_TRUE)
    {
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();
}

void BlastoiseLab::MyVirtualWorld::drawPokeballBox()
{
    glPushMatrix();

    float boxSize = 22.0f;  // Larger box
    float thickness = 1.2f;
    float wobbleAngle = 0.0f;
    float shakeX = 0.0f;
    float shakeZ = 0.0f;
    float pi = 3.14159265f;

    // Box wobble animation before opening
    if (boxTime > 0.5f && boxTime < 1.0f)
    {
        wobbleAngle = sin((boxTime - 0.5f) * pi * 2.0f) * 18.0f;
    }
    else if (boxTime > 1.5f && boxTime < 2.0f)
    {
        wobbleAngle = sin((boxTime - 1.5f) * pi * 2.0f) * -18.0f;
    }
    else if (boxTime > 2.5f && boxTime < 3.1f)
    {
        wobbleAngle = sin((boxTime - 2.5f) * pi * 1.666f) * 25.0f;
        shakeX = sin((boxTime - 2.5f) * pi * 12.0f) * 0.5f;
        shakeZ = cos((boxTime - 2.5f) * pi * 10.0f) * 0.3f;
    }

    // Burst animation for box panels
    float burstAngle = 0.0f;
    float topFlyY = 0.0f;
    float topSpinX = 0.0f;
    float topSpinZ = 0.0f;
    float sinkY = 0.0f;

    if (boxTime >= 2.0f)
    {
        float progress = (boxTime - 2.0f);
        burstAngle = progress * 90.0f;
        topFlyY = progress * 35.0f;
        topSpinX = progress * 200.0f;
        topSpinZ = progress * 180.0f;
        sinkY = progress * -10.0f;
    }

    // Position box on ground (same ground level as Blastoise will stand)
    glTranslatef(shakeX, -7.0f + sinkY, shakeZ);
    glRotatef(wobbleAngle, 0.0f, 0.0f, 1.0f);

    // Draw the 4 side panels with Pokeball design
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();

        bool isFront = false;
        if (i == 0)
        {
            glTranslatef(0.0f, 0.0f, boxSize / 2.0f);
            glRotatef(burstAngle, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, boxSize / 2.0f, 0.0f);
            isFront = true;
        }
        else if (i == 1)
        {
            glTranslatef(0.0f, 0.0f, -boxSize / 2.0f);
            glRotatef(-burstAngle, 1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, boxSize / 2.0f, 0.0f);
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        }
        else if (i == 2)
        {
            glTranslatef(-boxSize / 2.0f, 0.0f, 0.0f);
            glRotatef(burstAngle, 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, boxSize / 2.0f, 0.0f);
            glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        }
        else if (i == 3)
        {
            glTranslatef(boxSize / 2.0f, 0.0f, 0.0f);
            glRotatef(-burstAngle, 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, boxSize / 2.0f, 0.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        }

        // Upper half - RED (Pokeball top)
        glPushMatrix();
        glTranslatef(0.0f, boxSize * 0.25f + 0.25f, 0.0f);
        glScalef(boxSize, boxSize * 0.5f - 0.5f, thickness);
        glColor3f(POKEBALL_RED_R, POKEBALL_RED_G, POKEBALL_RED_B);
        glutSolidCube(1.0);
        glPopMatrix();

        // Black center band
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        glScalef(boxSize + 0.3f, 1.2f, thickness + 0.2f);
        glColor3f(POKEBALL_BLACK_R, POKEBALL_BLACK_G, POKEBALL_BLACK_B);
        glutSolidCube(1.0);
        glPopMatrix();

        // Lower half - WHITE (Pokeball bottom)
        glPushMatrix();
        glTranslatef(0.0f, -boxSize * 0.25f - 0.25f, 0.0f);
        glScalef(boxSize, boxSize * 0.5f - 0.5f, thickness);
        glColor3f(POKEBALL_WHITE_R, POKEBALL_WHITE_G, POKEBALL_WHITE_B);
        glutSolidCube(1.0);
        glPopMatrix();

        // Front panel special details (Button)
        if (isFront)
        {
            // Button base ring (black)
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, thickness * 0.5f + 0.15f);
            glScalef(5.2f, 1.8f, 0.5f);
            glColor3f(POKEBALL_BLACK_R, POKEBALL_BLACK_G, POKEBALL_BLACK_B);
            glutSolidCube(1.0);
            glPopMatrix();

            // Glowing center button
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, thickness * 0.5f + 0.45f);

            if (boxTime < 3.6f)
            {
                if (boxTime > 3.1f && boxTime < 3.6f)
                {
                    float throb = 1.0f + 0.5f * fabs(sin(boxTime * 35.0f));
                    glColor3f(POKEBALL_BUTTON_GLOW_R * throb,
                              POKEBALL_BUTTON_GLOW_G * throb,
                              POKEBALL_BUTTON_GLOW_B * throb);
                    glScalef(3.0f * throb, 2.2f * throb, 0.35f);
                }
                else
                {
                    float throb = 1.0f;
                    if ((boxTime > 1.0f && boxTime < 1.3f) || (boxTime > 2.0f && boxTime < 2.3f))
                    {
                        throb = 1.4f;
                        glColor3f(0.45f, 0.65f, 1.0f);
                    }
                    else
                    {
                        glColor3f(POKEBALL_BUTTON_R, POKEBALL_BUTTON_G, POKEBALL_BUTTON_B);
                    }
                    glScalef(3.0f * throb, 2.2f * throb, 0.35f);
                }
            }
            else
            {
                glColor3f(0.15f, 0.20f, 0.40f);
                glScalef(3.0f, 2.2f, 0.35f);
            }

            glutSolidCube(1.0);
            glPopMatrix();

            // Button highlight
            glPushMatrix();
            glTranslatef(0.0f, 0.15f, thickness * 0.5f + 0.5f);
            glScalef(2.2f, 0.6f, 0.2f);
            glColor3f(0.85f, 0.85f, 0.9f);
            glutSolidCube(1.0);
            glPopMatrix();
        }
        glPopMatrix();
    }

    // Bottom panel - White
    glPushMatrix();
    glTranslatef(0.0f, thickness / 2.0f, 0.0f);
    glScalef(boxSize, thickness, boxSize);
    glColor3f(POKEBALL_WHITE_R, POKEBALL_WHITE_G, POKEBALL_WHITE_B);
    glutSolidCube(1.0);
    glPopMatrix();

    // Top panel - Red (flies upward when opening)
    glPushMatrix();
    glTranslatef(0.0f, boxSize + topFlyY, 0.0f);
    glRotatef(topSpinX, 1.0f, 0.0f, 0.0f);
    glRotatef(topSpinZ, 0.0f, 0.0f, 1.0f);
    glScalef(boxSize + 0.5f, thickness, boxSize + 0.5f);
    glColor3f(POKEBALL_RED_R, POKEBALL_RED_G, POKEBALL_RED_B);
    glutSolidCube(1.0);
    glPopMatrix();

    // Black band around the middle edge
    glPushMatrix();
    glTranslatef(0.0f, boxSize / 2.0f, 0.0f);
    glScalef(boxSize + 0.6f, 1.0f, boxSize + 0.6f);
    glColor3f(POKEBALL_BLACK_R, POKEBALL_BLACK_G, POKEBALL_BLACK_B);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}

void BlastoiseLab::MyVirtualWorld::draw()
{
    drawEnvironment();

    if (hasBox)
    {
        drawPokeballBox();
    }

    float blastoiseScale = 1.0f;
    float blastoiseYOffset = 0.0f;

    if (hasBox)
    {
        if (boxTime < 3.6f)
        {
            blastoiseScale = 0.0f;
        }
        else if (boxTime < 4.2f)
        {
            blastoiseScale = (boxTime - 3.6f) * 2.0f;
            blastoiseYOffset = (1.0f - blastoiseScale) * 8.0f;  // Drop down as it grows
        }
        else if (boxTime < 4.8f)
        {
            blastoiseScale = 1.2f - (boxTime - 4.2f) * 0.333f;
            blastoiseYOffset = 0.0f;
        }
        else
        {
            blastoiseScale = 1.0f;
            blastoiseYOffset = 0.0f;
        }
    }

    if (blastoiseScale > 0.0f)
    {
        glPushMatrix();

        // Position Blastoise on the ground
        // The ground is at Y = -7.0f, Blastoise feet should touch it
        glTranslatef(0.0f, -7.0f + blastoiseYOffset, 0.0f);

        // Apply scale from box opening animation
        glScalef(blastoiseScale, blastoiseScale, blastoiseScale);

        blastoise.draw();
        glPopMatrix();
    }
}

// MyVirtualWorld tickTime function
void BlastoiseLab::MyVirtualWorld::tickTime()
{
    blastoise.tickTime(1);
    battleClock += 0.08f;

    if (isOpening && hasBox)
    {
        boxTime += 0.006f;
        if (boxTime >= 7.0f)
        {
            hasBox = false;
        }
    }
}

// Control functions
void BlastoiseLab::MyVirtualWorld::toggleBattlePhase()
{
    battlePhase = !battlePhase;
}

void BlastoiseLab::MyVirtualWorld::castBlastoiseSkill()
{
    battlePhase = true;
    blastoise.startSkill();
}

void BlastoiseLab::MyVirtualWorld::castArmSwingSkill()
{
    battlePhase = true;
    blastoise.startArmSwingSkill();
}

void BlastoiseLab::MyVirtualWorld::damageBlastoise()
{
    blastoise.takeHit();
}

void BlastoiseLab::MyVirtualWorld::triggerBoxOpen()
{
    if (hasBox && !isOpening)
    {
        isOpening = true;
    }
}

bool BlastoiseLab::MyVirtualWorld::isBattlePhase() const
{
    return battlePhase;
}
