/*
 * TCG6223 Computer Graphics - Group Project
 *
 * Project.cpp (Final Version)
 * ---------------------------
 * Implementation of the Mega Charizard Y 3D character model.
 * Provides hierarchical 3D transformations, keyframe animation loops,
 * and modular controller API wrappers for easy integration.
 */

#include "charizardy.hpp"
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace Project;

// Reference to external virtual world singleton
extern MyVirtualWorld myvirtualworld;

// Static control flags for flight movement and air resistance logic
static bool isMovingForward = false;
static bool isMovingBackward = false;

// ----------------------------------------------------
// Auxiliary Utilities
// ----------------------------------------------------

float randomFloat(float min, float max) {
  return min + static_cast<float>(rand()) /
                   (static_cast<float>(RAND_MAX) / (max - min));
}

// ----------------------------------------------------
// Mega Charizard Y Class Implementation
// ----------------------------------------------------

MegaCharizardY::MegaCharizardY() {
  posX = 0.0f;
  posY = 0.0f;
  posZ = 0.0f;
  rotationY = 0.0f;

  wingAngle = 0.0f;
  tailSwing = 0.0f;
  neckAngle = 0.0f;
  jawAngle = 0.0f;
  armAngle = 0.0f;
  legAngle = 0.0f;

  isWalking = false;
  isFlying = false;
  isCastingSkill = false;
  skillTimer = 0.0f;

  // Initialize random seed
  srand(1337);
}

// =============================================================================
// MODULAR CONTROLLER IMPLEMENTATIONS (Team integration endpoints)
// =============================================================================

void MegaCharizardY::setPosition(float x, float y, float z) {
  posX = x;
  posY = y;
  posZ = z;
}

void MegaCharizardY::setRotationY(float angle) { rotationY = angle; }

void MegaCharizardY::moveForward(float dt, float speed) {
  isWalking = true;
  isMovingForward = true;
  posX += speed * dt * sin(rotationY * M_PI / 180.0f);
  posZ += speed * dt * cos(rotationY * M_PI / 180.0f);
}

void MegaCharizardY::moveBackward(float dt, float speed) {
  isWalking = true;
  isMovingBackward = true;
  posX -= speed * dt * sin(rotationY * M_PI / 180.0f);
  posZ -= speed * dt * cos(rotationY * M_PI / 180.0f);
}

void MegaCharizardY::turnLeft(float dt, float angularSpeed) {
  rotationY += angularSpeed * dt;
}

void MegaCharizardY::turnRight(float dt, float angularSpeed) {
  rotationY -= angularSpeed * dt;
}

void MegaCharizardY::flyUp(float dt, float verticalSpeed) {
  isFlying = true;
  if (posY < 10.0f) {
    posY += verticalSpeed * dt;
  }
}

void MegaCharizardY::flyDown(float dt, float verticalSpeed) {
  if (posY > 0.0f) {
    posY -= verticalSpeed * dt;
    if (posY <= 0.0f) {
      posY = 0.0f;
      isFlying = false;
    }
  }
}

void MegaCharizardY::castSkill() {
  if (!isCastingSkill) {
    isCastingSkill = true;
    skillTimer = 0.0f;
  }
}

void MegaCharizardY::drawSolidCube(float dx, float dy, float dz, float r,
                                   float g, float b) {
  float x = dx / 2.0f;
  float y = dy / 2.0f;
  float z = dz / 2.0f;

  glColor3f(r, g, b);
  glBegin(GL_QUADS);

  // Front Face
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-x, -y, z);
  glVertex3f(x, -y, z);
  glVertex3f(x, y, z);
  glVertex3f(-x, y, z);

  // Back Face
  glNormal3f(0.0f, 0.0f, -1.0f);
  glVertex3f(-x, -y, -z);
  glVertex3f(-x, y, -z);
  glVertex3f(x, y, -z);
  glVertex3f(x, -y, -z);

  // Top Face
  glNormal3f(0.0f, 1.0f, 0.0f);
  glVertex3f(-x, y, -z);
  glVertex3f(-x, y, z);
  glVertex3f(x, y, z);
  glVertex3f(x, y, -z);

  // Bottom Face
  glNormal3f(0.0f, -1.0f, 0.0f);
  glVertex3f(-x, -y, -z);
  glVertex3f(x, -y, -z);
  glVertex3f(x, -y, z);
  glVertex3f(-x, -y, z);

  // Right Face
  glNormal3f(1.0f, 0.0f, 0.0f);
  glVertex3f(x, -y, -z);
  glVertex3f(x, y, -z);
  glVertex3f(x, y, z);
  glVertex3f(x, -y, z);

  // Left Face
  glNormal3f(-1.0f, 0.0f, 0.0f);
  glVertex3f(-x, -y, -z);
  glVertex3f(-x, -y, z);
  glVertex3f(-x, y, z);
  glVertex3f(-x, y, -z);

  glEnd();
}

