/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"


int GzRotXMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along x axis
// Pass back the matrix using mat value
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (i == j)
			{
				mat[i][j] = 1;
			}
			else
				mat[i][j] = 0;
		}
	}

	mat[0][0] = 1;
	mat[1][1] = cos(degree *(3.14159265 / 180));
	mat[1][2] = -sin(degree *(3.14159265 / 180));
	mat[2][1] = sin(degree *(3.14159265 / 180));
	mat[2][2] = cos(degree *(3.14159265 / 180));

	return GZ_SUCCESS;
}


int GzRotYMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along y axis
// Pass back the matrix using mat value
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (i == j)
			{
				mat[i][j] = 1;
			}
			else
				mat[i][j] = 0;
		}
	}

	mat[0][0] = cos(degree *(3.14159265 / 180));
	mat[0][2] = sin(degree *(3.14159265 / 180));
	mat[1][1] = 1;
	mat[2][0] = -sin(degree *(3.14159265 / 180));
	mat[2][2] = cos(degree *(3.14159265 / 180));
	return GZ_SUCCESS;
}


int GzRotZMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along z axis
// Pass back the matrix using mat value
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (i == j)
			{
				mat[i][j] = 1;
			}
			else
				mat[i][j] = 0;
		}
	}

	mat[0][0] = cos(degree *(3.14159265 / 180));
	mat[0][1] = -sin(degree *(3.14159265 / 180));
	mat[1][0] = sin(degree *(3.14159265 / 180));
	mat[1][1] = cos(degree *(3.14159265 / 180));
	mat[2][2] = 1;
	return GZ_SUCCESS;
}


int GzTrxMat(GzCoord translate, GzMatrix mat)
{
// Create translation matrix
// Pass back the matrix using mat value
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (i == j)
			{
				mat[i][j] = 1;
			}
			else
				mat[i][j] = 0;
		}
	}

	mat[0][3] = translate[X];
	mat[1][3] = translate[Y];
	mat[2][3] = translate[Z];
	return GZ_SUCCESS;
}


int GzScaleMat(GzCoord scale, GzMatrix mat)
{
// Create scaling matrix
// Pass back the matrix using mat value

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (i == j)
			{
				mat[i][j] = 1;
			}
			else
				mat[i][j] = 0;
		}
	}

	mat[0][0] = scale[X];
	mat[1][1] = scale[Y];
	mat[2][2] = scale[Z];

	return GZ_SUCCESS;
}


//----------------------------------------------------------
// Begin main functions

