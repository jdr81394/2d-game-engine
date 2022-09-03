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
    std::unique_ptr<EventBus>& eventBus
): lua(lua), eventBus(eventBus) {

    window = SDL_CreateWindow(
        "World Editor",
        static_cast<int>(INITIAL_X),
        static_cast<int>(INITIAL_Y),
        INITIAL_WIDTH,
        INITIAL_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        printf("Error creating window: %s\n", SDL_GetError());
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    camera.x = 0;
    camera.y = 0;
    camera.w = INITIAL_WIDTH;
    camera.h = INITIAL_HEIGHT;
    
    isRunning = false;
    
}
WorldEditor::~WorldEditor() {}

SDL_Window* WorldEditor::GetWindow(){
    return window;
}

void WorldEditor::Initialize() {
    assetStore = std::make_unique<AssetStore>();
    registry = std::make_unique<Registry>();
    isRunning = true;

    SetUp();

};

void WorldEditor::SetUp() {
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
        if(path.find("spritesheet") != std::string::npos) {
            continue;
        }
        std::string assetId = path.substr(0, path.size() - 4);
        assetStore->AddTexture(renderer, assetId, path );
    }


    registry->AddSystem<RenderSystem>();
    ArrangeAssetsOnWindow();
}



SDL_Renderer * WorldEditor::GetRenderer() {
    return renderer;
}

void WorldEditor::Run() {

    while(isRunning) {
        Update();
        Render();
    }




}



void WorldEditor::Update() {
    registry->Update();
}

void WorldEditor::Render() {

    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    registry->GetSystem<RenderSystem>().Update(
        renderer,
        assetStore,
        camera
    );

    SDL_RenderPresent(renderer);
}

void WorldEditor::ArrangeAssetsOnWindow() {

        int * width = new int;
        int * height = new int; 
        SDL_GetWindowSize(window, width, height);  


        // // Render all the tiles in the window
        int tileSize = 25;
        int numRows = *height / tileSize;
        int numCols = *width / tileSize;
        int mapScale = 1;

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

                    tile.Group("World Editor");
                    it++;

                    
                }

            }
        }

        
        delete width;
        delete height;

}


void WorldEditor::Destroy() {
    isRunning = false;
}