void MegaCharizardY::drawSolidCubeWithBelly(float dx, float dy, float dz,
                                            float r, float g, float b,
                                            float bellyR, float bellyG,
                                            float bellyB) {
  // Draw base orange block
  drawSolidCube(dx, dy, dz, r, g, b);

  // Draw front belly plate overlay with offset on Z axis
  float x = (dx * 0.8f) / 2.0f;
  float y = (dy * 0.9f) / 2.0f;
  float z = (dz / 2.0f) + 0.05f;

  glColor3f(bellyR, bellyG, bellyB);
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-x, -y, z);
  glVertex3f(x, -y, z);
  glVertex3f(x, y, z);
  glVertex3f(-x, y, z);
  glEnd();
}

void MegaCharizardY::drawHead() {
  glPushMatrix();
  // Main cranial volume
  drawSolidCube(2.4f, 2.2f, 2.4f, 0.95f, 0.40f, 0.05f);

  // Left ocular assembly
  glPushMatrix();
  glTranslatef(-1.21f, 0.3f, 0.4f);
  drawSolidCube(0.05f, 0.5f, 0.5f, 0.0f, 0.6f, 0.7f);
  glTranslatef(-0.01f, 0.1f, 0.15f);
  drawSolidCube(0.05f, 0.2f, 0.2f, 1.0f, 1.0f, 1.0f);
  glPopMatrix();

  // Right ocular assembly
  glPushMatrix();
  glTranslatef(1.21f, 0.3f, 0.4f);
  drawSolidCube(0.05f, 0.5f, 0.5f, 0.0f, 0.6f, 0.7f);
  glTranslatef(0.01f, 0.1f, 0.15f);
  drawSolidCube(0.05f, 0.2f, 0.2f, 1.0f, 1.0f, 1.0f);
  glPopMatrix();

  // Rostral/nasal structures
  glPushMatrix();
  glTranslatef(0.0f, 0.1f, 1.6f);
  drawSolidCube(2.0f, 1.1f, 1.4f, 0.95f, 0.40f, 0.05f);

  // Nostril overlays
  glTranslatef(-0.5f, 0.51f, 0.5f);
  drawSolidCube(0.3f, 0.1f, 0.3f, 0.6f, 0.2f, 0.0f);
  glTranslatef(1.0f, 0.0f, 0.0f);
  drawSolidCube(0.3f, 0.1f, 0.3f, 0.6f, 0.2f, 0.0f);
  glPopMatrix();

  // Mandibular structure
  glPushMatrix();
  glTranslatef(0.0f, -0.6f, 1.1f);
  glRotatef(-jawAngle, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, -0.2f, 0.4f);
  drawSolidCube(1.9f, 0.5f, 1.3f, 0.95f, 0.40f, 0.05f);

  // Glossal tissue (tongue)
  glPushMatrix();
  glTranslatef(0.0f, 0.3f, -0.1f);
  drawSolidCube(1.4f, 0.15f, 0.9f, 0.9f, 0.4f, 0.5f);
  glPopMatrix();

  // Dental structure (teeth)
  glPushMatrix();
  glTranslatef(-0.8f, 0.3f, 0.5f);
  drawSolidCube(0.2f, 0.2f, 0.2f, 1.0f, 1.0f, 1.0f);
  glTranslatef(0.8f, 0.0f, 0.0f);
  drawSolidCube(0.2f, 0.2f, 0.2f, 1.0f, 1.0f, 1.0f);
  glTranslatef(0.8f, 0.0f, 0.0f);
  drawSolidCube(0.2f, 0.2f, 0.2f, 1.0f, 1.0f, 1.0f);
  glPopMatrix();
  glPopMatrix();

  // Cranial horn sweep system
  // Medial primary horn
  glPushMatrix();
  glTranslatef(0.0f, 1.1f, -1.0f);
  glRotatef(-25.0f, 1.0f, 0.0f, 0.0f);
  drawSolidCube(0.6f, 0.6f, 1.8f, 0.95f, 0.40f, 0.05f);
  glPopMatrix();

  // Left lateral horn
  glPushMatrix();
  glTranslatef(-0.8f, 0.9f, -1.0f);
  glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-10.0f, 0.0f, 1.0f, 0.0f);
  drawSolidCube(0.5f, 0.5f, 1.5f, 0.95f, 0.40f, 0.05f);
  glPopMatrix();

  // Right lateral horn
  glPushMatrix();
  glTranslatef(0.8f, 0.9f, -1.0f);
  glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(10.0f, 0.0f, 1.0f, 0.0f);
  drawSolidCube(0.5f, 0.5f, 1.5f, 0.95f, 0.40f, 0.05f);
  glPopMatrix();

  glPopMatrix();
}

