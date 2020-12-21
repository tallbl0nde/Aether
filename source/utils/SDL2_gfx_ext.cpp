// ======================================================================================= //
// Apart from this section of comments + includes, the contents of this file
// were written by 'rtrussell' on GitHub (under Zlib license). You can find the original
// source here: https://github.com/rtrussell/BBCSDL/blob/master/src/SDL2_gfxPrimitives.c
#include "Aether/utils/SDL2_gfx_ext.hpp"
#include <SDL2/SDL2_gfxPrimitives.h>
// ======================================================================================= //

/*!
\brief Internal function to calculate bezier interpolator of data array with ndata values at position 't'.
\param data Array of values.
\param ndata Size of array.
\param t Position for which to calculate interpolated value. t should be between [0, ndata].
\returns Interpolated value at position t, value[0] when t<0, value[n-1] when t>n.
*/
static double _evaluateBezier (double *data, int ndata, double t)
{
	double mu, result;
	int n,k,kn,nn,nkn;
	double blend,muk,munk;

	/* Sanity check bounds */
	if (t<0.0) {
		return(data[0]);
	}
	if (t>=(double)ndata) {
		return(data[ndata-1]);
	}

	/* Adjust t to the range 0.0 to 1.0 */
	mu=t/(double)ndata;

	/* Calculate interpolate */
	n=ndata-1;
	result=0.0;
	muk = 1;
	munk = pow(1-mu,(double)n);
	for (k=0;k<=n;k++) {
		nn = n;
		kn = k;
		nkn = n - k;
		blend = muk * munk;
		muk *= mu;
		munk /= (1-mu);
		while (nn >= 1) {
			blend *= nn;
			nn--;
			if (kn > 1) {
				blend /= (double)kn;
				kn--;
			}
			if (nkn > 1) {
				blend /= (double)nkn;
				nkn--;
			}
		}
		result += data[k] * blend;
	}

	return (result);
}

// Extensions for thick outline ellipses and arcs by Richard Russell 19-Feb-2019

// SDL_RenderDrawLine() is documented as including both end points, but this isn't
// reliable in Linux so use SDL_RenderDrawPoints() instead, despite being slower.
static int renderdrawline(SDL_Renderer *renderer, int x1, int y1, int x2, int y2)
{
	int result ;
	if ((x1 == x2) && (y1 == y2))
		result = SDL_RenderDrawPoint (renderer, x1, y1) ;
	else if ((y1 == y2) && (x1 != x2))
	    {
		int x ;
		if (x1 > x2) { x = x1 ; x1 = x2 ; x2 = x ; }
		SDL_Point *points = (SDL_Point*) malloc ((x2 - x1 + 1) * sizeof(SDL_Point)) ;
		if (points == NULL) return -1 ;
		for (x = x1; x <= x2; x++)
		    {
			points[x - x1].x = x ;
			points[x - x1].y = y1 ;
		    }
		result = SDL_RenderDrawPoints (renderer, points, x2 - x1 + 1) ;
		free (points) ;
	    }
	else if ((x1 == x2) && (y1 != y2))
	    {
		int y ;
		if (y1 > y2) { y = y1 ; y1 = y2 ; y2 = y ; }
		SDL_Point *points = (SDL_Point*) malloc ((y2 - y1 + 1) * sizeof(SDL_Point)) ;
		if (points == NULL) return -1 ;
		for (y = y1; y <= y2; y++)
		    {
			points[y - y1].x = x1 ;
			points[y - y1].y = y ;
		    }
		result = SDL_RenderDrawPoints (renderer, points, y2 - y1 + 1) ;
		free (points) ;
	    }
	else
		result = SDL_RenderDrawLine (renderer, x1, y1, x2, y2) ;
	return result ;
}

static int hlinecliparc(SDL_Renderer *renderer, int x1, int x2, int y, int xc, int yc, double s, double f)
{
	int result = 0 ;
	double a1, a2 ;
	a1 = atan2(y, x1) ;
	a2 = atan2(y, x2) ;
	if (a1 > a2)
	    {
		double a = a1 ; a1 = a2 ; a2 = a ;
		int x = x1 ; x1 = x2 ; x2 = x ;
	    }
	if (f < s)
	    {
		if ((a1 > f) && (a2 < s)) return result ;
		if ((a1 < s) && (a1 > f)) x1 = y / tan(s) ;
		if ((a2 > f) && (a2 < s)) x2 = y / tan(f) ;
		if ((a1 < f) && (a2 > s))
		    {
			result |= renderdrawline(renderer, x1+xc, y+yc, y/tan(f)+xc, y+yc) ;
			result |= renderdrawline(renderer, y/tan(s)+xc, y+yc, x2+xc, y+yc) ;
			return result ;
		    }
	    }
	else
	    {
		if ((a1 > f) || (a2 < s)) return result ;
		if (a1 < s) x1 = y / tan(s) ;
		if (a2 > f) x2 = y / tan(f) ;
	    }
	result |= renderdrawline(renderer, x1+xc, y+yc, x2+xc, y+yc) ;
	return result ;
}

