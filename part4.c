

static uint32_t backgroundUpscrollColor[] = {
	0x110000ff, 0x110000ff, 0x110000ff, 0x220000ff, 0x220000ff, 0x220000ff, 0x330000ff, 0x330000ff,
	0x330000ff, 0x440011ff, 0x440011ff, 0x440011ff, 0x550011ff, 0x550011ff, 0x550011ff, 0x660011ff,
	0x660011ff, 0x660011ff, 0x770022ff, 0x770022ff, 0x770022ff, 0x880022ff, 0x880022ff, 0x880022ff,
	0x990022ff, 0x990022ff, 0x990022ff, 0xaa0033ff, 0xaa0033ff, 0xaa0033ff, 0xbb0033ff, 0xbb0033ff,
	0xbb0033ff, 0xcc0033ff, 0xcc0033ff, 0xcc0033ff, 0xdd0044ff, 0xdd0044ff, 0xdd0044ff, 0xee0044ff,
	0xee0044ff, 0xee0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff,
	0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff,
	0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff,
	0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff,
	0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff,
	0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff,
	0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff,
	0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff,
	0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff, 0xff0044ff,
	0xff0044ff, 0xff0044ff, 0xff0044ff, 0xee0044ff, 0xee0044ff, 0xee0044ff, 0xdd0044ff, 0xdd0044ff,
	0xdd0044ff, 0xcc0033ff, 0xcc0033ff, 0xcc0033ff, 0xbb0033ff, 0xbb0033ff, 0xbb0033ff, 0xaa0033ff,
	0xaa0033ff, 0xaa0033ff, 0x990022ff, 0x990022ff, 0x990022ff, 0x880022ff, 0x880022ff, 0x880022ff,
	0x770022ff, 0x770022ff, 0x770022ff, 0x660011ff, 0x660011ff, 0x660011ff, 0x550011ff, 0x550011ff,
	0x550011ff, 0x440011ff, 0x440011ff, 0x440011ff, 0x330000ff, 0x330000ff, 0x330000ff, 0x220000ff,
	0x220000ff, 0x220000ff, 0x110000ff, 0x110000ff, 0x110000ff, 0x000000ff,
};

static uint32_t fsn_top_logo_colors[32] = {			// Blue
	0x00000000, 0xeeddffff, 0xbbbbffff, 0xaaaaffff, 0xaaaaffff, 0x9999ffff, 0x8888ffff, 0x7777ffff,
	0x7777ffff, 0x6666ffff, 0x4444eeff, 0x2222ccff, 0x0000bbff, 0x000099ff, 0x000077ff, 0x000044ff,
	0x333399ff, 0x4444aaff, 0x6666ccff, 0x8888ddff, 0xbbbbffff, 0xbbbbffff, 0x8888ddff, 0x6666ccff,
	0x4444aaff, 0x333399ff, 0x111177ff, 0x111166ff, 0x000044ff, 0x000044ff, 0x111166ff, 0x111177ff
};

static uint32_t fsn_bottom_logo_colors[32] = {	// Green
	0x00000000, 0xeeffddff, 0xbbffbbff, 0xaaffaaff, 0xaaffaaff, 0x99ff99ff, 0x88ff88ff, 0x77ff77ff,
	0x77ff77ff, 0x66ff66ff, 0x44ee44ff, 0x22cc22ff, 0x00bb00ff, 0x009900ff, 0x007700ff, 0x004400ff,
	0x339933ff, 0x44aa44ff, 0x66cc66ff, 0x88dd88ff, 0xbbffbbff, 0xbbffbbff, 0x88dd88ff, 0x66cc66ff,
	0x44aa44ff, 0x339933ff, 0x117711ff, 0x116611ff, 0x004400ff, 0x004400ff, 0x116611ff, 0x117711ff
};

// Macro definitions for dimensions and offsets
#define P4_LOGO_X_OFFSET                            55
#define P4_TOP_LOGO_Y_OFFSET                        22
#define P4_BOTTOM_LOGO_Y_OFFSET                     231
#define P4_UPSCROLL_Y_POS                           69
#define P4_UPSCROLL_WIDTH                           352
#define P4_UPSCROLL_HEIGHT                          220
#define P4_UPSCROLL_VISIBLE_HEIGHT                  157
#define P4_UPSCROLL_RENDER_CONTENT_Y_OFFSET         158

static uint8_t p4_scroll_buffer[P4_UPSCROLL_WIDTH * P4_UPSCROLL_HEIGHT];

