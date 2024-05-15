
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

#define UTILS_IMPLEMENTATION
#include <utils.h>

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
#include "data/p3_stars.h"
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

#include "graphics_utils.h"

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

struct remake_state {
	struct loader_shared_state *shared;
	struct pt_state p1_fashionating;
	struct pt_state p2_world_of_the_dj;
	struct pt_state p3_parallax_2;
	struct pt_state p4_ivory_tower;
	struct sample_state sample;
	struct rng_state rand_state;
	uint32_t demo_state;									// Which demopart we are in
};

/*
 * Remake stuff
 */
#include "loader.c"
#include "part1.c"
#include "part2.c"
#include "part3.c"
#include "part4.c"

void setup(struct loader_shared_state *state) {
	state->remake_state = (struct remake_state *)calloc(1, sizeof(struct remake_state));
	struct remake_state *remake = (struct remake_state *)state->remake_state;
	remake->shared = state;

	remake->sample.data = resample_audio((int8_t*)fashion_sample_data, FASHION_SAMPLE_DATA_SIZE, 267, &remake->sample.size);
	pt2play_initPlayer(48000);
	pt2play_PlaySong(&remake->p1_fashionating, fashionating_data, CIA_TEMPO_MODE, 48000);
	pt2play_PlaySong(&remake->p2_world_of_the_dj, the_world_of_the_dj_data, CIA_TEMPO_MODE, 48000);
	pt2play_PlaySong(&remake->p3_parallax_2, parallax_ii_data, CIA_TEMPO_MODE, 48000);
	pt2play_PlaySong(&remake->p4_ivory_tower, ivorytowers_data, CIA_TEMPO_MODE, 48000);

	xor_init_rng(&remake->rand_state, 0x44780142);
}

void cleanup(struct loader_shared_state *state) {
	struct remake_state *remake = (struct remake_state *)state->remake_state;
	// NOTE(peter): clean up allocations here et.c
	free(remake->sample.data);
	free(state->remake_state);
	state->remake_state = 0;
}

void audio_callback(struct remake_state *state, int16_t *audio_buffer, size_t frames) {

	switch(state->demo_state) {

		// TODO(peter): make this a statemachine for part 1, as the sample shouldn't begin playing until a certain time, and then the module should start at yet another later time.
		case 1: {
			if(state->sample.done) {
				pt2play_FillAudioBuffer(&state->p1_fashionating, audio_buffer, frames);
			} else {
				int16_t *dst = audio_buffer;
				for(uint32_t i = 0; i < frames; ++i) {
					int16_t sample = state->sample.data[state->sample.position];
					sample = (int16_t)(sample * 0.707);
					*dst++ = sample;
					*dst++ = sample;
					state->sample.position++;
					if(state->sample.position == state->sample.size) {
						state->sample.done = true;
						break;
					}
				}
			}
		} break;

		case 3: {
			pt2play_FillAudioBuffer(&state->p2_world_of_the_dj, audio_buffer, frames);
		} break;

		case 5: {
			pt2play_FillAudioBuffer(&state->p3_parallax_2, audio_buffer, frames);
		} break;

		case 7: {
			pt2play_FillAudioBuffer(&state->p4_ivory_tower, audio_buffer, frames);
		} break;

		default: {
			memset(audio_buffer, 0, frames*2*sizeof(int16_t));
		} break;
	}
}


void key_callback(struct remake_state *state, int key) {
	if(state->shared->keyboard_state[REMAKE_KEY_1]) {
		state->demo_state = 1;
	}
	if(state->shared->keyboard_state[REMAKE_KEY_2]) {
		state->demo_state = 3;
	}
	if(state->shared->keyboard_state[REMAKE_KEY_3]) {
		state->demo_state = 5;
	}
	if(state->shared->keyboard_state[REMAKE_KEY_4]) {
		state->demo_state = 7;
	}
}

uint32_t mainloop_callback(struct remake_state *state) {
	uint32_t *buffer = state->shared->buffer;

	memset(buffer, 0, state->shared->buffer_width * state->shared->buffer_height * sizeof(uint32_t));

#if 0
	switch(state->demo_state) {
		case 1: { part_1_render(state); } break;
		case 3: { part_1_render(state); } break;
		case 5: { part_1_render(state); } break;
		case 7: { part_1_render(state); } break;
		default: {
			if(loader(state)) {
				state->demo_state++;
			}
		} break;

	}
#endif

	switch(state->demo_state) {
		case 1: {
			part_1_render(state);
		} break;
		case 3: {
			part_2_render(state);
		} break;
		case 5:
		case 7: {
			part_4_render(state);

			if(state->shared->mouse_button_state[REMAKE_MOUSE_BUTTON_LEFT]) {
				state->demo_state++;
				if(state->demo_state == 8) {
					state->demo_state = 0;
				}
				printf("%d\n", state->demo_state);
			}
		} break;
		default: {
#if 0
			if(loader(state)) {
				state->demo_state++;
			}
#else
			state->demo_state++;
#endif
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