/*!
\brief Draw thick ellipse with blending.
\param renderer The renderer to draw on.
\param xc X coordinate of the center of the ellipse.
\param yc Y coordinate of the center of the ellipse.
\param xr Horizontal radius in pixels of the ellipse.
\param yr Vertical radius in pixels of the ellipse.
\param r The red value of the ellipse to draw.
\param g The green value of the ellipse to draw.
\param b The blue value of the ellipse to draw.
\param a The alpha value of the ellipse to draw.
\param thick The line thickness in pixels
\returns Returns 0 on success, -1 on failure.
*/
int thickEllipseRGBA(SDL_Renderer * renderer, Sint16 xc, Sint16 yc, Sint16 xr, Sint16 yr, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick)
{
	int result = 0 ;
	int xi, yi, xo, yo, x, y, z ;
	double xi2, yi2, xo2, yo2 ;

	if (thick <= 1)
		return ellipseRGBA(renderer, xc, yc, xr, yr, r, g, b, a) ;

	xi = xr - thick / 2 ;
	xo = xi + thick - 1 ;
	yi = yr - thick / 2 ;
	yo = yi + thick - 1 ;

	if ((xi <= 0) || (yi <= 0))
		return -1 ;

	xi2 = xi * xi ;
	yi2 = yi * yi ;
	xo2 = xo * xo ;
	yo2 = yo * yo ;

	result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

	if (xr < yr)
	    {
		for (x = -xo; x <= -xi; x++)
		    {
			y = sqrt(yo2 * (1.0 - x*x/xo2)) + 0.5 ;
			result |= renderdrawline(renderer, xc+x, yc-y, xc+x, yc+y) ;
		    }
		for (x = -xi + 1; x <= xi - 1; x++)
		    {
			y = sqrt(yo2 * (1.0 - x*x/xo2)) + 0.5 ;
			z = sqrt(yi2 * (1.0 - x*x/xi2)) + 0.5 ;
			result |= renderdrawline(renderer, xc+x, yc+z, xc+x, yc+y) ;
			result |= renderdrawline(renderer, xc+x, yc-z, xc+x, yc-y) ;
		    }
		for (x = xo; x >= xi; x--)
		    {
			y = sqrt(yo2 * (1.0 - x*x/xo2)) + 0.5 ;
			result |= renderdrawline(renderer, xc+x, yc-y, xc+x, yc+y) ;
		    }
	    }
	else
	    {
		for (y = -yo; y <= -yi; y++)
		    {
			x = sqrt(xo2 * (1.0 - y*y/yo2)) + 0.5 ;
			result |= renderdrawline(renderer, xc-x, yc+y, xc+x, yc+y) ;
		    }
		for (y = -yi + 1; y <= yi - 1; y++)
		    {
			x = sqrt(xo2 * (1.0 - y*y/yo2)) + 0.5 ;
			z = sqrt(xi2 * (1.0 - y*y/yi2)) + 0.5 ;
			result |= renderdrawline(renderer, xc+z, yc+y, xc+x, yc+y) ;
			result |= renderdrawline(renderer, xc-z, yc+y, xc-x, yc+y) ;
		    }
		for (y = yo; y >= yi; y--)
		    {
			x = sqrt(xo2 * (1.0 - y*y/yo2)) + 0.5 ;
			result |= renderdrawline(renderer, xc-x, yc+y, xc+x, yc+y) ;
		    }
	    }
	return result ;
}

