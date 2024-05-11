#pragma once

#include <cpuid.h>

#include <immintrin.h>
#include <xmmintrin.h>  // SSE
#include <emmintrin.h>  // SSE2
#include <smmintrin.h>	// SSE4.1

// Function pointer types for each function
typedef uint32_t *(*pre_render_to_32bit_func)(const uint8_t *, uint32_t, uint32_t, const uint32_t *);
typedef void (*fast_blit_func)(struct loader_shared_state *, const uint32_t *, uint32_t, uint32_t, uint32_t, uint32_t);
typedef void (*fast_blit_transparency_func)(struct loader_shared_state *, const uint32_t *, uint32_t, uint32_t, uint32_t, uint32_t);
typedef void (*fast_blit_with_palette_func)(struct loader_shared_state *, const uint8_t *, uint32_t, uint32_t, const uint32_t *, uint32_t, uint32_t);
typedef void (*render_and_clip_image_func)(struct loader_shared_state *, uint8_t *, int, int, uint32_t *, int, int);

// Global function pointers
static pre_render_to_32bit_func pre_render_to_32bit_ptr = NULL;
static fast_blit_func fast_blit_ptr = NULL;
static fast_blit_transparency_func fast_blit_transparency_ptr = NULL;
static fast_blit_with_palette_func fast_blit_with_palette_ptr = NULL;
static render_and_clip_image_func render_and_clip_image_ptr = NULL;

// Forward declarations for all functions
static uint32_t *pre_render_to_32bit_default(const uint8_t *image_8bit, uint32_t width, uint32_t height, const uint32_t *palette);
static void fast_blit_default(struct loader_shared_state *state, const uint32_t *image_data, uint32_t image_width, uint32_t image_height, uint32_t x_offset, uint32_t y_offset);
static void fast_blit_transparency_default(struct loader_shared_state *state, const uint32_t *image_data, uint32_t image_width, uint32_t image_height, uint32_t x_offset, uint32_t y_offset);
static void fast_blit_with_palette_default(struct loader_shared_state *state,  const uint8_t *image_data, uint32_t image_width, uint32_t image_height, const uint32_t *palette, uint32_t x_offset, uint32_t y_offset);
static void fast_blit_with_palette_sse4_1(struct loader_shared_state *state, const uint8_t *image_data, uint32_t image_width, uint32_t image_height, const uint32_t *palette, uint32_t x_offset, uint32_t y_offset);
static void fast_blit_with_palette_avx(struct loader_shared_state *state, const uint8_t *image_data, uint32_t image_width, uint32_t image_height, const uint32_t *palette, uint32_t x_offset, uint32_t y_offset);
static void fast_blit_with_palette_avx2(struct loader_shared_state *state, const uint8_t *image_data, uint32_t image_width, uint32_t image_height, const uint32_t *palette, uint32_t x_offset, uint32_t y_offset);
static void render_and_clip_image_default (struct loader_shared_state *state,        uint8_t *image_data,      int image_width,      int image_height, uint32_t *palette, int xOffset, int yOffset);

// Check OS support for YMM (used by AVX and AVX2)
int check_ymm_support() {
	unsigned int eax, edx;
	__asm__ __volatile__(
		"xgetbv" : "=a"(eax), "=d"(edx) : "c"(0)
	);
	return (eax & 0x6) == 0x6;
}

// Example initialization function (extend with real checks and additional implementations)
void initialize_graphics_functions(void) {
	unsigned int eax, ebx, ecx, edx, saved_ecx;

	pre_render_to_32bit_ptr = pre_render_to_32bit_default;
	fast_blit_ptr = fast_blit_default;
	fast_blit_transparency_ptr = fast_blit_transparency_default;
	fast_blit_with_palette_ptr = fast_blit_with_palette_default;
	render_and_clip_image_ptr = render_and_clip_image_default;

	if(__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
		saved_ecx = ecx;  // Save the ecx value after calling __get_cpuid(1, ...)
		if(ecx & bit_SSE3) {
//			op = sse3_operation;
		}
		if(ecx & bit_SSE4_1) {
			fast_blit_with_palette_ptr = fast_blit_with_palette_sse4_1;
		}
		if(ecx & bit_AVX) {
			if(ecx & bit_OSXSAVE && check_ymm_support()) {
				// fast_blit_with_palette_ptr = fast_blit_with_palette_avx;
			}
		}
	}

	// Check for AVX2 support
	if(__get_cpuid_count(7, 0, &eax, &ebx, &ecx, &edx)) {
		if(ebx & bit_AVX2) {
			if(saved_ecx & bit_OSXSAVE && check_ymm_support()) {  // Use the saved ecx value
				fast_blit_with_palette_ptr = fast_blit_with_palette_avx2;
			}
		}
	}

}

