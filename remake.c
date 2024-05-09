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

#define ARRAYSIZE(x) (sizeof(x) / sizeof(*x))


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
			if(state->mouse_button_state[REMAKE_MOUSE_BUTTON_LEFT]) {
				remake->demo_state++;
				if(remake->demo_state == 8) {
					remake->demo_state = 0;
				}
				printf("%d\n", remake->demo_state);
			}
		} break;
		default: {
			// remake->demo_state++;	// TODO(peter): REMOVE
			loader(state);
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
