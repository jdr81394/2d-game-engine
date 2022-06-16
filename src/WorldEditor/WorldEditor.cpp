#include "./WorldEditor.h"
// #include "../../"
const float INITIAL_X = 750.0;
const float INITIAL_Y = 200.0;
const int INITIAL_WIDTH = 450;
const int INITIAL_HEIGHT = 600;

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


void WorldEditor::Run() {}


void WorldEditor::Destroy() {}