/*!
\brief thick Arc with blending.
\param renderer The renderer to draw on.
\param xc X coordinate of the center of the arc.
\param yc Y coordinate of the center of the arc.
\param rad Radius in pixels of the arc.
\param start Starting radius in degrees of the arc. 0 degrees is right, increasing clockwise.
\param end Ending radius in degrees of the arc. 0 degrees is right, increasing clockwise.
\param r The red value of the arc to draw.
\param g The green value of the arc to draw.
\param b The blue value of the arc to draw.
\param a The alpha value of the arc to draw.
\param thick The line thickness in pixels.
\returns Returns 0 on success, -1 on failure.
*/
int thickArcRGBA(SDL_Renderer * renderer, Sint16 xc, Sint16 yc, Sint16 rad, Sint16 start, Sint16 end, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick)
{
	int result = 0 ;
	int ri, ro, x, y, z ;
	double ri2, ro2, s, f ;

	if (thick <= 1)
		return arcRGBA(renderer, xc, yc, rad, start, end, r, g, b, a) ;

	while (start < -180) start += 360 ;
	while (start >= 180) start -= 360 ;
	while (end < -180) end += 360 ;
	while (end >= 180) end -= 360 ;
	s = M_PI * (double)start / 180.0 ;
	f = M_PI * (double)end / 180.0 ;
	if (start == end) return 0 ;

	ri = rad - thick / 2 ;
	ro = ri + thick - 1 ;
	if (ri <= 0) return -1 ;

	ri2 = ri * ri ;
	ro2 = ro * ro ;

	result |= SDL_SetRenderDrawBlendMode(renderer, (a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
	result |= SDL_SetRenderDrawColor(renderer, r, g, b, a);

	for (y = -ro; y <= -ri; y++)
	    {
		x = sqrt(ro2 * (1.0 - y*y/ro2)) + 0.5 ;
		result |= hlinecliparc(renderer, -x, x, y, xc, yc, s, f) ;
	    }
	for (y = -ri + 1; y <= ri - 1; y++)
	    {
		x = sqrt(ro2 * (1.0 - y*y/ro2)) + 0.5 ;
		z = sqrt(ri2 * (1.0 - y*y/ri2)) + 0.5 ;
		result |= hlinecliparc(renderer, z, x, y, xc, yc, s, f) ;
		result |= hlinecliparc(renderer, -z, -x, y, xc, yc, s, f) ;
	    }
	for (y = ro; y >= ri; y--)
	    {
		x = sqrt(ro2 * (1.0 - y*y/ro2)) + 0.5 ;
		result |= hlinecliparc(renderer, -x, x, y, xc, yc, s, f) ;
	    }
	return result ;
}

// returns Returns 0 on success, -1 on failure.
int thickCircleRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thick)
{
	return thickEllipseRGBA(renderer, x, y, rad, rad, r, g, b, a, thick);
}

// returns Returns 0 on success, -1 on failure.
int thickEllipseColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color, Uint8 thick)
{
	Uint8 *c = (Uint8 *)&color;
	return thickEllipseRGBA(renderer, x, y, rx, ry, c[0], c[1], c[2], c[3], thick);
}

// returns Returns 0 on success, -1 on failure.
int thickArcColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Sint16 start, Sint16 end, Uint32 color, Uint8 thick)
{
	Uint8 *c = (Uint8 *)&color;
	return thickArcRGBA(renderer, x, y, rad, start, end, c[0], c[1], c[2], c[3], thick);
}

// returns Returns 0 on success, -1 on failure.
int thickCircleColor(SDL_Renderer * renderer, Sint16 x, Sint16 y, Sint16 rad, Uint32 color, Uint8 thick)
{
	Uint8 *c = (Uint8 *)&color;
	return thickEllipseRGBA(renderer, x, y, rad, rad, c[0], c[1], c[2], c[3], thick);
}

// Extensions for anti-aliased filled ellipses and polygons by Richard Russell 20-Aug-2019

