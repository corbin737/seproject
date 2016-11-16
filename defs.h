#ifndef DEFS_H
#define DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define SCREEN_HEIGHT 127
#define SCREEN_WIDTH 32
#define NUMBER_OF_LANES 3
#define NUMBER_OF_TILES 6

// Track UI
#define TILE_WIDTH 10
#define TILE_WIDTH_BORDER 1
#define TILE_HEIGHT 21
#define TILE_HEIGHT_BORDER 1
#define BARRICADE_WIDTH 4
#define BARRICADE_HEIGHT 17
#define BARRICADE_BUFFER_X (TILE_HEIGHT-BARRICADE_HEIGHT)/2
#define BARRICADE_BUFFER_Y (TILE_WIDTH-BARRICADE_WIDTH)/2

#ifdef __cplusplus
}
#endif

#endif