int GzNewRender(GzRender **render, GzRenderClass renderClass, GzDisplay	*display)
{
/*  
- malloc a renderer struct 
- keep closed until all inits are done 
- setup Xsp and anything only done once 
- span interpolator needs pointer to display 
- check for legal class GZ_Z_BUFFER_RENDER 
- init default camera 
*/ 
	if (renderClass != GZ_Z_BUFFER_RENDER)
	{
		return GZ_FAILURE;
	}

	if (render == NULL){
		return GZ_FAILURE;
	}
	if (display == NULL){
		return GZ_FAILURE;
	}
	GzRender *render1 = (GzRender*)malloc(sizeof(GzRender));
	render1->camera.FOV = DEFAULT_FOV;
	render1->camera.position[X] = DEFAULT_IM_X;
	render1->camera.position[Y] = DEFAULT_IM_Y;
	render1->camera.position[Z] = DEFAULT_IM_Z;

	render1->camera.lookat[X] = 0.0;
	render1->camera.lookat[Y] = 0.0;
	render1->camera.lookat[Z] = 0.0;

	render1->camera.worldup[X] = 0.0;
	render1->camera.worldup[Y] = 1.0;
	render1->camera.worldup[Z] = 0.0;
	render1->open = 0;
	render1->matlevel = -1;
	render1->display = display;
	render1->Xsp[0][0] = (display->xres) / 2;
	render1->Xsp[0][1] = 0;
	render1->Xsp[0][2] = 0;
	render1->Xsp[0][3] = (display->xres) / 2;
	render1->Xsp[1][0] = 0;
	render1->Xsp[1][1] = -((display->yres)/2);
	render1->Xsp[1][2] = 0;
	render1->Xsp[1][3] = (display->yres)/2;
	render1->Xsp[2][0] = 0;
	render1->Xsp[2][1] = 0;
	render1->Xsp[2][2] = INT_MAX * tan(((render1->camera.FOV) / 2 )* (3.14159265 / 180));
	render1->Xsp[2][3] = 0;
	render1->Xsp[3][0] = 0;
	render1->Xsp[3][1] = 0;
	render1->Xsp[3][2] = 0;
	render1->Xsp[3][3] = 1;
	
	*render = render1;
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


int GzBeginRender(GzRender *render)
{
/*  
- set up for start of each frame - clear frame buffer 
- compute Xiw and projection xform Xpi from camera definition 
- init Ximage - put Xsp at base of stack, push on Xpi and Xiw 
- now stack contains Xsw and app can push model Xforms if it want to. 
*/ 
	if (render == NULL){
		return GZ_FAILURE;
	}

	render->open = 1;
	free(render->display->fbuf);
	render->display->fbuf = (GzPixel *)malloc(sizeof(GzPixel)* (render->display)->xres * (render->display)->yres);
	GzInitDisplay(render->display);

	GzMatrix Xpi,Xiw;
	float d;
	d = tan(((render->camera.FOV) / 2) * (3.14159265 / 180));
	
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (i == j){
				Xpi[i][j] = 1;
			}
			else
				Xpi[i][j] = 0;
		}
	}
	Xpi[2][3] = 0;
	Xpi[3][2] = d;

	GzCoord cl;
	cl[X] = render->camera.lookat[X] - render->camera.position[X];
	cl[Y] = render->camera.lookat[Y] - render->camera.position[Y];
	cl[Z] = render->camera.lookat[Z] - render->camera.position[Z];

	float mod_cl;

	mod_cl = sqrt((cl[X] * cl[X]) + (cl[Y] * cl[Y]) + (cl[Z] * cl[Z]));

	GzCoord camera_z_axis;

	camera_z_axis[X] = cl[X] / mod_cl;
	camera_z_axis[Y] = cl[Y] / mod_cl;
	camera_z_axis[Z] = cl[Z] / mod_cl;

	GzCoord up, camera_y_axis;
	float dot_up_z = (((render->camera.worldup[X]) * (camera_z_axis[X])) + ((render->camera.worldup[Y]) * (camera_z_axis[Y])) + ((render->camera.worldup[Z]) * (camera_z_axis[Z])));
	up[X] = (render->camera.worldup[X]) - (dot_up_z * (camera_z_axis[X]));
	up[Y] = (render->camera.worldup[Y]) - (dot_up_z * (camera_z_axis[Y]));
	up[Z] = (render->camera.worldup[Z]) - (dot_up_z * (camera_z_axis[Z]));

	float mod_up;
	mod_up = sqrt((up[X] * up[X]) + (up[Y] * up[Y]) + (up[Z] * up[Z]));
	
	camera_y_axis[X] = up[X] / mod_up;
	camera_y_axis[Y] = up[Y] / mod_up;
	camera_y_axis[Z] = up[Z] / mod_up;

	GzCoord camera_x_axis;

	camera_x_axis[X] = (camera_y_axis[Y] * camera_z_axis[Z]) - (camera_y_axis[Z] * camera_z_axis[Y]);
	camera_x_axis[Y] = (camera_y_axis[Z] * camera_z_axis[X]) - (camera_y_axis[X] * camera_z_axis[Z]);
	camera_x_axis[Z] = (camera_y_axis[X] * camera_z_axis[Y]) - (camera_y_axis[Y] * camera_z_axis[X]);

	float xc, yc, zc;
	xc = (camera_x_axis[X] * render->camera.position[X]) + (camera_x_axis[Y] * render->camera.position[Y]) + (camera_x_axis[Z] * render->camera.position[Z]);
	yc = (camera_y_axis[X] * render->camera.position[X]) + (camera_y_axis[Y] * render->camera.position[Y]) + (camera_y_axis[Z] * render->camera.position[Z]);
	zc = (camera_z_axis[X] * render->camera.position[X]) + (camera_z_axis[Y] * render->camera.position[Y]) + (camera_z_axis[Z] * render->camera.position[Z]);
	
	Xiw[0][0] = camera_x_axis[X];
	Xiw[0][1] = camera_x_axis[Y];
	Xiw[0][2] = camera_x_axis[Z];
	Xiw[0][3] = -(xc);
	Xiw[1][0] = camera_y_axis[X];
	Xiw[1][1] = camera_y_axis[Y];
	Xiw[1][2] = camera_y_axis[Z];
	Xiw[1][3] = -(yc);
	Xiw[2][0] = camera_z_axis[X];
	Xiw[2][1] = camera_z_axis[Y];
	Xiw[2][2] = camera_z_axis[Z];
	Xiw[2][3] = -(zc);
	Xiw[3][0] = 0;
	Xiw[3][1] = 0; 
	Xiw[3][2] = 0;
	Xiw[3][3] = 1;

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){

			render->camera.Xpi[i][j] = Xpi[i][j];
			render->camera.Xiw[i][j] = Xiw[i][j];
			
		}
	}
	

	GzPushMatrix(render,render->Xsp);
	GzPushMatrix(render, render->camera.Xpi);
	GzPushMatrix(render, render->camera.Xiw);

	return GZ_SUCCESS;
}

