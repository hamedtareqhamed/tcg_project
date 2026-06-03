#include "charizardy.hpp"

namespace charizardx {

    namespace {
        // Struct to hold RGB color values
        struct Color {
            float r;
            float g;
            float b;
        };

        // Color Palette for Charizard (Voxel Style) - Orange/Fire theme from image
        static const Color kBodyArmor      = {0.88f, 0.42f, 0.08f}; // Vivid orange for main body
        static const Color kBodyShadow     = {0.62f, 0.25f, 0.04f}; // Deep burnt orange for shadows/crevices
        static const Color kLimbArmor      = {0.75f, 0.33f, 0.06f}; // Mid orange for limbs
        static const Color kJointArmor     = {0.95f, 0.55f, 0.15f}; // Bright orange for joints/highlights
        static const Color kBellyBright    = {0.98f, 0.92f, 0.62f}; // Bright cream/yellow for belly core
        static const Color kBellyMid       = {0.95f, 0.85f, 0.50f}; // Mid yellow-cream belly
        static const Color kBellyDark      = {0.88f, 0.72f, 0.35f}; // Darker warm yellow belly shade
        static const Color kWingMembrane   = {0.25f, 0.72f, 0.78f}; // Teal/cyan for inner wing membrane (from image)
        static const Color kWingDeep       = {0.12f, 0.52f, 0.60f}; // Deeper teal for outer wing areas
        static const Color kElectricBlue   = {0.12f, 0.65f, 0.72f}; // Teal-cyan fire (replaces electric blue)
        static const Color kClawIvory      = {0.95f, 0.93f, 0.88f}; // Off-white/cream for claws/teeth
        static const Color kEyeWhite       = {0.94f, 0.96f, 0.93f}; // Sclera color
        static const Color kEyeOrange      = {0.95f, 0.28f, 0.02f}; // Orange for eye iris
        static const Color kMouthPurple    = {0.78f, 0.28f, 0.22f}; // Pinkish-red inner mouth
        static const Color kNostrilBlack   = {0.08f, 0.05f, 0.03f}; // Dark brown-black for nostrils/pupils
        static const Color kRed            = {0.95f, 0.28f, 0.02f}; // Orange-red for the eyes (matches image)

        static const float kPi = 3.14159265f;

        // Helper function to quickly set OpenGL color using the Color struct
        void setColor(const Color& color) {
            glColor3f(color.r, color.g, color.b);
        }
    }