/*!
\brief Draw anti-aliased filled ellipse with blending.
\param renderer The renderer to draw on.
\param cx X coordinate of the center of the filled ellipse.
\param cy Y coordinate of the center of the filled ellipse.
\param rx Horizontal radius in pixels of the filled ellipse.
\param ry Vertical radius in pixels of the filled ellipse.
\param r The red value of the filled ellipse to draw.
\param g The green value of the filled ellipse to draw.
\param b The blue value of the filled ellipse to draw.
\param a The alpha value of the filled ellipse to draw.
\returns Returns 0 on success, -1 on failure.
*/
int aaFilledEllipseRGBA(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	int n, xi, yi, result = 0 ;
	double s, v, x, y, dx, dy ;

	if ((rx <= 0.0) || (ry <= 0.0))
		return -1 ;

	result |= SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) ;
	if (rx >= ry)
	    {
		n = ry + 1 ;
		for (yi = cy - n - 1; yi <= cy + n + 1; yi++)
		    {
			if (yi < (cy - 0.5))
				y = yi ;
			else
				y = yi + 1 ;
			s = (y - cy) / ry ;
			s = s * s ;
			x = 0.5 ;
			if (s < 1.0)
			    {
				x = rx * sqrt(1.0 - s) ;
				if (x >= 0.5)
				    {
					result |= SDL_SetRenderDrawColor (renderer, r, g, b, a ) ;
					result |= renderdrawline (renderer, cx - x + 1, yi, cx + x - 1, yi) ;
				    }
			    }
			s = 8 * ry * ry ;
			dy = fabs(y - cy) - 1.0 ;
			xi = cx - x ; // left
			while (1)
			    {
				dx = (cx - xi - 1) * ry / rx ;
				v = s - 4 * (dx - dy) * (dx - dy) ;
				if (v < 0) break ;
				v = (sqrt(v) - 2 * (dx + dy)) / 4 ;
				if (v < 0) break ;
				if (v > 1.0) v = 1.0 ;
				result |= SDL_SetRenderDrawColor (renderer, r, g, b, (double)a * v) ;
				result |= SDL_RenderDrawPoint (renderer, xi, yi) ;
				xi -= 1 ;
			    }
			xi = cx + x ; // right
			while (1)
			    {
				dx = (xi - cx) * ry / rx ;
				v = s - 4 * (dx - dy) * (dx - dy) ;
				if (v < 0) break ;
				v = (sqrt(v) - 2 * (dx + dy)) / 4 ;
				if (v < 0) break ;
				if (v > 1.0) v = 1.0 ;
				result |= SDL_SetRenderDrawColor (renderer, r, g, b, (double)a * v) ;
				result |= SDL_RenderDrawPoint (renderer, xi, yi) ;
				xi += 1 ;
			    }
		    }
	    }
	else
	    {
		n = rx + 1 ;
		for (xi = cx - n - 1; xi <= cx + n + 1; xi++)
		    {
			if (xi < (cx - 0.5))
				x = xi ;
			else
				x = xi + 1 ;
			s = (x - cx) / rx ;
			s = s * s ;
			y = 0.5 ;
			if (s < 1.0)
			    {
				y = ry * sqrt(1.0 - s) ;
				if (y >= 0.5)
				    {
					result |= SDL_SetRenderDrawColor (renderer, r, g, b, a ) ;
					result |= renderdrawline (renderer, xi, cy - y + 1, xi, cy + y - 1) ;
				    }
			    }
			s = 8 * rx * rx ;
			dx = fabs(x - cx) - 1.0 ;
			yi = cy - y ; // top
			while (1)
			    {
				dy = (cy - yi - 1) * rx / ry ;
				v = s - 4 * (dy - dx) * (dy - dx) ;
				if (v < 0) break ;
				v = (sqrt(v) - 2 * (dy + dx)) / 4 ;
				if (v < 0) break ;
				if (v > 1.0) v = 1.0 ;
				result |= SDL_SetRenderDrawColor (renderer, r, g, b, (double)a * v) ;
				result |= SDL_RenderDrawPoint (renderer, xi, yi) ;
				yi -= 1 ;
			    }
			yi = cy + y ; // bottom
			while (1)
			    {
				dy = (yi - cy) * rx / ry ;
				v = s - 4 * (dy - dx) * (dy - dx) ;
				if (v < 0) break ;
				v = (sqrt(v) - 2 * (dy + dx)) / 4 ;
				if (v < 0) break ;
				if (v > 1.0) v = 1.0 ;
				result |= SDL_SetRenderDrawColor (renderer, r, g, b, (double)a * v) ;
				result |= SDL_RenderDrawPoint (renderer, xi, yi) ;
				yi += 1 ;
			    }
		    }
	    }
	return result ;
}

// returns Returns 0 on success, -1 on failure.
int aaFilledEllipseColor(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, Uint32 color)
{
	Uint8 *c = (Uint8 *)&color;
	return aaFilledEllipseRGBA(renderer, cx, cy, rx, ry, c[0], c[1], c[2], c[3]);
}

static int _gfxPrimitivesCompareFloat2(const void *a, const void *b)
{
	float diff = *(float *)(a + sizeof(float)) - *(float *)(b + sizeof(float)) ;
	if (diff != 0.0) return (diff > 0) - (diff < 0) ;
	diff = *(float *)a - *(float *)b ;
	return (diff > 0) - (diff < 0) ;
}

// This constant determines the maximum size and/or complexity of polygon that can be
// drawn. Set to 16K the maximum aaArc height is approximately 1100 lines.
#define POLYSIZE 16384