int GzPutCamera(GzRender *render, GzCamera *camera)
{
/*
- overwrite renderer camera structure with new camera definition
*/
	if (render == NULL){
		return GZ_FAILURE;
	}
	render->camera.position[X] = camera->position[X];
	render->camera.position[Y] = camera->position[Y];
	render->camera.position[Z] = camera->position[Z];
	render->camera.lookat[X] = camera->lookat[X];
	render->camera.lookat[Y] = camera->lookat[Y];
	render->camera.lookat[Z] = camera->lookat[Z];
	render->camera.worldup[X] = camera->worldup[X];
	render->camera.worldup[Y] = camera->worldup[Y];
	render->camera.worldup[Z] = camera->worldup[Z];
	render->camera.FOV = camera->FOV;
	float d;
	d = tan(((render->camera.FOV) / 2) * (3.14159265 / 180));
	render->Xsp[2][2] = INT_MAX * d;

	return GZ_SUCCESS;	
}

int GzPushMatrix(GzRender *render, GzMatrix	matrix)
{
/*
- push a matrix onto the Ximage stack
- check for stack overflow
*/
	if (render == NULL){
		return GZ_FAILURE;
	}
	if (render->matlevel == MATLEVELS){
		return GZ_FAILURE;
	}
	
	else {
		GzMatrix result;
		if (render->matlevel == -1) {
			GzMatrix identity_mat;
			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++){
					if (i == j){
						identity_mat[i][j] = 1;
					}
					else
						identity_mat[i][j] = 0;
				}
			}
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result[i][j] = 0;
					for (int k = 0; k < 4; k++) {
						result[i][j] += (identity_mat[i][k] * matrix[k][j]);
					}
				}
			}
		}
		else {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result[i][j] = 0;
					for (int k = 0; k < 4; k++) {
						result[i][j] += (render->Ximage[render->matlevel][i][k] * matrix[k][j]);
					}
				}
			}
		}
		render->matlevel++;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				render->Ximage[render->matlevel][i][j] = result[i][j];
			}
		}
	}
	
	return GZ_SUCCESS;
}

int GzPopMatrix(GzRender *render)
{
/*
- pop a matrix off the Ximage stack
- check for stack underflow
*/
	if (render == NULL){
		return GZ_FAILURE;
	}
	if (render->matlevel < 0){
		return GZ_FAILURE;
	}
	render->matlevel--;
	return GZ_SUCCESS;
}


int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList, 
	GzPointer	*valueList) /* void** valuelist */
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

int GzPutTriangle(GzRender	*render, int numParts, GzToken *nameList, 
				  GzPointer	*valueList)
