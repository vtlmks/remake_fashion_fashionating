

// uint32_t loader_logo_red_palette[] = {
// 	0x000000ff,0xffeeeeff,0xffddddff,0xffccccff,0xffbbbbff,0xffaaaaff,0xff9999ff,0xff8888ff,
// 	0xff7777ff,0xff6666ff,0xee4444ff,0xcc2222ff,0xbb0000ff,0x990000ff,0x770000ff,0x440000ff,
// 	0x771111ff,0x993333ff,0xaa4444ff,0xcc6666ff,0xdd8888ff,0xffbbbbff,0xeeaaaaff,0xdd8888ff,
// 	0xcc6666ff,0xaa4444ff,0x993333ff,0x771111ff,0x661111ff,0x440000ff,0x330000ff,0x661111ff,
// };

static uint32_t loader_logo_red_palette[] = {
	0x00000000, 0x00330000, 0x00440000, 0x00770000, 0x11661100, 0x11771100, 0x00990000, 0x00bb0000,
	0x33993300, 0x22cc2200, 0x44aa4400, 0x44ee4400, 0x66cc6600, 0x66ff6600, 0x77ff7700, 0x88dd8800,
	0x88ff8800, 0x99ff9900, 0xaaeeaa00, 0xaaffaa00, 0xbbffbb00, 0xccffcc00, 0xddffdd00,
	0x33993300, 0x22cc2200, 0x44aa4400, 0x44ee4400, 0x66cc6600, 0x66ff6600, 0x77ff7700, 0x88dd8800,
	0x88ff8800, 0x99ff9900, 0xaaeeaa00, 0xaaffaa00, 0xbbffbb00, 0xccffcc00, 0xddffdd00,
};

uint32_t loader_timer = 200;

uint32_t loader(struct loader_shared_state *state) {
	int32_t result = false;

	static uint32_t counter = 0;
	static uint32_t roller = 0;
	++counter;

	if((counter % 21) == 0) roller += 2;
	if((counter % 3) == 0) roller += 1;

	switch(roller) {
		case 0: {
		} break;

		default: {
			uint32_t temp = loader_logo_red_palette[16];
			for(uint8_t i = 16; i < 31; ++i) {
				loader_logo_red_palette[i] = loader_logo_red_palette[i + 1];
			}
			loader_logo_red_palette[31] = temp;
			--roller;
		} break;
	}

	uint32_t yOffset = (state->buffer_height - LOADER_LOGO_HEIGHT) / 2;
	uint32_t *Row = state->buffer + (yOffset * state->buffer_width) + ((state->buffer_width - LOADER_LOGO_WIDTH) / 2);
	uint8_t *logo = loader_logo_data;
	for(uint32_t y = 0; y < LOADER_LOGO_HEIGHT; ++y) {
		uint32_t *Pixel = Row;
		for(uint32_t x = 0; x < LOADER_LOGO_WIDTH; ++x) {
			uint8_t color_id = *logo++;
			if(color_id) {
				*Pixel = loader_logo_red_palette[color_id];
			}
			++Pixel;
		}
		Row += state->buffer_width;
	}

	if(loader_timer) {
		--loader_timer;
	} else {
		result = true;
	}

	return result;
}