static uint32_t greetings_heights[] = { 8, 57, 61, 61, 61, 17, 17, 28 };
static uint8_t *greetings_offsets[] = {
	p4_greetings_text_data,
	p4_greetings_text_data + P4_GREETINGS_TEXT_WIDTH * 18,
	p4_greetings_text_data + P4_GREETINGS_TEXT_WIDTH * 85,
	p4_greetings_text_data + P4_GREETINGS_TEXT_WIDTH * 157,
	p4_greetings_text_data + P4_GREETINGS_TEXT_WIDTH * 226,
	p4_greetings_text_data + P4_GREETINGS_TEXT_WIDTH * 297,
	p4_greetings_text_data + P4_GREETINGS_TEXT_WIDTH * 320,
	p4_greetings_text_data + P4_GREETINGS_TEXT_WIDTH * 351
};

// Function to handle palette cycling
static void p4_color_cycle(uint32_t *colors) {
    uint32_t temp = colors[16];
    for (uint8_t i = 16; i < 31; ++i) {
        colors[i] = colors[i + 1];
    }
    colors[31] = temp;
}

static int32_t part_4_render(struct loader_shared_state *state) {
	static uint32_t counter = 153;  // Frame counter
	static uint32_t p4_scroll = 103;  // Scroll control

	// Centering the logos
	uint32_t x_center = (state->buffer_width - LOADER_LOGO_WIDTH) >> 1;

	// Blitting top and bottom logos with color palette
	fast_blit_with_palette(state, loader_logo_data, LOADER_LOGO_WIDTH, LOADER_LOGO_HEIGHT, fsn_top_logo_colors, x_center, P4_TOP_LOGO_Y_OFFSET);
	fast_blit_with_palette(state, loader_logo_data, LOADER_LOGO_WIDTH, LOADER_LOGO_HEIGHT, fsn_bottom_logo_colors, x_center, P4_BOTTOM_LOGO_Y_OFFSET);

	// Color cycling every second frame
	if(counter % 2 == 0) {
		p4_color_cycle(fsn_top_logo_colors);
		p4_color_cycle(fsn_bottom_logo_colors);
	}

	// Reset scrolling and prepare for new text
	if(counter == 512 || counter == 1536 || counter == 2560 || counter == 3584 || counter == 4608 || counter == 5632 || counter == 6656) {
		p4_scroll = 256;  // Start scroll reset to prepare for next text
	}

	// Update text at specific offsets after scroll reset
	if(counter == 153 || counter == 512 + 153 || counter == 1536 + 153 || counter == 2560 + 153 || counter == 3584 + 153 || counter == 4608 + 153 || counter == 5632 + 153 || counter == 6656 + 153) {
		uint32_t index = (counter - 153) / 512;
		uint8_t *src = greetings_offsets[index];
		uint8_t *scroll_dst = p4_scroll_buffer + P4_UPSCROLL_RENDER_CONTENT_Y_OFFSET * P4_UPSCROLL_WIDTH + ((P4_UPSCROLL_WIDTH - P4_GREETINGS_TEXT_WIDTH) / 2);
		for(uint32_t i = 0; i < greetings_heights[index]; ++i) {
			memcpy(scroll_dst, src, P4_GREETINGS_TEXT_WIDTH);
			scroll_dst += P4_UPSCROLL_WIDTH;
			src += P4_GREETINGS_TEXT_WIDTH;
		}
	}

	// Perform the scroll
	if(p4_scroll) {
		memmove(p4_scroll_buffer, p4_scroll_buffer + P4_UPSCROLL_WIDTH, sizeof(p4_scroll_buffer) - P4_UPSCROLL_WIDTH);
		--p4_scroll;
	}

	// Render the scroll buffer to the main buffer
	uint32_t *data = state->buffer + (P4_UPSCROLL_Y_POS * state->buffer_width + ((state->buffer_width - P4_UPSCROLL_WIDTH) / 2));
	uint8_t *src = p4_scroll_buffer;
	for(uint32_t y = 0; y < P4_UPSCROLL_VISIBLE_HEIGHT; ++y) {
		uint32_t temp_color = backgroundUpscrollColor[y];
		for(uint32_t x = 0; x < P4_UPSCROLL_WIDTH; ++x) {
			if(*src++) {
				data[x] = temp_color;
			}
		}
		data += state->buffer_width;
	}

	++counter;
	return 0; // Indicate continuous running
}

