#include "./WorldEditor.h"
// #include "../../"
const float INITIAL_X = 750.0;
const float INITIAL_Y = 200.0;
const int INITIAL_WIDTH = 450;
const int INITIAL_HEIGHT = 600;
const int FPS = 60;
const int MILLISECS_PER_FRAME_EDITOR = 1000 / FPS;

// Jake - I must change the z index so the world editor screen is always in front of the
// game screen
WorldEditor::WorldEditor(
    sol::state & lua, 
    const std::unique_ptr<Registry>& registry,
    const std::unique_ptr<AssetStore>& assetStore,
    std::unique_ptr<EventBus>& eventBus
): lua(lua), registry(registry), assetStore(assetStore), eventBus(eventBus) {

    isRunning = false;
}
WorldEditor::~WorldEditor() {}

SDL_Window* WorldEditor::GetWindow(){
    return window;
}

void WorldEditor::Initialize() {
    window = SDL_CreateWindow(
        "World Editor",
        static_cast<int>(INITIAL_X),
        static_cast<int>(INITIAL_Y),
        INITIAL_WIDTH,
        INITIAL_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );

    renderer = SDL_CreateRenderer(window, -1, 0);

    isRunning = true;

    windowWidth = 450;
    windowHeight  = 600;

    // Create WorldEditorWindow entity
    // Entity windowEntity = registry->CreateEntity();
    // windowEntity.Tag("WorldEditorWindow");
    // windowEntity.AddComponent<BoxColliderComponent>(INITIAL_WIDTH,INITIAL_HEIGHT);
    // windowEntity.AddComponent<TransformComponent>(glm::vec2(INITIAL_X,INITIAL_Y));
    // windowEntity.AddComponent<MouseControlledComponent>(true, true);


    // we will display the tiles, entities, components, and the relationships that could exist
    // there really is components, sprites 

    // First lets get all the sprites that exist
    // What if I can just get every file in the assets/images folder using some kind of
    // system library
    // I need a function that loops through directory
    // I need something that opens files.. <stdio.h> has something like open_memstream or fopen()
    // Actually i dont even need anything to open file itself i need string

    const std::filesystem::path assetPath {"/Users/jaroberts/Desktop/Programming/2d-game-engine/assets/images/"};

    for(auto directoryEntry : std::filesystem::directory_iterator{assetPath}) {
        const std::string path = directoryEntry.path().u8string();
        std::string assetId = path.substr(0, path.size() - 4);
        assetStore->AddTexture(renderer, assetId, path );
    }


};

SDL_Renderer * WorldEditor::GetRenderer() {
    return renderer;
}

void WorldEditor::Run() {

    Update();
    Render();

}


void WorldEditor::Update() {}

void WorldEditor::Render() {


       // Get Window size
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(0, &displayMode);
        windowWidth = displayMode.w;
        windowHeight = displayMode.h;

        // // Render all the tiles in the window
        int timeToWait = MILLISECS_PER_FRAME_EDITOR  - ( SDL_GetTicks() - millisecsPreviousFrame);
        //     Logger::Log("TIM TO WAIT: !" + std::to_string(timeToWait));

        if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME_EDITOR) {

            SDL_Delay(timeToWait);
        }

        // Logger::Log("World Editor Render!");

        // The difference in ticks since the last frame, converted to seconds
        double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

        millisecsPreviousFrame = SDL_GetTicks();

        int * width = new int;
        int * height = new int; 
        SDL_GetWindowSize(window, width, height);  


        // // Render all the tiles in the window


        camera.x = 0;
        camera.y = 0;
        // camera.w = 450;
        // camera.h = 450;

        camera.w = *width;
        camera.h = *height;

        // // int numRows = camera.h * 1.5 < 20 ? 20 : camera.h * 1.5;
        // // int numCols = camera.w * 1.5 < 15 ? 15 : camera.w * 1.5;
        // // int mapScale = 1;
        // // int tileSizeHeight = static_cast<int>(camera.h / numRows * mapScale);
        // // int tileSizeWidth = static_cast<int>(camera.w / numCols * mapScale);

        int numRows = 20;
        int numCols = 15;
        int mapScale = 1;
        int tileSize = 10;

        // Logger::Log("width: " + std::to_string(*width) + "  hegiht: " + std::to_string(*height));

        /*

        Total Width of window is 1000 pixels
        
        10 tiles in each row
        width of the tile will be 1000 / 10 = 100 pixels


        */

        std::map<std::string, SDL_Texture*> allTextures = assetStore->GetAllTextures();
        auto it = allTextures.begin();

        if(it != allTextures.end()) {




            for(int y = 0; y < numRows; y++) {

                for(int x = 0; x < numCols; x++) {

                    if(it == allTextures.end()) break;

                    Entity tile = registry->CreateEntity();

                    tile.AddComponent<TransformComponent>(
                        glm::vec2(x * (mapScale * tileSize), y * (mapScale * tileSize) ),
                        glm::vec2(mapScale, mapScale),
                        0.0
                    );

                    tile.AddComponent<SpriteComponent>(
                        it->first,
                        tileSize,
                        tileSize,
                        1000
                    );
                    Logger::Log("Name: " + it->first + " X: " + std::to_string(x) + " Y: " + std::to_string(y));

                    it++;
                    tile.Group("World Editor");
                    
                }

            }
        }

        // for (int y = 0; y < numRows; y++) {

        //     for (int x = 0; x < numCols; x++) {

                
        //         Entity tile = registry->CreateEntity();

        //         tile.AddComponent<TransformComponent>(
        //             glm::vec2(x * (mapScale * tileSize) , y * (mapScale * tileSize)),
        //             glm::vec2(mapScale, mapScale),
        //             0.0
        //         );

        //         if(it != allTextures.end() ) {
        //             tile.AddComponent<SpriteComponent>(
        //                 it->first,
        //                 tileSize,
        //                 tileSize,
        //                 1000,
        //                 false
        //             );
        //             it++;
        //         }
        //         else {
        //             break;
        //         }

        //         tile.Group("World Editor");

        //     }
        
        // }

        delete width;
        delete height;




    

}


void WorldEditor::Destroy() {
}