/*!
\brief Draw anti-aliased filled polygon with alpha blending.
\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the filled polygon.
\param vy Vertex array containing Y coordinates of the points of the filled polygon.
\param n Number of points in the vertex array. Minimum number is 3.
\param r The red value of the filled polygon to draw.
\param g The green value of the filled polygon to draw.
\param b The blue value of the filed polygon to draw.
\param a The alpha value of the filled polygon to draw.
\returns Returns 0 on success, -1 on failure, or -2 if the polygon is too large and/or complex.
*/
int aaFilledPolygonRGBA(SDL_Renderer * renderer, const double * vx, const double * vy, int n, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	int i, j, xi, yi, result ;
	double x1, x2, y0, y1, y2, minx, maxx, prec ;
	float *list, *strip ;

	if (n < 3)
		return -1 ;

	result = SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) ;

	// Find extrema:
	minx = 99999.0 ;
	maxx = -99999.0 ;
	prec = 0.00001 ;
	for (i = 0; i < n; i++)
	    {
		double x = vx[i] ;
		double y = fabs(vy[i]) ;
		if (x < minx) minx = x ;
		if (x > maxx) maxx = x ;
		if (y > prec) prec = y ;
	    }
	minx = floor (minx) ;
	maxx = floor (maxx) ;
	prec = floor (pow(2,19) / prec) ;

	// Allocate main array, this determines the maximum polygon size and complexity:
	list = (float *) malloc (POLYSIZE * sizeof(float)) ;
	if (list == NULL)
		return -2 ;

	// Build vertex list.  Special x-values used to indicate vertex type:
	// x = -100001.0 indicates /\, x = -100003.0 indicates \/, x = -100002.0 neither
	yi = 0 ;
	y0 = floor(vy[n - 1] * prec) / prec ;
	y1 = floor(vy[0] * prec) / prec ;
	for (i = 1; i <= n; i++)
	    {
		if (yi > POLYSIZE - 4)
		    {
			free (list) ;
			return -2 ;
		    }
		y2 = floor(vy[i % n] * prec) / prec ;
		if (((y1 < y2) - (y1 > y2)) == ((y0 < y1) - (y0 > y1)))
		    {
			list[yi++] = -100002.0 ;
			list[yi++] = y1 ;
			list[yi++] = -100002.0 ;
			list[yi++] = y1 ;
		    }
		else
		    {
			if (y0 != y1)
			    {
				list[yi++] = (y1 < y0) - (y1 > y0) - 100002.0 ;
				list[yi++] = y1 ;
			    }
			if (y1 != y2)
			    {
				list[yi++] = (y1 < y2) - (y1 > y2) - 100002.0 ;
				list[yi++] = y1 ;
			    }
		    }
		y0 = y1 ;
		y1 = y2 ;
	    }
	xi = yi ;

	// Sort vertex list:
	qsort (list, yi / 2, sizeof(float) * 2, _gfxPrimitivesCompareFloat2) ;

	// Append line list to vertex list:
	for (i = 1; i <= n; i++)
	    {
		double x, y ;
		double d = 0.5 / prec ;

		x1 = vx[i - 1] ;
		y1 = floor(vy[i - 1] * prec) / prec ;
		x2 = vx[i % n] ;
		y2 = floor(vy[i % n] * prec) / prec ;

		if (y2 < y1)
		    {
			double tmp ;
			tmp = x1 ; x1 = x2 ; x2 = tmp ;
			tmp = y1 ; y1 = y2 ; y2 = tmp ;
		    }
		if (y2 != y1)
			y0 = (x2 - x1) / (y2 - y1) ;

		for (j = 1; j < xi; j += 4)
		    {
			y = list[j] ;
			if (((y + d) <= y1) || (y == list[j + 4]))
				continue ;
			if ((y -= d) >= y2)
				break ;
			if (yi > POLYSIZE - 4)
			    {
				free (list) ;
				return -2 ;
			    }
			if (y > y1)
			    {
				list[yi++] = x1 + y0 * (y - y1) ;
				list[yi++] = y ;
			    }
			y += d * 2.0 ;
			if (y < y2)
			    {
				list[yi++] = x1 + y0 * (y - y1) ;
				list[yi++] = y ;
			    }
		    }

		y = floor(y1) + 1.0 ;
		while (y <= y2)
		    {
			x = x1 + y0 * (y - y1) ;
			if (yi > POLYSIZE - 2)
			    {
				free (list) ;
				return -2 ;
			    }
			list[yi++] = x ;
			list[yi++] = y ;
			y += 1.0 ;
		    }
	    }

	// Sort combined list:
	qsort (list, yi / 2, sizeof(float) * 2, _gfxPrimitivesCompareFloat2) ;

	// Plot lines:
	strip = (float *) malloc ((maxx - minx + 2) * sizeof(float)) ;
	if (strip == NULL)
	    {
		free (list) ;
		return -1 ;
	    }
	memset (strip, 0, (maxx - minx + 1) * sizeof(float)) ;
	n = yi ;
	yi = list[1] ;
	j = 0 ;

	for (i = 0; i < n - 7; i += 4)
	    {
		float x1 = list[i + 0] ;
		float y1 = list[i + 1] ;
		float x3 = list[i + 2] ;
		float x2 = list[i + j + 0] ;
		float y2 = list[i + j + 1] ;
		float x4 = list[i + j + 2] ;

		if (x1 + x3 == -200002.0)
			j += 4 ;
		else if (x1 + x3 == -200006.0)
			j -= 4 ;
		else if ((x1 >= minx) && (x2 >= minx))
		    {
			if (x1 > x2) { float tmp = x1 ; x1 = x2 ; x2 = tmp ; }
			if (x3 > x4) { float tmp = x3 ; x3 = x4 ; x4 = tmp ; }

			for ( xi = x1 - minx; xi <= x4 - minx; xi++ )
			    {
				float u, v ;
				float x = minx + xi ;
				if (x < x2)  u = (x - x1 + 1) / (x2 - x1 + 1) ; else u = 1.0 ;
				if (x >= x3 - 1) v = (x4 - x) / (x4 - x3 + 1) ; else v = 1.0 ;
				if ((u > 0.0) && (v > 0.0))
					strip[xi] += (y2 - y1) * (u + v - 1.0) ;
			    }
		    }

		if ((yi == (list[i + 5] - 1.0)) || (i == n - 8))
		    {
			for (xi = 0; xi <= maxx - minx; xi++)
			    {
				if (strip[xi] != 0.0)
				    {
					if (strip[xi] >= 0.996)
					    {
						int x0 = xi ;
						while (strip[++xi] >= 0.996) ;
						xi-- ;
						result |= SDL_SetRenderDrawColor (renderer, r, g, b, a) ;
						result |= renderdrawline (renderer, minx + x0, yi, minx + xi, yi) ;
					    }
					else
					    {
						result |= SDL_SetRenderDrawColor (renderer, r, g, b, a * strip[xi]) ;
						result |= SDL_RenderDrawPoint (renderer, minx + xi, yi) ;
					    }
				    }
			    }
			memset (strip, 0, (maxx - minx + 1) * sizeof(float)) ;
			yi++ ;

		    }
	    }

	// Free arrays:
	free (list) ;
	free (strip) ;
	return result ;
}