// API functions that users will call, using the function pointers
uint32_t *pre_render_to_32bit(const uint8_t *image_8bit, uint32_t width, uint32_t height, const uint32_t *palette) {
    if(!pre_render_to_32bit_ptr) {
        initialize_graphics_functions();
    }
    return pre_render_to_32bit_ptr(image_8bit, width, height, palette);
}

void fast_blit(struct loader_shared_state *state, const uint32_t *image, uint32_t image_width, uint32_t image_height, uint32_t x_offset, uint32_t y_offset) {
    if(!fast_blit_ptr) {
        initialize_graphics_functions();
    }
    fast_blit_ptr(state, image, image_width, image_height, x_offset, y_offset);
}

void fast_blit_transparency(struct loader_shared_state *state, const uint32_t *image_data, uint32_t image_width, uint32_t image_height, uint32_t x_offset, uint32_t y_offset) {
    if(!fast_blit_transparency_ptr) {
        initialize_graphics_functions();
    }
    fast_blit_transparency_ptr(state, image_data, image_width, image_height, x_offset, y_offset);
}

void fast_blit_with_palette(struct loader_shared_state *state, const uint8_t *image_data, uint32_t image_width, uint32_t image_height, const uint32_t *palette, uint32_t x_offset, uint32_t y_offset) {
    if(!fast_blit_with_palette_ptr) {
        initialize_graphics_functions();
    }
    fast_blit_with_palette_ptr(state, image_data, image_width, image_height, palette, x_offset, y_offset);
}

void render_and_clip_image(struct loader_shared_state *state, uint8_t *image_data, int image_width, int image_height, uint32_t *palette, int xOffset, int yOffset) {
    if(!render_and_clip_image_ptr) {
        initialize_graphics_functions();
    }
    render_and_clip_image_ptr(state, image_data, image_width, image_height, palette, xOffset, yOffset);
}


////////////////////////////////////////////


static uint32_t *pre_render_to_32bit_default(const uint8_t *image_8bit, uint32_t width, uint32_t height, const uint32_t *palette) {
	uint32_t *image_32bit = (uint32_t *)malloc(width * height * sizeof(uint32_t));
	if(image_32bit) {
		for (uint32_t i = 0; i < width * height; ++i) {
			image_32bit[i] = palette[image_8bit[i]];
		}
	}
	return image_32bit;
}

static void fast_blit_default(struct loader_shared_state *state, const uint32_t *image, uint32_t image_width, uint32_t image_height, uint32_t x_offset, uint32_t y_offset) {
	uint32_t *dest = state->buffer + y_offset * state->buffer_width + x_offset;
	for (uint32_t y = 0; y < image_height; ++y) {
		memcpy(dest, image, image_width * sizeof(uint32_t));
		dest += state->buffer_width;
		image += image_width;
	}
}

