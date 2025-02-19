#ifndef PLAYERKNUCKLES_H
#define PLAYERKNUCKLES_H

class TexturedModel;
class Triangle3D;
class ParticleTexture;
class Source;
class PlayerModel;
class Source;
class ShieldGreen;
class ShieldMagnet;
class GuiImage;

#include <unordered_set>
#include <vector>
#include "entity.hpp"
#include "controllableplayer.hpp"
#include "../toolbox/vector.hpp"

class PlayerKnuckles : public ControllablePlayer
{
private:
    PlayerModel* playerModel = nullptr;

    //bool onGround = false;
    //Vector3f vel; //Direction we are currently going
    //Vector3f relativeUp; //What direction is "up", aka grounds normal
    Vector3f relativeUpSmooth; //What direction is "up", aka grounds normal
    Vector3f relativeUpAnim; //relative up to use for animations. smooth less than relativeUpSmooth
    //Vector3f camDir; //Direction the camera wants to be facing
    //Vector3f camDirSmooth; //Direction the camera is facing curerntly
    Vector3f camUpSmooth; //the up value to use for camera

    const float FLOOR_OFFSET = 0.1f; //How much you stay away from the floor
    Triangle3D* currentTriangle = nullptr; //the triangle object that you are touching
    const float smoothTransitionThreshold = 0.6f; //Dot product threshold for running between triangles
    const float surfaceTension = 10.0f;     //To not fly off the ground
    const float hitWallTimePunish = 0.0f;   //How long you can't move after hitting a wall 0.125

    const float wallStickThreshold = 0.45f;  //How steep a slope must be to be considered a wall
    const float wallStickTimerMax = 0.5f; //How much time you can stick to a wall while running slow
    const float wallStickSpeedRequirement = 100.0f; //how much speed you need to stick to a wall
    float wallStickTimer = 0.0f;

    const float gravityForce = 280.0f;
    const float gravityTerminal = -650.0f;
    const float gravityApproach = 0.45f;

    bool isJumping = false;
    const float jumpPower = 120.0f; //Initial speed you get form jump
    const float hoverPower = 190.0f; //How much hover adds to your vel
    const float hoverTimerThreshold = 1.0f; //How long you can hover for
    float hoverTimer = 0.0f;
    const float jumpPowerPulley = 102.0f;

    const float groundRunPush = 140.0f;
    const float groundRunPushSpeedshoes = 300.0f;
    const float groundRunPushThreshold = 33.0f; //when going slower than this, give extra push
    const float groundRunFriction = 0.5f;
    const float groundNeutralFriction = 1.5f;
    const float skidPower = -5.0f;
    const float airRunPush = 25.0f;
    const float airRunFrictionUp = 0.75f;   //maybe make another variable for this to use when vel.y < 0?
    const float airRunFrictionDown = 0.25f; //For when sonic is falling down
    const float airFrictionThreshold = 50.0f; //When youre going slower than this horizontally, don't apply friction
    const float airNeutralFriction = 1.25f;
    const float slopeRunAccel = 15000.0f;

    bool isBall = false;
    //version 1
    //const float groundBallFriction = 0.1f;
    //const float slopeBallAccel = 42000.0f;
    //version 2
    const float groundBallFriction = 0.1f; //0.02f;
    const float slopeBallAccel = 140.0f; //50.0f;
    const float autoUnrollThreshold = 120.0f; //60
    const float maxBallSpeed = 800.0f; //you wont go faster than this when rolling down a hill in ball form

    bool isSpindashing = false;
    Vector3f spindashDirection;
    bool canStartSpindash = false;
    bool bufferedSpindashInput = false;
    const float spindashPowerMax = 500.0f; //380.0f for tails
    const float spindashChargeRate = 0.4f*60*60;
    const float spindashFriction = 2.0f;
    const float spindashPowerfulFriction = 8.0f;
    const float spindashPowerfulFrictionThreshold = 100.0f;
    const float spindashDelay = 0; //how long the timer waits before it starts applying spindash friction
    float spindashTimer = 0;
    const float spindashTimerMax = 35.0f/60.0f;
    float spindashReleaseTimer = 0;
    const float spindashReleaseTimerMax = 30.0f/60.0f;
    float spindashRestartDelay = 0;
    const float spindashRestartDelayMax = 25.0f/60.0f; // 3.0f/60.0f
    float storedSpindashSpeed = 0;

