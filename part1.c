
// static uint32_t RoundReal32ToUInt32(float value) { return (uint32_t)_mm_cvtss_si32(_mm_set_ss(value)); }
// static V4 RGBAUnpack4x8(uint32_t Packed) { return { (float)((Packed >> 0) & 0xff), (float)((Packed >> 8) & 0xff), (float)((Packed >> 16) & 0xff), (float)((Packed >> 24) & 0xff) }; }
// static uint32_t RGBAPack4x8(V4 Unpacked) { return ((RoundReal32ToUInt32(Unpacked.a) << 24) | (RoundReal32ToUInt32(Unpacked.b) << 16) | (RoundReal32ToUInt32(Unpacked.g) << 8) | (RoundReal32ToUInt32(Unpacked.r) << 0)); }

#define p1_rotating_logo_steps 30
#define p1_rotating_logo_frame_height (P1_ROTATING_LOGO_HEIGHT / p1_rotating_logo_steps)

#define blend(a, t, b) (((1.f - t) * a) + (t * b))

#define p1_c64CursorWidth 7
#define p1_c64CursorHeight 8


#define p1_c64ScreenStartX	57
#define p1_c64ScreenWidth	280
#define p1_c64ScreenStartY	39
#define p1_c64ScreenHeight	200

// NOTE(peter): The following 5 all start at X = 0
#define p1_c64TopTextPosY			47
#define p1_c64LoadFashionatingPosY	87
#define p1_c64SearchingLoadingPosY	103
#define p1_c64ReadyPosY				119
#define p1_c64RunPosY				127


static uint32_t p1_rotating_logo_fade_colors[64] = {
};

static uint32_t c64_colors[] = {
	0x00000000, 0xffffffff, 0xcc0000ff, 0x00ffddff, 0xbb00ffff, 0x00cc00ff, 0x0000ccff, 0xffff00ff,
	0xcc7700ff, 0x773300ff, 0xff6688ff, 0x666666ff, 0x999999ff, 0x99ff77ff, 0x8899ffff, 0xccccccff,
};

static uint32_t p1_rotating_logo_final_colors[64] = {
	0xffddddff, 0xffddddff, 0xffbbbbff, 0xffbbbbff, 0xee9999ff, 0xee9999ff, 0xee8888ff, 0xee8888ff,
	0xdd7777ff, 0xdd7777ff, 0xdd6666ff, 0xdd6666ff, 0xdd5555ff, 0xdd5555ff, 0xcc4444ff, 0xcc4444ff,
	0xcc3333ff, 0xcc3333ff, 0xcc3322ff, 0xcc3322ff, 0xbb2222ff, 0xbb2222ff, 0xaa1111ff, 0xaa1111ff,
	0x880000ff, 0x880000ff, 0x660000ff, 0x660000ff, 0x440000ff, 0x440000ff, 0x220000ff, 0x220000ff,
	0x111144ff, 0x111144ff, 0x111199ff, 0x111199ff, 0x2222ccff, 0x2222ccff, 0x2222ffff, 0x2222ffff,
	0x3333ffff, 0x3333ffff, 0x4444ffff, 0x4444ffff, 0x4455ffff, 0x4455ffff, 0x5555ffff, 0x5555ffff,
	0x6666ffff, 0x6666ffff, 0x7777ffff, 0x7777ffff, 0x8888ffff, 0x8888ffff, 0x9999ffff, 0x9999ffff,
	0xaaaaffff, 0xaaaaffff, 0xbbbbffff, 0xbbbbffff, 0xccbbffff, 0xccbbffff, 0xccccffff, 0xccccffff,
};

