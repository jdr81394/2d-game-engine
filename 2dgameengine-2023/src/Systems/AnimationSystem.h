#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"

class AnimationSystem : public System {

    public:
        AnimationSystem() {
            RequireComponent<SpriteComponent>();
            RequireComponent<AnimationComponent>();
        }

        void Update() {
            for (auto entity : GetSystemEntities()) {
                auto& animation = entity.GetComponent<AnimationComponent>();    // since we are going to change the actual animation component we will receive a reference.
                auto& sprite = entity.GetComponent<SpriteComponent>();
            
                // currentFrame = what frame is the animation on now
                // SDL_GetTicks() = when was the animation system initialized
                // animation.startTime = Take the difference. Not sure what that would yield..
                // animation.frameSpeedRate / 1000 = how many frames you want per second
                // % animation.numFrames - we get the remidner from the division... not sure how that yields currentFrame. 
                // He says we guarantee we will always get a frame between 0 and the number of frames we will have minus 1
                
                // Jake common sense notes
                // SDL_GetTicks - animation.startTime is essentially just how long it has been existing for.
                // 



                // ( (2000 - 1000) *  (1 / 1000) ) % 2
                // (1000 * 1/1000) % 2
                // 1 % 2 = 1
                // the modolo of 2 is 1. I guess logically thats because you cant divide it..?
                // heres a link with explanation https://divisible.info/Modulo/What-is-1-mod-2.html
                //( (2500 - 1000) * (1/1000)) % 2
                // ( 1500 * 1/1000  ) % 2
                // 1.5 % 2 = 1.5...?
                // I guess the modolus only takes integer values? - YES. It'll only work with integers.
                // So, when have a situation that is like :
                //     animation.currentFrame = 2  % 2
                // That'll bring it back to the original frame by resulting in a value of 0. 
                
                animation.currentFrame = (
                (SDL_GetTicks() - animation.startTime)
                    * animation.frameSpeedRate / 1000
                ) % animation.numFrames;
            
                sprite.srcRect.x = animation.currentFrame * sprite.width;
            
    
            
            }


        }


};
#endif