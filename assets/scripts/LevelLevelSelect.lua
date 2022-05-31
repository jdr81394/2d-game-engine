


-- Define a table with start values of the first level
Level = {

    assets = {
        [0] =
            { type = "texture", id = "tilemap-texture-day",  file = "./assets/tilemaps/jungle.png" },
            { type = "font" ,   id = "arial-font",         file = "./assets/fonts/arial.ttf" , font_size = 10    },
            { type = "texture", id = "radar-texture",        file = "./assets/images/radar-spritesheet.png" },

    },

    ----------------------------------------------------
    -- table to define the map config variables
    ----------------------------------------------------

    tilemap = {
        map_file = "./assets/tilemaps/levelSelect.map",
        texture_asset_id = "tilemap-texture-day",
        num_rows = 20,
        num_cols = 25,
        tile_size = 32,
        scale = 2.0,
    },



    entities = {
        [0] =
        -- Go to level 1
        { 
            group = "text",     
            components = {
                transform = {
                    position = {
                        x = 50,
                        y = 150
                    },
                    scale = { x = 1.0 , y = 1.0},
                    rotation = { 0.0 }
                },
                boxcollider = {
                    width = 64,
                    height = 64,
                    offset = { x = 0, y = 0 }
                },
                sprite = {
                    texture_asset_id = "radar-texture",
                    width = 64,
                    height = 64,
                    z_index = 10,
                    fixed = true
                },
                animation = {
                    num_frames = 8,
                    speed_rate = 7      
                },
                mouse_controlled = {
                    is_clickable = true,
                    is_draggable = true,
                    link = "1"
                },
                text_label = {
                    position = {
                        x = 300.0,
                        y = 300.0
                    },
                    text = "Level One",
                    asset_id = "arial-font",
                    sdl_color = { 
                        r = 100,
                        g = 255, 
                        b = 100
                    },
                    is_fixed = true
                }
            }
        },
    }










}