void MegaCharizardY::drawNeck() {
  glPushMatrix();
  glTranslatef(0.0f, 2.5f, 0.8f);

  // Proximal cervical segment
  glRotatef(neckAngle * 0.4f, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, 0.6f, 0.3f);
  drawSolidCube(1.8f, 1.6f, 1.8f, 0.95f, 0.40f, 0.05f);

  // Medial cervical segment
  glPushMatrix();
  glRotatef(neckAngle * 0.3f, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, 1.2f, 0.3f);
  drawSolidCube(1.6f, 1.6f, 1.6f, 0.95f, 0.40f, 0.05f);

  // Distal cervical segment
  glPushMatrix();
  glRotatef(neckAngle * 0.3f, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, 1.1f, 0.2f);
  drawSolidCube(1.4f, 1.4f, 1.4f, 0.95f, 0.40f, 0.05f);

  // Cranial assembly
  glPushMatrix();
  glTranslatef(0.0f, 1.1f, 0.2f);
  drawHead();
  glPopMatrix();

  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
}

void MegaCharizardY::drawTorso() {
  drawSolidCubeWithBelly(4.2f, 6.0f, 3.6f, 0.95f, 0.40f, 0.05f, 0.98f, 0.85f,
                         0.45f);
}

void MegaCharizardY::drawArm(bool isLeft) {
  float side = isLeft ? -1.0f : 1.0f;

  glPushMatrix();
  // Glenohumeral joint definition
  glTranslatef(side * 2.3f, 1.8f, 0.6f);
  glRotatef(armAngle, 1.0f, 0.0f, 0.0f);
  glRotatef(side * 20.0f, 0.0f, 0.0f, 1.0f);

  // Brachial segment (upper arm)
  glTranslatef(side * 0.6f, -0.6f, 0.0f);
  drawSolidCube(1.2f, 1.8f, 1.2f, 0.95f, 0.40f, 0.05f);

  // Antebrachial segment (forearm) and elbow articulation
  glTranslatef(0.0f, -1.5f, 0.2f);
  glRotatef(isLeft ? 25.0f : -25.0f, 0.0f, 1.0f, 0.0f);
  drawSolidCube(1.0f, 1.6f, 1.0f, 0.95f, 0.40f, 0.05f);

  // Carpal membrane feature (wrist wing blade)
  glPushMatrix();
  glTranslatef(side * 0.6f, 0.0f, -0.2f);
  glRotatef(side * 35.0f, 0.0f, 1.0f, 0.0f);
  drawSolidCube(0.2f, 1.5f, 1.0f, 0.0f, 0.5f, 0.5f);
  glPopMatrix();

  // Manus segment (hand)
  glTranslatef(0.0f, -1.0f, 0.2f);
  drawSolidCube(1.0f, 0.4f, 1.0f, 0.95f, 0.40f, 0.05f);

  // Digital appendages (claws)
  glPushMatrix();
  glTranslatef(-0.3f, -0.3f, 0.3f);
  drawSolidCube(0.2f, 0.4f, 0.3f, 1.0f, 1.0f, 1.0f);
  glTranslatef(0.3f, 0.0f, 0.0f);
  drawSolidCube(0.2f, 0.4f, 0.3f, 1.0f, 1.0f, 1.0f);
  glTranslatef(0.3f, 0.0f, 0.0f);
  drawSolidCube(0.2f, 0.4f, 0.3f, 1.0f, 1.0f, 1.0f);
  glPopMatrix();

  glPopMatrix();
}

