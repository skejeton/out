#include "bmath.h"

static u32 seed = 1;

void game_seed(int ns) 
{
    seed = (u32) ns & 0x7fffffffU;
}

int game_random() 
{
    seed = (seed * 1103515245U + 12345U) & 0x7fffffffU;
    return (int)seed;
}

/**
 * Quick square root function (can be replaced with regular one)
 */
double Q_sqrt( double number )
{	
	const double x2 = number * 0.5;
	const double threehalfs = 1.5;

	union {
		double f;
		u32 i;
	} conv = {number}; // member 'f' set to value of 'number'.
	conv.i  = 0x5f3759df - ( conv.i >> 1 );
	conv.f  *= ( threehalfs - ( x2 * conv.f * conv.f ) );
	return 1/conv.f;
}

double game_vector_magnitude(Game_Vec2 *vec)
{
    return Q_sqrt((vec->x*vec->x) + (vec->y*vec->y));
}

Game_Vec2* game_vector_normal(Game_Vec2 *vec)
{
    double m = game_vector_magnitude(vec);
    vec->x /= m;
    vec->y /= m;
    return vec;
}

void game_remap_to_camera(Game_Vec2 *orig, Game_Camera *cam)
{
    orig->x = orig->x + (game_screen.width/2) - cam->x;
    orig->y = orig->y + (game_screen.height/2) - cam->y;
}

bool game_check_collision(Game_Collider *c1, Game_Collider *c2)
{
    return (c2->r.x+c2->r.w) > (c1->r.x) &&
           (c2->r.y+c2->r.h) > (c1->r.y) &&
           (c2->r.x) < (c1->r.x+c1->r.w) &&
           (c2->r.y) < (c1->r.y+c1->r.h);
}
