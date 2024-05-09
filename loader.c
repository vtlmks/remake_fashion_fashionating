

uint32_t loader_logo_red_palette[] = {
	0x00000000,0xffeeeeff,0xffddddff,0xffccccff,0xffbbbbff,0xffaaaaff,0xff9999ff,0xff8888ff,
	0xff7777ff,0xff6666ff,0xee4444ff,0xcc2222ff,0xbb0000ff,0x990000ff,0x770000ff,0x440000ff,
	0x771111ff,0x993333ff,0xaa4444ff,0xcc6666ff,0xdd8888ff,0xffbbbbff,0xeeaaaaff,0xdd8888ff,
	0xcc6666ff,0xaa4444ff,0x993333ff,0x771111ff,0x661111ff,0x440000ff,0x330000ff,0x661111ff,
};

uint32_t loader(struct loader_shared_state *state) {
	static uint32_t loader_timer = 200;

	if(state->frame_number & 0x1) {
		uint32_t temp = loader_logo_red_palette[16];
		memmove(&loader_logo_red_palette[16], &loader_logo_red_palette[17], 15 * sizeof(uint32_t));
		loader_logo_red_palette[31] = temp;
	}

	uint32_t x = (state->buffer_width - LOADER_LOGO_WIDTH) >> 1;
	uint32_t y = (state->buffer_height - LOADER_LOGO_HEIGHT) >> 1;
	fast_blit_with_palette(state, loader_logo_data, LOADER_LOGO_WIDTH, LOADER_LOGO_HEIGHT, loader_logo_red_palette, x, y);

	if(loader_timer) {
		--loader_timer;
	} else {
		loader_timer = 200;
		return true;
	}

	return false;
}
