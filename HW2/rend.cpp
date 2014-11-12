#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"


int GzNewRender(GzRender **render, GzRenderClass renderClass, GzDisplay *display)
{
	/*
	- malloc a renderer struct
	- keep closed until BeginRender inits are done
	- span interpolator needs pointer to display for pixel writes
	- check for legal class GZ_Z_BUFFER_RENDER
	*/
	if (renderClass != GZ_Z_BUFFER_RENDER)
	{
		return GZ_FAILURE;
	}
	GzRender *render1 = (GzRender *)malloc(sizeof(GzRender));
	*render = render1;
	(*render)->display = display;
	return GZ_SUCCESS;
}


int GzFreeRender(GzRender *render)
{
	/*
	-free all renderer resources
	*/
	if (render == NULL){
		return GZ_FAILURE;
	}
	free(render);
	return GZ_SUCCESS;
}


int GzBeginRender(GzRender	*render)
{
	/*
	- set up for start of each frame - init frame buffer
	*/
	if (render == NULL){
		return GZ_FAILURE;
	}
	render->open = 1;
	render->flatcolor[RED] = 0;
	render->flatcolor[GREEN] = 0;
	render->flatcolor[BLUE] = 0;
	return GZ_SUCCESS;
}


int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList,
	GzPointer *valueList) /* void** valuelist */
{
	/*
	- set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	- later set shaders, interpolaters, texture maps, and lights
	*/
	if (render == NULL){
		return GZ_FAILURE;
	}
	
	for (int i = 0; i < numAttributes; i++){
		if (nameList[i] == GZ_RGB_COLOR){
			GzColor *color = (GzColor *)valueList[0];
			if ((*color)[0] < 0){
				(*color)[0] = 0;
			}
			if ((*color)[1] < 0){
				(*color)[1] = 0;
			}
			if ((*color)[2] < 0){
				(*color)[2] = 0;
			}
			if ((*color)[0] > 4095){
				(*color)[0] = 4095;
			}
			if ((*color)[1] > 4095){
				(*color)[1] = 4095;
			}
			if ((*color)[2] > 4095){
				(*color)[2] = 4095;
			}
			render->flatcolor[RED] = (*color)[0];
			render->flatcolor[GREEN] = (*color)[1];
			render->flatcolor[BLUE] = (*color)[2];

		}
	}

	return GZ_SUCCESS;
}


