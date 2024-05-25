
static struct point stars[31];
int32_t p3Init = false;

#define p3_scroller_width 352
#define p3_scroller_height 16
#define p3_scroller_char_width 16

// Copper Colors

uint32_t p3_copper_colors[] = {
	0x333333ff, 0x555555ff, 0x888888ff, 0xbbbbbbff, 0xddddddff, 0xeeeeeeff, 0xddddddff, 0xbbbbbbff, 0x888888ff, 0x555555ff, 0x333333ff,
};

uint8_t p3_scroll_text[] = {
	"                 welcome to another part of 'fashionating'. first of all we want you to look at that picture! great i"
	"sn't it? ofcourse it was drawn by the master of art : scum !! and the great music was composed and arranged by the re-in"
	"carnation of mozart : the d.j.!! and who is me? i am just the incredible shark, who is just busy beating his keyboard. bythewa"
	"y,ithinkmyspacebarisbroken. ha there it works again.    enough, lets move on to the most important thing of a demo : the greet"
	"ings!  first of all we do not have a ranking list, just because some people could get embarrased when they read their name at "
	"number 234!  so we have a greetingslist in random order. we send our fashionating greetings to : pentagon - dominators - rage "
	"- helloween - the web inc. - bamiga sector 1 - 7 up crew - starlight - e$g/ibb - the kent team - axenon - tedware - general/or"
	"acle - demian - dma - axxess - random access - flashpoint - omicrons - eric/fac - f4cg - tic/supersonics - the weird science -"
	" finnish gold - mc.crack/apt - amiga industries - amiga gang - cbc - tsk crew - the bos team - dbs - the bitboys - new generat"
	"ion crew - gcc - taurus 1 - new frontiers and all the others we forgot to mention here!!       we also want to mention all the"
	" others who wrote us a letter but never got an answer from us. we say sorry to : the pioneers/tbc - rs6502 - xades society - b"
	"ad boys - the new dimension - the future team - softrunner crew - u.s.s. - the honey crew - tff/dragon lights - mr. michael - "
	"tcf 2087 - dexion - errorsoft - catlon crew - excess - powerstation - term of notice - the rest. also sorry to the new aces, t"
	"he l.n.c.c. and tc/shadow software.     the reason why these people didn't got an answer from us was that we were very b"
	"usy with this demo. we also make demos for a videocompany, and we have our normal jobs too. anyway, thank you for writing us. "
	"                    the next people we are going to greet are no contacts of us, but we greet them for their work on the amiga"
	". we greet : hqc, unit a, blizzard, karsten obarski, defjam, knight hawks, antitrax, sodan, magician 42, etc.               if"
	" you want to write us then press the mouse button to enter the last part of this fashionating mega-demo. else, keep on reading"
	"!                      fashion is now officially no longer a member of tls!               fashion members today are : the d.j."
	" - scum - shark - etonic - joshua 5 - solo - the coach and skully.        watch out for solo's game : outrun!         th"
	"is demo was produced in co-operation with tdk, 3m, random access virus killer v1.2 and coca-cola. (i'm trying to bring s"
	"ome humor in this scroll)                 we have a question : why doesn't anyone use the build-in speech-synthesizer? ("
	"maybe because it is rotten?)                       well, i'm out of text, so this is the point where i have to say : goodb"
	"ye!! see you next time!!                                                    all routines, music, graphics and ideas are copyri"
	"ght by fashion (tm) in the year 1988                                                                                          "
	"                                                                                             @ "
};

static uint8_t *p3_scroll_text_ptr = p3_scroll_text;

static uint32_t p3_star_colors[] = {
	0x777777ff, 0xaaaaaaff, 0xffffffff,
};

static uint8_t p3_scroll_buffer[(p3_scroller_width + p3_scroller_char_width) * p3_scroller_height];
static uint8_t p3_bob_buffer[((2 * P3_BOB_WIDTH) + 376) * P3_BOB_HEIGHT];		// TODO(peter): Fix this, can't be having fixed width 376........
static uint32_t p3_scroll_count = 8;

static uint32_t p3_frame = 0;
static int32_t p3_flirt_frames = 0;
static uint32_t p3_eyes = 0;

static uint32_t p3_bling_star_phase_index = 0;
static uint32_t p3_bling_pos_index = 0;
static uint32_t p3_bling_star_phases[] = {
	0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 11, 11, 11, 11, 11,
	10, 10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0,
};

static struct point p3_bling_sprite_locations[] = {
	{ 296,	110 },				// Bottom left on right N
	{ 268,	146 },				// Right shoulder
	{  36,	170 },				// Left 'thing'
	{ 287,	 77 },				// top left (right N)
	{  48,	120 },				// Left shoulder
	{  35,	 12 },				// Top right (left F)
	{ 233,	 66 },				// Top right, female helmet
	{ 296,	 36 },				// Bottom of right F
	{ 296,	186 },				// 'U' on signature
	{ 311,	 39 },				// Top of right S
	{ 232,	122 },				// Top of right shoulder
	{ 102,	 15 },				// Top left, male helmet
	{  35,	 76 },				// top Right (left N)
	{ 189,	 33 },				// right side of male helmet
	{  35,	 39 },				// Top, right (Left S)
	{ 264,	176 },				// S on signature
};

