#include "battleArena.hpp"
#include <iostream>
#include <cmath>

namespace battleArena {

    MyVirtualWorld::MyVirtualWorld() {
        battleArenaActive = true;
        energyPulseTimer = 0.0f;
        isBoxOpen = false;
        damageFlashTimer = 0.0f;
        skill1Active = false;
        skill1Timer = 0.0f;
        skill2Active = false;
        skill2Timer = 0.0f;
    }

    void MyVirtualWorld::init() {
        battleArenaActive = true;
        energyPulseTimer = 0.0f;
        isBoxOpen = false;
        damageFlashTimer = 0.0f;
        skill1Active = false;
        skill2Active = false;
    }

    void MyVirtualWorld::toggleBattlePhase() {
        battleArenaActive = !battleArenaActive;
    }

    void MyVirtualWorld::castBlastoiseSkill() {
        std::cout << "Skill 1 activated: Blastoise casts Hydro Pump stream!\n";
        skill1Active = true;
        skill1Timer = 0.0f;
    }

    void MyVirtualWorld::castArmSwingSkill()  {
        std::cout << "Skill 2 activated: Floating platform rotating system initiated!\n";
        skill2Active = true;
        skill2Timer = 0.0f;
    }

    void MyVirtualWorld::triggerBoxOpen()     {
        std::cout << "Item Box crate storage module toggled!\n";
        isBoxOpen = !isBoxOpen;
    }

    void MyVirtualWorld::damageBlastoise()    {
        std::cout << "Direct Hit registered! Initializing flash overlay sequence!\n";
        damageFlashTimer = 1.0f;
    }

    void MyVirtualWorld::tickTime() {
        if (battleArenaActive) {
            energyPulseTimer += 0.05f;
            if (energyPulseTimer > 2.0f * 3.14159f) energyPulseTimer = 0.0f;
        }
        if (skill1Active) {
            skill1Timer += 0.03f;
            if (skill1Timer > 1.0f) skill1Active = false;
        }
        if (skill2Active) {
            skill2Timer += 0.06f;
            if (skill2Timer > 2.0f * 3.14159f) skill2Active = false;
        }
        if (damageFlashTimer > 0.0f) {
            damageFlashTimer -= 0.04f;
        }
    }

    void MyVirtualWorld::drawCube(float x, float y, float z, float sx, float sy, float sz, float r, float g, float b) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(sx, sy, sz);
        glColor3f(r, g, b);