    const float waterPush = 460.0f;
    const float waterFriction = 4.0f;
    float swimStrokeTimer = 0.0f;

    bool isStomping = false;
    const float stompSpeed = -350.0f;

    bool isSkidding = false;
    const float skidAudioThreshold = 150.0f;

    bool isGrinding = false;
    Source* sourceGrind = nullptr;

    bool isGrabbing = false;

    bool isGliding = false;
    float canGlideTimer = 0.0f;
    const float canGlideTimerMax = 0.5f;
    const float glideGravityTerminal = -150.0f;//-150.0f;
    const float glideGravityApproach = 0.1f;
    //const float glideTerminal = 400.0f;
    //const float glideApproach = 0.4f;
    const float airGlideSpeedMax = 210.0f;
    const float airGlideSpeedMin = 60.0f;
    const float airGlideSpeedStart = 110.0f;
    const float airGlidePush = 111.0f; //0.03104575 = sa2
    const float airGlideFriction = 0.24f;
    const float airGlideNeutralFriction = 0.24f;
    Source* sourceGlide = nullptr;

    bool isDrillDiving = false;

    //If true, the velocity the player has will make them move.
    //If false, the velocity will only change the camera direction.
    bool velocityMovesPlayer = true;

    bool isLightdashing = false;
    std::vector<Vector3f> lightdashTrail;
    float lightdashTrailProgress = -1.0f;
    const float lightdashStartRingMinDist = 35.0f; //how close does the ring need to be to start lightdashing?
    const float lightdashContinueRingMinDist = 40.0f; //how close does the next ring have to be to coninue the trail?
    const float lightdashSpeed = 600.0f;
    const float lightdashExitSpeed = 300.0f;

    const float bounceVel = -300.0f;
    const float bounceFactor = 0.7f;
    bool isBouncing = false;
    bool justBounced = false; //Are we in the air after doing a bounce?

    bool justHomingAttacked = false; //Are we in the air after doing a homing attack?
    float homingAttackTimer = -1.0f; // less than 0 = can start a homing attack
    const float homingAttackTimerMax = 0.2f;
    bool isHomingOnPoint = false;
    const float homingAttackRangeMax =  115.0f;
    const float homingAttackDotThreshold = 0.65f;
    GuiImage* homingAttackReticle = nullptr;

    const float breakObjectsSpeed = 200.0f; //how fast you need to be going to break through objects like boxes

    //const float camAngleLookdown = 0.0f; //0.2f; //How much the camera looks down
    float camRadius               = 60.0f;  //Camera radius at zero speed
    float camRadiusTarget         = 60.0f;
    const float camRadiusMax      = 60.0f;
    const float camRadiusMin      = 25.0f;
    const float camRadiusApproach = 3.0f;
    const float camRadiusSpeed    = 20.0f;
    const float camHeightOffset   = 5.0f; //Point above the vehicle that the camera focuses on

    float hitTimer = 0.0f; //when you get hit by something, like an enemy or spikes

    float hitFlashingTimer = 0.0f;

    float hitSpringTimer = 0.0f;

    float deadTimer = -1.0f;

    float invincibleTimer = 0.0f;

    float speedShoesTimer = 0.0f;

    float diggingTimer = 0.0f;
    const float diggingTimerMax = 1.116666f;

    bool isClimbing = false;
    const float climbSpeed = 60.0f;
    const float climbSlopeStopFloor = 0.6f; //normal y component that makes knuckles stop climbing
    const float climbSlopeStopCeiling = -0.8f;
    const float smoothClimbTransitionThreshold = 0.3f; //How similar 2 triangles must be to climb between them

