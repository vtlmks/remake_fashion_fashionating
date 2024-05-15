
static uint32_t RoundReal32ToUInt32(float value) {
	return (uint32_t)_mm_cvtss_si32(_mm_set_ss(value));
}

struct v4{
	float r, g, b, a;
};

static struct v4 rgba_unpack_4x8(uint32_t packed) {
	struct v4 result;
	result.a = (float)((packed >>  0) & 0xff) / 255.0f;
	result.b = (float)((packed >>  8) & 0xff) / 255.0f;
	result.g = (float)((packed >> 16) & 0xff) / 255.0f;
	result.r = (float)((packed >> 24) & 0xff) / 255.0f; // Divide by 255 for 0-1 range
	return result;
}

static uint32_t rgba_pack_4x8(struct v4 unpacked) {
	uint32_t result = 0;
	result |= (uint32_t)(unpacked.a * 255.0f) << 0;
	result |= (uint32_t)(unpacked.g * 255.0f) << 16;
	result |= (uint32_t)(unpacked.b * 255.0f) << 8;
	result |= (uint32_t)(unpacked.r * 255.0f) << 24; // Multiply by 255 for 0-255 range
	return result;
}

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

#define p1_scroll_buffer_width 352
static uint8_t p1_scroll_buffer[p1_scroll_buffer_width * 2 * 18];
static uint8_t p1_temp_buffer[(352 + P1_PRESENTS_FASHIONATING_WIDTH) * 15];

static int32_t p1_stars[276];
static int32_t p1_initialized = false;

static uint32_t p1_star_colors[] = { 0x444444ff, 0x888888ff, 0xbbbbbbff, 0xffffffff };

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

#define mo (368 - P1_ROTATING_LOGO_WIDTH)
static struct point p1_bling_sprite_locations[] = {
	{ 156-6-4, 103-6 },
	{ 131-6-4,  79-6 },
	{ 208-6-4,  79-6 },
	{ 214-4-4,  55-6 },
	{  92-6-4,  73-6 },
	{ 258-4-4, 114-6 },
	{ 278-3-4,  55-6 },
	{ 258-4-4, 103-6 },
	{ 189-6-4,  55-6 },
	{  92-6-4, 113-6 },
};

typedef struct {
	uint32_t startFrame;
	uint32_t endFrame;	// Inclusive
	void (*renderFunction)(struct remake_state *, uint32_t);
} AnimationStep;

void render_blinking_cursor(struct remake_state *state, uint32_t frame);
void render_repositioned_blinking_cursor(struct remake_state *state, uint32_t frame);
void render_type_load_command(struct remake_state *state, uint32_t frame);
void render_search_and_load_text(struct remake_state *state, uint32_t frame);
void render_type_run_command(struct remake_state *state, uint32_t frame);
void finalize_animation_sequence(struct remake_state *state, uint32_t frame);

// Array of animation steps (sentinel value at the end)
static const AnimationStep animationSteps[] = {

	{   0, 261, render_blinking_cursor },							// Displays a blinking cursor
	{ 262, 421, render_type_load_command },						// Types out the command `load "fashionating",8,1`
	{ 422, 599, render_search_and_load_text },					// Displays `Searching for fashionating\nloading`
	{ 600, 859, render_repositioned_blinking_cursor },			// Repositions and displays a blinking cursor
	{ 860, 905, render_type_run_command },							// Types out the word `run`
	{ 600, 905, finalize_animation_sequence },					// Finalizes the sequence, overlaps with previous steps
	{  -1,  -1, 0 }														// Sentinel value to mark the end of the array
};

static void c64_effect(struct remake_state *state) {
	// background_audio_state.mute_sound = true;

	// Fill background with the first color in the palette
	uint32_t *dst = (uint32_t*)state->shared->buffer;
	uint32_t color = c64_colors_load_run[0];
	for(uint32_t i = 0; i < state->shared->buffer_width * state->shared->buffer_height; ++i) {
		*dst++ = color;
	}

	// Calculate offset for centering C64 screen data
	uint32_t skip = (state->shared->buffer_width - P1_C64_SCREEN_WIDTH);
	dst = (uint32_t*)(state->shared->buffer + 28 * state->shared->buffer_width + skip / 2);

	// Copy C64 screen data with color lookups
	uint8_t *src = p1_c64_screen_data;
	for(uint32_t y = 0; y < P1_C64_SCREEN_HEIGHT; ++y) {
		for(uint32_t x = 0; x < P1_C64_SCREEN_WIDTH; ++x) {
			*dst++ = c64_colors_load_run[*src++];  // Lookup color from LUT
		}
		dst += skip;
	}

	// Iterate through animation steps
	int i = 0;
	do {
		if(p1_frame >= animationSteps[i].startFrame && p1_frame <= animationSteps[i].endFrame) {
			animationSteps[i].renderFunction(state, p1_frame);
		}
		i++;
	} while (animationSteps[i].startFrame != -1);  // Check sentinel value
}