        glBegin(GL_QUADS);
            // Top Face
            glVertex3f( 0.5f,  0.5f, -0.5f); glVertex3f(-0.5f,  0.5f, -0.5f);
            glVertex3f(-0.5f,  0.5f,  0.5f); glVertex3f( 0.5f,  0.5f,  0.5f);
            // Bottom Face
            glVertex3f( 0.5f, -0.5f,  0.5f); glVertex3f(-0.5f, -0.5f,  0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f( 0.5f, -0.5f, -0.5f);
            // Front Face
            glVertex3f( 0.5f,  0.5f,  0.5f); glVertex3f(-0.5f,  0.5f,  0.5f);
            glVertex3f(-0.5f, -0.5f,  0.5f); glVertex3f( 0.5f, -0.5f,  0.5f);
            // Back Face
            glVertex3f( 0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f,  0.5f, -0.5f); glVertex3f( 0.5f,  0.5f, -0.5f);
            // Left Face
            glVertex3f(-0.5f,  0.5f,  0.5f); glVertex3f(-0.5f,  0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, -0.5f,  0.5f);
            // Right Face
            glVertex3f( 0.5f,  0.5f, -0.5f); glVertex3f( 0.5f,  0.5f,  0.5f);
            glVertex3f( 0.5f, -0.5f,  0.5f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glEnd();
        glPopMatrix();
    }

    void MyVirtualWorld::draw() {
        drawStadiumSeatingStructure();
        drawStadiumJumbotronScreen();   // Render the large Poké Ball screen display
        drawBattlegroundField();
        drawInteractiveProps();
        drawSkillVFX();

        if (battleArenaActive) {
            drawEnergyCoreEffects();
        }
    }

    void MyVirtualWorld::drawBattlegroundField() {
        float floatY = 6.0f;
        float fieldHalfWidth = 24.0f;

        // 1. ALTERNATING PREMIUM GRASS STRIPS
        int numStrips = 26;
        float stripLength = 3.0f;
        float startZ = -39.0f;

        for (int i = 0; i < numStrips; ++i) {
            float zPos = startZ + (i * stripLength);

            float baseR = (i % 2 == 0) ? 0.32f : 0.42f;
            float baseG = (i % 2 == 0) ? 0.72f : 0.84f;
            float baseB = (i % 2 == 0) ? 0.18f : 0.24f;

            if (damageFlashTimer > 0.0f) {
                baseR += damageFlashTimer * 0.45f;
                baseG -= damageFlashTimer * 0.35f;
                baseB -= damageFlashTimer * 0.15f;
            }

            drawCube(0.0f, floatY, zPos + (stripLength * 0.5f), fieldHalfWidth * 2.0f, 0.4f, stripLength, baseR, baseG, baseB);
        }

        // MODERN SCI-FI METALLIC OUTER TRIM & CASING
        drawCube(0.0f, floatY, 0.0f, (fieldHalfWidth * 2.0f) + 1.6f, 0.42f, 79.6f, 0.18f, 0.20f, 0.22f);
        float cyanPulse = (sin(energyPulseTimer) * 0.2f) + 0.8f;
        drawCube(0.0f, floatY + 0.02f, 0.0f, (fieldHalfWidth * 2.0f) + 0.6f, 0.4f, 78.6f, 0.0f, 0.8f * cyanPulse, 0.9f * cyanPulse);
        drawCube(0.0f, floatY + 0.03f, 0.0f, fieldHalfWidth * 2.0f, 0.4f, 78.0f, 0.18f, 0.20f, 0.22f);

        // DETAILED FLOATING UNDER-HULL ASSEMBLY
        drawCube(0.0f, floatY - 0.6f, 0.0f, (fieldHalfWidth * 2.0f) - 1.0f, 0.8f, 76.0f, 0.25f, 0.28f, 0.32f);
        drawCube(0.0f, floatY - 1.4f, 0.0f, (fieldHalfWidth * 2.0f) - 6.0f, 0.8f, 68.0f, 0.15f, 0.16f, 0.19f);
        drawCube(0.0f, floatY - 2.2f, 0.0f, 20.0f, 0.8f, 44.0f, 0.08f, 0.09f, 0.11f);

        // TRANS-PANEL SECURITY SHIELD BARRIERS
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawCube(-fieldHalfWidth, floatY + 1.2f, -38.5f, 0.3f, 2.0f, 8.0f, 0.2f, 0.6f, 1.0f);
        drawCube( fieldHalfWidth, floatY + 1.2f, -38.5f, 0.3f, 2.0f, 8.0f, 0.2f, 0.6f, 1.0f);
        drawCube(-fieldHalfWidth, floatY + 1.2f,  38.5f, 0.3f, 2.0f, 8.0f, 0.2f, 0.6f, 1.0f);
        drawCube( fieldHalfWidth, floatY + 1.2f,  38.5f, 0.3f, 2.0f, 8.0f, 0.2f, 0.6f, 1.0f);
        glDisable(GL_BLEND);

        // 2. WHITE BOUNDARY LINES
        drawCube(-fieldHalfWidth, floatY + 0.22f, 0.0f, 0.4f, 0.02f, 78.0f, 0.98f, 0.98f, 0.98f);
        drawCube( fieldHalfWidth, floatY + 0.22f, 0.0f, 0.4f, 0.02f, 78.0f, 0.98f, 0.98f, 0.98f);
        drawCube(0.0f, floatY + 0.22f, -39.0f, fieldHalfWidth * 2.0f, 0.02f, 0.4f, 0.98f, 0.98f, 0.98f);
        drawCube(0.0f, floatY + 0.22f,  39.0f, fieldHalfWidth * 2.0f, 0.02f, 0.4f, 0.98f, 0.98f, 0.98f);
        drawCube(0.0f, floatY + 0.23f, 0.0f, fieldHalfWidth * 2.0f, 0.02f, 0.8f, 0.98f, 0.98f, 0.98f);

        // 3. PIXEL-ART POKEBALL GRAPHIC (On Field)
        float cellSz = 1.0f;
        int ballRadiusInCells = 10;

        for (int xCell = -ballRadiusInCells; xCell <= ballRadiusInCells; ++xCell) {
            for (int zCell = -ballRadiusInCells; zCell <= ballRadiusInCells; ++zCell) {
                float dist = sqrt(static_cast<float>(xCell * xCell + zCell * zCell));

                if (dist <= ballRadiusInCells) {
                    float xc = xCell * cellSz;
                    float zc = zCell * cellSz;

                    if (dist <= 2.2f) {
                        drawCube(xc, floatY + 0.26f, zc, cellSz, 0.03f, cellSz, 0.98f, 0.98f, 0.98f);
                    } else if (dist <= 3.6f) {
                        drawCube(xc, floatY + 0.25f, zc, cellSz, 0.03f, cellSz, 0.05f, 0.05f, 0.06f);
                    } else if (abs(zc) < 1.0f) {
                        drawCube(xc, floatY + 0.24f, zc, cellSz, 0.03f, cellSz, 0.05f, 0.05f, 0.06f);
                    } else if (zc < 0.0f) {
                        drawCube(xc, floatY + 0.23f, zc, cellSz, 0.03f, cellSz, 0.88f, 0.12f, 0.12f);
                    } else {
                        drawCube(xc, floatY + 0.23f, zc, cellSz, 0.03f, cellSz, 0.94f, 0.94f, 0.94f);
                    }
                }
            }
        }
    }

    void MyVirtualWorld::drawStadiumSeatingStructure() {
        int seatingLevels = 14;
        float baseLimitX = 50.0f;
        float baseLimitZ = 64.0f;

        float riserHeight = 1.5f;
        float treadDepth = 2.5f;

        drawCube(0.0f, -0.2f, 0.0f, 240.0f, 0.2f, 240.0f, 0.08f, 0.09f, 0.11f);

        for (int level = 0; level < seatingLevels; ++level) {
            float currentRadiusX = baseLimitX + (level * treadDepth);
            float currentRadiusZ = baseLimitZ + (level * treadDepth);
            float currentElevationY = 0.0f + (level * riserHeight);

            int stepDeg = (level > 6) ? 3 : 4;

            for (int deg = 0; deg < 360; deg += stepDeg) {
                if ((deg >= 40 && deg <= 50) || (deg >= 130 && deg <= 140) ||
                    (deg >= 220 && deg <= 230) || (deg >= 310 && deg <= 320)) {

                    drawCube(cos(deg*3.14159f/180.0f)*currentRadiusX, currentElevationY, sin(deg*3.14159f/180.0f)*currentRadiusZ, 5.4f, riserHeight * 0.5f, 5.4f, 0.16f, 0.18f, 0.20f);
                    continue;
                }

                float rad = deg * 3.14159f / 180.0f;
                float blockX = cos(rad) * currentRadiusX;
                float blockZ = sin(rad) * currentRadiusZ;

                float rColor, gColor, bColor;
                if (level < 4) {
                    rColor = 0.10f; gColor = 0.25f; bColor = 0.60f;
                } else if (level < 9) {
                    rColor = 0.15f; gColor = 0.55f; bColor = 0.70f;
                } else {
                    rColor = 0.90f; gColor = 0.92f; bColor = 0.95f;
                }

                if ((deg / stepDeg) % 3 == 0) {
                    rColor *= 0.85f; gColor *= 0.85f; bColor *= 0.85f;
                } else if ((deg / stepDeg) % 3 == 1) {
                    rColor = (rColor + 0.1f > 1.0f) ? 1.0f : rColor + 0.1f;
                    gColor = (gColor + 0.1f > 1.0f) ? 1.0f : gColor + 0.1f;
                    bColor = (bColor + 0.1f > 1.0f) ? 1.0f : bColor + 0.1f;
                }

                drawCube(blockX, currentElevationY, blockZ, 5.4f, riserHeight, 5.4f, 0.38f, 0.40f, 0.44f);
                drawCube(blockX, currentElevationY + (riserHeight * 0.5f) + 0.20f, blockZ, 4.2f, 0.35f, 2.0f, rColor, gColor, bColor);
                drawCube(blockX * 1.02f, currentElevationY + (riserHeight * 0.5f) + 0.6f, blockZ * 1.02f, 4.2f, 0.6f, 0.4f, rColor * 0.9f, gColor * 0.9f, bColor * 0.9f);
            }
        }

        // HIGH MOUNTED SPECTATOR COMMAND DECK
        float deckZ = baseLimitZ + (seatingLevels * treadDepth) + 5.0f;
        float deckY = seatingLevels * riserHeight;
        drawCube(0.0f, deckY, deckZ, 36.0f, 1.2f, 16.0f, 0.16f, 0.18f, 0.22f);

        // Security Fence Guards & Handrails
        drawCube(-18.0f, deckY + 1.2f, deckZ, 0.4f, 2.4f, 16.0f, 0.70f, 0.72f, 0.75f);
        drawCube( 18.0f, deckY + 1.2f, deckZ, 0.4f, 2.4f, 16.0f, 0.70f, 0.72f, 0.75f);
        drawCube(0.0f, deckY + 1.2f, deckZ - 8.0f, 36.0f, 2.4f, 0.4f, 0.70f, 0.72f, 0.75f);
    }

    void MyVirtualWorld::drawStadiumJumbotronScreen() {
        // OVERHEAD 4-SIDED HANGING JUMBOTRON (Optimized size & lifted higher)
        float screenX = 0.0f;
        float screenY = 46.0f; // Lifted up from 38.0f to clear vertical battlefield space
        float screenZ = 0.0f;

        // 1. CEILING STRUCTURAL GRID & STRUTS
        // Overhead anchor plate attached to the upper stadium limit
        drawCube(0.0f, screenY + 14.0f, 0.0f, 40.0f, 1.5f, 40.0f, 0.12f, 0.14f, 0.16f);

        // Four compact suspension cables holding the display cube frame
        drawCube(screenX - 12.0f, screenY + 7.0f, screenZ - 12.0f, 0.4f, 14.0f, 0.4f, 0.25f, 0.27f, 0.30f);
        drawCube(screenX + 12.0f, screenY + 7.0f, screenZ - 12.0f, 0.4f, 14.0f, 0.4f, 0.25f, 0.27f, 0.30f);
        drawCube(screenX - 12.0f, screenY + 7.0f, screenZ + 12.0f, 0.4f, 14.0f, 0.4f, 0.25f, 0.27f, 0.30f);
        drawCube(screenX + 12.0f, screenY + 7.0f, screenZ + 12.0f, 0.4f, 14.0f, 0.4f, 0.25f, 0.27f, 0.30f);

        // 2. COMPACT 4-SIDED CUBE SCOREBOARD HOUSING
        // Proportions reduced down to a sleek 30x18x30 block shape
        drawCube(screenX, screenY, screenZ, 30.0f, 18.0f, 30.0f, 0.18f, 0.20f, 0.22f);

        // INNER BACKING BOX HOUSINGS FOR ALL 4 SCREENS (Recessed slightly)
        drawCube(screenX, screenY, screenZ - 14.9f, 26.0f, 15.0f, 0.4f, 0.04f, 0.05f, 0.06f); // Front (Z-)
        drawCube(screenX, screenY, screenZ + 14.9f, 26.0f, 15.0f, 0.4f, 0.04f, 0.05f, 0.06f); // Back  (Z+)
        drawCube(screenX - 14.9f, screenY, screenZ, 0.4f, 15.0f, 26.0f, 0.04f, 0.05f, 0.06f); // Left  (X-)
        drawCube(screenX + 14.9f, screenY, screenZ, 0.4f, 15.0f, 26.0f, 0.04f, 0.05f, 0.06f); // Right (X+)

        // 3. 4-WAY DISPLAY PIXEL ART ARRAYS
        float dotSz = 0.9f;   // Scaled down matrix pixel dots
        int screenRadius = 6;  // Adjusted icon radius to fit scaled down monitor area

        // We iterate through a 2D matrix frame and map it to all 4 matching side surfaces
        for (int rowU = -13; rowU <= 13; ++rowU) {
            for (int colV = -7; colV <= 7; ++colV) {
                float centerDist = sqrt(static_cast<float>(rowU * rowU + colV * colV));
                float deltaPos = rowU * dotSz;
                float posY = screenY + (colV * dotSz);

                // Default Screen Matrix Shrub Ambiance
                float r = 0.04f, g = 0.05f, b = 0.06f;
                bool isBallPixel = false;

                if (centerDist <= screenRadius) {
                    isBallPixel = true;
                    if (centerDist <= 1.5f) {
                        r = 0.98f; g = 0.98f; b = 0.98f; // Inner Button Core
                    } else if (centerDist <= 2.4f) {
                        r = 0.06f; g = 0.06f; b = 0.08f; // Inner Ring Divide
                    } else if (std::abs(colV) < 1) {
                        r = 0.06f; g = 0.06f; b = 0.08f; // Black Midline Split
                    } else if (colV > 0) {
                        r = 0.88f; g = 0.12f; b = 0.12f; // Top Red Base
                    } else {
                        r = 0.94f; g = 0.94f; b = 0.94f; // Bottom White Base
                    }
                } else {
                    // Futuristic scrolling cyber grids on monitor bounds
                    if ((rowU + colV) % 5 == 0) {
                        float shine = (sin(energyPulseTimer * 1.5f) * 0.12f) + 0.28f;
                        r = 0.0f; g = 0.55f * shine; b = 0.75f * shine;
                    }
                }

                // If it is a part of the design pattern, render to all four spatial surface coordinate offsets
                if (isBallPixel || (rowU + colV) % 5 == 0) {
                    // FACE 1: FRONT PANEL (Z-)
                    drawCube(screenX + deltaPos, posY, screenZ - 15.1f, dotSz, dotSz, 0.15f, r, g, b);

                    // FACE 2: REAR PANEL (Z+)
                    drawCube(screenX + deltaPos, posY, screenZ + 15.1f, dotSz, dotSz, 0.15f, r, g, b);

                    // FACE 3: LEFT PANEL (X-)
                    drawCube(screenX - 15.1f, posY, screenZ + deltaPos, 0.15f, dotSz, dotSz, r, g, b);

                    // FACE 4: RIGHT PANEL (X+)
                    drawCube(screenX + 15.1f, posY, screenZ + deltaPos, 0.15f, dotSz, dotSz, r, g, b);
                }
            }
        }
    }

    void MyVirtualWorld::drawInteractiveProps() {
        float surfaceY = 6.0f;
        float pulseBrightness = (sin(energyPulseTimer) * 0.25f) + 0.75f;

        // HIGH-TECH STRUCTURAL CONTAINMENT PYLONS
        float pX = 32.0f;
        float pZ = 46.0f;

        // Front Left Pylon
        drawCube(-pX, surfaceY + 1.5f, -pZ, 2.4f, 5.0f, 2.4f, 0.20f, 0.22f, 0.25f);
        drawCube(-pX, surfaceY + 4.4f, -pZ, 1.4f, 1.2f, 1.4f, 0.0f, 0.90f * pulseBrightness, 0.95f * pulseBrightness);
        // Front Right Pylon
        drawCube(pX, surfaceY + 1.5f, -pZ, 2.4f, 5.0f, 2.4f, 0.20f, 0.22f, 0.25f);
        drawCube(pX, surfaceY + 4.4f, -pZ, 1.4f, 1.2f, 1.4f, 0.0f, 0.90f * pulseBrightness, 0.95f * pulseBrightness);
        // Back Left Pylon
        drawCube(-pX, surfaceY + 1.5f, pZ, 2.4f, 5.0f, 2.4f, 0.20f, 0.22f, 0.25f);
        drawCube(-pX, surfaceY + 4.4f, pZ, 1.4f, 1.2f, 1.4f, 0.0f, 0.90f * pulseBrightness, 0.95f * pulseBrightness);
        // Back Right Pylon
        drawCube(pX, surfaceY + 1.5f, pZ, 2.4f, 5.0f, 2.4f, 0.20f, 0.22f, 0.25f);
        drawCube(pX, surfaceY + 4.4f, pZ, 1.4f, 1.2f, 1.4f, 0.0f, 0.90f * pulseBrightness, 0.95f * pulseBrightness);

        // FLOATING COACH DUELLING STANDS
        drawCube(-30.0f, surfaceY + 0.8f, 0.0f, 6.0f, 1.0f, 5.0f, 0.25f, 0.27f, 0.30f);
        drawCube(-30.0f, surfaceY + 1.31f, 0.0f, 5.0f, 0.2f, 4.0f, 0.15f, 0.65f, 0.25f);

        drawCube(30.0f, surfaceY + 0.8f, 0.0f, 6.0f, 1.0f, 5.0f, 0.25f, 0.27f, 0.30f);
        drawCube(30.0f, surfaceY + 1.31f, 0.0f, 5.0f, 0.2f, 4.0f, 0.15f, 0.65f, 0.25f);

        // STORAGE CHEST LOOT CRATE ASSEMBLY
        glPushMatrix();
        glTranslatef(-18.0f, surfaceY + 0.2f, -25.0f);
        drawCube(0.0f, 1.5f, 0.0f, 3.5f, 3.0f, 3.5f, 0.48f, 0.31f, 0.15f);

        if (isBoxOpen) {
            glPushMatrix();
            glTranslatef(0.0f, 3.0f, -1.75f);
            glRotatef(-65.0f, 1.0f, 0.0f, 0.0f);
            drawCube(0.0f, 0.4f, 1.75f, 3.7f, 0.8f, 3.7f, 0.58f, 0.40f, 0.22f);
            glPopMatrix();
        } else {
            drawCube(0.0f, 3.4f, 0.0f, 3.7f, 0.8f, 3.7f, 0.40f, 0.24f, 0.10f);
        }
        glPopMatrix();

        // ROTATING SIGNAL RADAR BEACON UNIT
        glPushMatrix();
        glTranslatef(18.0f, surfaceY + 0.2f, 25.0f);
        drawCube(0.0f, 2.5f, 0.0f, 1.5f, 5.0f, 1.5f, 0.38f, 0.40f, 0.44f);

        if (skill2Active) {
            float rotDeg = (skill2Timer * 180.0f) / 3.14159f;
            glRotatef(rotDeg, 0.0f, 1.0f, 0.0f);
        }
        drawCube(2.0f, 4.5f, 0.0f, 4.0f, 0.8f, 0.8f, 0.85f, 0.12f, 0.12f);
        glPopMatrix();
    }

    void MyVirtualWorld::drawSkillVFX() {
        if (skill1Active) {
            glDisable(GL_LIGHTING);
            float zStart = -35.0f;
            float zEnd = 35.0f;
            float currentZ = zStart + (zEnd - zStart) * skill1Timer;
            float parabolaY = 15.0f + sin(skill1Timer * 3.14159f) * 8.0f;

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            drawCube(0.0f, parabolaY, currentZ, 4.0f, 4.0f, 6.0f, 0.20f, 0.60f, 1.0f);
            glDisable(GL_BLEND);
            glEnable(GL_LIGHTING);
        }
    }

    void MyVirtualWorld::drawEnergyCoreEffects() {
        float waveScale = (sin(energyPulseTimer) * 0.12f) + 0.88f;
        glDisable(GL_LIGHTING);
        glLineWidth(5.0f);

        glBegin(GL_LINE_LOOP);
        glColor3f(0.0f, 0.85f * waveScale, 0.95f * waveScale);
        for (int i = 0; i < 360; i += 8) {
            float angleRad = i * 3.14159f / 180.0f;
            glVertex3f(cos(angleRad) * 33.5f, 6.42f, sin(angleRad) * 45.5f);
        }
        glEnd();

        glLineWidth(1.0f);
        glEnable(GL_LIGHTING);
    }

} // namespace battleArena
