/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"
#include	"disp.h"


int GzNewFrameBuffer(char** framebuffer, int width, int height)
{
	/* create a framebuffer:
	-- allocate memory for framebuffer : (sizeof)GzPixel x width x height
	-- pass back pointer
	*/
	*framebuffer = (char *)malloc(sizeof(GzPixel)* width * height);
	return GZ_SUCCESS;
}

int GzNewDisplay(GzDisplay	**display, GzDisplayClass dispClass, int xRes, int yRes)
{

	/* create a display:
	-- allocate memory for indicated class and resolution
	-- pass back pointer to GzDisplay object in display

	if (display == NULL){
	return GZ_FAILURE;
	}*/
	if (xRes < 0){
		xRes = 0;
	}
	if (yRes < 0){
		yRes = 0;
	}
	if (xRes > MAXXRES){
		xRes = MAXXRES;
	}
	if (yRes > MAXYRES){
		yRes = MAXYRES;
	}
	if (display != NULL){
		GzDisplay *display1 = (GzDisplay*)malloc(sizeof(GzDisplay));
		(display1)->xres = xRes;
		(display1)->yres = yRes;
		(display1)->dispClass = dispClass;
		(display1)->open = 0;
		//(display1)->fbuf = (GzPixel *)malloc(sizeof(GzPixel)* xRes * yRes);
		*display = display1;
		(*display)->fbuf = (GzPixel *)malloc(sizeof(GzPixel)* (*display)->xres * (*display)->yres);
		return GZ_SUCCESS;
	}
}
int GzFreeDisplay(GzDisplay	*display)
{
	/* clean up, free memory
	if (display == NULL){
	return GZ_FAILURE;
	}*/
	if (display != NULL){
		free(display);
		return GZ_SUCCESS;
	}
}


int GzGetDisplayParams(GzDisplay *display, int *xRes, int *yRes, GzDisplayClass	*dispClass)
{
	/* pass back values for an open display
	if (display == NULL){
	return GZ_FAILURE;
	}*/
	if (display != NULL){

		*xRes = display->xres;
		*yRes = display->yres;
		*dispClass = display->dispClass;
		return GZ_SUCCESS;
	}
}


int GzInitDisplay(GzDisplay	*display)
{
	/* set everything to some default values - start a new frame */
	if (display == NULL){
		return GZ_FAILURE;
	}
	//	(display)->fbuf = (GzPixel *)malloc(sizeof(GzPixel)* display->xres * display->yres);
	display->open = 1;
	if (display != NULL){
		for (int i = 0; i < display->xres; i++){
			for (int j = 0; j < display->yres; j++){

				display->fbuf[ARRAY(i, j)].red = 3095;
				display->fbuf[ARRAY(i, j)].green = 3095;
				display->fbuf[ARRAY(i, j)].blue = 3095;
				display->fbuf[ARRAY(i, j)].alpha = 1;
				display->fbuf[ARRAY(i, j)].z = 0;
			}
		}
		return GZ_SUCCESS;
	}
}


int GzPutDisplay(GzDisplay *display, int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* write pixel values into the display
	if (display == NULL){
	return GZ_FAILURE;
	}*/
	if (r < 0){
		r = 0;
	}
	if (g < 0){
		g = 0;
	}
	if (b < 0){
		b = 0;

	}
	if (r > 4095){
		r = 4095;
	}
	if (g > 4095){
		g = 4095;
	}
	if (b > 4095){
		b = 4095;

	}
	if (display != NULL){
		if ((i >= 0 && i <= display->xres) && (j >= 0 && j <= display->yres)){
			display->fbuf[ARRAY(i, j)].red = r;
			display->fbuf[ARRAY(i, j)].green = g;
			display->fbuf[ARRAY(i, j)].blue = b;
			display->fbuf[ARRAY(i, j)].alpha = a;
			display->fbuf[ARRAY(i, j)].z = z;
		}
		return GZ_SUCCESS;
	}
}


int GzGetDisplay(GzDisplay *display, int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
	/* pass back pixel value in the display */
	/* check display class to see what vars are valid
	if (display == NULL){
	return GZ_FAILURE;
	}*/
	if (display != NULL){
		if ((i >= 0 && i <= display->xres) && (j >= 0 && j <= display->yres)){
			*r = display->fbuf[ARRAY(i, j)].red;
			*g = display->fbuf[ARRAY(i, j)].green;
			*b = display->fbuf[ARRAY(i, j)].blue;
			*a = display->fbuf[ARRAY(i, j)].alpha;
			*z = display->fbuf[ARRAY(i, j)].z;
		}
		return GZ_SUCCESS;
	}
}


int GzFlushDisplay2File(FILE* outfile, GzDisplay *display)
{

	/* write pixels to ppm file based on display class -- "P6 %d %d 255\r"
	if (display == NULL){
	return GZ_FAILURE;
	}*/
	if (outfile == NULL){
		return GZ_FAILURE;
	}
	if (display != NULL){
		fprintf(outfile, "P6 %d %d 255\r", display->xres, display->yres);
		char a, b, c;
		for (int j = 0; j < display->xres; j++){
			for (int i = 0; i < display->yres; i++){
				a = display->fbuf[ARRAY(i, j)].red >> 4;
				b = display->fbuf[ARRAY(i, j)].green >> 4;
				c = display->fbuf[ARRAY(i, j)].blue >> 4;
				fprintf(outfile, "%c%c%c", a, b, c);
			}
		}
		return GZ_SUCCESS;
	}
}

int GzFlushDisplay2FrameBuffer(char* framebuffer, GzDisplay *display)
{

	/* write pixels to framebuffer:
	- Put the pixels into the frame buffer
	- Caution: store the pixel to the frame buffer as the order of blue, green, and red
	- Not red, green, and blue !!!

	if (display == NULL){
	return GZ_FAILURE;
	}*/
	if (framebuffer == NULL){
		return GZ_FAILURE;
	}
	char a, b, c;
	if (display != NULL){
		for (int j = 0; j < display->xres; j++){
			for (int i = 0; i < display->yres; i++){
				a = display->fbuf[ARRAY(i, j)].red >> 4;
				b = display->fbuf[ARRAY(i, j)].green >> 4;
				c = display->fbuf[ARRAY(i, j)].blue >> 4;
				framebuffer[(i + (j*display->xres)) * 3] = c;
				framebuffer[((i + (j*display->xres)) * 3) + 1] = b;
				framebuffer[((i + (j*display->xres)) * 3) + 2] = a;
			}
		}
		return GZ_SUCCESS;
	}
}