#ifndef SDL2_GFX_EXT
#define SDL2_GFX_EXT

#include <SDL2/SDL.h>

int thickEllipseRGBA(SDL_Renderer * renderer, Sint16 xc, Sint16 yc, Sint16 xr, Sint16 yr, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick);
int thickArcRGBA(SDL_Renderer * renderer, Sint16 xc, Sint16 yc, Sint16 rad, Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick);
int thickCircleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick);
int thickEllipseColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color, Uint8 thick);
int thickArcColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint32 color, Uint8 thick);
int thickCircleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint32 color, Uint8 thick);
int aaFilledEllipseRGBA(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int aaFilledEllipseColor(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, Uint32 color);
int aaFilledPolygonRGBA(SDL_Renderer * renderer, const double * vx, const double * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int aaFilledPolygonColor(SDL_Renderer * renderer, const double * vx, const double * vy, int n, Uint32 color);
int aaFilledPieRGBA(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, float start, float end, Uint32 chord, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int aaFilledPieColor(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, float start, float end, Uint32 chord, Uint32 color);
int aaArcRGBA(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, float start, float end, float thick, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int aaArcColor(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, float start, float end, float thick, Uint32 color);
int aaBezierRGBA(SDL_Renderer * renderer, double *x, double *y, int n, int s, float thick, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int aaBezierColor(SDL_Renderer * renderer, double *x, double *y, int n, int s, float thick, int color);
int aaFilledPolyBezierRGBA(SDL_Renderer * renderer, double *x, double *y, int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
int aaFilledPolyBezierColor(SDL_Renderer * renderer, double *x, double *y, int n, int s, int color);

#endif