void MegaCharizardY::drawLeg(bool isLeft) {
  float side = isLeft ? -1.0f : 1.0f;

  glPushMatrix();
  // Acetabulofemoral articulation (hip joint)
  glTranslatef(side * 1.8f, -2.2f, 0.0f);

  // Conditional locomotion/flight posture logic
  if (isFlying) {
    glRotatef(legAngle, 1.0f, 0.0f, 0.0f);
  } else {
    glRotatef(isLeft ? legAngle : -legAngle, 1.0f, 0.0f, 0.0f);
  }

  // Femoral segment (thigh)
  glTranslatef(side * 0.3f, -1.0f, 0.0f);
  drawSolidCube(1.8f, 2.4f, 1.8f, 0.95f, 0.40f, 0.05f);

  // Tibial segment (calf)
  glTranslatef(0.0f, -1.8f, -0.1f);
  drawSolidCube(1.4f, 1.6f, 1.6f, 0.95f, 0.40f, 0.05f);

  // Pes segment (foot)
  glTranslatef(0.0f, -1.0f, 0.4f);
  drawSolidCube(1.6f, 0.6f, 2.2f, 0.95f, 0.40f, 0.05f);

  // Digital appendages (claws)
  glPushMatrix();
  glTranslatef(-0.5f, 0.0f, 1.2f);
  drawSolidCube(0.3f, 0.4f, 0.4f, 1.0f, 1.0f, 1.0f);
  glTranslatef(0.5f, 0.0f, 0.0f);
  drawSolidCube(0.3f, 0.4f, 0.4f, 1.0f, 1.0f, 1.0f);
  glTranslatef(0.5f, 0.0f, 0.0f);
  drawSolidCube(0.3f, 0.4f, 0.4f, 1.0f, 1.0f, 1.0f);
  glPopMatrix();

  glPopMatrix();
}

void MegaCharizardY::drawTail() {
  glPushMatrix();
  glTranslatef(0.0f, -2.0f, -1.5f);
  glRotatef(tailSwing * 0.4f, 0.0f, 1.0f, 0.0f);
  glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);

  // Segment 1
  glTranslatef(0.0f, 0.0f, -0.8f);
  drawSolidCube(1.4f, 1.4f, 1.6f, 0.95f, 0.40f, 0.05f);

  // Segment 2
  glPushMatrix();
  glTranslatef(0.0f, 0.4f, -1.4f);
  glRotatef(tailSwing * 0.4f, 0.0f, 1.0f, 0.0f);
  glRotatef(12.0f, 1.0f, 0.0f, 0.0f);
  drawSolidCube(1.2f, 1.2f, 1.4f, 0.95f, 0.40f, 0.05f);

  // Segment 3
  glPushMatrix();
  glTranslatef(0.0f, 0.6f, -1.2f);
  glRotatef(tailSwing * 0.4f, 0.0f, 1.0f, 0.0f);
  glRotatef(18.0f, 1.0f, 0.0f, 0.0f);
  drawSolidCube(1.0f, 1.0f, 1.2f, 0.95f, 0.40f, 0.05f);

  // Segment 4
  glPushMatrix();
  glTranslatef(0.0f, 0.6f, -1.0f);
  glRotatef(tailSwing * 0.5f, 0.0f, 1.0f, 0.0f);
  glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
  drawSolidCube(0.8f, 0.8f, 1.0f, 0.95f, 0.40f, 0.05f);

  // Segment 5 (Terminal tip)
  glPushMatrix();
  glTranslatef(0.0f, 0.5f, -0.8f);
  glRotatef(tailSwing * 0.5f, 0.0f, 1.0f, 0.0f);
  glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
  drawSolidCube(0.6f, 0.6f, 0.8f, 0.95f, 0.40f, 0.05f);

  // Flame core base volume
  glPushMatrix();
  glTranslatef(0.0f, 0.2f, -0.6f);
  glDisable(GL_LIGHTING);
  drawSolidCube(0.7f, 1.2f, 0.7f, 1.0f, 0.6f, 0.0f);
  glTranslatef(0.0f, 0.1f, 0.0f);
  drawSolidCube(0.4f, 0.8f, 0.4f, 1.0f, 0.9f, 0.1f);
  glEnable(GL_LIGHTING);
  glPopMatrix();

  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
}