    float punchingTimer = 0;
    const float punchingTimerMax = 0.3333333f;
    float punchSpeed = 0.0f;
    const float punchSpeedFast = 3.3f*1.1f*60;
    const float punchSpeedSlow = 0.7f*1.5f*60;
    const float punchSpeedThreshold = 1.7f*1.05f*60;

    bool isReadingHint = false;

    ShieldMagnet* myShieldMagnet = nullptr;
    ShieldGreen*  myShieldGreen  = nullptr;

    int combo = 0;

    // Entities that are nearby
    std::vector<std::unordered_set<Entity*>*> nearbyEntities;

    //animation variables
    float animationTime = 0.0f;
    Vector3f centerPosPrev;
    Vector3f invincibleColor1;
    Vector3f invincibleColor2;

    //input variables
    bool  inputJump;
    bool  inputJumpPrevious;
    bool  inputAction;
    bool  inputActionPrevious;
    bool  inputAction2;
    bool  inputAction2Previous;
    bool  inputAction3;
    bool  inputAction3Previous;
    float inputX;
    float inputY;
    float inputX2;
    float inputY2;
    int   inputZoom;

    void moveMeGround();

    void moveMeAir();

    void spindash();

    void homingAttack(Vector3f* target, bool homeInOnIt);

    bool findHomingTarget(Vector3f* target);

    void calcSpindashDirection();

    void setInputs();

    void updateAnimationValues();

public:
    PlayerKnuckles();
    PlayerKnuckles(float x, float y, float z);
    ~PlayerKnuckles();

    void step();

    //Call this to "redraw" sonic
    void animate();

    //updates the global camera's values based on 
    // this player's values
    void refreshCamera();

    void setVelocity(float xVel, float yVel, float zVel);

    Vector3f* getVelocity();

    void popOffWall();

    void die();

    bool isDying();

    void hitSpring(Vector3f* direction, float power, float lockInputTime, bool resetsCamera);

    void hitSpringYellow(const Vector3f* direction, float power, float lockInputTime);

    void hitSpringTriple(Vector3f* direction, float power, float lockInputTime);

    void hitSpeedRamp(Vector3f* direction, float speed, float lockInputTime);

    void hitDashpad();

    bool isVulnerable();

    bool canDealDamage();

    bool canBreakObjects();

    void takeDamage(Vector3f* source);

    void rebound(Vector3f* source);

    void setCanMoveTimer(float newTimer);

    void startGrinding();

    void stopGrinding();

    //void setIsBall(bool newIsBall);

    void setInWater(float newWaterHeight);

    void increaseCombo();

    void startGrabbing();

    void stopGrabbing();

    //void setOnGround(bool newOnGround);

    ShieldMagnet* getShieldMagnet();

    void setShieldMagnet(ShieldMagnet* newMagnet);

    ShieldGreen* getShieldGreen();

    void setShieldGreen(ShieldGreen* newGreen);

    void setInvincibleTimer(float newTimer);

    void setSpeedshoesTimer(float newTimer);

    float getDiggingTimer();

    //called by Rail when sonic jumps off
    void jump();

    //Called by the Pulley when Sonic jumps off, separate due to unique properties required for pulley jump.
    //Specifically, Sonic needs to basically go completely horizontally off the pulley, and should go straight
    //forward with neutral stick.
    void jumpOffPulley(Vector3f forwardDirectionVector);

    void setHoverTimer(float newTime);

    void setRelativeUp(Vector3f* newUp);

    std::vector<TexturedModel*>* getModels();

    void loadVehicleInfo();

    static void deleteStaticModels();

    bool isVehicle() const;

    float getHitboxHorizontal();

    float getHitboxVertical();

    Vector3f* getCameraDirection();

    void setCameraDirection(Vector3f* newDirection);

    Vector3f getCenterPosition();

    bool canGrabDiggablePiece();

    bool canReadHint();

    void setReadingHint(bool isReadingHint);
};
#endif
