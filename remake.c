
// IMPORTANT(peter): Make sure the first color has alpha set to 0x00 and all other black colors has alpha set to 0xff, else havok..

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Framework
 */
#include <loader.h>
#include <remake.h>

/*
 * Audio data
 */
#include "data/fashion_sample.h"
#include "data/fashionating.h"
#include "data/ivorytowers.h"
#include "data/parallax_ii.h"
#include "data/the_world_of_the_dj.h"

/*
 * Graphics
 */
#include "data/loader_logo.h"
#include "data/p1_c64_loading_run.h"
#include "data/p1_c64_loading_run_original.h"
#include "data/p1_c64_screen.h"
#include "data/p1_presents_fashionating.h"
#include "data/p1_rotating_logo.h"
#include "data/p1_scroll_font.h"
#include "data/p2_bouncing_ball_springs.h"
#include "data/p2_bouncing_balls.h"
#include "data/p2_large_scroll_font.h"
#include "data/p2_logo.h"
#include "data/p2_stalaktites.h"
#include "data/p3_Stars.h"
#include "data/p3_bob.h"
#include "data/p3_eyes.h"
#include "data/p3_flirty_eye.h"
#include "data/p3_game_over_logo.h"
#include "data/p3_small_scroll_font.h"
#include "data/p4_greetings_text.h"

#ifndef arraysize
#define arraysize(x) (sizeof(x) / sizeof(*x))
#endif

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

uint32_t *pre_render_to_32bit(const uint8_t *image_8bit, uint32_t width, uint32_t height, const uint32_t *palette) {
	uint32_t *image_32bit = malloc(width * height * sizeof(uint32_t));
	if(image_32bit) {
		for(uint32_t i = 0; i < width * height; ++i) {
			image_32bit[i] = palette[image_8bit[i]];
		}
	}
	return image_32bit;
}

void fast_blit(struct loader_shared_state *state, const uint32_t *image,  uint32_t image_width, uint32_t image_height, uint32_t x_offset, uint32_t y_offset) {
	uint32_t *dest = state->buffer + y_offset * state->buffer_width + x_offset;

	for (uint32_t y = 0; y < image_height; ++y) {
		memcpy(dest, image, image_width * sizeof(uint32_t));
		dest += state->buffer_width;
		image += image_width;
	}
}

void fast_blit_transparency(struct loader_shared_state *state, const uint32_t *image_data, uint32_t image_width, uint32_t image_height, uint32_t x_offset, uint32_t y_offset) {
	uint32_t *dest = state->buffer + y_offset * state->buffer_width + x_offset;

	for(uint32_t y = 0; y < image_height; ++y) {
		for(uint32_t x = 0; x < image_width; ++x) {
			uint32_t color = image_data[x];
			if(color) {
				dest[x] = color;
			}
		}
		dest += state->buffer_width;
		image_data += image_width;
	}
}

void fast_blit_with_palette(struct loader_shared_state *state, const uint8_t *image_data, uint32_t image_width, uint32_t image_height, const uint32_t *palette, uint32_t x_offset, uint32_t y_offset) {
	uint32_t *dest = state->buffer + y_offset * state->buffer_width + x_offset;

	for (uint32_t y = 0; y < image_height; ++y) {
		for (uint32_t x = 0; x < image_width; ++x) {
			uint32_t color_id = image_data[x];
			if(color_id) {
				dest[x] = palette[color_id];
			}
		}
		dest += state->buffer_width;
		image_data += image_width;
	}
}


void render_and_clip_image(struct loader_shared_state *state, uint8_t *image_data, int image_width, int image_height, uint32_t *palette, int xOffset, int yOffset) {
	// Clipping calculations to ensure we don't write outside the buffer
	int startX = xOffset < 0 ? -xOffset : 0;
	int startY = yOffset < 0 ? -yOffset : 0;
	int endX = xOffset + image_width > state->buffer_width ? state->buffer_width - xOffset : image_width;
	int endY = yOffset + image_height > state->buffer_height ? state->buffer_height - yOffset : image_height;

	// Calculate the starting point in the buffer
	uint32_t *row = state->buffer + max(0, yOffset) * state->buffer_width + max(0, xOffset);
	uint8_t *image = image_data + startY * image_width + startX;

	// Iterate through the image data and render only the visible parts
	for(int y = startY; y < endY; ++y) {
		for(int x = startX; x < endX; ++x) {
			uint8_t color_id = image[x];
			if(color_id) { // Assuming 0 is transparent or should not be rendered
				row[x] = palette[color_id];
			}
		}
		row += state->buffer_width;
		image += image_width;
	}
}

/*
 * Remake stuff
 */
#include "loader.c"
#include "part1.c"
#include "part2.c"
#include "part3.c"
#include "part4.c"

/*
 * local use
 */
#include "protracker2.c"
#define MKS_RESAMPLER_IMPLEMENTATION
#include "../utility/resampler.h"