void MegaCharizardY::drawWing(bool isLeft) {
  float side = isLeft ? -1.0f : 1.0f;

  glPushMatrix();
  // Dorsal attachment interface coordinates
  glTranslatef(side * 0.9f, 2.2f, -1.8f);

  // Primary joint flap angle
  glRotatef(isLeft ? -wingAngle : wingAngle, 0.0f, 0.0f, 1.0f);
  glRotatef(side * 15.0f, 0.0f, 1.0f, 0.0f);

  // Humeral wing structure
  glPushMatrix();
  glTranslatef(side * 2.0f, 0.4f, 0.0f);
  drawSolidCube(4.0f, 1.0f, 1.0f, 0.95f, 0.40f, 0.05f);
  glPopMatrix();

  // Secondary joint elbow articulation
  glPushMatrix();
  glTranslatef(side * 4.0f, 0.8f, 0.0f);
  glRotatef(isLeft ? -wingAngle * 0.3f : wingAngle * 0.3f, 0.0f, 0.0f, 1.0f);

  // Radial wing structure extending outwards
  glPushMatrix();
  glTranslatef(side * 2.5f, 0.6f, 0.0f);
  glRotatef(side * 12.0f, 0.0f, 0.0f, 1.0f);
  drawSolidCube(5.0f, 0.8f, 0.8f, 0.95f, 0.40f, 0.05f);
  glPopMatrix();

  // Carpal claw appendage
  glPushMatrix();
  glTranslatef(0.0f, 0.6f, 0.4f);
  drawSolidCube(0.6f, 0.6f, 0.6f, 1.0f, 1.0f, 1.0f);
  glPopMatrix();

  // Stepped voxel wing membrane columns
  for (int col = 0; col < 12; ++col) {
    glPushMatrix();
    float dx = side * (col * 0.55f);
    glTranslatef(dx, 0.0f, -0.05f);

    float yMin, yMax;
    if (col < 3) {
      yMin = -5.5f + col * 0.8f;
      yMax = 0.0f + col * 0.6f;
    } else if (col < 7) {
      yMin = -3.9f - (col - 3) * 0.6f;
      yMax = 1.8f + (col - 3) * 0.4f;
    } else {
      yMin = -6.3f + (col - 7) * 1.8f;
      yMax = 3.4f - (col - 7) * 1.0f;
    }

    for (float y = yMin; y <= yMax; y += 0.5f) {
      glPushMatrix();
      glTranslatef(0.0f, y, 0.0f);
      bool isBorder = (y + 0.5f > yMax) || (y - 0.5f < yMin) || (col == 11);
      if (isBorder) {
        drawSolidCube(0.55f, 0.5f, 0.5f, 0.95f, 0.40f, 0.05f);
      } else {
        drawSolidCube(0.55f, 0.5f, 0.35f, 0.0f, 0.45f, 0.50f);
      }
      glPopMatrix();
    }
    glPopMatrix();
  }
  glPopMatrix();
  glPopMatrix();
}

void MegaCharizardY::draw() {
  glPushMatrix();
  // Relocate base transformation offset to align feet at y = 0.0
  glTranslatef(posX, posY + 6.3f, posZ);
  glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

  drawWing(true);
  drawWing(false);
  drawTail();
  drawTorso();
  drawNeck();
  drawArm(true);
  drawArm(false);
  drawLeg(true);
  drawLeg(false);

  glPopMatrix();

  drawParticles();
}

// ----------------------------------------------------
// Particle Systems & Updates
// ----------------------------------------------------