// returns Returns 0 on success, -1 on failure.
int aaFilledPolygonColor(SDL_Renderer * renderer, const double * vx, const double * vy, int n, Uint32 color)
{
	Uint8 *c = (Uint8 *)&color;
	return aaFilledPolygonRGBA(renderer, vx, vy, n, c[0], c[1], c[2], c[3]);
}

/*!
\brief Draw anti-aliased filled ellipical pie (or chord) with alpha blending.
\param renderer The renderer to draw on.
\param cx X coordinate of the center of the filled pie.
\param cy Y coordinate of the center of the filled pie.
\param rx Horizontal radius in pixels of the filled pie.
\param ry Vertical radius in pixels of the filled pie.
\param start Starting angle in degrees of the filled pie; zero is right, increasing clockwise.
\param end Ending angle in degrees of the filled pie; zero is right, increasing clockwise.
\param chord Set to 0 for a pie (sector) or 1 for a chord (segment).
\param r The red value of the filled pie to draw.
\param g The green value of the filled pie to draw.
\param b The blue value of the filled pie to draw.
\param a The alpha value of the filled pie to draw.
/
\returns Returns 0 on success, -1 on failure.
*/
int aaFilledPieRGBA(SDL_Renderer * renderer, float cx, float cy, float rx, float ry,
	float start, float end, Uint32 chord, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	int nverts, i, result;
	double *vx, *vy;

	// Sanity check radii
	if ((rx <= 0) || (ry <= 0) || (start == end))
		return -1;

	// Convert degrees to radians
	start = fmod(start, 360.0) * 2.0 * M_PI / 360.0 ;
	end = fmod(end, 360.0) * 2.0 * M_PI / 360.0 ;
	while (start >= end)
		end += 2.0 * M_PI ;

	// Calculate number of vertices on perimeter
	nverts = (end - start) * sqrt(rx * ry) / M_PI ;
	if (nverts < 6)
		nverts = 6 ;
	if (nverts > 180)
		nverts = 180 ;

	// Allocate combined vertex array
	vx = vy = (double *) malloc(2 * sizeof(double) * (nverts + 1));
	if (vx == NULL)
		return (-1);

	// Update pointer to start of vy
	vy += nverts + 1 ;

	// Calculate vertices:
	for (i = 0; i < nverts; i++)
	    {
		double angle = start + (end - start) * (double) i / (double) (nverts - 1) ;
		vx[i] = cx + rx * cos(angle);
		vy[i] = cy + ry * sin(angle);
	    }

	// Center:
	vx[i] = cx ;
	vy[i] = cy ;

	result = aaFilledPolygonRGBA(renderer, vx, vy, nverts + 1 - (chord != 0), r, g, b, a);

	// Free combined vertex array
	free(vx);

	return (result);
}