void render_blinking_square(struct remake_state *state, uint32_t onoff, uint32_t *dst) {
	uint32_t color = c64_colors_load_run[onoff];
	for(uint32_t y = 0; y < 8; ++y) {
		uint32_t *row = dst;
		for(uint32_t x = 0; x < 8; ++x) {
			*row++ = color;
		}
		dst += state->shared->buffer_width;
	}
}

void render_blinking_cursor(struct remake_state *state, uint32_t frame) {
	uint32_t skip = (state->shared->buffer_width - P1_C64_SCREEN_WIDTH);
	uint32_t *dst = (state->shared->buffer + (28 + 48) * state->shared->buffer_width + 20 + (skip / 2));
	render_blinking_square(state, (frame >> 4) & 1, dst);
}

void render_repositioned_blinking_cursor(struct remake_state *state, uint32_t frame) {
	uint32_t skip = (state->shared->buffer_width - P1_C64_SCREEN_WIDTH);
	uint32_t *dst = (state->shared->buffer + (92 + 24) * state->shared->buffer_width + 20 + (skip / 2));
	render_blinking_square(state, (frame >> 4) & 1, dst);
}

void render_type_load_command(struct remake_state *state, uint32_t frame) {
	uint32_t skip = (state->shared->buffer_width - P1_C64_SCREEN_WIDTH);
	uint32_t *dst = (state->shared->buffer + (28 + 48) * state->shared->buffer_width + 20 + (skip / 2));
	uint8_t *src = p1_c64_loading_run_data;

	for(uint32_t y = 0; y < 8; ++y) {
		uint32_t *row = dst;
		uint8_t *source = src;
		for(uint32_t x = 0; x < 7 * ((p1_frame - 256) >> 3); ++x) {
			*row++ = c64_colors_load_run[*source++];
		}
		dst += state->shared->buffer_width;
		src += P1_C64_LOADING_RUN_WIDTH;
	}
}

void render_search_and_load_text(struct remake_state *state, uint32_t frame) {
	uint32_t skip = (state->shared->buffer_width - P1_C64_SCREEN_WIDTH);
	uint32_t *dst = (state->shared->buffer + 76 * state->shared->buffer_width + 20 + (skip / 2));
	uint8_t *src = p1_c64_loading_run_data;

	for(uint32_t y = 0; y < 8; ++y) {
		uint32_t *row = dst;
		uint8_t *source = src;
		for(uint32_t x = 0; x < P1_C64_LOADING_RUN_WIDTH; ++x) {
			*row++ = c64_colors_load_run[*source++];
		}
		dst += state->shared->buffer_width;
		src += P1_C64_LOADING_RUN_WIDTH;
	}

	dst = (state->shared->buffer + 92 * state->shared->buffer_width + 20 + (skip / 2));
	for(uint32_t y = 0; y < 16; ++y) {
		uint32_t *row = dst;
		uint8_t *source = src;
		for(uint32_t x = 0; x < P1_C64_LOADING_RUN_WIDTH; ++x) {
			*row++ = c64_colors_load_run[*source++];
		}
		dst += state->shared->buffer_width;
		src += P1_C64_LOADING_RUN_WIDTH;
	}
}

void render_type_run_command(struct remake_state *state, uint32_t frame) {
	uint32_t skip = (state->shared->buffer_width - P1_C64_SCREEN_WIDTH);
	uint32_t *dst = (state->shared->buffer + (92 + 24) * state->shared->buffer_width + 20 + (skip / 2));
	uint8_t *src = p1_c64_loading_run_data + 32 * P1_C64_LOADING_RUN_WIDTH;

	for(uint32_t y = 0; y < 8; ++y) {
		uint32_t *row = dst;
		uint8_t *source = src;
		for(uint32_t x = 0; x < 7 * ((frame - 860) >> 3); ++x) {
			*row++ = c64_colors_load_run[*source++];
		}
		dst += state->shared->buffer_width;
		src += P1_C64_LOADING_RUN_WIDTH;
	}
}

void finalize_animation_sequence(struct remake_state *state, uint32_t frame) {
	uint32_t skip = (state->shared->buffer_width - P1_C64_SCREEN_WIDTH);
	uint32_t *dst = (state->shared->buffer + 76 * state->shared->buffer_width + 20 + (skip / 2));
	uint8_t *src = p1_c64_loading_run_data;

	for(uint32_t y = 0; y < 8; ++y) {
		uint32_t *row = dst;
		uint8_t *source = src;
		for(uint32_t x = 0; x < P1_C64_LOADING_RUN_WIDTH; ++x) {
			*row++ = c64_colors_load_run[*source++];
		}
		dst += state->shared->buffer_width;
		src += P1_C64_LOADING_RUN_WIDTH;
	}

	dst = (state->shared->buffer + 92 * state->shared->buffer_width + 20 + (skip / 2));
	for(uint32_t y = 0; y < 24; ++y) {
		uint32_t *row = dst;
		uint8_t *source = src;
		for(uint32_t x = 0; x < P1_C64_LOADING_RUN_WIDTH; ++x) {
			*row++ = c64_colors_load_run[*source++];
		}
		dst += state->shared->buffer_width;
		src += P1_C64_LOADING_RUN_WIDTH;
	}
}

