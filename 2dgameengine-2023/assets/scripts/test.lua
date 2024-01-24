Level = {
	assets = {
		[0] = 
		{ type = "texture" , id = "./assets/tilemaps/test",  file = "./assets/tilemaps/test.png" },
		{ type = "texture", id="tree-19-texture",	file ="./assets/images/tree-19.png" },
	}
,	tilemap = {
		map_file="./assets/tilemaps/test.map",
		texture_asset_id="./assets/tilemaps/test",
		num_rows=20,
		num_cols=20,
		tile_size=32,
		scale=2.0
	},
	entities = {
		[0] = {
				components = {
					transform = {
						position = { x = 706.000000, y= 532.000000 },
						scale = { x = 2.000000, y= 2.000000 },
						rotation = 0.000000
					},
					sprite = {
						texture_asset_id = "tree-19-texture",
						width = 32,
						height = 32,
						z_index = 2
					},
					rigidbody = {
						velocity = { x = 0.000000, y = 0.000000 }
					},
				boxcollider = {
					width = 32,
					height = 32,
					offset = { x = 0.000000, y = 0.000000 }
				},
				player_controlled = {
				},
				keyboard_controller = {
					up_velocity = { x = 0.000000, y = -50.000000 },
					right_velocity = { x = 50.000000, y = 0.000000 },
					left_velocity = { x = -50.000000, y = 0.000000 },
					down_velocity = { x = 0.000000, y = 50.000000 }
				},
				}
			},
	}
}