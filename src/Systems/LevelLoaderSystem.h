#ifndef LEVELLOADERSYSTEM_H
#define LEVELLOADERSYSTEM_H
#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include <memory>
#include <SDL2/SDL.h>
#include "../Logger/Logger.h"
#include <sol/sol.hpp>
#include <string>
#include "../Components/MouseControlledComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/MouseControlledComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/ScriptComponent.h"
#include "../Components/CameraFollowComponent.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include "../Events/WorldEditorStartEvent.h"


class LevelLoaderSystem : public System {

    public:
        LevelLoaderSystem() {
            RequireComponent<MouseControlledComponent>();
        }

        void SubscribeToEvent(std::unique_ptr<EventBus>& eventBus ) {
            eventBus->SubscribeToEvent<WorldEditorStartEvent>(this, &LevelLoaderSystem::CheckToLoadLevel);
        }

        void CheckToLoadLevel(
            WorldEditorStartEvent& event
        ) {

            sol::state& lua = event.lua;
            std::unique_ptr<Registry>& registry = event.registry; 
            std::unique_ptr<AssetStore>& assetStore = event.assetStore;
            SDL_Renderer* renderer = event.renderer;
            std::unique_ptr<EventBus>& eventBus = event.eventBus;

            int x, y;
            SDL_GetMouseState(&x, &y);
            glm::vec2 mouseCoordinates = glm::vec2(static_cast<int>(x),static_cast<int>(y));
            Logger::Log(" Here are the mousecoordinates,  heres the x: " +  std::to_string(mouseCoordinates.x) +  "    and heres y:  " + std::to_string(mouseCoordinates.y));

            for(auto entity : GetSystemEntities()) {
                auto const mouseControlledComponent = entity.GetComponent<MouseControlledComponent>();
                if(mouseControlledComponent.isClickable) {
                    std::string const link = mouseControlledComponent.link;

                    if(link.size() > 0 || entity.HasTag("WorldEditorLink")) {

                        double initialX = 0.0, initialY = 0.0, width = 0.0, height = 0.0;

                        // If entity belongs to group text, then we should look at text component
                        if(entity.BelongsToGroup("text") && entity.HasComponent<TextLabelComponent>()) {
                            TextLabelComponent textLabelComponent = entity.GetComponent<TextLabelComponent>();

                            SDL_Surface* surface = TTF_RenderText_Blended(
                                assetStore->GetFont(textLabelComponent.assetId),
                                textLabelComponent.text.c_str(),
                                textLabelComponent.color
                            );
                            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                            SDL_FreeSurface(surface);

                            int labelWidth = 0;
                            int labelHeight = 0;

                            // Query font to find width and height; This populates the labelWidth and labelHeight values.
                            SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

                            initialX = textLabelComponent.position.x;
                            initialY = textLabelComponent.position.y;
                            width    = labelWidth;
                            height   = labelHeight;

                            
                        }
                        else if(entity.HasComponent<BoxColliderComponent>() && entity.HasComponent<TransformComponent>()) {
                            // lets see if there is collision between mouse click and objct
                            auto const boxColliderComponent = entity.GetComponent<BoxColliderComponent>();
                            auto const transformComponent = entity.GetComponent<TransformComponent>();

                            initialX = transformComponent.position.x + boxColliderComponent.offset.y;
                            initialY = transformComponent.position.y + boxColliderComponent.offset.y;
                            width    = boxColliderComponent.width;
                            height   = boxColliderComponent.height;

                        }

                        bool isClickInEntity = CheckAABBCollision(
                                initialX,
                                initialY,
                                width,
                                height,
                                mouseCoordinates.x,
                                mouseCoordinates.y,
                                5.0,
                                5.0
                        );


                        // If cick is within entity, we will go to link
                        if(isClickInEntity) {

                            if(entity.HasTag("WorldEditorLink")) {
                                eventBus->EmitEvent<WorldEditorStartEvent>(lua, registry, assetStore, renderer, eventBus);
                            }
                            else {
                                LoadLevel(lua,registry,assetStore,renderer,link);

                            }
                        }

                        
                    }
                }
            }
        }

        bool CheckAABBCollision(
        double aX, 
        double aY, 
        double aW, 
        double aH,
        double bX,
        double bY,
        double bW,
        double bH    
        ) {
            // This is the AABB collision algorithm. 
            // You can find it here https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
            return (
                aX < bX + bW &&
                aX + aW > bX &&
                aY < bY + bH && 
                aY + aH > bY 
            );
        };


