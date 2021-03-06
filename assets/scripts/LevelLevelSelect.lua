window_width = config["window_width"]
window_height = config["window_height"]

Level = {

    assets = {
        [0] =
            { type = "texture", id = "tilemap-texture-day",  file = "./assets/tilemaps/jungle.png" },
            { type = "font" ,   id = "arial-font",         file = "./assets/fonts/arial.ttf" , font_size = 40    },
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
                mouse_controlled = {
                    is_clickable = true,
                    is_draggable = true,
                    link = "1"
                },
                text_label = {
                    position = {
                        x = window_width / 2 - (window_width * .1),
                        y = window_height / 2 - (window_height * .15)
                    },
                    text = "Level One",
                    asset_id = "arial-font",
                    sdl_color = { 
                        r = 255,
                        g = 255, 
                        b = 255
                    },
                    is_fixed = true
                }
            }
        },
        { 
            group = "text",     
            components = {
                mouse_controlled = {
                    is_clickable = true,
                    is_draggable = true,
                    link = "2"
                },
                text_label = {
                    position = {
                        x = window_width / 2 - (window_width * .1),
                        y = window_height / 2 - (window_height * .1)
                    },
                    text = "Level Two",
                    asset_id = "arial-font",
                    sdl_color = { 
                        r = 255,
                        g = 255, 
                        b = 255
                    },
                    is_fixed = true
                }
            }
        },
    }










}