static uint8_t scroll_speed[96] = {		// these scroll-values totals 352 pixels of scrolling
	1, 1, 1, 1, 2, 2, 4, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static uint32_t scroll_colors[] = {
	0x550000ff, 0x770000ff, 0x990000ff, 0xaa3333ff, 0xbb6666ff, 0xcc9999ff, 0xddccccff, 0xeeeeeeff, 0xffffffff,
	0xffffffff, 0xeeeeeeff, 0xccccddff, 0x9999ccff, 0x6666bbff, 0x3333aaff, 0x000099ff, 0x000077ff, 0x000055ff,
};

static uint8_t p1_scroll_text[] = {			// each line is 352 pixels wide, or 22 characters; not strictly neccesary, but it was done this way in the original code.
	"   fashion presents   "
	"  their newest demo!  "
	"                      "
	"  designed & created  "
	"          by          "
	"  shark (programming) "
	"    scum (artworx)    "
	"  the dj. (all music) "
	"                      "
	"   this demo is just  "
	"   knock em dead!!!   "
	"   so fashion says:   "
	"    spread this!!!    "
	"                      "
	"  all routines, music "
	"   and graphics are   "
	"      homemade!!!     "
	"                      "
	"     release date:    "
	"       may 1988       "
	"                      "
	"  press mousebutton   "
	"  to start the show!  "
	"                      "
	"                      "
	"                      "
	"                      "
	"@"
};

static uint32_t c64_colors_load_run[] = {
	0x8899ffff, 0x0000ccff,
};

static uint32_t p1_presents_colors[] = {
	0xffddddff, 0xffbbbbff, 0xee9999ff, 0xee8888ff, 0xdd7777ff, 0xdd6666ff, 0xdd5555ff,
	0xcc4444ff, 0xcc3333ff, 0xcc3232ff, 0xbb2222ff, 0xaa1111ff, 0x990000ff, 0x880000ff,
};

static uint32_t p1_fashionating_colors[] = {
	0x2222bbff, 0x2222ccff, 0x2222ffff, 0x3333ffff, 0x4444ffff, 0x4545ffff, 0x5555ffff,
	0x6666ffff, 0x7777ffff, 0x8888ffff, 0x9999ffff, 0xaaaaffff, 0xbbbbffff, 0xcbcbffff,
};

static uint8_t p1_scroll_buffer[352 * 2 * 18];
static uint8_t p1_temp_buffer[(352 + P1_PRESENTS_FASHIONATING_WIDTH) * 15];

static int32_t p1_stars[276];
static int32_t p1_initialized = false;

static uint32_t p1_star_colors[] = { 0xff444444, 0xff888888, 0xffbbbbbb, 0xffffffff };

static uint32_t p1_presents_counter = 0;
static uint32_t pixel_counter = 0;
static uint32_t color_index = 0;
static uint32_t p1_frame = 0;

static uint32_t p1_bling_star_phase_index = 0;
static uint32_t p1_bling_pos_index = 0;

static uint32_t p1_bling_star_phases[] = {
	0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10,
	11, 11, 11, 11, 11, 11, 11,
	10, 10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0,
};

struct point {
	uint32_t x, y;
};

static struct point p1_bling_sprite_locations[] = {
	{ 156-6, 103-6 },
	{ 131-6,  79-6 },
	{ 208-6,  79-6 },
	{ 214-4,  55-6 },
	{  92-6,  73-6 },
	{ 258-4, 114-6 },
	{ 278-3,  55-6 },
	{ 258-4, 103-6 },
	{ 189-6,  55-6 },
	{  92-6, 113-6 },
};


static void c64_effect(struct loader_shared_state *state) {
	//  background_audio_state.mute_sound = true;

	// Fill background
	memset(state->buffer, c64_colors_load_run[0], state->buffer_width * state->buffer_height * sizeof(uint32_t));

	// Calculate offset for centering C64 screen data
	uint32_t skip = (state->buffer_width - P1_C64_SCREEN_WIDTH);
	uint32_t *dst = (uint32_t*)(state->buffer + 28 * state->buffer_width + skip / 2);

	// Copy C64 screen data
	uint8_t *src = p1_c64_screen_data;
	for (uint32_t y = 0; y < P1_C64_SCREEN_HEIGHT; ++y) {
		uint32_t *row = dst;
		for(uint32_t x = 0; x < P1_C64_SCREEN_WIDTH; ++x) {
			if(*src++) {
				dst[x] = c64_colors_load_run[1];
			}
		}
		dst += state->buffer_width;
	}

	// Handle loading animation based on frame number
	if (p1_frame >= 0 && p1_frame < 906) {
		uint32_t yOffset, xOffset, height, width;
		if (p1_frame < 262) { // Blinking square
			yOffset = 28 + 48;
			xOffset = 20;
			height = 8;
			width = 8;
			src = p1_c64_loading_run_data + ((p1_frame >> 4) & 1) * P1_C64_LOADING_RUN_WIDTH;
		} else if (p1_frame < 422) { // Partial loading run
			yOffset = 28 + 48;
			xOffset = 20;
			height = 8;
			width = 7 * ((p1_frame - 256) >> 3);
			src = p1_c64_loading_run_data;
		} else if (p1_frame < 600) { // Full loading run (top)
			yOffset = 76;
			xOffset = 20;
			height = 8;
			width = P1_C64_LOADING_RUN_WIDTH;
			src = p1_c64_loading_run_data;
		} else { // Full loading run (top and bottom) & potentially blinking square
			yOffset = 76;
			xOffset = 20;
			height = (p1_frame < 906) ? 32 : 8;
			width = P1_C64_LOADING_RUN_WIDTH;
			src = p1_c64_loading_run_data;
		}

		dst = (uint32_t*)(state->buffer + yOffset * state->buffer_width + xOffset + (skip / 2));
		for (uint32_t y = 0; y < height; ++y) {
			memcpy(dst, c64_colors_load_run + *src, width * sizeof(uint32_t));
			src += P1_C64_LOADING_RUN_WIDTH;
			dst += state->buffer_width;
		}

		// Additional blinking square for the later part of the animation
		if (p1_frame >= 600 && p1_frame < 906) {
			dst = (uint32_t*)(state->buffer + (92 + 24) * state->buffer_width + 20 + (skip / 2));
			if (p1_frame < 860) {
				src = p1_c64_loading_run_data + ((p1_frame >> 4) & 1) * P1_C64_LOADING_RUN_WIDTH; // Blinking square
			} else {
				src = p1_c64_loading_run_data + 32 * P1_C64_LOADING_RUN_WIDTH; // Partial loading run
				width = 7 * ((p1_frame - 860) >> 3);
			}

			for (uint32_t y = 0; y < 8; ++y) {
				memcpy(dst, c64_colors_load_run + *src, width * sizeof(uint32_t));
				src += P1_C64_LOADING_RUN_WIDTH;
				dst += state->buffer_width;
			}
		}
	}
}

#if 0
// Constants for frame ranges
enum {
    FRAME_STATIC_SCREEN_START = 0,
    FRAME_STATIC_SCREEN_END = 262,
    FRAME_LOADING_RUN_START = 262,
    FRAME_LOADING_RUN_END = 422,
    FRAME_EXPANDED_LOADING_RUN_START = 422,
    FRAME_EXPANDED_LOADING_RUN_END = 906,
    FRAME_FINAL_FLASHING_BLOCK_START = 600,
    FRAME_FINAL_FLASHING_BLOCK_END = 860
};

// Animation stage definitions
typedef struct {
    uint32_t startFrame;
    uint32_t endFrame;
    void (*renderFunc)(struct loader_shared_state *state, uint32_t frame);
} AnimationStage;

// Function prototypes for animation stages
static void renderInitialScreen(struct loader_shared_state *state, uint32_t frame);
static void renderStaticScreen(struct loader_shared_state *state, uint32_t frame);
static void renderFlashingBlock(struct loader_shared_state *state, uint32_t frame);
static void renderLoadingRun(struct loader_shared_state *state, uint32_t frame);
static void renderExpandedLoadingRun(struct loader_shared_state *state, uint32_t frame);
static void renderFinalFlashingBlock(struct loader_shared_state *state, uint32_t frame);

// Animation stages array
static const AnimationStage animation_stages[] = {
	{FRAME_STATIC_SCREEN_START, FRAME_STATIC_SCREEN_END, renderStaticScreen},
	{FRAME_STATIC_SCREEN_START, FRAME_STATIC_SCREEN_END, renderFlashingBlock},
	{FRAME_LOADING_RUN_START, FRAME_LOADING_RUN_END, renderLoadingRun},
	{FRAME_EXPANDED_LOADING_RUN_START, FRAME_EXPANDED_LOADING_RUN_END, renderExpandedLoadingRun},
	{FRAME_FINAL_FLASHING_BLOCK_START, FRAME_FINAL_FLASHING_BLOCK_END, renderFlashingBlock},
	{FRAME_LOADING_RUN_START, FRAME_LOADING_RUN_END, renderLoadingRun}
};

static void c64_effect(struct loader_shared_state *state) {
	// background_audio_state.mute_sound = true;
	renderInitialScreen(state, 0); // Always render the initial screen
	// Find the current animation stage based on p1_frame
	for(int i = 0; i < sizeof(animation_stages) / sizeof(animation_stages[0]); ++i) {
		if(p1_frame >= animation_stages[i].startFrame && p1_frame < animation_stages[i].endFrame) {
			animation_stages[i].renderFunc(state, p1_frame);
			break; // Found and rendered, no need to check further
		}
	}
}

static void renderInitialScreen(struct loader_shared_state *state, uint32_t frame) {
	uint32_t *dst = state->buffer;
	for (uint32_t i = 0; i < state->buffer_width * state->buffer_height; ++i) {
		*dst++ = c64_colors_load_run[0]; // Solid color
	}
}

static void renderStaticScreen(struct loader_shared_state *state, uint32_t frame) {
	uint32_t skip = (state->buffer_width - P1_C64_SCREEN_WIDTH);
	uint32_t *dst = (state->buffer + 28 * state->buffer_width + skip / 2);
	uint8_t *src = p1_c64_screen_data;
	for (uint32_t y = 0; y < P1_C64_SCREEN_HEIGHT; ++y) {
		uint32_t *row = dst;
		for(uint32_t x = 0; x < P1_C64_SCREEN_WIDTH; ++x) {
			uint8_t val = *src++;
			if(val) {
				dst[x] = c64_colors_load_run[val];
			}
		}
		dst += state->buffer_width;
		// src += P1_C64_SCREEN_WIDTH;
	}
}

static void renderFlashingBlock(struct loader_shared_state *state, uint32_t frame) {
	uint32_t skip = (state->buffer_width - P1_C64_SCREEN_WIDTH);
	uint32_t *dst = (state->buffer + (28 + 48) * state->buffer_width + 20 + (skip / 2));
	for (uint32_t y = 0; y < 8; ++y) {
		for (uint32_t x = 0; x < 8; ++x) {
			*dst++ = c64_colors_load_run[(frame >> 4) & 1]; // Flashing color
		}
		dst += state->buffer_width - 8;
	}
}

static void renderLoadingRun(struct loader_shared_state *state, uint32_t frame) {
	uint32_t skip = (state->buffer_width - P1_C64_SCREEN_WIDTH);
	uint32_t *dst = (state->buffer + (frame < 600 ? 76 : 92 + 24) * state->buffer_width + 20 + (skip / 2));
	uint8_t *src = p1_c64_loading_run_data + (frame >= 860 ? 32 * P1_C64_LOADING_RUN_WIDTH : 0); // Offset for final run

	for (uint32_t y = 0; y < 8; ++y) {
		uint32_t runLength = (frame < 860) ? 7 * ((frame - (frame < 422 ? 256 : 860)) >> 3) : P1_C64_LOADING_RUN_WIDTH;
		for (uint32_t x = 0; x < runLength; ++x) {
			*dst++ = c64_colors_load_run[*src++];
		}
		dst += state->buffer_width - runLength;
		src += P1_C64_LOADING_RUN_WIDTH;
	}
}

static void renderExpandedLoadingRun(struct loader_shared_state *state, uint32_t frame) {
	renderLoadingRun(state, frame); // Reuse the loading run rendering

	// Expand vertically
	uint32_t skip = (state->buffer_width - P1_C64_SCREEN_WIDTH);
	uint32_t *dst = (state->buffer + 92 * state->buffer_width + 20 + (skip / 2));
	uint8_t *src = p1_c64_loading_run_data;
	uint32_t height = (frame < 600) ? 16 : 24; // Increase height in the later stage

	for (uint32_t y = 0; y < height; ++y) {
		memcpy(dst, src, P1_C64_LOADING_RUN_WIDTH);
		dst += state->buffer_width;
		src += P1_C64_LOADING_RUN_WIDTH;
	}
}
#endif

// [=]===^=====================================================================================^===[=]
static void decrunchEffect(struct loader_shared_state *state) {
	uint32_t *dest = state->buffer;
	uint32_t temp_color_index = color_index++;
	uint32_t temp_color = c64_colors[temp_color_index & 0xf];
	uint32_t total_pixels = state->buffer_width * state->buffer_height;
	uint32_t set_size = 9 * state->buffer_width + ((temp_color_index % 3) * state->buffer_width);
	uint32_t i = 0;

	for(i = 0; i < (set_size - (pixel_counter % set_size)); ++i) {
		*dest++ = temp_color;
	}

	pixel_counter += set_size;
	total_pixels -= set_size - (pixel_counter % set_size);

	for(;;) {
		temp_color = c64_colors[++temp_color_index % 0xf];
		set_size = 5 * state->buffer_width + ((temp_color_index % 10) * state->buffer_width);

		if(total_pixels < set_size) {
			for(i = 0; i < total_pixels; ++i) {
				*dest++ = temp_color;
			}
			pixel_counter += total_pixels;
			break;
		} else {
			for(i = 0; i < set_size; ++i) {
				*dest++ = temp_color;
			}
			pixel_counter += set_size;
			total_pixels -= set_size;
		}
	}
}

// [=]===^=====================================================================================^===[=]
static void p1_scroller(struct loader_shared_state *state) {
	static uint8_t *scroll_text_ptr = p1_scroll_text;
	//static uint32_t xx = 0;
	static uint32_t count = 96;
	if(count == 96) {
		count = 0;

		if(*scroll_text_ptr == '@') {
			scroll_text_ptr = p1_scroll_text;
		}

		/* clear right side of buffer */
		uint8_t *dest = p1_scroll_buffer + 352;
		for(uint32_t y = 0; y < 18; ++y) {
			for(uint32_t x = 0; x < 352; ++x) {
				*dest++ = 0;
			}
			dest += 352;
		}

		/* write 22 characters to buffer */
		for(uint32_t i = 0; i < 22; ++i) {
			uint8_t character = *scroll_text_ptr++;
			if(character >= 'a') {
				character -= 'a';
			} else {
				character -= 1;
			}
			uint8_t *font = p1_scroll_font_data + character * 24 * 16;
			uint8_t *dest = p1_scroll_buffer + 352 + 16 * i;
			for(uint32_t y = 0; y < 18; ++y) {
				for(uint32_t x = 0; x < 16; ++x) {
					*dest++ = *font++;
				}
				dest += 352 * 2 - 16;
			}
		}
	}

	uint8_t *scr_src = p1_scroll_buffer + scroll_speed[count];
	uint8_t *scr_dst = p1_scroll_buffer;

	for(int32_t x = scroll_speed[count]; x < 704 * 18; ++x) {
		*scr_dst++ = *scr_src++;
	}

	uint32_t *data = state->buffer + 248 * state->buffer_width;

	scr_src = p1_scroll_buffer;
	uint32_t *scr_dest = data + ((state->buffer_width - 352) / 2);
	for(uint32_t y = 0; y < 18; ++y) {
		uint32_t *row = scr_dest;
		for(uint32_t x = 0; x < 352; ++x) {
			if(*scr_src++) {
				*row = scroll_colors[y];
			}
			row++;
		}
		scr_src += 352;
		scr_dest += state->buffer_width;
	}
	count++;
}


static int32_t part_1_render(struct loader_shared_state *state) {
	static uint32_t decrunchTime = 256;
	static float accumulator = 0.f;
	static uint32_t cursorCounter = 0;
	static int32_t cursorVisible = false;

	static uint32_t rotatingLogoStep = 30;		// test
	static uint32_t writtenLength = 0;

	// accumulator += dt;

	if(p1_frame >= 0 && p1_frame < 906) {
		c64_effect(state);
	}

	if(p1_frame >= 906 && p1_frame < 1092) {
		decrunchEffect(state);
	}

	if(p1_frame == 1080) {
		// audio_set_current_stream(&background_audio_state, streams + demo_part_1_sample);
		// background_audio_state.mute_sound = false;
	}

	if(p1_frame == 1950) {
		// audio_set_current_stream(&background_audio_state, streams + demo_part_1);
		// background_audio_state.mute_sound = false;
	}

	/*
	 * Start rotating the logo
	*/
	if(p1_frame < 1453 - 12 + 30) {
		++rotatingLogoStep;
	}

	if(p1_frame >= 1188 && p1_frame < 1444) {		// Fade from black to white in 256 frames

		// float t = (256.f - (1443.f - (float)p1_frame)) / 256;
		// for(uint32_t i = 0; i < arraysize(p1_rotating_logo_fade_colors); ++i) {
		// 	float col = blend(0.f, t, 1.f);
		// 	uint32_t color = (uint8_t)(col * 255.f) << 24 | (uint8_t)(col * 255.f) << 16 | (uint8_t)(col * 255.f) << 8 | (uint8_t)(col * 255.f);
		// 	p1_rotating_logo_fade_colors[i] = color;
		// }
	}

	if(p1_frame >= 1444 && p1_frame < 1494) {
		float t = (50.f - (1493.f - (float)p1_frame)) / 50;

		// for(uint32_t i = 0; i < arraysize(p1_rotating_logo_fade_colors); ++i) {
		// 	V4 src_color = { 255.f, 255.f, 255.f, 255.f };
		// 	V4 dest_color = RGBAUnpack4x8(p1_rotating_logo_final_colors[i]);
		// 	V4 new_color;
		// 	new_color.r = blend(src_color.r, t, dest_color.r);
		// 	new_color.g = blend(src_color.g, t, dest_color.g);
		// 	new_color.b = blend(src_color.b, t, dest_color.b);
		// 	new_color.a = blend(src_color.a, t, dest_color.a);
		// 	uint32_t color = RGBAPack4x8(new_color);
		// 	p1_rotating_logo_fade_colors[i] = color;
		// }
	}

	if(p1_frame >= 1154) {
		struct remake *remake = (struct remake *)state->remake_userdata;
		/*
		 *  Stars
		 */
		if(!p1_initialized) {	// NOTE(peter): I really hate these kinds of lazy initializations, should be done as one p1_initialize() call instead
			uint32_t offset = state->buffer_width + 30;
			for(uint32_t i = 0; i < state->buffer_height; ++i) {
				p1_stars[i] = offset + xor_generate_random(&remake->rand_state) % (state->buffer_width + 30);

			}
			p1_initialized = true;
		}

		uint32_t *row = state->buffer;
		for(uint32_t i = 0; i < state->buffer_height; ++i) {
			uint32_t offs = p1_stars[i];

			if((offs > 0) & (offs < state->buffer_width)) {
				*(row + offs) = p1_star_colors[(i % 4)];
			}

			p1_stars[i] -= (1 + (i % 4));
			if(p1_stars[i] <= 0) {
				p1_stars[i] += state->buffer_width + 30;
			}

			row += state->buffer_width;
		}

		/*
		 *  Logo
		 */
		uint32_t *data = state->buffer + 55 * state->buffer_width + ((state->buffer_width - P1_ROTATING_LOGO_WIDTH) / 2);
		uint8_t *src = p1_rotating_logo_data + 2 * P1_ROTATING_LOGO_WIDTH + (((rotatingLogoStep / 2) % p1_rotating_logo_steps) * P1_ROTATING_LOGO_WIDTH * p1_rotating_logo_frame_height);
		for(uint32_t y = 0; y < p1_rotating_logo_frame_height - 6; ++y) {
			row = data;
			for(uint32_t x = 0; x < P1_ROTATING_LOGO_WIDTH; ++x) {
				if(*src++) {
					*row = p1_rotating_logo_fade_colors[y];
				}
				++row;
			}
			data += state->buffer_width;
		}
	}

	/*
	 * Show Presents + Fashionating
	 * Presents from right, fashionating from the left.
	 */
	if(p1_frame >= 1500) {
		uint8_t *source;
		uint8_t *dest;
		uint32_t *row;


		if(p1_presents_counter != 18) {
			++p1_presents_counter;
		}

		memset(p1_temp_buffer, 0, sizeof(p1_temp_buffer));
		source = p1_presents_fashionating_data;
		dest = p1_temp_buffer + state->buffer_width - (p1_presents_counter * 15);
		for(uint32_t y = 0; y < 14; ++y) {
			for(uint32_t x = 0; x < P1_PRESENTS_FASHIONATING_WIDTH; ++x) {
				*dest++ = *source++;
			}
			dest += state->buffer_width;
		}

		source = p1_temp_buffer;
		row = state->buffer + 166 * state->buffer_width;
		for(uint32_t y = 0; y < 14; ++y) {
			for(uint32_t x = 0; x < state->buffer_width; ++x) {
				if(*source++) {
					*row = p1_presents_colors[y];
				}
				++row;
			}
			source += P1_PRESENTS_FASHIONATING_WIDTH;
		}

		memset(p1_temp_buffer, 0, sizeof(p1_temp_buffer));
		source = p1_presents_fashionating_data + P1_PRESENTS_FASHIONATING_WIDTH * 14;
		dest = p1_temp_buffer + (p1_presents_counter * 15);
		for(uint32_t y = 0; y < 14; ++y) {
			for(uint32_t x = 0; x < P1_PRESENTS_FASHIONATING_WIDTH; ++x) {
				*dest++ = *source++;
			}
			dest += state->buffer_width;
		}

		source = p1_temp_buffer + P1_PRESENTS_FASHIONATING_WIDTH;
		row = state->buffer + (206 * state->buffer_width);
		for(uint32_t y = 0; y < 14; ++y) {
			for(uint32_t x = 0; x < state->buffer_width; ++x) {
				if(*source++) {
					*row = p1_fashionating_colors[y];
				}
				++row;
			}
			source += P1_PRESENTS_FASHIONATING_WIDTH;
		}

	}

	/*
	 * scroller
	 */
	if(p1_frame >= 1950) {
		p1_scroller(state);

		/*
		 * Render Bling stars
		 */
		if((p1_frame % 81) == 0) {
			p1_bling_star_phase_index = arraysize(p1_bling_star_phases);
			if(++p1_bling_pos_index == arraysize(p1_bling_sprite_locations)) {
				p1_bling_pos_index = 0;
			}
		}

		if(p1_bling_star_phase_index) {

			uint8_t *src = p3_stars_data + p1_bling_star_phases[p1_bling_star_phase_index] * 15;
			uint32_t *dest = state->buffer + (state->buffer_width * p1_bling_sprite_locations[p1_bling_pos_index].y + p1_bling_sprite_locations[p1_bling_pos_index].x);
			for(uint32_t y = 0; y < P3_STARS_HEIGHT; ++y) {
				uint32_t *row = dest;
				for(uint32_t x = 0; x < P3_STARS_HEIGHT; ++x) {
					if(*src) {
						*row = p3_stars_palette[*src];
					}
					++src;
					++row;
				}
				dest += state->buffer_width;
				src += P3_STARS_WIDTH - P3_STARS_HEIGHT;

			}
			--p1_bling_star_phase_index;
		}
	}

	++p1_frame;

	return false;
}