struct sample_state {
	int16_t *data;
	uint32_t size;
	uint32_t position;
	bool done;
};

struct remake  {
	struct pt_state p1_fashionating;
	struct pt_state p2_world_of_the_dj;
	struct pt_state p3_parallax_2;
	struct pt_state p4_ivory_tower;
	struct sample_state sample;
	uint32_t demo_state;									// Which demopart we are in
};

void setup(struct loader_shared_state *state) {
	state->remake_userdata = (struct remake *)calloc(1, sizeof(struct remake));
	struct remake *remake = (struct remake *)state->remake_userdata;

	remake->sample.data = resample_audio(fashion_sample_data, FASHION_SAMPLE_DATA_SIZE, 267, &remake->sample.size);
	pt2play_initPlayer(48000);
	pt2play_PlaySong(&remake->p1_fashionating, fashionating_data, CIA_TEMPO_MODE, 48000);
	pt2play_PlaySong(&remake->p2_world_of_the_dj, the_world_of_the_dj_data, CIA_TEMPO_MODE, 48000);
	pt2play_PlaySong(&remake->p3_parallax_2, parallax_ii_data, CIA_TEMPO_MODE, 48000);
	pt2play_PlaySong(&remake->p4_ivory_tower, ivorytowers_data, CIA_TEMPO_MODE, 48000);
}

void cleanup(struct loader_shared_state *state) {
	struct remake *remake = (struct remake *)state->remake_userdata;
	// NOTE(peter): clean up allocations here et.c
	free(remake->sample.data);
	free(state->remake_userdata);
	state->remake_userdata = 0;
}

void audio_callback(struct loader_shared_state *state, int16_t *audio_buffer, size_t frames) {
	struct remake *remake = (struct remake *)state->remake_userdata;

	switch(remake->demo_state) {

		// TODO(peter): make this a statemachine for part 1, as the sample shouldn't begin playing until a certain time, and then the module should start at yet another later time.
		case 1: {
			if(remake->sample.done) {
				pt2play_FillAudioBuffer(&remake->p1_fashionating, audio_buffer, frames);
			} else {
				int16_t *dst = audio_buffer;
				for(uint32_t i = 0; i < frames; ++i) {
					int16_t sample = remake->sample.data[remake->sample.position];
					*dst++ = sample;
					*dst++ = sample;
					remake->sample.position++;
					if(remake->sample.position == remake->sample.size) {
						remake->sample.done = true;
						break;
					}
				}
			}
		} break;

		case 3: {
			pt2play_FillAudioBuffer(&remake->p2_world_of_the_dj, audio_buffer, frames);
		} break;

		case 5: {
			pt2play_FillAudioBuffer(&remake->p3_parallax_2, audio_buffer, frames);
		} break;

		case 7: {
			pt2play_FillAudioBuffer(&remake->p4_ivory_tower, audio_buffer, frames);
		} break;

		default: {
			memset(audio_buffer, 0, frames*2*sizeof(int16_t));
		} break;
	}
}


void key_callback(struct loader_shared_state *state, int key) {
	struct remake *remake = (struct remake *)state->remake_userdata;
}

int32_t mainloop_callback(struct loader_shared_state *state) {
	struct remake *remake = (struct remake *)state->remake_userdata;
	uint32_t *buffer = state->buffer;

	memset(buffer, 0, state->buffer_width*state->buffer_height*sizeof(uint32_t));

#if 0
	switch(remake->demo_state) {
		case 1: { part_1_render(state); } break;
		case 3: { part_1_render(state); } break;
		case 5: { part_1_render(state); } break;
		case 7: { part_1_render(state); } break;
		default: {
			if(loader(state)) {
				remake->demo_state++;
			}
		} break;

	}
#endif

	switch(remake->demo_state) {
		case 1:
		case 3:
		case 5:
		case 7: {
			part_4_render(state);

			if(state->mouse_button_state[REMAKE_MOUSE_BUTTON_LEFT]) {
				remake->demo_state++;
				if(remake->demo_state == 8) {
					remake->demo_state = 0;
				}
				printf("%d\n", remake->demo_state);
			}
		} break;
		default: {
			if(loader(state)) {
				remake->demo_state++;
			}
		}
	}

	return 0;
}

struct remake_info remake_information = {
	.release_name = "Fashion - Fashionating (1988-05)",
	.display_name = "Fashion - Fashionating",
	.author_name = "ViTAL/Mindkiller Systems",
	.buffer_width = 368,
	.buffer_height = 276,
	.frames_per_second = 50,
	.setup = setup,
	.cleanup = cleanup,
	.key_callback = key_callback,
	.audio_callback = audio_callback,
	.mainloop_callback = mainloop_callback,
};

// NOTE(peter): This is only for windows, as it's too lame to be able to getProcessAddress of the struct, like dlsym can on linux.
EXPORT struct remake_info *get_remake_information() { return &remake_information; }
