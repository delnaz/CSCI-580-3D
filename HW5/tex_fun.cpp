/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"
#include	"math.h"


GzColor	*image;
int xs, ys;
int reset = 1;

#define	ARRAY(x,y)	(x + (y * xs))
/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
  unsigned char		pixel[3];
  unsigned char     dummy;
  char  		foo[8];
  int   		i, j;
  FILE			*fd;

  if (reset) {          /* open and load texture file */
    fd = fopen ("texture", "rb");
    if (fd == NULL) {
      fprintf (stderr, "texture file not found\n");
      exit(-1);
    }
    fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
    image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
    if (image == NULL) {
      fprintf (stderr, "malloc for texture image failed\n");
      exit(-1);
    }

    for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
      fread(pixel, sizeof(pixel), 1, fd);
      image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
      image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
      image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
      }

    reset = 0;          /* init is done */
	fclose(fd);
  }

/* bounds-test u,v to make sure nothing will overflow image array bounds */
/* determine texture cell corner values and perform bilinear interpolation */
/* set color to interpolated GzColor value and return */

  if (u < 0){
	  u = 0;
  }

  if (v < 0){
	  v = 0;
  }

  if (u > 1){
	  u = 1;
  }
  if (v > 1){
	  v = 1;
  }

  u = u * (xs - 1);
  v = v * (ys - 1);

  int A, B, C, D;
  float s, t;
  A = floor(u);
  B = ceil(u);
  C = floor(v);
  D = ceil(v);
  s = u - A;
  t = v - C;
 
  color[RED] = ((s * t * image[ARRAY(B, D)][RED]) 
	  + ((1 - s) * (t)* image[ARRAY(A, D)][RED]) 
	  + ((s)* (1 - t) * image[ARRAY(B, C)][RED]) 
	  + ((1 - s) * (1 - t) * image[ARRAY(A, C)][RED]));


  color[GREEN] = ((s * t * image[ARRAY(B, D)][GREEN]) 
	  + ((1 - s) * (t)* image[ARRAY(A, D)][GREEN]) 
	  + ((s)* (1 - t) * image[ARRAY(B, C)][GREEN]) 
	  + ((1 - s) * (1 - t) * image[ARRAY(A, C)][GREEN]));

  color[BLUE] = ((s * t * image[ARRAY(B, D)][BLUE]) 
	  + ((1 - s) * (t)* image[ARRAY(A, D)][BLUE]) 
	  + ((s)* (1 - t) * image[ARRAY(B, C)][BLUE]) 
	  + ((1 - s) * (1 - t) * image[ARRAY(A, C)][BLUE]));
 
  return GZ_SUCCESS;
}


/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
	

	if (u < 0){
		u = 0;
	}

	if (v < 0){
		v = 0;
	}

	if (u > 1){
		u = 1;
	}
	if (v > 1){
		v = 1;
	}

	double r;
	GzColor spot_color = { 0.35, 0.53, 0.73 };
	double radius = 0.2;
	u = u * 0.8;
	v = v * 0.8;
	u = (u - floor(u)) - 0.5;
	v = (v - floor(v)) - 0.5;

	r = sqrt(u * u + v * v);

	for (int j = 0; j < 1000; j++){
		if (r < radius){
			color[X] = spot_color[0];
			color[Y] = spot_color[1];
			color[Z] = spot_color[2];
		}
	}
	// else
	for (int i = 0; i < 1000; i++){
		if (r < radius + 0.25){
			r = (r - radius) / 0.25;
			color[X] = color[X] * r + spot_color[0] * (1 - r);
			color[Y] = color[Y] * r + spot_color[1] * (1 - r);
			color[Z] = color[Z] * r + spot_color[2] * (1 - r);
		}
	}
	return GZ_SUCCESS;
	
}