/* numParts : how many names and values */
{
/*  
- pass in a triangle description with tokens and values corresponding to 
      GZ_POSITION:3 vert positions in model space 
- Xform positions of verts  
- Clip - just discard any triangle with verts behind view plane 
       - test for triangles with all three verts off-screen 
- invoke triangle rasterizer  
*/ 


	if (render == NULL){
		return GZ_FAILURE;
	}
	for (int i = 0; i < numParts; i++){
		if (nameList[i] == GZ_NULL_TOKEN){
			return GZ_FAILURE;
		}
		if (nameList[i] == GZ_POSITION){
			float* vertices = (float*)valueList[0];
			float x1 = *(vertices);
			float y1 = *(vertices + 1);
			float z1 = *(vertices + 2);
			float x2 = *(vertices + 3);
			float y2 = *(vertices + 4);
			float z2 = *(vertices + 5);
			float x3 = *(vertices + 6);
			float y3 = *(vertices + 7);
			float z3 = *(vertices + 8);

			//homogeneous matrix

			float coord_x[4], coord_y[4], coord_z[4];
			
			coord_x[0] = x1;
			coord_x[1] = y1;
			coord_x[2] = z1;
			coord_x[3] = 1;

			coord_y[0] = x2;
			coord_y[1] = y2;
			coord_y[2] = z2;
			coord_y[3] = 1;

			coord_z[0] = x3;
			coord_z[1] = y3;
			coord_z[2] = z3;
			coord_z[3] = 1;

			float resultant_x[4][1], resultant_y[4][1],resultant_z[4][1];

			for (int i = 0; i < 4; i++){
				resultant_x[i][0] = 0;
				for (int j = 0; j < 4; j++){
					resultant_x[i][0] = resultant_x[i][0] + ((render->Ximage[render->matlevel][i][j]) * coord_x[j]);
				}
			}

			for (int i = 0; i < 4; i++){
				resultant_y[i][0] = 0;
				for (int j = 0; j < 4; j++){
					resultant_y[i][0] = resultant_y[i][0] + ((render->Ximage[render->matlevel][i][j]) * coord_y[j]);
				}
			}

			for (int i = 0; i < 4; i++){
				resultant_z[i][0] = 0;
				for (int j = 0; j < 4; j++){
					resultant_z[i][0] = resultant_z[i][0] + ((render->Ximage[render->matlevel][i][j]) * coord_z[j]);
				}
			}

			GzCoord x, y, z;
			x[X] = resultant_x[0][0] / resultant_x[3][0];
			x[Y] = resultant_y[0][0] / resultant_y[3][0];
			x[Z] = resultant_z[0][0] / resultant_z[3][0];
			
			y[X] = resultant_x[1][0] / resultant_x[3][0];
			y[Y] = resultant_y[1][0] / resultant_y[3][0];
			y[Z] = resultant_z[1][0] / resultant_z[3][0];

			z[X] = resultant_x[2][0] / resultant_x[3][0];
			z[Y] = resultant_y[2][0] / resultant_y[3][0];
			z[Z] = resultant_z[2][0] / resultant_z[3][0];
		

				// clipping part

			if ((x[X] < 0 || x[X] > (render->display->xres)) && (x[Y] < 0 || x[Y] > (render->display->xres)) && (x[Z] < 0 || x[Z] > (render->display->xres))){
				return GZ_FAILURE;

			}

			if ((y[X] < 0 || y[X] > (render->display->yres)) && (y[Y] < 0 || y[Y] > (render->display->yres)) && (y[Z] < 0 || y[Z] > (render->display->yres))){
				return GZ_FAILURE;

			}
			
			if (z[X] < 0 && z[Y] < 0 && z[Z] < 0){
				return GZ_FAILURE;

			}

			GzCoord tempx = { x1, x2, x3 };
			float y_min, y_max;
			sort_on_y(x, y, z);		//sort on y 
			y_min = y[0];			// find minimum y for boundary box
			y_max = y[2];			// find maximum y for boundary box
			float tmp;

			if ((y[1] == y[0]) || (x[1] < x[0]))
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
		for (int j = 0; j < 2 - i; j++) {
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


/* NOT part of API - just for general assistance */

short	ctoi(float color)		/* convert float color to GzIntensity short */
{
  return(short)((int)(color * ((1 << 12) - 1)));
}

