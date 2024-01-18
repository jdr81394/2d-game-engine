Level = {
	assets = {
		[0] = 
		{ type = "texture" , id = "./assets/tilemaps/test",  file = "./assets/tilemaps/test.png" },
		{ type = "texture", id="su27-spritesheet-texture",	file ="./assets/images/su27-spritesheet.png" },
	}
,	tilemap = {
		map_file="./assets/tilemaps/test.map",
		texture_asset_id="./assets/tilemaps/test",
		num_rows=2,
		num_cols=2,
		tile_size=32,
		scale=2.0
	},
	entities = {
		[0] = {
				components = {
					transform = {
						position = { x = 24.000000, y= 28.000000 },
						scale = { x = 2.000000, y= 2.000000 },
						rotation = 0.000000
					},
					sprite = {
						texture_asset_id = "su27-spritesheet-texture",
						width = 32,
						height = 32,
						z_index = 2
					},
							rigidbody = {
					velocity = { x = 0.000000, y = 0.000000 }
				},
				player_controlled = {
				},
				keyboard_controller = {
					up_velocity = { x = 0.000000, y = -12.000000 },
					right_velocity = { x = -30.000000, y = 0.000000 },
					left_velocity = { x = 30.000000, y = 0.000000 },
					down_velocity = { x = 0.000000, y = 12.000000 }
				},
				}
			},
	}
}