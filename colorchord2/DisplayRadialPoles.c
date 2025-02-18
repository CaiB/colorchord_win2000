//Copyright 2015 <>< Charles Lohr under the ColorChord License.

#include "outdrivers.h"
#include "notefinder.h"
#include <stdio.h>
#include "parameters.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "color.h"
#include "CNFG.h"

//Uses: note_amplitudes2[note] for how many lights to use.
//Uses: note_amplitudes_out[note] for how bright it should be.

extern short screenx, screeny;

struct DPRDriver
{
	float centeroffset;
	float radialscale;
	float polewidth;
	int radialmode;
};


static void DPRUpdate(void * id, struct NoteFinder*nf)
{
	struct DPRDriver * d = (struct DPRDriver*)id;
	if( d->radialmode == 0 )
	{
		int pole = 0;
		int freqbins = nf->freqbins;
		for( pole = 0; pole < freqbins; pole++ )
		{
			float angle = pole / (float)freqbins * 3.14159 * 2;
			float cx = screenx/2;
			float cy = screeny/2;
			float extentx, extenty, orthox, orthoy;
			float p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y;

			cx += cos( angle ) * d->centeroffset;
			cy += sin( angle ) * d->centeroffset;
			extentx = cx + cos(angle) * d->radialscale * nf->folded_bins[pole];
			extenty = cy + sin(angle) * d->radialscale * nf->folded_bins[pole];
			orthox = sin(angle) * d->polewidth;
			orthoy = -cos(angle) * d->polewidth;

			p1x = cx + orthox;
			p1y = cy + orthoy;
			p2x = cx - orthox;
			p2y = cy - orthoy;
			p3x = extentx + orthox;
			p3y = extenty + orthoy;
			p4x = extentx - orthox;
			p4y = extenty - orthoy;

			{
				RDPoint pts[6] = {
					{ p1x, p1y },
					{ p2x, p2y },
					{ p3x, p3y },
					{ p4x, p4y }, 
					{ p3x, p3y },
					{ p2x, p2y },
				};
				CNFGColor( CCtoHEX( (float)pole / (float)freqbins, 1.0, 1.0 ) );
				CNFGTackPoly( pts, 3 );
				CNFGTackPoly( pts+3, 3 );
			}


			CNFGColor( 0x000000ff );
			CNFGTackSegment( p1x, p1y, p2x, p2y);
			CNFGTackSegment( p2x, p2y, p4x, p4y);
			CNFGTackSegment( p3x, p3y, p1x, p1y);
			CNFGTackSegment( p4x, p4y, p3x, p3y);

		}
	}
	else if( d->radialmode == 1 )
	{
		int pole = 0;
		int polen = 0;
		int freqbins = nf->freqbins;
		for( pole = 0; pole < freqbins; pole++ )
		{
			float angleT, angleN, cx, cy, p1x, p1y, p2x, p2y, binval, binvalN, p3x, p3y, p4x, p4y;
			polen = (pole+1)%freqbins;

			angleT = pole / (float)freqbins * 3.14159 * 2;
			angleN = polen / (float)freqbins * 3.14159 * 2;
			cx = screenx/2;
			cy = screeny/2;

			p1x = cx + cos( angleT ) * d->centeroffset;
			p1y = cy + sin( angleT ) * d->centeroffset;

			p2x = cx + cos( angleN ) * d->centeroffset;
			p2y = cy + sin( angleN ) * d->centeroffset;

			binval = nf->folded_bins[pole];
			binvalN = nf->folded_bins[polen];

			p3x = cx + cos( angleT ) * (d->radialscale * binval + d->centeroffset);
			p3y = cy + sin( angleT ) * (d->radialscale * binval + d->centeroffset);

			p4x = cx + cos( angleN ) * (d->radialscale * binvalN + d->centeroffset);
			p4y = cy + sin( angleN ) * (d->radialscale * binvalN + d->centeroffset);

			{
				RDPoint pts[6] = {
					{ p1x, p1y },
					{ p2x, p2y },
					{ p3x, p3y },
					{ p4x, p4y }, 
					{ p3x, p3y },
					{ p2x, p2y },
				};
				CNFGColor( CCtoHEX( (float)pole / (float)freqbins, 1.0, 1.0 ) );
				CNFGTackPoly( pts, 3 );
				CNFGTackPoly( pts+3, 3 );
			}

			CNFGColor( 0x000000ff );
			CNFGTackSegment( p1x, p1y, p2x, p2y);
			CNFGTackSegment( p2x, p2y, p4x, p4y);
			CNFGTackSegment( p3x, p3y, p1x, p1y);
			CNFGTackSegment( p4x, p4y, p3x, p3y);
		}
	}



	CNFGColor( 0xffffffff );
}

static void DPRParams(void * id )
{
	struct DPRDriver * d = (struct DPRDriver*)id;

	d->centeroffset = 160;	RegisterValue(  "centeroffset", PAFLOAT, &d->centeroffset, sizeof( d->centeroffset ) );
	d->radialscale = 1000;	RegisterValue(  "radialscale", PAFLOAT, &d->radialscale, sizeof( d->radialscale ) );
	d->polewidth = 20;		RegisterValue(  "polewidth", PAFLOAT, &d->polewidth, sizeof( d->polewidth ) );
	d->radialmode = 0;		RegisterValue(  "radialmode", PAINT, &d->radialmode, sizeof( d->radialmode ) );
}

static struct DriverInstances * DisplayRadialPoles(const char * parameters)
{
	struct DriverInstances * ret = malloc( sizeof( struct DriverInstances ) );
	struct DPRDriver * d = ret->id = malloc( sizeof( struct DPRDriver ) );
	memset( d, 0, sizeof( struct DPRDriver ) );
	ret->Func = DPRUpdate;
	ret->Params = DPRParams;
	DPRParams( d );
	return ret;
}

REGISTER_OUT_DRIVER(DisplayRadialPoles);