// returns Returns 0 on success, -1 on failure.
int aaFilledPieColor(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, float start, float end, Uint32 chord, Uint32 color)
{
	Uint8 *c = (Uint8 *)&color;
	return aaFilledPieRGBA(renderer, cx, cy, rx, ry, start, end, chord, c[0], c[1], c[2], c[3]);
}

/*!
\brief Draw anti-aliased ellipical arc with alpha blending.
\param renderer The renderer to draw on.
\param cx X coordinate of the center of the filled pie.
\param cy Y coordinate of the center of the filled pie.
\param rx Horizontal radius in pixels of the filled pie.
\param ry Vertical radius in pixels of the filled pie.
\param start Starting angle in degrees of the filled pie; zero is right, increasing clockwise.
\param end Ending angle in degrees of the filled pie; zero is right, increasing clockwise.
\param thick The thickness of the line in pixels.
\param r The red value of the filled pie to draw.
\param g The green value of the filled pie to draw.
\param b The blue value of the filled pie to draw.
\param a The alpha value of the filled pie to draw.
/
\returns Returns 0 on success, -1 on failure.
*/
int aaArcRGBA(SDL_Renderer * renderer, float cx, float cy, float rx, float ry,
	float start, float end, float thick, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	int nverts, i, result;
	double *vx, *vy;

	// Sanity check radii and thickness
	if ((rx <= 0) || (ry <= 0) || (start == end) || (thick <= 0))
		return -1;

	// Convert degrees to radians
	start = fmod(start, 360.0) * 2.0 * M_PI / 360.0 ;
	end = fmod(end, 360.0) * 2.0 * M_PI / 360.0 ;
	while (start >= end)
		end += 2.0 * M_PI ;

	// Calculate number of vertices
	nverts = 6 * floor((end - start) * sqrt(rx * ry) / M_PI) ;
	if (nverts < 6)
		nverts = 6 ;
	if (nverts > 360)
		nverts = 360 ;

	// Allocate combined vertex array
	vx = vy = (double *) malloc(2 * sizeof(double) * nverts);
	if (vx == NULL)
		return (-1);

	// Update pointer to start of vy
	vy += nverts ;

	// Calculate vertices:
	for (i = 0; i < nverts / 2; i++)
	    {
		double angle = start + (end - start) * (double) i / (double) (nverts / 2 - 1) ;
		vx[i] = cx + (rx + thick/2) * cos(angle);
		vy[i] = cy + (ry + thick/2) * sin(angle);
		vx[nverts - 1 - i] = cx + (rx - thick/2) * cos(angle);
		vy[nverts - 1 - i] = cy + (ry - thick/2) * sin(angle);

	    }

	result = aaFilledPolygonRGBA(renderer, vx, vy, nverts, r, g, b, a);

	// Free combined vertex array
	free(vx);

	return (result);
}

// returns Returns 0 on success, -1 on failure.
int aaArcColor(SDL_Renderer * renderer, float cx, float cy, float rx, float ry, float start, float end, float thick, Uint32 color)
{
	Uint8 *c = (Uint8 *)&color;
	return aaArcRGBA(renderer, cx, cy, rx, ry, start, end, thick, c[0], c[1], c[2], c[3]);
}

