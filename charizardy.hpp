/*
 * TCG6223 Computer Graphics - Group Project
 * 
 * Project.hpp (Final Version)
 * ---------------------------
 * Interface declarations for the Mega Charizard Y 3D character model.
 * Provides hierarchical 3D transformations, interactive keyframe animation logic,
 * procedural dynamic particle simulation systems, and clean API control wrappers.
 */

#ifndef PROJECT_HPP
#define PROJECT_HPP

#include "main.hpp"
#include <vector>
#include <cmath>

namespace MegaCharizardY {

/**
 * @brief Represents a single particle in the dynamic physics simulation system.
 */
struct Particle {
    float x, y, z;
    float vx, vy, vz;
    float r, g, b, a;
    float size;
    float life;    // Current life value ranging from 0.0 (inactive) to 1.0 (spawned)
    float decay;   // Linear rate of life depletion per frame
};

/**
 * @brief Implements the hierarchical 3D mesh model, animation states, and actions for Mega Charizard Y.
 */
class MegaCharizardY {
public:
    // Spatial positioning and orientation parameters
    float posX, posY, posZ;
    float rotationY;
    
    // Joint rotation angles (measured in degrees)
    float wingAngle;
    float tailSwing;
    float neckAngle;
    float jawAngle;
    float armAngle;
    float legAngle;
    
    // Locomotion and action status flags
    bool isWalking;
    bool isFlying;
    bool isCastingSkill;
    float skillTimer; // Accumulated duration since the skill execution started
    
    // Particle containers for dynamic simulations
    std::vector<Particle> tailParticles;
    std::vector<Particle> skillParticles;

    MegaCharizardY();
    
    void update(float dt);
    void draw();
    
    // =========================================================================
    // MODULAR CONTROLLER INTEGRATION API
    // The team can bind these methods to ANY keyboard inputs, AI routines,
    // or game state callbacks in the main files.
    // =========================================================================
    
    /**
     * @brief Instantly teleports/spawns the character at a specific 3D coordinate.
     */
    void setPosition(float x, float y, float z);
    
    /**
     * @brief Instantly sets the character orientation around the Y-axis.
     */
    void setRotationY(float angle);
    
    /**
     * @brief Commands the character to move forward relative to its yaw angle.
     * @param dt Delta time since the last frame.
     * @param speed Linear movement speed.
     */
    void moveForward(float dt, float speed = 12.0f);
    
    /**
     * @brief Commands the character to retreat backward relative to its yaw angle.
     * @param dt Delta time since the last frame.
     * @param speed Linear movement speed.
     */
    void moveBackward(float dt, float speed = 12.0f);
    
    /**
     * @brief Rotates the character left (counter-clockwise).
     * @param dt Delta time since the last frame.
     * @param angularSpeed Rotation speed in degrees per second.
     */
    void turnLeft(float dt, float angularSpeed = 120.0f);
    
    /**
     * @brief Rotates the character right (clockwise).
     * @param dt Delta time since the last frame.
     * @param angularSpeed Rotation speed in degrees per second.
     */
    void turnRight(float dt, float angularSpeed = 120.0f);
    
    /**
     * @brief Elevates the character altitude.
     * @param dt Delta time since the last frame.
     * @param verticalSpeed Ascending speed.
     */
    void flyUp(float dt, float verticalSpeed = 10.0f);
    
    /**
     * @brief Decrements the character altitude and triggers landing sequences.
     * @param dt Delta time since the last frame.
     * @param verticalSpeed Descending speed.
     */
    void flyDown(float dt, float verticalSpeed = 10.0f);
    
    /**
     * @brief Triggers the "Blast Burn" fire breathing special skill sequence.
     */
    void castSkill();
    
private:
    // Hierarchical geometric drawing functions
    void drawSolidCube(float dx, float dy, float dz, float r, float g, float b);
    void drawSolidCubeWithBelly(float dx, float dy, float dz, float r, float g, float b, float bellyR, float bellyG, float bellyB);
    void drawHead();
    void drawTorso();
    void drawNeck();
    void drawArm(bool isLeft);
    void drawLeg(bool isLeft);
    void drawTail();
    void drawWing(bool isLeft);
    
    // Particle state management and spawning routines
    void updateParticles(float dt);
    void drawParticles();
    void spawnTailFlame();
    void spawnBlastBurnParticles();
};

/**
 * @brief Core application class containing the virtual scene, scheduling ticks, and rendering pipeline interface.
 */
class MyVirtualWorld
{
public:
    long int timeold, timenew, elapseTime;
    
    MegaCharizardY charizard;

    void draw();
    void tickTime();
    void init();
};

} // namespace Project

#endif // PROJECT_HPP