static void part_3_render(struct remake_state *state) {
	static uint32_t first_line_of_logo_rendered = P3_GAME_OVER_LOGO_HEIGHT - 1;
	static int32_t flood_fill = true;

	uint32_t x;
	uint32_t y;
	uint32_t *row;

	if(!p3Init) {
		for(uint16_t i = 0; i < arraysize(stars); ++i) {
			stars[i].x = state->shared->buffer_width + xor_generate_random(&state->rand_state) % (state->shared->buffer_width + 30);
			stars[i].y = i;
		}
		p3Init = true;
	}

	// Render copperbars
	row = state->shared->buffer;
	for(y = 0; y < 8; ++y) {
		for(x = 0; x < state->shared->buffer_width; ++x) {
			*row++ = p3_copper_colors[3 + y];
		}
	}

	row = state->shared->buffer + 208 * state->shared->buffer_width;
	for(y = 0; y < 11; ++y) {
		for(x = 0; x < state->shared->buffer_width; ++x) {
			*row++ = p3_copper_colors[y];
		}
	}

	row = state->shared->buffer + 237 * state->shared->buffer_width;
	for(y = 0; y < 11; ++y) {
		for(x = 0; x < state->shared->buffer_width; ++x) {
			*row++ = p3_copper_colors[y];
		}
	}

	static uint32_t counter = 0;
	if((counter++ % 2) == 0) {
		--first_line_of_logo_rendered;
		if(first_line_of_logo_rendered == 0xffffffff) {
			flood_fill = false;
		}
	}

	uint32_t *game_over_logo_start = state->shared->buffer + 8 * state->shared->buffer_width + ((state->shared->buffer_width - P3_GAME_OVER_LOGO_WIDTH) / 2);
	if(flood_fill) {
		// render floodfilling logo
		uint8_t *src;
		uint32_t *row = game_over_logo_start;
		uint32_t floodY = 0;

		while(floodY < first_line_of_logo_rendered) {
			uint32_t *pixel = row;
			src = p3_game_over_logo_data + first_line_of_logo_rendered * P3_GAME_OVER_LOGO_WIDTH;
			for(x = 0; x < P3_GAME_OVER_LOGO_WIDTH; ++x) {
				*pixel++ = p3_game_over_logo_palette[*src++];
			}
			row += state->shared->buffer_width;
			++floodY;
		}

		row = game_over_logo_start + first_line_of_logo_rendered * state->shared->buffer_width;
		src = p3_game_over_logo_data + first_line_of_logo_rendered * P3_GAME_OVER_LOGO_WIDTH;
		for(y = 0; y < (P3_GAME_OVER_LOGO_HEIGHT - first_line_of_logo_rendered); ++y) {
			uint32_t *pixel = row;
			for(x = 0; x < P3_GAME_OVER_LOGO_WIDTH; ++x) {
				*pixel++ = p3_game_over_logo_palette[*src++];
			}
			row += state->shared->buffer_width;
		}
	} else {

		uint32_t *row = game_over_logo_start;					// render complete logo
		uint8_t	*src = p3_game_over_logo_data;

		for(y = 0; y < P3_GAME_OVER_LOGO_HEIGHT; ++y) {
			uint32_t *pixel = row;
			for(x = 0; x < P3_GAME_OVER_LOGO_WIDTH; ++x) {
				*pixel++ = p3_game_over_logo_palette[*src++];
			}
			row += state->shared->buffer_width;
		}
	}

	if(p3_frame > 402) {

		if(--p3_scroll_count == 0) {
			p3_scroll_count = 8;
			if(*p3_scroll_text_ptr == '@') {
				p3_scroll_text_ptr = p3_scroll_text;
			}

			uint8_t character = *p3_scroll_text_ptr++;
			if(character >= 'a') {
				character -= 'a';
			} else {
				character -= 1;
			}

			uint8_t *dest = p3_scroll_buffer + p3_scroller_width;
			uint8_t *src = p3_small_scroll_font_data + character * 16 * 16;
			for(y = 0; y < 16; ++y) {
				for(x = 0; x < 16; ++x) {
					*dest++ = *src++;
				}
				dest += p3_scroller_width;
			}
		}

		uint8_t *src = p3_scroll_buffer + 2;
		uint8_t *dst = p3_scroll_buffer;
		for(x = 0; x < sizeof(p3_scroll_buffer) - 2; ++x) {
			*dst++ = *src++;
		}

		/*
		 * render flirty eyes 4 x 16x12
		 */
		if((p3_frame % 167) == 0) {
			p3_flirt_frames = 7;
		}

		if(p3_flirt_frames) {
			static uint32_t flirty_eyes_offsets[] = {
				0, 16, 32, 48, 32, 16, 0,
			};
			src = p3_flirty_eye_data + flirty_eyes_offsets[7 - p3_flirt_frames];
			uint32_t *dest = state->shared->buffer + 97 * state->shared->buffer_width + 202;
			for(uint32_t y = 0; y < 11; ++y) {
				for(uint32_t x = 0; x < 16; ++x) {
					if(*src) {
						*dest = p3_flirty_eye_palette[*src];
					}
					src++;
					dest++;
				}
				src += 48;
				dest += (state->shared->buffer_width - 16);
			}
			if(p3_frame % 2 == 0) {
				--p3_flirt_frames;
			}
		}

		/*
		 * render eyes 3x
		 */
		if((p3_frame % 80) == 0) {
			++p3_eyes;
		}
		src = p3_eyes_data + ((p3_eyes % 3) * 8) * P3_EYES_WIDTH;
		uint32_t *dest = state->shared->buffer + 43 * state->shared->buffer_width + 125;
		for(uint32_t y = 0; y < 8; ++y) {
			for(uint32_t x = 0; x < P3_EYES_WIDTH; ++x) {
				*dest++ = p3_eyes_palette[*src++];
			}
			dest += (state->shared->buffer_width - P3_EYES_WIDTH);
		}

		/*
		 * render stars
		 */
		for(uint32_t i = 0; i < arraysize(stars); ++i) {
			stars[i].x -= (i % 3) + 1;
			if(stars[i].x < 0) {
				stars[i].x += state->shared->buffer_width;
			}
		}

		uint32_t *stars_start = state->shared->buffer + 249 * state->shared->buffer_width;
		uint32_t star_lines = 0x133 - 0x114;
		row = stars_start;

		for(x = 0; x < arraysize(stars); ++x) {
			if((stars[x].x > 0) & (stars[x].x < (int32_t)state->shared->buffer_width)) {
				uint32_t *pixel = row;
				pixel += stars[x].x;
				*pixel = p3_star_colors[x % 3];
			}
			row += state->shared->buffer_width;
		}

		/*
		 * render scroller
		 */
		row = state->shared->buffer + (220 * state->shared->buffer_width) + ((state->shared->buffer_width - p3_scroller_width) / 2);
		src = p3_scroll_buffer;
		for(y = 0; y < p3_scroller_height; ++y) {
			uint32_t *pixel = row;
			for(x = 0; x < p3_scroller_width; ++x) {
				if(*src) {
					*pixel = p3_small_scroll_font_palette[*src];
				}
				++pixel;
				++src;
			}
			row += state->shared->buffer_width;
			src += p3_scroller_char_width;
		}

		/*
		 * render Bob
		 */
		if((p3_frame % 512) == 0) {
			src = p3_bob_data;
			dst = p3_bob_buffer;
			for(y = 0; y < P3_BOB_HEIGHT; ++y) {
				for(x = 0; x < P3_BOB_WIDTH; ++x) {
					if(*src) {
						*dst = *src;
					}
					++src;
					++dst;
				}
				dst += state->shared->buffer_width + P3_BOB_WIDTH;
			}
		}

		dst = p3_bob_buffer + 2 * P3_BOB_WIDTH + state->shared->buffer_width - 1;
		for(y = 0; y < P3_BOB_HEIGHT; ++y) {
			*dst = 0;
			dst += 2 * P3_BOB_WIDTH + state->shared->buffer_width;
		}

		memmove(p3_bob_buffer + 1, p3_bob_buffer, sizeof(p3_bob_buffer) - 1);

		src = p3_bob_buffer + P3_BOB_WIDTH;
		dest = state->shared->buffer + 257 * state->shared->buffer_width;
		for(y = 0; y < P3_BOB_HEIGHT; ++y) {
			for(x = 0; x < state->shared->buffer_width; ++x) {
				if(*src) {
					*dest = p3_bob_palette[*src];
				}
				++src;
				++dest;
			}
			src += P3_BOB_WIDTH * 2;
		}

		/*
		 * Render Bling stars
		 */
		if((p3_frame % 81) == 0) {
			p3_bling_star_phase_index = arraysize(p3_bling_star_phases);
			if(++p3_bling_pos_index == arraysize(p3_bling_sprite_locations)) {
				p3_bling_pos_index = 0;
			}
		}

		if(p3_bling_star_phase_index) {

			src = p3_stars_data + p3_bling_star_phases[p3_bling_star_phase_index] * 15;
			dest = state->shared->buffer + (state->shared->buffer_width * p3_bling_sprite_locations[p3_bling_pos_index].y + p3_bling_sprite_locations[p3_bling_pos_index].x) + 20;
			for(y = 0; y < P3_STARS_HEIGHT; ++y) {
				row = dest;
				for(x = 0; x < P3_STARS_HEIGHT; ++x) {
					if(*src) {
						*row = p3_stars_palette[*src];
					}
					++src;
					++row;
				}
				dest += state->shared->buffer_width;
				src += P3_STARS_WIDTH - P3_STARS_HEIGHT;

			}
			--p3_bling_star_phase_index;
		}

	}

	++p3_frame;
}