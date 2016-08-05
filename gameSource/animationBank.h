#ifndef ANIMATION_BANK_INCLUDED
#define ANIMATION_BANK_INCLUDED


#include "minorGems/game/doublePair.h"

#include "minorGems/util/SimpleVector.h"

#include "objectBank.h"


typedef enum AnimType {
    ground = 0,
    held,
    moving,
    endAnimType
    } AnimType;


typedef struct SpriteAnimationRecord {
        double xOscPerSec;
        // in pixels
        double xAmp;
        // between 0 and 1
        double xPhase;

        double yOscPerSec;
        double yAmp;
        double yPhase;

        doublePair rotationCenterOffset;

        // can be positive (CW) or negative (CCW)
        double rotPerSec;
        double rotPhase;

        double rockOscPerSec;
        // between 0 and 1, where 1 is full rotation before coming back
        double rockAmp;
        double rockPhase;
        
        // for animations that run for a while and pause between runs
        // ticking cogs, twitching animal noses, etc.
        double durationSec;
        double pauseSec;
        
    } SpriteAnimationRecord;


void zeroRecord( SpriteAnimationRecord *inRecord );



typedef struct AnimationRecord {
        int objectID;
        
        AnimType type;
        
        int numSprites;
        int numSlots;
        
        SpriteAnimationRecord *spriteAnim;
        SpriteAnimationRecord *slotAnim;
        
    } AnimationRecord;




typedef struct LayerSwapRecord {
        int indexA;
        int indexB;
    } LayerSwapRecord;



AnimationRecord *copyRecord( AnimationRecord *inRecord );


// returns number of animations that need to be loaded
int initAnimationBankStart( char *outRebuildingCache );

float initAnimationBankStep();
void initAnimationBankFinish();



void freeAnimationBank();



// return value not destroyed by caller
// should not be modifed by caller
// change an animation by re-adding it using addAnimation below
AnimationRecord *getAnimation( int inID, AnimType inType );


// record destroyed by caller
void addAnimation( AnimationRecord *inRecord );


void clearAnimation( int inObjectID, AnimType inType );


// do we need a smooth fade when transitionion from current animation
// to a given target animation?
char isAnimFadeNeeded( int inObjectID, AnimType inCurType, 
                       AnimType inTargetType );

char isAnimFadeNeeded( int inObjectID,
                       AnimationRecord *inCurR, AnimationRecord *inTargetR );


// returns true if animation is all-0 for both motion and phase
char isAnimEmpty( int inObjectID, AnimType inType );



// sets per-sprite/slot alpha fades to use for next drawObjectAnim call
// must match number of sprites and slot of next object drawn
// sprites fades come first, followed by slot fades
// inFades will be destroyed internally and cleared after
// the next drawObjectAnim call
void setAnimLayerFades( float *inFades );



HandPos drawObjectAnim( int inObjectID, AnimType inType, double inFrameTime,
                        double inAnimFade,
                        // if inAnimFade < 1, this is the animation
                        // that we are fading to (fading to the time-0 config
                        // for this animation)
                        AnimType inFadeTargetType,
                        double inFadeTargetFrameTime,
                        // for any zero rotations in current anim or fade
                        // target, if either is 'held',
                        // we apply frozen rotations from the 'moving' 
                        // animation using this frame time
                        double inFrozenRotFrameTime,
                        // or'd with true on return 
                        // if inFrozenRotFrameTime was applied
                        // to some sprite layer (and thus that frame
                        // time should be used as the resume point when
                        // returning to 'moving' animation later)
                        char *outFrozenRotFrameTimeUsed,
                        doublePair inPos,
                        char inFlipH,
                        double inAge, char inHoldingSomething,
                        ClothingSet inClothing );


HandPos drawObjectAnim( int inObjectID, AnimationRecord *inAnim, 
                        double inFrameTime,
                        double inAnimFade,
                        AnimationRecord *inFadeTargetAnim,
                        double inFadeTargetFrameTime,
                        double inFrozenRotFrameTime,
                        char *outFrozenRotFrameTimeUsed,
                        AnimationRecord *inFrozenRotAnim,
                        doublePair inPos,
                        char inFlipH,
                        double inAge, char inHoldingSomething,
                        ClothingSet inClothing );


void drawObjectAnim( int inObjectID, AnimType inType, double inFrameTime, 
                     double inAnimFade,
                     AnimType inFadeTargetType,
                     double inFadeTargetFrameTime,
                     double inFrozenRotFrameTime,
                     char *outFrozenRotFrameTimeUsed,
                     doublePair inPos,
                     char inFlipH,
                     double inAge,
                     char inHoldingSomething,
                     ClothingSet inClothing,
                     int inNumContained, int *inContainedIDs );


void drawObjectAnim( int inObjectID, AnimationRecord *inAnim,
                     double inFrameTime,
                     double inAnimFade,
                     AnimationRecord *inFadeTargetAnim,
                     double inFadeTargetFrameTime,
                     double inFrozenRotFrameTime,
                     char *outFrozenRotFrameTimeUsed,
                     AnimationRecord *inFrozenRotAnim,
                     doublePair inPos,
                     char inFlipH,
                     double inAge,
                     char inHoldingSomething,
                     ClothingSet inClothing,
                     int inNumContained, int *inContainedIDs );



void performLayerSwaps( int inObjectID, 
                        SimpleVector<LayerSwapRecord> *inSwapList,
                        int inNewNumSprites );

#endif