void MegaCharizardY::spawnTailFlame() {
  float radian = rotationY * M_PI / 180.0f;

  float tailRad = tailSwing * 1.1f * M_PI / 180.0f;
  float tx = -5.4f * sin(tailRad);
  float ty = posY + 6.6f;
  float tz = -7.0f * cos(tailRad);

  float worldTx = tx * cos(radian) + tz * sin(radian) + posX;
  float worldTz = -tx * sin(radian) + tz * cos(radian) + posZ;
  float worldTy = ty;

  Particle p;
  p.x = worldTx + randomFloat(-0.15f, 0.15f);
  p.y = worldTy + randomFloat(-0.15f, 0.15f);
  p.z = worldTz + randomFloat(-0.15f, 0.15f);

  p.vx = randomFloat(-0.3f, 0.3f);
  p.vy = randomFloat(0.8f, 2.0f);
  p.vz = randomFloat(-0.3f, 0.3f);

  p.r = 1.0f;
  p.g = randomFloat(0.3f, 0.7f);
  p.b = 0.0f;
  p.a = 1.0f;

  p.size = randomFloat(0.3f, 0.6f);
  p.life = 1.0f;
  p.decay = randomFloat(0.04f, 0.08f);

  tailParticles.push_back(p);
}

void MegaCharizardY::spawnBlastBurnParticles() {
  float radian = rotationY * M_PI / 180.0f;

  float localX = 0.0f;
  float localY = 4.5f;
  float localZ = 4.7f;

  if (isCastingSkill && skillTimer > 1.0f && skillTimer < 3.0f) {
    localZ += 1.0f;
  }

  float worldMx = localX * cos(radian) + localZ * sin(radian) + posX;
  float worldMz = -localX * sin(radian) + localZ * cos(radian) + posZ;
  float worldMy = posY + 6.3f + localY;

  for (int i = 0; i < 8; ++i) {
    Particle p;
    p.x = worldMx;
    p.y = worldMy + randomFloat(-0.1f, 0.1f);
    p.z = worldMz;

    float forwardX = sin(radian);
    float forwardZ = cos(radian);

    p.vx = forwardX * randomFloat(8.0f, 15.0f) + randomFloat(-1.5f, 1.5f);
    p.vy = randomFloat(-1.0f, 2.0f);
    p.vz = forwardZ * randomFloat(8.0f, 15.0f) + randomFloat(-1.5f, 1.5f);

    float colorSelect = randomFloat(0.0f, 1.0f);
    if (colorSelect > 0.6f) {
      p.r = 1.0f;
      p.g = 0.9f;
      p.b = 0.1f;
    } else if (colorSelect > 0.2f) {
      p.r = 1.0f;
      p.g = 0.4f;
      p.b = 0.0f;
    } else {
      p.r = 0.8f;
      p.g = 0.1f;
      p.b = 0.0f;
    }

    p.a = 1.0f;
    p.size = randomFloat(0.4f, 1.0f);
    p.life = 1.0f;
    p.decay = randomFloat(0.02f, 0.05f);

    skillParticles.push_back(p);
  }
}

void MegaCharizardY::updateParticles(float dt) {
  // Update tail-tip flame system
  for (size_t i = 0; i < tailParticles.size();) {
    tailParticles[i].x += tailParticles[i].vx * dt;
    tailParticles[i].y += tailParticles[i].vy * dt;
    tailParticles[i].z += tailParticles[i].vz * dt;

    tailParticles[i].g -= 0.5f * dt;
    if (tailParticles[i].g < 0.0f)
      tailParticles[i].g = 0.0f;

    tailParticles[i].life -= tailParticles[i].decay;

    if (tailParticles[i].life <= 0.0f) {
      tailParticles.erase(tailParticles.begin() + i);
    } else {
      ++i;
    }
  }

  // Update active skill flame system
  for (size_t i = 0; i < skillParticles.size();) {
    skillParticles[i].x += skillParticles[i].vx * dt;
    skillParticles[i].y += skillParticles[i].vy * dt;
    skillParticles[i].z += skillParticles[i].vz * dt;

    skillParticles[i].vx *= 0.95f;
    skillParticles[i].vz *= 0.95f;
    skillParticles[i].vy -= 2.0f * dt;

    skillParticles[i].life -= skillParticles[i].decay;

    if (skillParticles[i].life <= 0.0f) {
      skillParticles.erase(skillParticles.begin() + i);
    } else {
      ++i;
    }
  }
}