static void fast_blit_transparency_default(struct loader_shared_state *state, const uint32_t *image_data, uint32_t image_width, uint32_t image_height, uint32_t x_offset, uint32_t y_offset) {
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

static void fast_blit_with_palette_default(struct loader_shared_state *state, const uint8_t *image_data, uint32_t image_width, uint32_t image_height, const uint32_t *palette, uint32_t x_offset, uint32_t y_offset) {
	uint32_t *dest = state->buffer + y_offset * state->buffer_width + x_offset;
	for(uint32_t y = 0; y < image_height; ++y) {
		for(uint32_t x = 0; x < image_width; ++x) {
			uint32_t color_id = image_data[x];
			if(color_id) {
				dest[x] = palette[color_id];
			}
		}
		dest += state->buffer_width;
		image_data += image_width;
	}
}

__attribute__((target("sse4.1")))
static void fast_blit_with_palette_sse4_1(struct loader_shared_state *state, const uint8_t *image_data, uint32_t image_width, uint32_t image_height, const uint32_t *palette, uint32_t x_offset, uint32_t y_offset) {
	uint32_t *dest = state->buffer + y_offset * state->buffer_width + x_offset;

	for(uint32_t y = 0; y < image_height; ++y) {
		const uint8_t *row_src = image_data + y * image_width;
		uint32_t *row_dest = dest + y * state->buffer_width;

		uint32_t x = 0;
		for(; x <= image_width - 4; x += 4) {  // Process in blocks of 4
			// Load 4 color IDs (8-bit values) at once
			__m128i color_ids = _mm_loadl_epi64((const __m128i*)(row_src + x));
			color_ids = _mm_unpacklo_epi8(color_ids, _mm_setzero_si128());  // Zero extend to 16-bit
			color_ids = _mm_unpacklo_epi16(color_ids, _mm_setzero_si128());  // Zero extend to 32-bit

			// Manually gather palette colors
			uint32_t idx0 = _mm_extract_epi32(color_ids, 0);
			uint32_t idx1 = _mm_extract_epi32(color_ids, 1);
			uint32_t idx2 = _mm_extract_epi32(color_ids, 2);
			uint32_t idx3 = _mm_extract_epi32(color_ids, 3);

			__m128i new_colors = _mm_setr_epi32(palette[idx0], palette[idx1], palette[idx2], palette[idx3]);

			// Create a mask where color_ids are not zero
			__m128i mask = _mm_cmpeq_epi32(color_ids, _mm_setzero_si128());
			mask = _mm_xor_si128(mask, _mm_set1_epi32(-1));  // Invert mask: now `1` where color_id != 0

			// Load existing colors from the destination
			__m128i old_colors = _mm_loadu_si128((__m128i*)(row_dest + x));

			// Blend based on mask
			__m128i blended_colors = _mm_blendv_epi8(old_colors, new_colors, mask);

			// Store results
			_mm_storeu_si128((__m128i*)(row_dest + x), blended_colors);
		}

		// Handle remaining pixels
		for(; x < image_width; ++x) {
			uint32_t color_id = row_src[x];
			if(color_id != 0) {
				row_dest[x] = palette[color_id];
			}
		}
	}
}


__attribute__((target("avx")))
static void fast_blit_with_palette_avx(struct loader_shared_state *state, const uint8_t *image_data, uint32_t image_width, uint32_t image_height, const uint32_t *palette, uint32_t x_offset, uint32_t y_offset) {
	uint32_t *dest = state->buffer + y_offset * state->buffer_width + x_offset;
	uint32_t buffer_width = state->buffer_width;

	for(uint32_t y = 0; y < image_height; ++y) {
		uint32_t *row_dest = dest + y * buffer_width;
		const uint8_t *row_src = image_data + y * image_width;

		for(uint32_t x = 0; x < image_width; x += 8) {
			// Load 8 pixels, these are 8 palette indices.
			__m128i index_pack = _mm_loadl_epi64((__m128i*)(row_src + x)); // Load 8 bytes (palette indices)

			// Since we cannot use gather in AVX1, manually process indices
			uint32_t colors[8];
			for(int i = 0; i < 8; ++i) {
				uint8_t idx = ((uint8_t*)&index_pack)[i];
				colors[i] = palette[idx];
			}

			// Store results using a single 256-bit store
			_mm256_storeu_si256((__m256i*)(row_dest + x), _mm256_loadu_si256((__m256i*)&colors[0]));
		}
	}
}


__attribute__((target("avx2")))
static void fast_blit_with_palette_avx2(struct loader_shared_state *state, const uint8_t *image_data, uint32_t image_width, uint32_t image_height, const uint32_t *palette, uint32_t x_offset, uint32_t y_offset) {
	uint32_t *dest = state->buffer + y_offset * state->buffer_width + x_offset;

	for(uint32_t y = 0; y < image_height; ++y) {
		const uint8_t *row_src = image_data + y * image_width;
		uint32_t *row_dest = dest + y * state->buffer_width;

		uint32_t x = 0;
		for(; x <= image_width - 8; x += 8) {  // Process in blocks of 8
			__m128i color_ids_low = _mm_loadl_epi64((const __m128i*)(row_src + x));
			__m256i color_ids = _mm256_cvtepu8_epi32(color_ids_low); // Zero extend to 32-bit
			__m256i colors = _mm256_i32gather_epi32((const int*)palette, color_ids, 4);
			__m256i mask = _mm256_cmpeq_epi32(color_ids, _mm256_setzero_si256());
			mask = _mm256_xor_si256(mask, _mm256_set1_epi32(-1)); // Invert mask: `1` where color_id != 0
			__m256i old_colors = _mm256_loadu_si256((__m256i*)(row_dest + x));
			__m256i blended_colors = _mm256_blendv_epi8(old_colors, colors, mask);
			_mm256_storeu_si256((__m256i*)(row_dest + x), blended_colors);
		}

		// Handle remaining pixels
		for(; x < image_width; ++x) {
			uint32_t color_id = row_src[x];
			if(color_id != 0) {
				row_dest[x] = palette[color_id];
			}
		}
	}
}


#if 0
__attribute__((target("avx2")))
static void fast_blit_with_palette_avx2(struct loader_shared_state *state, const uint8_t *image_data, uint32_t image_width, uint32_t image_height, const uint32_t *palette, uint32_t x_offset, uint32_t y_offset) {
    uint32_t *dest = state->buffer + y_offset * state->buffer_width + x_offset;
    uint32_t buffer_width = state->buffer_width;

    for(uint32_t y = 0; y < image_height; ++y) {
        uint32_t *row_dest = dest + y * buffer_width;
        const uint8_t *row_src = image_data + y * image_width;

        for(uint32_t x = 0; x < image_width; x += 8) {
            __m256i color_ids = _mm256_cvtepu8_epi32(_mm_loadu_si64((__m128i*)(row_src + x)));
            __m256i colors = _mm256_i32gather_epi32((const int*)palette, color_ids, 4);
            _mm256_storeu_si256((__m256i*)(row_dest + x), colors);
        }
    }
}
#endif

// __attribute__((target("avx2")))
// static void fast_blit_with_palette_avx2(struct loader_shared_state *state, const uint8_t *image_data, uint32_t image_width, uint32_t image_height, const uint32_t *palette, uint32_t x_offset, uint32_t y_offset) {
// 	uint32_t *dest = state->buffer + y_offset * state->buffer_width + x_offset;
// 	int num_pixels = image_width * image_height;
// 	int i = 0;

// 	// Process pixels in groups of 8
// 	for (; i < num_pixels - 7; i += 8) {
// 		// __m256i idx = _mm256_loadu_si256((__m256i*)(image_data + i));
// 		__m256i idx = _mm256_cvtepu8_epi32(_mm_loadl_epi64((__m128i*)(image_data + i)));

// 		__m256i colors = _mm256_i32gather_epi32((const int*)palette, idx, 4);
// 		_mm256_storeu_si256((__m256i*)(dest + i), colors);
// 	}

// 	// Process remaining pixels that don't fit into a full AVX register
// 	for (; i < num_pixels; ++i) {
// 		dest[i] = palette[image_data[i]];
// 	}
// }


static void render_and_clip_image_default(struct loader_shared_state *state, uint8_t *image_data, int image_width, int image_height, uint32_t *palette, int xOffset, int yOffset) {
	int startX = xOffset < 0 ? -xOffset : 0;
	int startY = yOffset < 0 ? -yOffset : 0;
	int endX = xOffset + image_width > state->buffer_width ? state->buffer_width - xOffset : image_width;
	int endY = yOffset + image_height > state->buffer_height ? state->buffer_height - yOffset : image_height;

	uint32_t *row = state->buffer + max(0, yOffset) * state->buffer_width + max(0, xOffset);
	uint8_t *image = image_data + startY * image_width + startX;

	for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
			uint8_t color_id = image[x];
			if(color_id) {
					row[x] = palette[color_id];
			}
		}
		row += state->buffer_width;
		image += image_width;
	}
}