int GzPutTriangle(GzRender *render, int	numParts, GzToken *nameList,
	GzPointer *valueList)
	/* numParts - how many names and values */
{
	/*
	- pass in a triangle description with tokens and values corresponding to
	GZ_NULL_TOKEN:		do nothing - no values
	GZ_POSITION:		3 vert positions in model space
	- Invoke the scan converter and return an error code
	*/
	GzCoord	vertexList[3];
	if (render == NULL){
		return GZ_FAILURE;
	}
	for (int i = 0; i < numParts; i++){
		if (nameList[i] == GZ_NULL_TOKEN){
			return GZ_FAILURE;
		}
		if (nameList[i] == GZ_POSITION){

			vertexList[0][0] = ((GzCoord*)(valueList[i]))[0][0];
			vertexList[0][1] = ((GzCoord*)(valueList[i]))[0][1];
			vertexList[0][2] = ((GzCoord*)(valueList[i]))[0][2];
			vertexList[1][0] = ((GzCoord*)(valueList[i]))[1][0];
			vertexList[1][1] = ((GzCoord*)(valueList[i]))[1][1];
			vertexList[1][2] = ((GzCoord*)(valueList[i]))[1][2];
			vertexList[2][0] = ((GzCoord*)(valueList[i]))[2][0];
			vertexList[2][1] = ((GzCoord*)(valueList[i]))[2][1];
			vertexList[2][2] = ((GzCoord*)(valueList[i]))[2][2];
			GzCoord x = { vertexList[0][0], vertexList[1][0], vertexList[2][0] };
			GzCoord y = { vertexList[0][1], vertexList[1][1], vertexList[2][1] };
			GzCoord z = { vertexList[0][2], vertexList[1][2], vertexList[2][2] };
			GzCoord tempx = { vertexList[0][0], vertexList[1][0], vertexList[2][0] };
			float y_min, y_max;
			sort_on_y(x, y, z);		//sort on y 
			y_min = y[0];			// find minimum y for boundary box
			y_max = y[2];			// find maximum y for boundary box
			float tmp;
			
			if ((y[1] == y[0])  || (x[1] < x[0]))
			{
					tmp = x[0];
					x[0] = x[1];
					x[1] = tmp;
					
					tmp = y[0];
					y[0] = y[1];
					y[1] = tmp;
					
					tmp = z[0];
					z[0] = z[1];
					z[1] = tmp;
				
			}
			
			else if ((y[1] == y[2]) || (x[1] > x[2])){ 
					tmp = x[1];
					x[1] = x[2];
					x[2] = tmp;

					tmp = y[1];
					y[1] = y[2];
					y[2] = tmp;
					
					tmp = z[1];
					z[1] = z[2];
					z[2] = tmp;
				
			}
			
				float coefA = (y[2] - y[0]);
				float coefB = -(x[2] - x[0]);
				float coefC = (-coefB*y[0]) - (coefA * x[0]);

				float xx1 = (-coefB*y[1] - coefC) / coefA;
				if (xx1 < x[1]){
					tmp = y[2];
					y[2] = y[1];
					y[1] = tmp;

					tmp = x[2];
					x[2] = x[1];
					x[1] = tmp;

					tmp = z[2];
					z[2] = z[1];
					z[1] = tmp;
				}

			
			// find minimum  and maximum x for boundary box
			float x_min, x_max;
			
			if (x[0] < x[1] && x[0] < x[2]){
						x_min = x[0];

					}
					else{
						if (x[1] < x[2] && x[1] < x[0]){
							x_min = x[1];
					}
					else{
						x_min = x[2];
					}
				}


				if (x[0] > x[1] && x[0] > x[2]){
					x_max = x[0];

				}
				else{
					if (x[1] > x[2] && x[1] > x[0]){
						x_max = x[1];
				}
				else{
					x_max = x[2];
				}
			}

				//check boundary conditions

				if (x_min < 0){
					x_min = 0;

				}
				if (x_min > 255){
					x_min = 255;

				}
				if (x_max < 0){
					x_max = 0;

				}
				if (x_max > 255){
					x_max = 255;

				}
				if (y_min < 0){
					y_min = 0;

				}
				if (y_min > 255){
					y_min = 255;

				}
				if (y_max < 0){
					y_max = 0;

				}
				if (y_max > 255){
					y_max = 255;

				}



				float coefA1 = (y[1] - y[0]);
				float coefA2 = (y[2] - y[1]);
				float coefA3 = (y[0] - y[2]);

				float coefB1 = -(x[1] - x[0]);
				float coefB2 = -(x[2] - x[1]);
				float coefB3 = -(x[0] - x[2]);

				float coefC1 = ((-(coefB1)*y[1]) - (coefA1 * x[1]));
				float coefC2 = ((-(coefB2)*y[2]) - (coefA2 * x[2]));
				float coefC3 = ((-(coefB3)*y[0]) - (coefA3 * x[0]));
				float d1, d2, d3;
				
				float A, B, C, D;
				float x11, x22, y11, y22, z11, z22;
				x11 = (x[1] - x[0]);
				y11 = (y[1] - y[0]);
				z11 = (z[1] - z[0]);
				x22 = (x[2] - x[0]);
				y22 = (y[2] - y[0]);
				z22 = (z[2] - z[0]);
				A = ((y11*z22) - (y22*z11));
				B = ((x22*z11) - (x11*z22));
				C = ((x11*y22) - (x22*y11));
				D = ((y[0] * (B)) + (x[0] * (A)) + (z[0] * (C))) * (-1);
				
				int z_inter = 0;
				
				for (int i = floor(x_min); i < ceil(x_max); i++){
					for (int j = floor(y_min); j < ceil(y_max); j++){
						 d1 = (coefA1*i) + (coefB1*j) + coefC1;
						 d2 = (coefA2*i) + (coefB2*j) + coefC2;
						 d3 = (coefA3*i) + (coefB3*j) + coefC3;
						 z_inter = -(A*i + B*j + D) / C;
						
						if (!(d1 < 0 || d2 < 0 || d3 < 0)){
							GzIntensity a, b, c, d;
							GzDepth z;
							GzGetDisplay(render->display, i, j, &a, &b, &c, &d, &z);
							if (z_inter < z || z == 0)
							{
								GzPutDisplay(render->display, i, j, ctoi(render->flatcolor[RED]), ctoi(render->flatcolor[GREEN]), ctoi(render->flatcolor[BLUE]), a, z_inter);
							}
						}		
					}
				}
			}
		}
		return GZ_SUCCESS;
	}


	void sort_on_y(GzCoord x, GzCoord y, GzCoord z){
		float temp111;
		for (int i = 0; i < 2; i++){
			for (int j = 0; j < 2-i; j++) {
				//Ascending order sorting
				if (y[j]> y[j + 1]) {
					temp111 = x[j];
					x[j] = x[j + 1];
					x[j + 1] = temp111;
					temp111 = y[j];
					y[j] = y[j + 1];
					y[j + 1] = temp111;
					temp111 = z[j];
					z[j] = z[j + 1];
					z[j + 1] = temp111;
				}
			}
		}
	}

/*
	void sort_on_x(GzCoord x, GzCoord y, GzCoord z){
		float tmp;
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < i; j++){
				//Ascending order sorting
				if (x[j] > x[j + 1]){
					tmp = x[j + 1];
					x[j + 1] = x[j];
					x[j] = tmp;
					tmp = y[j + 1];
					y[j + 1] = y[j];
					y[j] = tmp;
					tmp = z[j + 1];
					z[j + 1] = z[j];
					z[j] = tmp;
				}
			}
		}
	}
	*/

	/* NOT part of API - just for general assistance */

	short	ctoi(float color)		/* convert float color to GzIntensity short */
	{
		return(short)((int)(color * ((1 << 12) - 1)));
	}

