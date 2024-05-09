

uint32_t loader_logo_red_palette[] = {
	0x000000ff,0xffeeeeff,0xffddddff,0xffccccff,0xffbbbbff,0xffaaaaff,0xff9999ff,0xff8888ff,
	0xff7777ff,0xff6666ff,0xee4444ff,0xcc2222ff,0xbb0000ff,0x990000ff,0x770000ff,0x440000ff,
	0x771111ff,0x993333ff,0xaa4444ff,0xcc6666ff,0xdd8888ff,0xffbbbbff,0xeeaaaaff,0xdd8888ff,
	0xcc6666ff,0xaa4444ff,0x993333ff,0x771111ff,0x661111ff,0x440000ff,0x330000ff,0x661111ff,
};

// static uint32_t loader_logo_red_palette[] = {
// 	0x00000000, 0x00330000, 0x00440000, 0x00770000, 0x11661100, 0x11771100, 0x00990000, 0x00bb0000,
// 	0x33993300, 0x22cc2200, 0x44aa4400, 0x44ee4400, 0x66cc6600, 0x66ff6600, 0x77ff7700, 0x88dd8800,
// 	0x88ff8800, 0x99ff9900, 0xaaeeaa00, 0xaaffaa00, 0xbbffbb00, 0xccffcc00, 0xddffdd00,
// 	0x33993300, 0x22cc2200, 0x44aa4400, 0x44ee4400, 0x66cc6600, 0x66ff6600, 0x77ff7700, 0x88dd8800,
// 	0x88ff8800, 0x99ff9900, 0xaaeeaa00, 0xaaffaa00, 0xbbffbb00, 0xccffcc00, 0xddffdd00,
// };

// static uint32_t loader_logo_red_palette[LOADER_LOGO_COLORS] = {
// 	0x000000ff, 0xeeffeeff, 0xddffddff, 0xccffccff, 0xbbffbbff, 0xaaffaaff, 0x99ff99ff, 0x88ff88ff,
// 	0x77ff77ff, 0x66ff66ff, 0x44ee44ff, 0x22cc22ff, 0x00bb00ff, 0x009900ff, 0x007700ff, 0x004400ff,
// 	0x117711ff, 0x339933ff, 0x44aa44ff, 0x66cc66ff, 0x88dd88ff, 0xbbffbbff, 0xaaeeaaff, 0x88dd88ff,
// 	0x66cc66ff, 0x44aa44ff, 0x339933ff, 0x117711ff, 0x116611ff, 0x004400ff, 0x003300ff,
// };

uint32_t loader_timer = 200;

#define ALIGN_CENTER 0
#define ALIGN_LEFT   1
#define ALIGN_RIGHT  2
#define ALIGN_TOP    3
#define ALIGN_BOTTOM 4

uint32_t loader(struct loader_shared_state *state) {
	int32_t result = false;

	static uint32_t counter = 0;
	static uint32_t roller = 0;
	++counter;

	if((counter % 42) == 0) roller += 2;
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

	uint32_t x = (state->buffer_width - LOADER_LOGO_WIDTH) >> 1;
	uint32_t y = (state->buffer_height - LOADER_LOGO_HEIGHT) >> 1;
	fast_blit_with_palette(state, loader_logo_data, LOADER_LOGO_WIDTH, LOADER_LOGO_HEIGHT, loader_logo_red_palette, x, y);

	if(loader_timer) {
		--loader_timer;
	} else {
		result = true;
	}

	return result;
}