/*!
\brief Draw an anti-aliased bezier curve with alpha blending.
\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the bezier curve.
\param vy Vertex array containing Y coordinates of the points of the bezier curve.
\param n Number of points in the vertex array. Minimum number is 3.
\param s Number of steps for the interpolation. Minimum number is 2.
\param thick Thickness of line in pixels.
\param r The red value of the bezier curve to draw.
\param g The green value of the bezier curve to draw.
\param b The blue value of the bezier curve to draw.
\param a The alpha value of the bezier curve to draw.
\returns Returns 0 on success, -1 on failure.
*/
int aaBezierRGBA(SDL_Renderer * renderer, double *x, double *y, int n, int s, float thick, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	int i, nverts, result;
	double d, t, stepsize;
	double x1, y1, x2, y2, dx = 0.0, dy = 0.0 ;
	double *vx, *vy ;

	// Sanity check
	if ((n < 3) || (s < 2))
		return -1 ;

	// Create combined vertex array:
	nverts = n * s * 2 + 2 ;
	vx = (double *) malloc (nverts * 2 * sizeof(double)) ;
	if (vx == NULL)
		return -1 ;
	vy = vx + nverts ;

	// Draw Bezier
	t = 0.0 ;
	stepsize = 1.0 / (double)s ;
	x1 = _evaluateBezier(x, n, t) ;
	y1 = _evaluateBezier(y, n, t) ;
	for (i = 0; i < n*s; i++)
	    {
		t += stepsize ;
		x2 = _evaluateBezier(x, n, t) ;
		y2 = _evaluateBezier(y, n, t) ;

		dx = x2 - x1 ;
		dy = y2 - y1 ;
		d = thick * 0.5L / sqrt(dx*dx + dy*dy) ;
		dx *= d ;
		dy *= d ;

		vx[i] = x1 + dy ;
		vy[i] = y1 - dx ;
		vx[nverts-1-i] = x1 - dy ;
		vy[nverts-1-i] = y1 + dx ;

		x1 = x2 ;
		y1 = y2 ;
	    }

	vx[i] = x1 + dy ;
	vy[i] = y1 - dx ;
	vx[nverts-1-i] = x1 - dy ;
	vy[nverts-1-i] = y1 + dx ;

	result = aaFilledPolygonRGBA(renderer, vx, vy, nverts, r, g, b, a);

	free (vx) ;
	return (result);
}

// returns Returns 0 on success, -1 on failure.
int aaBezierColor(SDL_Renderer * renderer, double *x, double *y, int n, int s, float thick, int color)
{
	Uint8 *c = (Uint8 *)&color;
	return aaBezierRGBA(renderer, x, y, n, s, thick, c[0], c[1], c[2], c[3]);
}

/*!
\brief Fill a region bounded by cubic Bezier curves, with alpha blending.
\param renderer The renderer to draw on.
\param vx Vertex array containing X coordinates of the points of the bezier curves.
\param vy Vertex array containing Y coordinates of the points of the bezier curves.
\param n Number of points in the vertex array. Should be 3n + 1 for n bezier curves.
\param s Number of steps for the interpolation. Minimum number is 2.
\param r The red value of the bezier curve to draw.
\param g The green value of the bezier curve to draw.
\param b The blue value of the bezier curve to draw.
\param a The alpha value of the bezier curve to draw.
\returns Returns 0 on success, -1 on failure.
*/
int aaFilledPolyBezierRGBA(SDL_Renderer * renderer, double *x, double *y, int n, int s, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	int i, j, nbeziers, nverts, result;
	double t, stepsize;
	double x1, y1, x2, y2 ;
	double *vx, *vy ;

	// Sanity check
	if ((n < 7) || (s < 2))
		return -1 ;

	// Create combined vertex array:
	nbeziers = (n - 1) / 3 ;
	nverts = nbeziers * 4 * s + 1 ;
	vx = (double *) malloc (nverts * 2 * sizeof(double)) ;
	if (vx == NULL)
		return -1 ;
	vy = vx + nverts ;

	// Draw Beziers
	stepsize = 1.0 / (double)s ;
	for (j = 0; j < nbeziers; j++)
	    {
		t = 0.0 ;
		x1 = _evaluateBezier(x + j * 3, 4, t) ;
		y1 = _evaluateBezier(y + j * 3, 4, t) ;
		for (i = 0; i < 4*s; i++)
		    {
			t += stepsize ;
			x2 = _evaluateBezier(x + j * 3, 4, t) ;
			y2 = _evaluateBezier(y + j * 3, 4, t) ;

			vx[i + j * s * 4] = x1 ;
			vy[i + j * s * 4] = y1 ;

			x1 = x2 ;
			y1 = y2 ;
	    }
	}

	vx[j * s * 4] = x1 ;
	vy[j * s * 4] = y1 ;

	result = aaFilledPolygonRGBA(renderer, vx, vy, nverts, r, g, b, a);

	free (vx) ;
	return (result);
}

// returns Returns 0 on success, -1 on failure.
int aaFilledPolyBezierColor(SDL_Renderer * renderer, double *x, double *y, int n, int s, int color)
{
	Uint8 *c = (Uint8 *)&color;
	return aaFilledPolyBezierRGBA(renderer, x, y, n, s, c[0], c[1], c[2], c[3]);
}