// [=]===^=====================================================================================^===[=]
static void decrunchEffect(struct remake_state *state) {
	uint32_t *dest = state->shared->buffer;
	uint32_t temp_color_index = color_index++;
	uint32_t temp_color = c64_colors[temp_color_index & 0xf];
	uint32_t total_pixels = state->shared->buffer_width * state->shared->buffer_height;
	uint32_t set_size = 9 * state->shared->buffer_width + ((temp_color_index % 3) * state->shared->buffer_width);
	uint32_t i = 0;

	for(i = 0; i < (set_size - (pixel_counter % set_size)); ++i) {
		*dest++ = temp_color;
	}

	pixel_counter += set_size;
	total_pixels -= set_size - (pixel_counter % set_size);

	for(;;) {
		temp_color = c64_colors[++temp_color_index % 0xf];
		set_size = 5 * state->shared->buffer_width + ((temp_color_index % 10) * state->shared->buffer_width);

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
static void p1_scroller(struct remake_state *state) {
	static uint8_t *scroll_text_ptr = p1_scroll_text;
	static uint32_t count = 96;

	if(count == 96) {
		count = 0;

		if(*scroll_text_ptr == '@') {
			scroll_text_ptr = p1_scroll_text;
		}

		/* clear right side of buffer */
		uint8_t *dest = p1_scroll_buffer + p1_scroll_buffer_width;
		for(uint32_t y = 0; y < 18; ++y) {
			memset(dest, 0, p1_scroll_buffer_width);
			dest += p1_scroll_buffer_width*2;
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
			uint8_t *dest = p1_scroll_buffer + p1_scroll_buffer_width + 16 * i;
			for(uint32_t y = 0; y < 18; ++y) {
				_mm_storeu_si128((__m128i*)dest, _mm_loadu_si128((const __m128i*)font)); // Unaligned load and store
				font += P1_SCROLL_FONT_WIDTH;
				dest += p1_scroll_buffer_width * 2;
			}
		}
	}

	uint8_t *scr_src = p1_scroll_buffer + scroll_speed[count];
	uint8_t *scr_dst = p1_scroll_buffer;

	for(int32_t x = scroll_speed[count]; x < 704 * 18; ++x) {
		*scr_dst++ = *scr_src++;
	}

	uint32_t *data = state->shared->buffer + 248 * state->shared->buffer_width;

	uint8_t *source = p1_scroll_buffer;
	uint32_t *dest = data + ((state->shared->buffer_width - p1_scroll_buffer_width) / 2);
	__m128i zero = _mm_setzero_si128();  // Set a 128-bit zero for comparison

	for(uint32_t y = 0; y < 18; ++y) {
		uint32_t *row = dest;
		__m128i color = _mm_set1_epi32(scroll_colors[y]);  // Set the color for the whole row

		for(uint32_t x = 0; x < p1_scroll_buffer_width; x += 4) {
			// Load 4 bytes as 32-bit integer but treat them as individual bytes
			__m128i src = _mm_cvtsi32_si128(*(int32_t*)(source + x));
			src = _mm_unpacklo_epi8(src, zero);  // Unpack bytes to 16-bit
			src = _mm_unpacklo_epi16(src, zero);  // Unpack 16-bit to 32-bit
			__m128i mask = _mm_cmpgt_epi32(src, zero);  // Compare to zero to create a mask
			__m128i dest = _mm_loadu_si128((__m128i*)(row + x));  // Load 4 destination pixels
			__m128i maskedColor = _mm_and_si128(color, mask);  // Apply color where mask is true
			__m128i preservedDest = _mm_andnot_si128(mask, dest);  // Preserve dest where mask is false
			__m128i result = _mm_or_si128(maskedColor, preservedDest);  // Combine the two

			_mm_storeu_si128((__m128i*)(row + x), result);  // Store the result back to memory
		}
		source += p1_scroll_buffer_width*2;
		dest += state->shared->buffer_width;
	}


	count++;
}


static int32_t part_1_render(struct remake_state *state) {
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

	if (p1_frame >= 1188 && p1_frame < 1444) { // Fade from black to white in 256 frames
		uint8_t grayValue = (uint8_t)(p1_frame - 1188);
		uint32_t color = (grayValue << 24) | (grayValue << 16) | (grayValue << 8) | grayValue;

		for (uint32_t i = 0; i < arraysize(p1_rotating_logo_fade_colors); ++i) {
			p1_rotating_logo_fade_colors[i] = color; // All pixels get the same grayscale
		}
	}

	if (p1_frame >= 1444 && p1_frame < 1494) { // Fade from white to final colors
		float t = (float)(p1_frame - 1444) / 50.0f;  // 0.0 to 1.0 over 50 frames

		for(uint32_t i = 0; i < arraysize(p1_rotating_logo_fade_colors); ++i) {
			struct v4 srcColor = rgba_unpack_4x8(0xffffffff); // White
			struct v4 destColor = rgba_unpack_4x8(p1_rotating_logo_final_colors[i]);
			struct v4 newColor = {
				.r = blend(srcColor.r, t, destColor.r),
				.g = blend(srcColor.g, t, destColor.g),
				.b = blend(srcColor.b, t, destColor.b),
				.a = blend(srcColor.a, t, destColor.a)  // Blend alpha as well
			};

			p1_rotating_logo_fade_colors[i] = rgba_pack_4x8(newColor);
		}
	}

	if(p1_frame >= 1154) {
		// struct remake *remake = (struct remake *)state->remake_userdata;
		/*
		 *  Stars
		 */
		if(!p1_initialized) {	// NOTE(peter): I really hate these kinds of lazy initializations, should be done as one p1_initialize() call instead
			uint32_t offset = state->shared->buffer_width + 30;
			for(uint32_t i = 0; i < state->shared->buffer_height; ++i) {
				p1_stars[i] = offset + xor_generate_random(&state->rand_state) % (state->shared->buffer_width + 30);
			}
			p1_initialized = true;
		}

		uint32_t *row = state->shared->buffer;
		for(uint32_t i = 0; i < state->shared->buffer_height; ++i) {
			uint32_t offs = p1_stars[i];

			if((offs > 0) & (offs < state->shared->buffer_width)) {
				*(row + offs) = p1_star_colors[(i % 4)];
			}

			p1_stars[i] -= (1 + (i % 4));
			if(p1_stars[i] <= 0) {
				p1_stars[i] += state->shared->buffer_width + 30;
			}

			row += state->shared->buffer_width;
		}

		/*
		 *  Logo
		 */
		uint32_t *data = state->shared->buffer + 55 * state->shared->buffer_width + ((state->shared->buffer_width - P1_ROTATING_LOGO_WIDTH) / 2);
		uint8_t *src = p1_rotating_logo_data + 2 * P1_ROTATING_LOGO_WIDTH + (((rotatingLogoStep / 2) % p1_rotating_logo_steps) * P1_ROTATING_LOGO_WIDTH * p1_rotating_logo_frame_height);
		for(uint32_t y = 0; y < p1_rotating_logo_frame_height - 6; ++y) {
			row = data;
			for(uint32_t x = 0; x < P1_ROTATING_LOGO_WIDTH; ++x) {
				if(*src++) {
					*row = p1_rotating_logo_fade_colors[y];
				}
				++row;
			}
			data += state->shared->buffer_width;
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
		dest = p1_temp_buffer + state->shared->buffer_width - (p1_presents_counter * 15);
		for(uint32_t y = 0; y < 14; ++y) {
			for(uint32_t x = 0; x < P1_PRESENTS_FASHIONATING_WIDTH; ++x) {
				*dest++ = *source++;
			}
			dest += state->shared->buffer_width;
		}

		source = p1_temp_buffer;
		row = state->shared->buffer + 166 * state->shared->buffer_width;
		for(uint32_t y = 0; y < 14; ++y) {
			for(uint32_t x = 0; x < state->shared->buffer_width; ++x) {
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
			dest += state->shared->buffer_width;
		}

		source = p1_temp_buffer + P1_PRESENTS_FASHIONATING_WIDTH;
		row = state->shared->buffer + (206 * state->shared->buffer_width);
		for(uint32_t y = 0; y < 14; ++y) {
			for(uint32_t x = 0; x < state->shared->buffer_width; ++x) {
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
			uint32_t *dest = state->shared->buffer + (state->shared->buffer_width * p1_bling_sprite_locations[p1_bling_pos_index].y + p1_bling_sprite_locations[p1_bling_pos_index].x);
			for(uint32_t y = 0; y < P3_STARS_HEIGHT; ++y) {
				uint32_t *row = dest;
				for(uint32_t x = 0; x < P3_STARS_HEIGHT; ++x) {
					if(*src) {
						*row = p3_stars_palette[*src];
					}
					++src;
					++row;
				}
				dest += state->shared->buffer_width;
				src += P3_STARS_WIDTH - P3_STARS_HEIGHT;

			}
			--p1_bling_star_phase_index;
		}
	}

	++p1_frame;

	return false;
}