        void LoadLevel(sol::state& lua, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, std::string levelNumber) {


            // If level number empty, then just end function
            if(levelNumber == "") return;
            // First lets check if there is something loaded
            // If so deload it.
            if(registry->GetNumEntities() > 0) {
                registry->DeloadEverything();
            }

            // This checks the syntax of our script, but it does not execute the script
            sol::load_result script = lua.load_file("./assets/scripts/Level" + levelNumber + ".lua");
            
            if (!script.valid()) {
                sol::error err = script;
                std::string errorMessage = err.what();
                Logger::Err("Error loading the lua script: " + errorMessage);
                return;
            }

            // Executes the script using the Sol state
            lua.script_file("./assets/scripts/Level" + levelNumber + ".lua");

            // Read the big table for the current level
            sol::table level = lua["Level"];

            ////////////////////////////////////////////////////////////////////////////
            // Read the level assets
            ////////////////////////////////////////////////////////////////////////////
            sol::table assets = level["assets"];

            int i = 0;
            while (true) {
                sol::optional<sol::table> hasAsset = assets[i];
                if (hasAsset == sol::nullopt) {
                    break;
                }
                sol::table asset = assets[i];
                std::string assetType = asset["type"];
                std::string assetId = asset["id"];
                if (assetType == "texture") {
                    assetStore->AddTexture(renderer, assetId, asset["file"]);
                    Logger::Log("A new texture asset was added to the asset store, id: " + assetId);
                }
                if (assetType == "font") {
                    assetStore->AddFont(assetId, asset["file"], asset["font_size"]);
                    Logger::Log("A new font asset was added to the asset store, id: " + assetId);
                }
                i++;
            }

            ////////////////////////////////////////////////////////////////////////////
            // Read the level tilemap information
            ////////////////////////////////////////////////////////////////////////////
            sol::table map = level["tilemap"];
            std::string mapFilePath = map["map_file"];
            std::string mapTextureAssetId = map["texture_asset_id"];
            int mapNumRows = map["num_rows"];
            int mapNumCols = map["num_cols"];
            int tileSize = map["tile_size"];
            double mapScale = map["scale"];
            std::fstream mapFile;
            mapFile.open(mapFilePath);
            for (int y = 0; y < mapNumRows; y++) {
                for (int x = 0; x < mapNumCols; x++) {
                    char ch;
                    mapFile.get(ch);
                    int srcRectY = std::atoi(&ch) * tileSize;
                    mapFile.get(ch);
                    int srcRectX = std::atoi(&ch) * tileSize;
                    mapFile.ignore();

                    Entity tile = registry->CreateEntity();
                    tile.AddComponent<TransformComponent>(glm::vec2(x * (mapScale * tileSize), y * (mapScale * tileSize)), glm::vec2(mapScale, mapScale), 0.0);
                    tile.AddComponent<SpriteComponent>(mapTextureAssetId, tileSize, tileSize, 0, false, srcRectX, srcRectY);
                }
            }
            mapFile.close();
            Game::mapWidth = mapNumCols * tileSize * mapScale;
            Game::mapHeight = mapNumRows * tileSize * mapScale;

            ////////////////////////////////////////////////////////////////////////////
            // Read the level entities and their components
            ////////////////////////////////////////////////////////////////////////////
            sol::table entities = level["entities"];
            i = 0;
            while (true) {
                sol::optional<sol::table> hasEntity = entities[i];
                if (hasEntity == sol::nullopt) {
                    break;
                }

                sol::table entity = entities[i];

                Entity newEntity = registry->CreateEntity();

                // Tag
                sol::optional<std::string> tag = entity["tag"];
                if (tag != sol::nullopt) {
                    newEntity.Tag(entity["tag"]);
                }

                // Group
                sol::optional<std::string> group = entity["group"];
                if (group != sol::nullopt) {
                    newEntity.Group(entity["group"]);
                }

                // Components
                sol::optional<sol::table> hasComponents = entity["components"];
                if (hasComponents != sol::nullopt) {
                    // Transform
                    sol::optional<sol::table> transform = entity["components"]["transform"];
                    if (transform != sol::nullopt) {
                        newEntity.AddComponent<TransformComponent>(
                            glm::vec2(
                                entity["components"]["transform"]["position"]["x"],
                                entity["components"]["transform"]["position"]["y"]
                            ),
                            glm::vec2(
                                entity["components"]["transform"]["scale"]["x"].get_or(1.0),
                                entity["components"]["transform"]["scale"]["y"].get_or(1.0)
                            ),
                            entity["components"]["transform"]["rotation"].get_or(0.0)
                        );
                    }

                    // RigidBody
                    sol::optional<sol::table> rigidbody = entity["components"]["rigidbody"];
                    if (rigidbody != sol::nullopt) {
                        newEntity.AddComponent<RigidBodyComponent>(
                            glm::vec2(
                                entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
                                entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
                            )
                        );
                    }

                    // Sprite
                    sol::optional<sol::table> sprite = entity["components"]["sprite"];
                    if (sprite != sol::nullopt) {
                        newEntity.AddComponent<SpriteComponent>(
                            entity["components"]["sprite"]["texture_asset_id"],
                            entity["components"]["sprite"]["width"],
                            entity["components"]["sprite"]["height"],
                            entity["components"]["sprite"]["z_index"].get_or(1),
                            entity["components"]["sprite"]["fixed"].get_or(false),
                            entity["components"]["sprite"]["src_rect_x"].get_or(0),
                            entity["components"]["sprite"]["src_rect_y"].get_or(0)
                        );
                    }

                    // Animation
                    sol::optional<sol::table> animation = entity["components"]["animation"];
                    if (animation != sol::nullopt) {
                        newEntity.AddComponent<AnimationComponent>(
                            entity["components"]["animation"]["num_frames"].get_or(1),
                            entity["components"]["animation"]["speed_rate"].get_or(1)
                        );
                    }

                    // BoxCollider
                    sol::optional<sol::table> collider = entity["components"]["boxcollider"];
                    if (collider != sol::nullopt) {
                        newEntity.AddComponent<BoxColliderComponent>(
                            entity["components"]["boxcollider"]["width"],
                            entity["components"]["boxcollider"]["height"],
                            glm::vec2(
                                entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
                                entity["components"]["boxcollider"]["offset"]["y"].get_or(0)
                            )
                        );
                    }
                    
                    // Health
                    sol::optional<sol::table> health = entity["components"]["health"];
                    if (health != sol::nullopt) {
                        newEntity.AddComponent<HealthComponent>(
                            static_cast<int>(entity["components"]["health"]["health_percentage"].get_or(100))
                        );
                    }
                    
                    // ProjectileEmitter
                    sol::optional<sol::table> projectileEmitter = entity["components"]["projectile_emitter"];
                    if (projectileEmitter != sol::nullopt) {
                        newEntity.AddComponent<ProjectileEmitterComponent>(
                            glm::vec2(
                                entity["components"]["projectile_emitter"]["projectile_velocity"]["x"],
                                entity["components"]["projectile_emitter"]["projectile_velocity"]["y"]
                            ),
                            static_cast<int>(entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(1)) * 1000,
                            static_cast<int>(entity["components"]["projectile_emitter"]["projectile_duration"].get_or(10)) * 1000,
                            static_cast<int>(entity["components"]["projectile_emitter"]["hit_percentage_damage"].get_or(10)),
                            entity["components"]["projectile_emitter"]["friendly"].get_or(false)
                        );
                    }

                    // CameraFollow
                    sol::optional<sol::table> cameraFollow = entity["components"]["camera_follow"];
                    if (cameraFollow != sol::nullopt) {
                        newEntity.AddComponent<CameraFollowComponent>();
                    }

                    // KeyboardControlled
                    sol::optional<sol::table> keyboardControlled = entity["components"]["keyboard_controller"];
                    if (keyboardControlled != sol::nullopt) {
                        newEntity.AddComponent<KeyboardControlledComponent>(
                            glm::vec2(
                                entity["components"]["keyboard_controller"]["up_velocity"]["x"],
                                entity["components"]["keyboard_controller"]["up_velocity"]["y"]
                            ),
                            glm::vec2(
                                entity["components"]["keyboard_controller"]["right_velocity"]["x"],
                                entity["components"]["keyboard_controller"]["right_velocity"]["y"]
                            ),
                            glm::vec2(
                                entity["components"]["keyboard_controller"]["down_velocity"]["x"],
                                entity["components"]["keyboard_controller"]["down_velocity"]["y"]
                            ),
                            glm::vec2(
                                entity["components"]["keyboard_controller"]["left_velocity"]["x"],
                                entity["components"]["keyboard_controller"]["left_velocity"]["y"]
                            )
                        );
                    }
                
                    // MouseControlledComponent
                    sol::optional<sol::table> mouseControlled = entity["components"]["mouse_controlled"];
                    if(mouseControlled != sol::nullopt) {
                        newEntity.AddComponent<MouseControlledComponent>(
                            entity["components"]["mouse_controlled"]["is_clickable"].get_or(true),
                            entity["components"]["mouse_controlled"]["is_draggable"].get_or(false),
                            entity["components"]["mouse_controlled"]["link"]
                        );
                    }


                    // TextLabelComponent
                    sol::optional<sol::table> textLabel = entity["components"]["text_label"];

                    if(textLabel != sol::nullopt) {

                        SDL_Color color = {
                            entity["components"]["text_label"]["sdl_color"]["r"],
                            entity["components"]["text_label"]["sdl_color"]["g"],
                            entity["components"]["text_label"]["sdl_color"]["b"]
                        };

                        const std::string text = entity["components"]["text_label"]["text"];

                        const std::string assetId = entity["components"]["text_label"]["asset_id"];
                
                        newEntity.AddComponent<TextLabelComponent>(
                            glm::vec2(
                                entity["components"]["text_label"]["position"]["x"],
                                entity["components"]["text_label"]["position"]["y"]
                            ),
                            text,
                            assetId,
                            color,
                            entity["components"]["text_label"]["is_fixed"]
                        );
                    }


                    // Script
                    sol::optional<sol::table> updateScript = entity["components"]["on_update_script"];
                    if(updateScript != sol::nullopt) {
                        sol::function func = entity["components"]["on_update_script"][0];
                        newEntity.AddComponent<ScriptComponent>(func);
                    }
                
                }



                i++;
            }

        }



};

#endif