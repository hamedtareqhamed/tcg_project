#ifndef CHARIZARDX_HPP
#define CHARIZARDX_HPP

#include <GL/glut.h>
#include <cmath> // Required for sin() and cos() mathematical functions

namespace charizardx {

    class MyVirtualWorld {
    public:
        // Initialization and core update functions
        void init();
        void draw();
        void tickTime();

        // State toggle functions
        void toggleWalking();
        bool isWalking() const;
        void toggleFireBreath();
        void toggleFlying();

        void triggerBoxOpen();
        void setEnvironmentActive(bool active);
        bool isEnvironmentActive() const;

    private:
        // Helper functions for drawing individual body parts
        void drawBody();
        void drawLeg(bool isLeft);
        void drawTail();
        void drawWing(bool isLeft);
        void drawArm(bool isLeft);
        void drawHead();

        // Helper functions for particle effects and environment
        void drawWhiskerFire(bool isLeft);
        void drawFireBreath();
        void drawEnvironment();

        void drawBlindBox();

        // Animation state variables: Walking
        float walkTime;
        float walkDistance;
        bool walking;

        // Animation state variables: Fire Breath
        bool breathingFire;
        float fireTime;

        // Animation state variables: Flying
        bool flying;
        float flyTime;
        float flyHeight;

        // Global timer for continuous environmental and idle animations
        float alwaysTime;

        bool hasBox;
        bool isOpening;
        float boxTime;
        bool environmentActive;
    };

}

#endif // CHARIZARDX_HPP