    // Helper function to draw a cube with a different color on its front face (used for the belly)
    void drawTwoColorCube(float frontR, float frontG, float frontB,
                          float bodyR, float bodyG, float bodyB) {
        glBegin(GL_QUADS);
        // Front Face (Belly color)
        glColor3f(frontR, frontG, frontB); glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f,  0.5f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f); glVertex3f(-0.5f,  0.5f,  0.5f);
        // Other 5 Faces (Body Armor color)
        glColor3f(bodyR, bodyG, bodyB);
        // Back face
        glNormal3f(0.0f, 0.0f, -1.0f); glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f,  0.5f, -0.5f); glVertex3f( 0.5f,  0.5f, -0.5f); glVertex3f( 0.5f, -0.5f, -0.5f);
        // Left face
        glNormal3f(-1.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, -0.5f,  0.5f); glVertex3f(-0.5f,  0.5f,  0.5f); glVertex3f(-0.5f,  0.5f, -0.5f);
        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f); glVertex3f( 0.5f,  0.5f, -0.5f); glVertex3f( 0.5f,  0.5f,  0.5f); glVertex3f( 0.5f, -0.5f,  0.5f);
        // Top face
        glNormal3f(0.0f, 1.0f, 0.0f); glVertex3f(-0.5f,  0.5f, -0.5f); glVertex3f(-0.5f,  0.5f,  0.5f); glVertex3f( 0.5f,  0.5f,  0.5f); glVertex3f( 0.5f,  0.5f, -0.5f);
        // Bottom face
        glNormal3f(0.0f, -1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f( 0.5f, -0.5f, -0.5f); glVertex3f( 0.5f, -0.5f,  0.5f); glVertex3f(-0.5f, -0.5f,  0.5f);
        glEnd();
    }

    // Initialize all animation and state variables
    void MyVirtualWorld::init() {
        walkTime = 0.0f;
        walkDistance = 0.0f;
        walking = false;

        breathingFire = false;
        fireTime = 0.0f;

        flying = false;
        flyTime = 0.0f;
        flyHeight = 0.0f;

        hasBox = true;
        isOpening = false;
        boxTime = 0.0f;
        environmentActive = true;
    }

    // Update animation timers per frame
    void MyVirtualWorld::tickTime() {
        // Update walking animation only if on the ground
        if (walking && flyHeight == 0.0f) {
            walkTime += 0.03f;
            walkDistance += 0.03f;
        }

        // Update fire breath animation (speed adjusted for cinematic slow-motion effect)
        if (breathingFire) {
            fireTime += 0.003f;
        }

        // Update flying altitude and animation time
        if (flying) {
            flyTime += 0.01f;
            if (flyHeight < 18.0f) flyHeight += 0.3f; // Smooth takeoff
        } else {
            if (flyHeight > 0.0f) flyHeight -= 0.3f;  // Smooth landing
            if (flyHeight < 0.0f) flyHeight = 0.0f;
        }

        // Global timer for environment elements (lava, embers, horn particles)
        alwaysTime += 0.0001f;

        if (isOpening && hasBox) {
            boxTime += 0.006f;
            if (boxTime >= 7.0f) {
                hasBox = false;
            }
        }
    }

    void MyVirtualWorld::triggerBoxOpen() {
        if (hasBox && !isOpening) {
                isOpening = true;
            }
        }

    void MyVirtualWorld::toggleFlying() {
        flying = !flying;
        if (flying) walking = false; // 起飞时自动停止走路
    }

    // Toggle fire breath ability
    void MyVirtualWorld::toggleFireBreath() {
        breathingFire = !breathingFire;
        if (breathingFire) {
            fireTime = 0.0f; // Reset fire animation timer on trigger
        }
    }

    // Toggle walking state
    void MyVirtualWorld::toggleWalking() {
        walking = !walking;
    }

    // Return current walking state
    bool MyVirtualWorld::isWalking() const {
        return walking;
    }

    // Master draw function for the Virtual World
    void MyVirtualWorld::draw() {
        glPushMatrix();

        // 1. Render Environment (Moves with the dragon to create an infinite treadmill effect)
        if (environmentActive) {
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, walkDistance);
            drawEnvironment();
            glPopMatrix();
        }

        if (hasBox) {
            drawBlindBox();
        }

        float charizardScale = 1.0f;
        if (hasBox) {
            if (boxTime < 3.6f) {
                charizardScale = 0.0f;
            } else if (boxTime < 4.2f) {
                charizardScale = (boxTime - 3.6f) * 2.0f;
            } else if (boxTime < 4.8f) {
                charizardScale = 1.2f - (boxTime - 4.2f) * 0.333f;
            } else {
                charizardScale = 1.0f;
            }
        }

        if (charizardScale > 0.0f) {
            glScalef(charizardScale, charizardScale, charizardScale);

            float bob = walking && flyHeight == 0.0f ? fabs(sin(walkTime * 2.0f)) * 0.28f : 0.0f;
            float bodyLean = walking && flyHeight == 0.0f ? sin(walkTime) * 2.0f : 0.0f;
            float hover = flyHeight > 0.0f ? sin(flyTime * 3.0f) * 1.5f : 0.0f;

            glTranslatef(0.0f, bob + flyHeight + hover, walkDistance);
            float flightPitch = (flyHeight / 18.0f) * 25.0f;
            glRotatef(flightPitch, 1.0f, 0.0f, 0.0f);
            glRotatef(bodyLean, 0.0f, 0.0f, 1.0f);

            drawBody();
            drawLeg(true);
            drawLeg(false);
            drawTail();
            drawWing(true);
            drawWing(false);
            drawArm(true);
            drawArm(false);
            drawHead();
        }

        glPopMatrix();
    }

    // Draw Head & Neck (Voxel Style)
    void MyVirtualWorld::drawHead() {
        glPushMatrix();
        glTranslatef(0.0f, 16.5f, 0.5f);

        // ==== A. Neck ====
        glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
        glPushMatrix();
        glTranslatef(0.0f, 1.5f, 0.0f);
        glScalef(3.4f, 3.8f, 3.2f);
        setColor(kBodyArmor);
        glutSolidCube(1.0);
        glPopMatrix();

        // Move up to the base of the skull
        glTranslatef(0.0f, 4.0f, 0.3f);
        glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);

        // ==== B. Main Skull ====
        glPushMatrix();
        glScalef(5.5f, 4.2f, 5.0f);
        setColor(kBodyArmor);
        glutSolidCube(1.0);
        glPopMatrix();

        // Top skull armor plate
        glPushMatrix();
        glTranslatef(0.0f, 2.3f, -0.6f);
        glScalef(5.0f, 1.1f, 4.2f);
        setColor(kBodyShadow);
        glutSolidCube(1.0);
        glPopMatrix();

        // Forehead center ridge
        glPushMatrix();
        glTranslatef(0.0f, 2.0f, 1.5f);
        glScalef(1.8f, 1.5f, 2.5f);
        setColor(kBodyShadow);
        glutSolidCube(1.0);
        glPopMatrix();

        // ==== C. Snout / Muzzle ====
        glPushMatrix();
        glTranslatef(0.0f, -1.0f, 3.0f);
        glScalef(3.0f, 2.8f, 3.0f);
        setColor(kBodyArmor);
        glutSolidCube(1.0);
        glPopMatrix();

        // Upper snout bridge
        glPushMatrix();
        glTranslatef(0.0f, -0.8f, 5.2f);
        glScalef(2.8f, 1.9f, 2.4f);
        setColor(kBodyShadow);
        glutSolidCube(1.0);
        glPopMatrix();

        // Nostrils (Black)
        setColor(kNostrilBlack);
        glPushMatrix(); glTranslatef(-0.75f, 0.1f, 6.1f); glScalef(0.5f, 0.32f, 0.3f); glutSolidCube(1.0); glPopMatrix();
        glPushMatrix(); glTranslatef( 0.75f, 0.1f, 6.1f); glScalef(0.5f, 0.32f, 0.3f); glutSolidCube(1.0); glPopMatrix();

        // ==== D. Lower Jaw ====
        glPushMatrix();
        glTranslatef(0.0f, -2.5f, 2.2f);
        glRotatef(20.0f, 1.0f, 0.0f, 0.0f); // Jaw slightly open
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 1.4f);
        glScalef(3.6f, 1.4f, 4.2f);
        setColor(kBodyShadow);
        glutSolidCube(1.0);
        glPopMatrix();
        glPopMatrix();

        // Inner Mouth (Purple)
        setColor(kMouthPurple);
        glPushMatrix(); glTranslatef(0.0f, -1.5f, 3.6f); glScalef(2.4f, 1.0f, 2.0f); glutSolidCube(1.0); glPopMatrix();

        // Teeth / Fangs
        setColor(kClawIvory);
        glPushMatrix(); glTranslatef(-1.2f, -1.6f, 5.1f); glScalef(0.5f, 1.2f, 0.5f); glutSolidCube(1.0); glPopMatrix();
        glPushMatrix(); glTranslatef( 1.2f, -1.6f, 5.1f); glScalef(0.5f, 1.2f, 0.5f); glutSolidCube(1.0); glPopMatrix();
        glPushMatrix(); glTranslatef( 0.0f, -1.6f, 5.3f); glScalef(0.4f, 0.9f, 0.4f); glutSolidCube(1.0); glPopMatrix();

        // ==== E. Eyes ====
        // Render both left and right eyes using a loop
        for (int s = -1; s <= 1; s += 2) {
            glPushMatrix();
            // Position shifted outward (2.4f) to prevent clipping with the snout
            glTranslatef(s * 1.8f, 0.9f, 2.7f);

            // Sclera (White)
            setColor(kEyeWhite);
            glPushMatrix(); glScalef(1.2f, 1.2f, 0.15f); glutSolidCube(1.0); glPopMatrix();

            // Iris (Red - for intimidating gaze)
            setColor(kRed);
            glPushMatrix(); glTranslatef(0.0f, 0.0f, 0.1f); glScalef(0.82f, 0.82f, 0.15f); glutSolidCube(1.0); glPopMatrix();

            // Pupil (Black)
            setColor(kNostrilBlack);
            glPushMatrix(); glTranslatef(0.0f, 0.0f, 0.19f); glScalef(0.36f, 0.40f, 0.15f); glutSolidCube(1.0); glPopMatrix();

            glPopMatrix();
        }

        // ==== F. Horns - Charizard Y style: 3 horns (center + 2 sides) ====

        // --- Center Horn (tall, sweeps straight back) ---
        glPushMatrix();
        glTranslatef(0.0f, 2.5f, -0.5f);
        glRotatef(-55.0f, 1.0f, 0.0f, 0.0f); // Sweep sharply backward
        for (int i = 0; i < 4; i++) {
            glPushMatrix();
            glTranslatef(0.0f, i * 1.3f, 0.0f);
            float scale = 1.1f - i * 0.22f;
            glScalef(scale, 1.4f, scale);
            setColor(kBodyShadow);
            glutSolidCube(1.0);
            glPopMatrix();
        }
        glPopMatrix();

        // --- Two Side Horns (shorter, angled outward) ---
        for (int s = -1; s <= 1; s += 2) {
            glPushMatrix();
            glTranslatef(s * 1.8f, 1.8f, -0.8f);
            glRotatef(-40.0f, 1.0f, 0.0f, 0.0f);
            glRotatef(s * 22.0f, 0.0f, 0.0f, 1.0f); // Spread outward

            for (int i = 0; i < 3; i++) {
                glPushMatrix();
                glTranslatef(0.0f, i * 1.2f, 0.0f);
                float scale = 0.9f - i * 0.25f;
                glScalef(scale, 1.3f, scale);
                setColor(kBodyShadow);
                glutSolidCube(1.0);
                glPopMatrix();
            }
            glPopMatrix();
        }

        // Trigger the Fire Breath rendering logic
        drawFireBreath();

        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();
    }

    // Dynamic Whisker Fire Particles (Embers flowing from the mouth corners)
    void MyVirtualWorld::drawWhiskerFire(bool isLeft) {
        float time = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
        float phase = isLeft ? 1.7f : 0.0f;
        float pulse = sin(time * 18.0f + phase) * 0.05f;

        // Core flame volume
        setColor(kBellyBright);
        glPushMatrix();
        glTranslatef(0.0f, 1.2f + pulse, -1.2f);
        glScalef(0.62f + pulse, 0.72f + pulse, 2.4f);
        glutSolidCube(1.0);
        glPopMatrix();

        // Outer flame volume
        setColor(kElectricBlue);
        glPushMatrix();
        glTranslatef(0.0f, 2.0f + sin(time * 12.0f + phase) * 0.08f, -2.9f);
        glScalef(0.45f, 0.9f + pulse, 1.5f);
        glutSolidCube(1.0);
        glPopMatrix();

        // Pre-defined offset data for 18 particles to ensure consistent streams
        // Data format: {startX, startY, startZ, sizeScale}
        float particleData[18][4] = {
            {-0.30f, 0.1f, -0.2f, 0.22f}, { 0.28f, 0.5f, -0.8f, 0.18f},
            { 0.05f, 0.9f, -1.4f, 0.25f}, {-0.22f, 1.3f, -1.9f, 0.16f},
            { 0.34f, 1.6f, -2.3f, 0.20f}, {-0.10f, 2.0f, -2.8f, 0.15f},
            { 0.22f, 0.2f, -1.1f, 0.17f}, {-0.35f, 0.7f, -1.6f, 0.21f},
            { 0.12f, 1.1f, -2.1f, 0.14f}, {-0.18f, 1.8f, -2.6f, 0.18f},
            { 0.30f, 2.2f, -3.1f, 0.13f}, {-0.04f, 2.5f, -3.4f, 0.16f},
            { 0.18f, 0.4f, -0.4f, 0.12f}, {-0.26f, 1.0f, -0.9f, 0.15f},
            { 0.38f, 1.5f, -1.7f, 0.12f}, {-0.14f, 2.1f, -2.2f, 0.14f},
            { 0.08f, 2.7f, -2.9f, 0.10f}, {-0.32f, 3.0f, -3.3f, 0.11f}
        };

        const float maxTravel = 3.4f;
        for (int i = 0; i < 18; i++) {
            float speed = 1.6f + (i % 5) * 0.25f;
            float travel = fmod(particleData[i][1] + time * speed + phase, maxTravel);

            // Fade out as particle travels further
            float fade = 1.0f - travel / maxTravel;
            if (fade < 0.0f) fade = 0.0f;

            // Physical swaying simulation
            float swayX = sin(time * 6.0f + i + phase) * 0.18f * (1.0f + travel * 0.15f);
            float swayZ = cos(time * 5.0f + i) * 0.10f - travel * 0.45f;
            float size = particleData[i][3] * fade;

            glPushMatrix();
            glTranslatef(particleData[i][0] + swayX,
                         particleData[i][1] + travel,
                         particleData[i][2] + swayZ);
            glScalef(size, size, size);

            // Alternating colors
            if ((i + (int)(time * 7.0f)) % 3 == 0) setColor(kBellyBright);
            else setColor(kElectricBlue);

            glutSolidCube(1.0);
            glPopMatrix();
        }
    }

    // Arm Rendering (Short, thick, powerful with large fists and three sharp claws)
    void MyVirtualWorld::drawArm(bool isLeft) {
        glPushMatrix();

        float dir = isLeft ? -1.0f : 1.0f;
        float armPhase = walkTime + (isLeft ? kPi : 0.0f);
        float armSwing = walking ? sin(armPhase) * 12.0f : 0.0f;

        // Shoulder joint: Located on the sides of the chest armor, slightly below wings
        glTranslatef(dir * 5.6f, 13.5f, 1.2f);

        // ==== A. Shoulder Pad (Broad and prominent) ====
        glPushMatrix();
        glScalef(3.2f, 3.0f, 3.4f);
        setColor(kJointArmor);
        glutSolidCube(1.0);
        glPopMatrix();

        // Orange spikes on top of the shoulder (distinct warm orange, not teal)
        glColor3f(0.95f, 0.38f, 0.00f);
        glPushMatrix(); glTranslatef(dir * 0.4f, 2.0f,  0.5f); glScalef(0.6f, 1.4f, 0.6f); glutSolidCube(1.0); glPopMatrix();
        glPushMatrix(); glTranslatef(dir * 0.1f, 2.0f, -0.6f); glScalef(0.5f, 1.1f, 0.5f); glutSolidCube(1.0); glPopMatrix();

        // Arm Rotation: Spread outward and lean forward
        glRotatef(dir * 35.0f, 0.0f, 0.0f, 1.0f);  // Outward spread
        glRotatef(25.0f + armSwing, 1.0f, 0.0f, 0.0f); // Forward lean / swinging motion

        // ==== B. Upper Arm (Short & Thick) ====
        glPushMatrix();
        glTranslatef(0.0f, -1.8f, 0.0f);
        glScalef(2.8f, 3.5f, 2.8f);
        setColor(kLimbArmor);
        glutSolidCube(1.0);
        glPopMatrix();

        // Move to elbow joint
        glTranslatef(0.0f, -3.5f, 0.0f);
        glRotatef(-50.0f, 1.0f, 0.0f, 0.0f); // Fold forearm forward

        // ==== C. Elbow Joint Armor ====
        glPushMatrix();
        glScalef(2.5f, 1.8f, 2.5f);
        setColor(kJointArmor);
        glutSolidCube(1.0);
        glPopMatrix();

        // ==== D. Forearm (Slightly thinner than upper arm) ====
        glPushMatrix();
        glTranslatef(0.0f, -1.6f, 0.0f);
        glScalef(2.4f, 3.2f, 2.4f);
        setColor(kLimbArmor);
        glutSolidCube(1.0);
        glPopMatrix();

        // ==== D2. Arm Blades / Hand Spines (Charizard Y - on the forearm sides) ====
        // Two short flat spines jutting outward from the forearm
        setColor(kBodyShadow);
        // Front blade (pointing forward/outward)
        glPushMatrix();
        glTranslatef(dir * 1.4f, -2.2f, 1.0f);
        glRotatef(dir * 30.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(-20.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.4f, 0.4f, 2.2f);
        setColor(kClawIvory);
        glutSolidCube(1.0);
        glPopMatrix();
        // Rear blade (shorter, behind the first)
        glPushMatrix();
        glTranslatef(dir * 1.3f, -1.4f, 0.2f);
        glRotatef(dir * 25.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.35f, 0.35f, 1.6f);
        setColor(kClawIvory);
        glutSolidCube(1.0);
        glPopMatrix();

        // Move to wrist joint
        glTranslatef(0.0f, -3.2f, 0.0f);
        glRotatef(20.0f, 1.0f, 0.0f, 0.0f);  // Slightly raise wrist

        // ==== E. Fist / Palm (Boxy and full) ====
        glPushMatrix();
        glTranslatef(0.0f, -1.0f, 0.8f);
        glScalef(2.8f, 2.2f, 3.0f);
        setColor(kJointArmor);
        glutSolidCube(1.0);
        glPopMatrix();

        // Knuckles (Adds detail to the fist)
        setColor(kBodyShadow);
        for (int i = -1; i <= 1; i++) {
            glPushMatrix();
            glTranslatef(i * 0.85f, -0.6f, 1.8f);
            glScalef(0.65f, 0.5f, 0.5f);
            glutSolidCube(1.0);
            glPopMatrix();
        }

        // Move to finger tips
        glTranslatef(0.0f, -1.2f, 2.0f);

        // ==== F. 3 Sharp Claws (Thick and powerful) ====
        setColor(kClawIvory);
        float clawOffsets[3] = {-0.95f, 0.0f, 0.95f};
        float clawLengths[3] = {1.8f, 2.2f, 1.8f};
        for (int i = 0; i < 3; i++) {
            glPushMatrix();
            glTranslatef(clawOffsets[i], 0.0f, clawLengths[i] * 0.35f);
            glRotatef(22.0f, 1.0f, 0.0f, 0.0f); // Hook downwards
            glScalef(0.55f, 0.55f, clawLengths[i]);
            glutSolidCube(1.0);
            glPopMatrix();
        }

        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();
    }

    // Wings Rendering (Wide span, aligned with skeletal / membrane structure)
    void MyVirtualWorld::drawWing(bool isLeft) {
        glPushMatrix();

        float dir = isLeft ? -1.0f : 1.0f;

        // 1. Mount point: Upper back scapula, higher and further back than arms
        glTranslatef(dir * 2.8f, 14.8f, -3.8f);

        // Global scale: Wingspan is roughly 3x body width
        glScalef(2.5f, 2.5f, 2.5f);

        // Wing Flap Animation Logic
        float wingFlap = 0.0f;
        if (flyHeight > 0.0f) {
            wingFlap = sin(flyTime * 0.2f) * 35.0f; // Fierce, wide flaps during flight
        } else if (walking) {
            wingFlap = sin(walkTime * 1.0f) * 4.0f;  // Gentle swaying while walking
        }

        // Base Rotations: Spread out horizontally, tilted up and leaning back
        glRotatef(dir * (62.0f + wingFlap), 0.0f, 1.0f, 0.0f);
        glRotatef(dir * 18.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(-8.0f,       1.0f, 0.0f, 0.0f);

        // ==== A. Inner Main Bone (Thick) ====
        glPushMatrix();
        glTranslatef(dir * 5.0f, 0.3f, 0.0f);
        glScalef(10.0f, 1.8f, 1.8f);
        setColor(kBodyShadow);
        glutSolidCube(1.0);
        glPopMatrix();

        // ==== B. Inner Wing Membrane (3 segments, deeper towards the edge) ====
        setColor(kWingMembrane);
        float innerDrops[3] = {4.5f, 6.5f, 9.0f};
        for (int i = 0; i < 3; i++) {
            glPushMatrix();
            float xPos = dir * (1.2f + i * 3.0f);
            float dropH = innerDrops[i];
            glTranslatef(xPos, -dropH / 2.0f - 0.6f, 0.0f);
            glScalef(3.2f, dropH, 0.22f);
            glutSolidCube(1.0);
            glPopMatrix();
        }

        // Move to mid-wing joint
        glTranslatef(dir * 10.0f, 0.0f, 0.0f);

        // ==== C. Mid-Joint & Upward Bone Spike ====
        setColor(kJointArmor);
        glPushMatrix();
        glTranslatef(dir * 0.3f, 2.2f, 0.3f);
        glScalef(1.4f, 3.5f, 1.4f);
        glutSolidCube(1.0);
        glPopMatrix();
        // Joint thickening block
        glPushMatrix();
        glScalef(2.0f, 2.0f, 2.0f);
        setColor(kBodyShadow);
        glutSolidCube(1.0);
        glPopMatrix();

        // Fold outer wing downwards to form a curved frontal silhouette
        glRotatef(dir * -22.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(dir * 8.0f,  0.0f, 1.0f, 0.0f); // Sweep slightly forward

        // ==== D. Outer Main Bone ====
        glPushMatrix();
        glTranslatef(dir * 5.5f, -0.5f, 0.0f);
        glScalef(11.0f, 1.5f, 1.5f);
        setColor(kBodyShadow);
        glutSolidCube(1.0);
        glPopMatrix();

        // ==== E. Outer Wing Membrane (4 segments) ====
        setColor(kWingDeep);
        float outerDrops[4] = {11.5f, 9.5f, 7.5f, 5.5f};
        for (int i = 0; i < 4; i++) {
            glPushMatrix();
            float xPos = dir * (1.2f + i * 2.6f);
            float dropH = outerDrops[i];
            glTranslatef(xPos, -dropH / 2.0f - 0.5f, 0.0f);
            glScalef(2.8f, dropH, 0.22f);
            glutSolidCube(1.0);
            glPopMatrix();
        }

        // Move to the end of outer bone
        glTranslatef(dir * 11.0f, -0.5f, 0.0f);

        // ==== F. Wing Tip: 3 Finger Bones fanning out ====
        setColor(kBodyShadow);
        float fingerYaw[3]   = {dir * 0.0f, dir * 18.0f, dir * -18.0f};
        float fingerLen[3]   = {6.5f, 5.0f, 5.0f};
        for (int f = 0; f < 3; f++) {
            glPushMatrix();
            glRotatef(fingerYaw[f], 0.0f, 1.0f, 0.0f);
            glTranslatef(dir * fingerLen[f] / 2.0f, 0.0f, 0.0f);
            glScalef(fingerLen[f], 1.1f, 1.1f);
            glutSolidCube(1.0);
            glPopMatrix();
        }

        // ==== G. Wing Tip Membrane (Between the finger bones) ====
        setColor(kWingDeep);
        float tipDrops[2] = {4.5f, 3.5f};
        float tipXPos[2]  = {dir * 1.5f, dir * 4.0f};
        for (int i = 0; i < 2; i++) {
            glPushMatrix();
            glTranslatef(tipXPos[i], -tipDrops[i] / 2.0f - 0.4f, 0.0f);
            glScalef(2.2f, tipDrops[i], 0.22f);
            glutSolidCube(1.0);
            glPopMatrix();
        }

        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();
    }

    // Tail Rendering (Sweeping J-curve with highly raised tip and dynamic flame)
    void MyVirtualWorld::drawTail() {
        glPushMatrix();

        // 1. Move to the base of the lower back
        glTranslatef(0.0f, 2.8f, -4.8f);
        if (walking) {
            glRotatef(sin(walkTime * 1.4f) * 6.0f, 0.0f, 1.0f, 0.0f); // Tail sway
        }

        // =========================================================
        // 2. Skeletal Structure (Extends back, curves, then rises)
        // =========================================================
        const int numSegments = 10;
        float lengths[numSegments] = {4.0f, 3.8f, 3.6f, 3.4f, 3.2f, 3.0f, 2.8f, 2.6f, 2.4f, 2.2f};
        float widths[numSegments]  = {5.0f, 4.6f, 4.2f, 3.8f, 3.4f, 3.0f, 2.6f, 2.2f, 1.8f, 1.4f};
        float heights[numSegments] = {5.0f, 4.6f, 4.2f, 3.8f, 3.4f, 3.0f, 2.6f, 2.2f, 1.8f, 1.4f};

        // Pure X-axis pitch angles to form the J-curve shape
        float pitchAngles[numSegments] = {
            -8.0f,  -5.0f,  // Base slightly points down
             0.0f,   4.0f,  // Gradually levels out
             7.0f,   9.0f,  // Gently starts rising
            11.0f,  11.0f,  // Continues upward
             9.0f,   7.0f   // Tip straightens out upwards
        };

        for (int i = 0; i < numSegments; i++) {
            if (i > 0) {
                glTranslatef(0.0f, 0.0f, -lengths[i-1]);
            }
            glRotatef(pitchAngles[i], 1.0f, 0.0f, 0.0f);

            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -lengths[i] / 2.0f);

            // Top Half: Dark gray body armor
            glPushMatrix();
            glTranslatef(0.0f, heights[i] * 0.15f, 0.0f);
            glScalef(widths[i], heights[i] * 0.85f, lengths[i] * 1.1f);
            setColor(kBodyShadow);
            glutSolidCube(1.0);
            glPopMatrix();

            // Bottom Half: Light cyan belly stripe
            glPushMatrix();
            glTranslatef(0.0f, -heights[i] * 0.35f, 0.0f);
            glScalef(widths[i] * 0.9f, heights[i] * 0.3f, lengths[i] * 1.1f);
            setColor(kBellyMid);
            glutSolidCube(1.0);
            glPopMatrix();

            glPopMatrix();
        }

        // =========================================================
        // 3. Dynamic Tail Flame & Particle Stream Animation
        // =========================================================
        glTranslatef(0.0f, 0.0f, -lengths[numSegments-1]);
        glRotatef(15.0f, 1.0f, 0.0f, 0.0f);

        float time = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
        float coreFlicker = sin(time * 16.0f) * 0.06f;
        float coreSway = cos(time * 4.0f) * 0.08f;

        // Core flame blocks
        setColor(kElectricBlue);
        glPushMatrix();
        glTranslatef(coreSway, 1.5f + coreFlicker, -0.2f);
        glScalef(3.0f + coreFlicker, 3.8f + sin(time * 20.0f) * 0.15f, 3.0f + coreFlicker);
        glutSolidCube(1.0);
        glPopMatrix();

        setColor(kBellyBright);
        glPushMatrix();
        glTranslatef(coreSway, 2.5f + coreFlicker, -0.2f);
        glScalef(3.4f + coreFlicker, 2.6f + coreFlicker, 3.4f + coreFlicker);
        glutSolidCube(1.0);
        glPopMatrix();

        // Surrounding flickering sub-flames
        setColor(kElectricBlue);
        glPushMatrix();
        glTranslatef(1.8f + sin(time * 8.0f) * 0.1f, 2.2f + cos(time * 9.0f) * 0.1f, 0.5f);
        glScalef(1.4f, 2.0f + sin(time * 12.0f) * 0.2f, 1.4f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-1.8f + cos(time * 7.0f) * 0.12f, 1.8f + sin(time * 10.0f) * 0.08f, -1.5f);
        glScalef(1.6f, 1.6f + cos(time * 14.0f) * 0.15f, 1.6f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, 3.0f + coreFlicker, 1.2f + sin(time * 6.0f) * 0.15f);
        glScalef(2.0f + coreFlicker, 1.4f, 1.4f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, 3.4f + cos(time * 11.0f) * 0.15f, -2.5f);
        glScalef(1.4f, 2.4f + sin(time * 15.0f) * 0.25f, 1.4f);
        glutSolidCube(1.0);
        glPopMatrix();

        // 15 Ascending tail flame particles
        float particleData[15][4] = {
            { 0.8f,  0.5f, -0.4f, 0.75f}, {-1.2f,  1.2f, -1.0f, 0.65f}, { 1.8f,  0.2f,  0.6f, 0.55f},
            {-0.7f,  2.0f, -1.2f, 0.70f}, { 0.3f,  1.5f, -0.5f, 0.45f}, { 1.3f,  0.8f, -1.6f, 0.50f},
            {-1.8f,  2.5f,  0.4f, 0.60f}, { 0.5f,  0.1f, -0.8f, 0.35f}, {-1.0f,  1.8f,  0.4f, 0.40f},
            { 2.2f,  1.0f, -0.2f, 0.50f}, {-2.0f,  0.6f,  0.8f, 0.55f}, { 0.0f,  2.2f, -0.1f, 0.30f},
            { 1.0f,  1.4f,  1.0f, 0.45f}, {-1.5f,  0.4f, -1.8f, 0.35f}, { 1.5f,  2.8f,  1.2f, 0.38f}
        };
        float maxHeight = 7.5f;
        for (int i = 0; i < 15; i++) {
            glPushMatrix();
            float speed = 2.2f + (i % 4) * 0.3f;
            // Particles cycle upwards
            float currentY = fmod(particleData[i][1] + time * speed, maxHeight);
            float swayX = sin(time * 5.5f + i) * 0.18f * (1.0f + currentY * 0.15f);
            float swayZ = cos(time * 4.5f + i) * 0.12f;

            // Shrink as they rise
            float sizeScale = 1.0f - (currentY / maxHeight) * 0.85f;
            if (sizeScale < 0.0f) sizeScale = 0.0f;
            float currentSize = particleData[i][3] * sizeScale;

            glTranslatef(particleData[i][0] + swayX, 3.0f + currentY, particleData[i][2] + swayZ);
            glScalef(currentSize, currentSize, currentSize);

            if ((i + (int)(time * 4)) % 3 == 0) setColor(kBellyBright);
            else setColor(kElectricBlue);
            glutSolidCube(1.0);
            glPopMatrix();
        }

        glPopMatrix();
    }

    // Legs Rendering: bulky thighs, large feet, and four toe claws.
    void MyVirtualWorld::drawLeg(bool isLeft) {
        glPushMatrix();

        float dir   = isLeft ? -1.0f : 1.0f;
        float xOff  = isLeft ? -4.8f : 4.8f;

        // Ground walking animation parameters
        float legPhase = walkTime + (isLeft ? 0.0f : kPi);
        float hipSwing = walking ? sin(legPhase) * 18.0f : 0.0f;
        float kneeBend = walking ? fabs(cos(legPhase)) * 12.0f : 0.0f;
        float footTilt = walking ? sin(legPhase + kPi * 0.35f) * 8.0f : 0.0f;

        // Flight transition logic: Use flyHeight for smooth interpolation
        float flyRatio = flyHeight / 18.0f; // Range 0.0 to 1.0
        // Thighs trail backward (-35 degrees)
        hipSwing = hipSwing * (1.0f - flyRatio) - (35.0f * flyRatio);
        // Knees straighten out (-30 degrees to offset default -60)
        kneeBend = kneeBend * (1.0f - flyRatio) - (30.0f * flyRatio);
        // Feet point downward (+40 degrees)
        footTilt = footTilt * (1.0f - flyRatio) + (40.0f * flyRatio);

        // Leg Mount point: Lower sides of the belly
        glTranslatef(xOff, 5.0f, 0.5f);

        // Tilt the entire leg slightly forward for a bipedal stance
        glRotatef(15.0f + hipSwing, 1.0f, 0.0f, 0.0f);

        // ==== A. Thigh (Thick block) ====
        glPushMatrix();
        glTranslatef(0.0f, -2.5f, 0.0f);
        glScalef(6.0f, 5.0f, 6.0f);
        setColor(kLimbArmor);
        glutSolidCube(1.0);
        glPopMatrix();

        // Thigh outer armor plate
        glPushMatrix();
        glTranslatef(dir * 1.6f, -1.5f, 0.5f);
        glScalef(1.0f, 2.5f, 1.8f);
        setColor(kBodyShadow);
        glutSolidCube(1.0);
        glPopMatrix();

        // Move to knee joint
        glTranslatef(0.0f, -5.0f, 0.0f);
        // Bend knee forward (-60 degrees default)
        glRotatef(-60.0f - kneeBend, 1.0f, 0.0f, 0.0f);

        // ==== B. Knee Armor Block ====
        glPushMatrix();
        glScalef(3.2f, 2.0f, 3.2f);
        setColor(kJointArmor);
        glutSolidCube(1.0);
        glPopMatrix();

        // ==== C. Shin / Calf ====
        glPushMatrix();
        glTranslatef(0.0f, -2.2f, 0.0f);
        glScalef(5.0f, 4.5f, 5.0f);
        setColor(kLimbArmor);
        glutSolidCube(1.0);
        glPopMatrix();

        // Move to ankle joint
        glTranslatef(0.0f, -4.5f, 0.0f);
        // Bend ankle forward (+35 degrees) to place foot flat on ground
        glRotatef(35.0f + footTilt, 1.0f, 0.0f, 0.0f);

        // ==== D. Foot (Wide and flat) ====
        glPushMatrix();
        glTranslatef(0.0f, -0.8f, 1.6f);
        glScalef(4.5f, 1.8f, 6.5f);
        setColor(kJointArmor);
        glutSolidCube(1.0);
        glPopMatrix();

        // Heel armor block
        glPushMatrix();
        glTranslatef(0.0f, -0.5f, -1.0f);
        glScalef(3.2f, 1.5f, 2.0f);
        setColor(kBodyShadow);
        glutSolidCube(1.0);
        glPopMatrix();

        // ==== E. Toe Claws (4 spread-out claws) ====
        glTranslatef(0.0f, -1.0f, 4.5f);
        setColor(kClawIvory);
        float clawX[4] = {-1.65f, -0.55f, 0.55f, 1.65f};
        float clawLen[4] = {1.6f, 1.9f, 1.9f, 1.6f};
        for (int i = 0; i < 4; i++) {
            glPushMatrix();
            glTranslatef(clawX[i], 0.0f, clawLen[i] * 0.4f);
            glRotatef(18.0f, 1.0f, 0.0f, 0.0f); // Hook downwards
            glScalef(0.55f, 0.55f, clawLen[i]);
            glutSolidCube(1.0);
            glPopMatrix();
        }

        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();
    }

    // Body Rendering (Barrel-shaped proportions)
    void MyVirtualWorld::drawBody() {
        glPushMatrix();

        // 4 Vertical Layers: Tapering from the wide belly (h1) up to the neck base (h4)
        float h1 = 9.0f, h2 = 3.5f, h3 = 3.0f, h4 = 2.5f;

        // Layer 1: Main Belly (Widest part, bright cyan front, dark gray back)
        glPushMatrix();
        glTranslatef(0.0f, h1 / 2.0f, 0.0f);
        glScalef(9.5f, h1, 8.5f);
        drawTwoColorCube(kBellyBright.r, kBellyBright.g, kBellyBright.b,
                         kBodyArmor.r,   kBodyArmor.g,   kBodyArmor.b);
        glPopMatrix();

        // Layer 2: Upper Belly (Slightly narrower)
        glTranslatef(0.0f, h1, 0.0f);
        glPushMatrix();
        glTranslatef(0.0f, h2 / 2.0f, 0.0f);
        glScalef(8.0f, h2, 7.5f);
        drawTwoColorCube(kBellyMid.r, kBellyMid.g, kBellyMid.b,
                         kBodyArmor.r, kBodyArmor.g, kBodyArmor.b);
        glPopMatrix();

        // Layer 3: Chest Armor
        glTranslatef(0.0f, h2, 0.0f);
        glPushMatrix();
        glTranslatef(0.0f, h3 / 2.0f, 0.0f);
        glScalef(6.5f, h3, 6.0f);
        drawTwoColorCube(kBellyDark.r, kBellyDark.g, kBellyDark.b,
                         kBodyShadow.r, kBodyShadow.g, kBodyShadow.b);
        glPopMatrix();

        // Layer 4: Base of the Neck
        glTranslatef(0.0f, h3, 0.0f);
        glPushMatrix();
        glTranslatef(0.0f, h4 / 2.0f, 0.0f);
        glScalef(4.5f, h4, 4.5f);
        drawTwoColorCube(kBellyDark.r, kBellyDark.g, kBellyDark.b,
                         kBodyShadow.r, kBodyShadow.g, kBodyShadow.b);
        glPopMatrix();

        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();
    }

    // Core Animation: Orange Dragon Breath (Ultimate dual-layer: continuous beam + looping scattered particles)
    void MyVirtualWorld::drawFireBreath() {
        if (!breathingFire) {
            glDisable(GL_LIGHT1); // Turn off mouth light source when not breathing fire
            return;
        }

        glPushMatrix();

        // 1. Target the depths of the throat
        glTranslatef(0.0f, -1.5f, 4.0f);

        // 2. Enable dynamic orange light source from the mouth
        GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat lightColor[] = {1.00f, 0.50f, 0.05f, 1.0f};
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
        glEnable(GL_LIGHT1);

        float maxDist = 28.0f; // Maximum fire range

        // Calculate current beam reach to simulate "spraying" outwards
        float currentBeamLength = fireTime * 25.0f;
        if (currentBeamLength > maxDist) currentBeamLength = maxDist;

        // ====================================================================
        // 🔥 Layer 1: Continuous Main Beam (Simulates high pressure plasma)
        // ====================================================================
        for(float z = 0; z < currentBeamLength; z += 1.2f) {
            glPushMatrix();

            // Slower pulsing and swaying for a cinematic slow-motion feel
            float swayX = sin(z * 0.4f + fireTime * 5.0f) * (0.1f + z * 0.04f);
            float swayY = cos(z * 0.3f + fireTime * 4.0f) * (0.1f + z * 0.04f);
            glTranslatef(swayX, swayY, z);

            glRotatef(fireTime * 60.0f + z * 20.0f, 0.0f, 0.0f, 1.0f);

            float scale = 0.8f + (z * 0.12f);
            scale *= (1.0f + 0.15f * sin(fireTime * 25.0f + z));
            glScalef(scale, scale, scale);

            if ((int)(z * 10 + fireTime * 10) % 3 == 0) glColor3f(1.0f, 0.92f, 0.55f); // Bright yellow-orange core
            else glColor3f(1.0f, 0.42f, 0.02f);                                           // Deep orange beam

            glutSolidCube(1.0);
            glPopMatrix();
        }

        // ====================================================================
        // ✨ Layer 2: Infinite Looping Outer Particles (Sparks & Embers)
        // ====================================================================
        int numParticles = 60;
        for(int i = 0; i < numParticles; i++) {
            float speed = 6.0f + (i % 5) * 3.0f;
            float offset = (i % 10) * 0.2f;
            float angle = i * 0.628f;
            float radius = 0.2f + (i % 4) * 0.15f;

            // Use fmod to loop particles infinitely within the maxDist range
            float currentZ = fmod(fireTime * speed + offset, maxDist);

            // Only display particles that are within the current beam progression
            if (currentZ <= currentBeamLength) {

                float spreadX = cos(angle) * radius * currentZ;
                float spreadY = sin(angle) * radius * currentZ;

                spreadX += sin(currentZ * 0.5f + i) * 0.6f;
                spreadY += cos(currentZ * 0.5f + i) * 0.6f + (currentZ * 0.12f);

                float size = (1.0f - (currentZ / maxDist)) * (0.5f + (i % 3) * 0.3f);

                glPushMatrix();
                glTranslatef(spreadX, spreadY, currentZ);

                // Tumble the block geometry
                glRotatef(fireTime * 120.0f + i * 70.0f, 1.0f, 0.6f, 0.3f);
                glScalef(size, size, size);

                if (i % 5 == 0) glColor3f(1.0f, 0.95f, 0.70f);       // Bright cream-yellow sparks
                else if (i % 2 == 0) glColor3f(1.0f, 0.62f, 0.05f);  // Mid orange embers
                else glColor3f(0.90f, 0.28f, 0.02f);                  // Deep red-orange outer sparks

                glutSolidCube(1.0);
                glPopMatrix();
            }
        }

        glPopMatrix();
    }

    // Core Environment: Blue Abyss V2 — Obsidian Magma (Voxel Remastered)
    void MyVirtualWorld::drawEnvironment() {
        glPushMatrix();

        // 1. Lower the baseline to support the Charizard's feet (Y=-7.0f)
        glTranslatef(0.0f, -7.0f, 0.0f);

        // ====================================================================
        // 🔥 A. Obsidian Rift Surface (Voxel Broken Surface)
        // ====================================================================
        int gridSize = 8;
        float blockSize = 7.0f;

        for (int x = -gridSize; x <= gridSize; x++) {
            for (int z = -gridSize; z <= gridSize; z++) {
                // Generate irregular, chaotic terrain using sin/cos noise
                float noiseVal = fabs(sin(x * 9.3f + z * 5.1f));
                float baseHeight = 3.0f + noiseVal * 4.0f;

                glPushMatrix();
                glTranslatef(x * blockSize, -baseHeight / 2.0f, z * blockSize);

                // --- Voxel Obsidian Blocks ---
                glScalef(blockSize * 0.9f, baseHeight, blockSize * 0.9f); // 0.9 to leave cracks

                // Depth shading: Varying shades of warm volcanic dark stone
                if (noiseVal < 0.3f) glColor3f(0.06f, 0.03f, 0.02f);      // Deepest dark charcoal
                else if (noiseVal < 0.7f) glColor3f(0.10f, 0.06f, 0.03f); // Dark warm brown-black
                else glColor3f(0.14f, 0.09f, 0.04f);                      // Lighter volcanic rock

                glutSolidCube(1.0);
                glPopMatrix();

                // --- Voxel Lava Pools ---
                if (noiseVal > 0.45f) { // Spawn lava in specific cracks
                    glPushMatrix();

                    // Fast pulsing breathing effect for the lava
                    float magmaPulse = sin(alwaysTime * 9.0f + x + z) * 0.6f;

                    // Expand lava pools slightly irregularly across X/Z axes
                    float poolW = blockSize * (0.3f + (x%2==0?0.15f:0.05f));
                    float poolH = blockSize * (0.3f + (z%2==0?0.15f:0.05f));

                    glTranslatef(x * blockSize + poolW * 1.5f, -0.6f + magmaPulse, z * blockSize + poolH * 1.5f);
                    glScalef(poolW, 1.2f, poolH);

                    // High contrast between bright orange and red-orange for a scorching lava look
                    if ((x + z) % 3 == 0) glColor3f(0.99f, 0.45f, 0.01f);
                    else glColor3f(0.99f, 0.25f, 0.02f);

                    glutSolidCube(1.0);
                    glPopMatrix();
                }
            }
        }

        // ====================================================================
        // ✨ B. Environmental Voxel Abyssal Fog Layer
        // ====================================================================
        int fogGrid = 15;
        for (int fx = -fogGrid; fx <= fogGrid; fx += 2) {
            for (int fz = -fogGrid; fz <= fogGrid; fz += 2) {
                // Weak, slowly drifting fog particles hovering above the ground
                float fogY = 1.8f + sin(alwaysTime * 1.5f + fx + fz) * 0.5f;
                float size = 0.3f + 0.15f * sin(alwaysTime * 5.0f + fx);

                glPushMatrix();
                glTranslatef(fx * 4.0f, fogY, fz * 4.0f);
                glRotatef(alwaysTime * 30.0f, 0.0f, 1.0f, 0.0f);
                glScalef(size, size, size);

                // Warm orange ember-glow fog color
                glColor3f(0.70f, 0.28f, 0.05f);

                glutSolidCube(1.0);
                glPopMatrix();
            }
        }

        // ====================================================================
        // ✨ C. Blue Embers (Particles floating upwards)
        // ====================================================================
        int numEmbers = 150;
        float arenaSize = gridSize * blockSize;
        for (int i = 0; i < numEmbers; i++) {
            float startX = sin(i * 15.4f) * arenaSize;
            float startZ = cos(i * 9.2f) * arenaSize;
            float speed = 8.0f + (i % 5) * 2.0f;

            // Loop particles vertically from the ground to the sky
            float riseY = fmod(alwaysTime * speed + (i * 3.1f), 45.0f);

            // Simulating air currents
            float swayX = sin(riseY * 0.15f + i) * 4.0f;
            float swayZ = cos(riseY * 0.1f + i) * 3.0f;

            float size = (1.0f - (riseY / 45.0f)) * (0.6f + (i % 3) * 0.4f);
            size *= (1.0f + 0.3f * sin(alwaysTime * 10.0f + i));

            glPushMatrix();
            glTranslatef(startX + swayX, riseY, startZ + swayZ);
            glRotatef(alwaysTime * 80.0f + i * 40.0f, 1.0f, 1.0f, 1.0f); // Fast tumbling
            glScalef(size, size, size);

            if (i % 4 == 0) setColor(kClawIvory);
            else if (i % 2 == 0) setColor(kBellyBright);
            else setColor(kElectricBlue);

            glutSolidCube(1.0);
            glPopMatrix();
        }

        glPopMatrix();
    }


    void MyVirtualWorld::drawBlindBox() {
        glPushMatrix();

        float boxSize = 18.0f;
        float thickness = 1.0f;

        float wobbleAngle = 0.0f;
        float shakeX = 0.0f;
        float pi = 3.14159265f;

        if (boxTime > 0.5f && boxTime < 1.0f) {
            wobbleAngle = sin((boxTime - 0.5f) * pi * 2.0f) * 18.0f;
        } else if (boxTime > 1.5f && boxTime < 2.0f) {
            wobbleAngle = sin((boxTime - 1.5f) * pi * 2.0f) * -18.0f;
        } else if (boxTime > 2.5f && boxTime < 3.1f) {
            wobbleAngle = sin((boxTime - 2.5f) * pi * 1.666f) * 25.0f;
            shakeX = sin((boxTime - 2.5f) * pi * 10.0f) * 0.5f; // 附带平移震动
        }


        float burstAngle = 0.0f;
        float topFlyY = 0.0f;
        float topSpin = 0.0f;
        float sinkY = 0.0f;

        if (boxTime >= 2.0f) {
            float progress = (boxTime - 2.0f);
            burstAngle = progress * 80.0f;
            topFlyY = progress * 30.0f;
            topSpin = progress * 300.0f;
            sinkY = progress * -8.0f;
        }

        glTranslatef(shakeX, -7.0f + sinkY, 0.0f);
        glRotatef(wobbleAngle, 0.0f, 0.0f, 1.0f);


        for (int i = 0; i < 4; i++) {
            glPushMatrix();

            bool isFront = false;
            if (i == 0) {
                glTranslatef(0.0f, 0.0f, boxSize / 2.0f);
                glRotatef(burstAngle, 1.0f, 0.0f, 0.0f);
                glTranslatef(0.0f, boxSize / 2.0f, 0.0f);
                isFront = true;
            } else if (i == 1) {
                glTranslatef(0.0f, 0.0f, -boxSize / 2.0f);
                glRotatef(-burstAngle, 1.0f, 0.0f, 0.0f);
                glTranslatef(0.0f, boxSize / 2.0f, 0.0f);
                glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
            } else if (i == 2) {
                glTranslatef(-boxSize / 2.0f, 0.0f, 0.0f);
                glRotatef(burstAngle, 0.0f, 0.0f, 1.0f);
                glTranslatef(0.0f, boxSize / 2.0f, 0.0f);
                glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
            } else if (i == 3) {
                glTranslatef(boxSize / 2.0f, 0.0f, 0.0f);
                glRotatef(-burstAngle, 0.0f, 0.0f, 1.0f);
                glTranslatef(0.0f, boxSize / 2.0f, 0.0f);
                glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            }

            glPushMatrix();
            glTranslatef(0.0f, boxSize * 0.25f + 0.25f, 0.0f);
            glScalef(boxSize, boxSize * 0.5f - 0.5f, thickness);
            glColor3f(0.85f, 0.15f, 0.18f);
            glutSolidCube(1.0);
            glPopMatrix();

            glPushMatrix();
            glScalef(boxSize + 0.2f, 1.5f, thickness + 0.2f);
            glColor3f(0.15f, 0.15f, 0.15f);
            glutSolidCube(1.0);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(0.0f, -boxSize * 0.25f - 0.25f, 0.0f);
            glScalef(boxSize, boxSize * 0.5f - 0.5f, thickness);
            glColor3f(0.95f, 0.95f, 0.95f);
            glutSolidCube(1.0);
            glPopMatrix();

            if (isFront) {
                // 黑色底座圆环
                glPushMatrix();
                glTranslatef(0.0f, 0.0f, thickness * 0.5f + 0.2f);
                glScalef(5.0f, 5.0f, 0.6f);
                glColor3f(0.15f, 0.15f, 0.15f);
                glutSolidCube(1.0);
                glPopMatrix();


                glPushMatrix();
                glTranslatef(0.0f, 0.0f, thickness * 0.5f + 0.6f);

                if (boxTime < 3.6f) {
                    if (boxTime > 3.1f && boxTime < 3.6f) {
                        float throb = 1.0f + 0.4f * fabs(sin(boxTime * 30.0f));
                        glColor3f(1.0f, 0.2f, 0.2f);
                        glScalef(2.8f * throb, 2.8f * throb, 0.4f);
                    } else {
                        float throb = 1.0f;
                        if ((boxTime > 1.0f && boxTime < 1.2f) || (boxTime > 2.0f && boxTime < 2.2f)) {
                            throb = 1.3f;
                            glColor3f(1.0f, 1.0f, 0.8f);
                        } else {
                            glColor3f(0.6f, 0.6f, 0.6f);
                        }
                        glScalef(2.8f * throb, 2.8f * throb, 0.4f);
                    }
                } else {
                    glColor3f(0.2f, 0.2f, 0.2f);
                    glScalef(2.8f, 2.8f, 0.4f);
                }

                glutSolidCube(1.0);
                glPopMatrix();
            }
            glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(0.0f, thickness / 2.0f, 0.0f);
        glScalef(boxSize, thickness, boxSize);
        glColor3f(0.95f, 0.95f, 0.95f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, boxSize + topFlyY, 0.0f);
        glRotatef(topSpin, 1.0f, 1.0f, 0.0f);
        glScalef(boxSize + 0.5f, thickness, boxSize + 0.5f);
        glColor3f(0.85f, 0.15f, 0.18f);
        glutSolidCube(1.0);
        glPopMatrix();

        glPopMatrix();
    }
    void MyVirtualWorld::setEnvironmentActive(bool active) {
        environmentActive = active;
    }
    bool MyVirtualWorld::isEnvironmentActive() const {
        return environmentActive;
    }
}