void MegaCharizardY::drawParticles() {
  glDisable(GL_LIGHTING);
  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  // Draw tail particles
  for (const auto &p : tailParticles) {
    glPushMatrix();
    glTranslatef(p.x, p.y, p.z);
    glColor4f(p.r, p.g, p.b, p.life);
    drawSolidCube(p.size, p.size, p.size, p.r, p.g, p.b);
    glPopMatrix();
  }

  // Draw casting skill particles
  for (const auto &p : skillParticles) {
    glPushMatrix();
    glTranslatef(p.x, p.y, p.z);
    float currentSize = p.size * (2.0f - p.life);
    glColor4f(p.r, p.g, p.b, p.life);
    drawSolidCube(currentSize, currentSize, currentSize, p.r, p.g, p.b);
    glPopMatrix();
  }

  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
  glEnable(GL_LIGHTING);
}

void MegaCharizardY::update(float dt) {
  static float globalTime = 0.0f;
  globalTime += dt;

  // Wing flapping interpolation logic
  if (isFlying) {
    wingAngle = 35.0f * sin(globalTime * 8.0f);
  } else {
    wingAngle = 12.0f * sin(globalTime * 2.0f) + 5.0f;
  }

  // Tail sway interpolation logic
  tailSwing = 18.0f * sin(globalTime * 3.0f);

  // Walk state weight interpolation to prevent linear snapping
  static float walkWeight = 0.0f;
  if (isWalking) {
    walkWeight += 5.0f * dt;
    if (walkWeight > 1.0f)
      walkWeight = 1.0f;
  } else {
    walkWeight -= 3.0f * dt;
    if (walkWeight < 0.0f)
      walkWeight = 0.0f;
  }

  // Locomotion leg cycle and airborne resistance logic
  if (isFlying) {
    static float flyLegAngle = 0.0f;
    if (isMovingForward) {
      flyLegAngle += (-35.0f - flyLegAngle) * 5.0f * dt;
    } else if (isMovingBackward) {
      flyLegAngle += (25.0f - flyLegAngle) * 5.0f * dt;
    } else {
      float hoverSway = 6.0f * sin(globalTime * 2.0f);
      flyLegAngle += (hoverSway - flyLegAngle) * 3.0f * dt;
    }
    legAngle = flyLegAngle;
  } else {
    legAngle = walkWeight * 35.0f * sin(globalTime * 10.0f);
  }

  armAngle = walkWeight * 25.0f * cos(globalTime * 10.0f);

  // Reset movement flag values
  isWalking = false;
  isMovingForward = false;
  isMovingBackward = false;

  // Skill execution timeline
  if (isCastingSkill) {
    skillTimer += dt;

    if (skillTimer < 1.0f) {
      float progress = skillTimer / 1.0f;
      neckAngle = -25.0f * progress;
      jawAngle = 30.0f * progress;
      wingAngle = -45.0f * progress;
    } else if (skillTimer < 3.0f) {
      neckAngle = 25.0f;
      jawAngle = -45.0f;
      wingAngle = 40.0f * sin(globalTime * 20.0f);

      spawnBlastBurnParticles();
    } else if (skillTimer < 4.0f) {
      float progress = (skillTimer - 3.0f) / 1.0f;
      neckAngle = 25.0f * (1.0f - progress);
      jawAngle = -45.0f * (1.0f - progress);
    } else {
      isCastingSkill = false;
      skillTimer = 0.0f;
      neckAngle = 0.0f;
      jawAngle = 0.0f;
    }
  } else {
    // Normal resting posture sway
    neckAngle = 6.0f * sin(globalTime * 2.0f);
    jawAngle = 2.0f * sin(globalTime * 1.0f) + 2.0f;
  }

  spawnTailFlame();
  updateParticles(dt);
}

// ----------------------------------------------------
// MyVirtualWorld Engine Binding Class
// ----------------------------------------------------

void MyVirtualWorld::init() {
  timeold = glutGet(GLUT_ELAPSED_TIME);
}

void MyVirtualWorld::tickTime() {
  timenew = glutGet(GLUT_ELAPSED_TIME);
  elapseTime = timenew - timeold;
  timeold = timenew;

  float dt = elapseTime / 1000.0f;
  if (dt > 0.1f)
    dt = 0.1f;

  // Update character state
  charizard.update(dt);
}

void MyVirtualWorld::draw() {

  // Render the Mega Charizard Y voxel model
  charizard.draw();
}
