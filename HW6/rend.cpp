/* CS580 Homework 5 */

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


	//shading values

	render1->interp_mode = GZ_RGB_COLOR;
	render1->numlights = 0;
	GzColor ka = DEFAULT_AMBIENT;
	GzColor kd = DEFAULT_DIFFUSE;
	GzColor ks = DEFAULT_SPECULAR;
		
	render1->Ka[RED] = ka[RED];
	render1->Ka[GREEN] = ka[GREEN];
	render1->Ka[BLUE] = ka[BLUE];

	render1->Kd[RED] = kd[RED];
	render1->Kd[GREEN] = kd[GREEN];
	render1->Kd[BLUE] = kd[BLUE];

	render1->Ks[RED] = ks[RED];
	render1->Ks[GREEN] = ks[GREEN];
	render1->Ks[BLUE] = ks[BLUE];
	
	render1->spec = DEFAULT_SPEC;
	render1->dx = 0;
	render1->dy = 0;
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
		if (render->matlevel == -1) {

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result[i][j] = 0;
					for (int k = 0; k < 4; k++) {
						result[i][j] += (identity_mat[i][k] * matrix[k][j]);
					}
				}
			}
		} //if ends
		else {
			// Ximage stack
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result[i][j] = 0;
					for (int k = 0; k < 4; k++) {
						result[i][j] += (render->Ximage[render->matlevel][i][k] * matrix[k][j]);
					}
				}
			}

		} //else ends

		render->matlevel++;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				render->Ximage[render->matlevel][i][j] = (float)result[i][j];
			}
		}

		
		if (render->matlevel == 1 || render->matlevel == 0){

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					render->Xnorm[render->matlevel][i][j] = (float)identity_mat[i][j];
				}
			}
		} //if ends
		
		else{
			GzMatrix  unitary_result, unitary_res_xnorm;
			unitary_result[0][0] = matrix[0][0];
			unitary_result[0][1] = matrix[0][1];
			unitary_result[0][2] = matrix[0][2];
			unitary_result[0][3] = 0;
			unitary_result[1][0] = matrix[1][0];
			unitary_result[1][1] = matrix[1][1];
			unitary_result[1][2] = matrix[1][2];
			unitary_result[1][3] = 0;
			unitary_result[2][0] = matrix[2][0];
			unitary_result[2][1] = matrix[2][1];
			unitary_result[2][2] = matrix[2][2];
			unitary_result[2][3] = 0;
			unitary_result[3][0] = matrix[3][0];
			unitary_result[3][1] = matrix[3][1];
			unitary_result[3][2] = matrix[3][2];
			unitary_result[3][3] = 1;

			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++){
					unitary_res_xnorm[i][j] = 0.0;
					for (int k = 0; k < 4; k++){
						unitary_res_xnorm[i][j] = unitary_res_xnorm[i][j] + render->Xnorm[render->matlevel - 1][i][k] * unitary_result[k][j];
					}
				}

			}

			float temp_k = 1 / sqrt((unitary_res_xnorm[0][0] * unitary_res_xnorm[0][0]) + (unitary_res_xnorm[0][1] * unitary_res_xnorm[0][1]) + (unitary_res_xnorm[0][2] * unitary_res_xnorm[0][2]));
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++){
					unitary_res_xnorm[i][j] = unitary_res_xnorm[i][j] * temp_k;
				}
			}

		

			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++){
					render->Xnorm[render->matlevel][i][j] = (float)unitary_res_xnorm[i][j];
				}
			}

		}//else ends
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
		else if (nameList[i] == GZ_AMBIENT_COEFFICIENT){
			GzColor *ka = (GzColor*)valueList[i];
			render->Ka[RED] = (*ka)[RED];
			render->Ka[GREEN] = (*ka)[GREEN];
			render->Ka[BLUE] = (*ka)[BLUE];

		}

		else if (nameList[i] == GZ_DIFFUSE_COEFFICIENT){
			GzColor *kd = (GzColor*)valueList[i];
			render->Kd[RED] = (*kd)[RED];
			render->Kd[GREEN] = (*kd)[GREEN];
			render->Kd[BLUE] = (*kd)[BLUE];

		}

		else if (nameList[i] == GZ_SPECULAR_COEFFICIENT){
			GzColor *ks = (GzColor*)valueList[i];
			render->Ks[RED] = (*ks)[RED];
			render->Ks[GREEN] = (*ks)[GREEN];
			render->Ks[BLUE] = (*ks)[BLUE];

		}

		else if (nameList[i] == GZ_AMBIENT_LIGHT){
			GzLight *ambientlight = (GzLight*) valueList[i];
			render->ambientlight.color[RED] = ambientlight->color[RED];
			render->ambientlight.color[GREEN] = ambientlight->color[GREEN];
			render->ambientlight.color[BLUE] = ambientlight->color[BLUE];
		}

		else if (nameList[i] == GZ_DIRECTIONAL_LIGHT){
			if (render->numlights >= MAX_LIGHTS){
				return GZ_FAILURE;
			}
			GzLight* directionallight = (GzLight*)valueList[i];
			render->lights[render->numlights].color[RED] = directionallight->color[RED];
			render->lights[render->numlights].color[GREEN] = directionallight->color[GREEN];
			render->lights[render->numlights].color[BLUE] = directionallight->color[BLUE];
			render->lights[render->numlights].direction[RED] = directionallight->direction[RED];
			render->lights[render->numlights].direction[GREEN] = directionallight->direction[GREEN];
			render->lights[render->numlights].direction[BLUE] = directionallight->direction[BLUE];
			render->numlights++;
		}

		else if (nameList[i] == GZ_DISTRIBUTION_COEFFICIENT){
			float *spec = (float*) valueList[i];
			render->spec = *spec;
		}
		else if (nameList[i] == GZ_INTERPOLATE){
			int *interpolate_mode = (int*)valueList[i];
			render->interp_mode = *interpolate_mode;
		}

		else if (nameList[i] == GZ_TEXTURE_MAP){
			GzTexture texture = (GzTexture)valueList[i];
			render->tex_fun = texture; 
		}

		//hw6
		
		else if (nameList[i] == GZ_AASHIFTX){
			float *dx = (float*)valueList[i];
			render->dx = *dx;
		}
		else if (nameList[i] == GZ_AASHIFTY){
			float *dy = (float*)valueList[i];
			render->dy = *dy;
		}

	} // for ends


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
	vertexList[3]
	*/
	GzCoord	*vertexList, *normalList;
	GzTextureIndex *textureList;
	if (render == NULL){
		return GZ_FAILURE;
	}
	for (int i = 0; i < numParts; i++){
		if (nameList[i] == GZ_NULL_TOKEN){
			return GZ_FAILURE;
		}
		if (nameList[i] == GZ_POSITION){

			vertexList = static_cast<GzCoord *>(valueList[i]);
		}
		else if (nameList[i] == GZ_NORMALS){

			normalList = static_cast<GzCoord *>(valueList[i]);

		} 
		else if (nameList[i] == GZ_TEXTURE_INDEX){

			textureList = static_cast<GzTextureIndex *>(valueList[i]);
	
		}//if ends
	} //for ends

	//vertices
	float coord_1[4], coord_2[4], coord_3[4];

	coord_1[0] = vertexList[0][0];
	coord_1[1] = vertexList[0][1];
	coord_1[2] = vertexList[0][2];
	coord_1[3] = 1;

	coord_2[0] = vertexList[1][0];
	coord_2[1] = vertexList[1][1];
	coord_2[2] = vertexList[1][2];
	coord_2[3] = 1;

	coord_3[0] = vertexList[2][0];
	coord_3[1] = vertexList[2][1];
	coord_3[2] = vertexList[2][2];
	coord_3[3] = 1;


	GzCoord face_normal = { 0, 0, 0 };

	face_normal[X] = ((coord_2[Y] - coord_1[Y]) * (coord_3[Z] - coord_1[Z])) - ((coord_3[Y] - coord_1[Y]) * (coord_2[Z] - coord_1[Z]));
	face_normal[Y] = ((coord_2[Z] - coord_1[Z]) * (coord_3[X] - coord_1[X])) - ((coord_3[Z] - coord_1[Z]) * (coord_2[X] - coord_1[X]));
	face_normal[Z] = ((coord_2[X] - coord_1[X]) * (coord_3[Y] - coord_1[Y])) - ((coord_3[X] - coord_1[X]) * (coord_2[Y] - coord_1[Y]));

	GzCoord resultant_face_normal;

	resultant_face_normal[X] = face_normal[X] * (render)->Xnorm[render->matlevel][0][0] +
		face_normal[Y] * (render)->Xnorm[render->matlevel][0][1] +
		face_normal[Z] * (render)->Xnorm[render->matlevel][0][2];

	resultant_face_normal[Y] = face_normal[X] * (render)->Xnorm[render->matlevel][1][0] +
		face_normal[Y] * (render)->Xnorm[render->matlevel][1][1] +
		face_normal[Z] * (render)->Xnorm[render->matlevel][1][2];

	resultant_face_normal[Z] = face_normal[X] * (render)->Xnorm[render->matlevel][2][0] +
		face_normal[Y] * (render)->Xnorm[render->matlevel][2][1] +
		face_normal[Z] * (render)->Xnorm[render->matlevel][2][2];

	float mod_resultant_face_normal = sqrt((resultant_face_normal[0] * resultant_face_normal[0]) + (resultant_face_normal[1] * resultant_face_normal[1]) + (resultant_face_normal[2] * resultant_face_normal[2]));
	face_normal[0] = resultant_face_normal[0] / mod_resultant_face_normal;
	face_normal[1] = resultant_face_normal[1] / mod_resultant_face_normal;
	face_normal[2] = resultant_face_normal[2] / mod_resultant_face_normal;



	float resultant_1[4][1], resultant_2[4][1], resultant_3[4][1];


	for (int i = 0; i < 4; i++){
		resultant_1[i][0] = 0;
		for (int j = 0; j < 4; j++){
			resultant_1[i][0] = resultant_1[i][0] + ((render->Ximage[render->matlevel][i][j]) * coord_1[j]);
		}
	}

	for (int i = 0; i < 4; i++){
		resultant_2[i][0] = 0;
		for (int j = 0; j < 4; j++){
			resultant_2[i][0] = resultant_2[i][0] + ((render->Ximage[render->matlevel][i][j]) * coord_2[j]);
		}
	}

	for (int i = 0; i < 4; i++){
		resultant_3[i][0] = 0;
		for (int j = 0; j < 4; j++){
			resultant_3[i][0] = resultant_3[i][0] + ((render->Ximage[render->matlevel][i][j]) * coord_3[j]);
		}
	}


	//homogeneous matrix
	GzCoord x, y, z;
	x[X] = resultant_1[0][0] / resultant_1[3][0];
	x[Y] = resultant_2[0][0] / resultant_2[3][0];
	x[Z] = resultant_3[0][0] / resultant_3[3][0];

	y[X] = resultant_1[1][0] / resultant_1[3][0];
	y[Y] = resultant_2[1][0] / resultant_2[3][0];
	y[Z] = resultant_3[1][0] / resultant_3[3][0];

	z[X] = resultant_1[2][0] / resultant_1[3][0];
	z[Y] = resultant_2[2][0] / resultant_2[3][0];
	z[Z] = resultant_3[2][0] / resultant_3[3][0];




	//normals
	float normalcoord_1[4], normalcoord_2[4], normalcoord_3[4];

	normalcoord_1[0] = normalList[0][0];
	normalcoord_1[1] = normalList[0][1];
	normalcoord_1[2] = normalList[0][2];
	normalcoord_1[3] = 1;

	normalcoord_2[0] = normalList[1][0];
	normalcoord_2[1] = normalList[1][1];
	normalcoord_2[2] = normalList[1][2];
	normalcoord_2[3] = 1;

	normalcoord_3[0] = normalList[2][0];
	normalcoord_3[1] = normalList[2][1];
	normalcoord_3[2] = normalList[2][2];
	normalcoord_3[3] = 1;

	float normalresultant_1[4][1], normalresultant_2[4][1], normalresultant_3[4][1];

	for (int i = 0; i < 4; i++){
		normalresultant_1[i][0] = 0;
		for (int j = 0; j < 4; j++){
			normalresultant_1[i][0] = normalresultant_1[i][0] + ((render->Xnorm[render->matlevel][i][j]) * normalcoord_1[j]);
		}
	}
	for (int i = 0; i < 4; i++){
		normalresultant_2[i][0] = 0;
		for (int j = 0; j < 4; j++){
			normalresultant_2[i][0] = normalresultant_2[i][0] + ((render->Xnorm[render->matlevel][i][j]) * normalcoord_2[j]);
		}
	}
	for (int i = 0; i < 4; i++){
		normalresultant_3[i][0] = 0;
		for (int j = 0; j < 4; j++){
			normalresultant_3[i][0] = normalresultant_3[i][0] + ((render->Xnorm[render->matlevel][i][j]) * normalcoord_3[j]);
		}
	}
	//homogeneous matrix

	GzCoord normal1, normal2, normal3;
	normal1[X] = normalresultant_1[0][0];
	normal1[Y] = normalresultant_1[0][1];
	normal1[Z] = normalresultant_1[0][2];

	normal2[X] = normalresultant_2[0][0];
	normal2[Y] = normalresultant_2[0][1];
	normal2[Z] = normalresultant_2[0][2];

	normal3[X] = normalresultant_3[0][0];
	normal3[Y] = normalresultant_3[0][1];
	normal3[Z] = normalresultant_3[0][2];

	float mod_normal1 = sqrt((normal1[X] * normal1[X]) + (normal1[Y] * normal1[Y]) + (normal1[Z] * normal1[Z]));
	float mod_normal2 = sqrt((normal2[X] * normal2[X]) + (normal2[Y] * normal2[Y]) + (normal2[Z] * normal2[Z]));
	float mod_normal3 = sqrt((normal3[X] * normal3[X]) + (normal3[Y] * normal3[Y]) + (normal3[Z] * normal3[Z]));

	normal1[X] = normal1[X] / mod_normal1;
	normal1[Y] = normal1[Y] / mod_normal1;
	normal1[Z] = normal1[Z] / mod_normal1;

	normal2[X] = normal2[X] / mod_normal2;
	normal2[Y] = normal2[Y] / mod_normal2;
	normal2[Z] = normal2[Z] / mod_normal2;

	normal3[X] = normal3[X] / mod_normal3;
	normal3[Y] = normal3[Y] / mod_normal3;
	normal3[Z] = normal3[Z] / mod_normal3;
	
	//texture mapping
	
	GzTextureIndex uvtexture_1, uvtexture_2, uvtexture_3;
	uvtexture_1[0] = textureList[0][0];
	uvtexture_1[1] = textureList[0][1];
	uvtexture_2[0] = textureList[1][0];
	uvtexture_2[1] = textureList[1][1];
	uvtexture_3[0] = textureList[2][0];
	uvtexture_3[1] = textureList[2][1];

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

	//hw6
	for (int i = 0; i < 3; i++){
		x[i] = x[i] + render->dx;
		y[i] = y[i] + render->dy;
	}

	float y_min, y_max;
	sort_on_y(x, y, z, normal1, normal2, normal3, uvtexture_1, uvtexture_2, uvtexture_3); //sort on y
	
	GzColor color_v1 = { 0, 0, 0 };
	GzColor color_v2 = { 0, 0, 0 };
	GzColor color_v3 = { 0, 0, 0 };
	if (render->interp_mode == GZ_COLOR){

		getcolor(render, normal1, color_v1);
		getcolor(render, normal2, color_v2);
		getcolor(render, normal3, color_v3);
	}


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

		tmp = color_v1[RED];
		color_v1[RED] = color_v2[RED];
		color_v2[RED] = tmp;

		tmp = color_v1[GREEN];
		color_v1[GREEN] = color_v2[GREEN];
		color_v2[GREEN] = tmp;

		tmp = color_v1[BLUE];
		color_v1[BLUE] = color_v2[BLUE];
		color_v2[BLUE] = tmp;

		tmp = normal1[X];
		normal1[X] = normal2[X];
		normal2[X] = tmp;

		tmp = normal1[Y];
		normal1[Y] = normal2[Y];
		normal2[Y] = tmp;

		tmp = normal1[Z];
		normal1[Z] = normal2[Z];
		normal2[Z] = tmp;

		tmp = uvtexture_1[X];
		uvtexture_1[X] = uvtexture_2[X];
		uvtexture_2[X] = tmp;

		tmp = uvtexture_1[Y];
		uvtexture_1[Y] = uvtexture_2[Y];
		uvtexture_2[Y] = tmp;

		tmp = uvtexture_1[Z];
		uvtexture_1[Z] = uvtexture_2[Z];
		uvtexture_2[Z] = tmp;
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

		tmp = color_v2[RED];
		color_v2[RED] = color_v3[RED];
		color_v3[RED] = tmp;

		tmp = color_v2[GREEN];
		color_v2[GREEN] = color_v3[GREEN];
		color_v3[GREEN] = tmp;

		tmp = color_v2[BLUE];
		color_v2[BLUE] = color_v3[BLUE];
		color_v3[BLUE] = tmp;


		tmp = normal2[X];
		normal2[X] = normal3[X];
		normal3[X] = tmp;

		tmp = normal2[Y];
		normal2[Y] = normal3[Y];
		normal3[Y] = tmp;

		tmp = normal2[Z];
		normal2[Z] = normal3[Z];
		normal3[Z] = tmp;

		tmp = uvtexture_2[X];
		uvtexture_2[X] = uvtexture_3[X];
		uvtexture_3[X] = tmp;

		tmp = uvtexture_2[Y];
		uvtexture_2[Y] = uvtexture_3[Y];
		uvtexture_3[Y] = tmp;

		tmp = uvtexture_2[Z];
		uvtexture_2[Z] = uvtexture_3[Z];
		uvtexture_3[Z] = tmp;

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

		tmp = color_v2[RED];
		color_v2[RED] = color_v3[RED];
		color_v3[RED] = tmp;

		tmp = color_v2[GREEN];
		color_v2[GREEN] = color_v3[GREEN];
		color_v3[GREEN] = tmp;

		tmp = color_v2[BLUE];
		color_v2[BLUE] = color_v3[BLUE];
		color_v3[BLUE] = tmp;


		tmp = normal2[X];
		normal2[X] = normal3[X];
		normal3[X] = tmp;

		tmp = normal2[Y];
		normal2[Y] = normal3[Y];
		normal3[Y] = tmp;

		tmp = normal2[Z];
		normal2[Z] = normal3[Z];
		normal3[Z] = tmp;

		tmp = uvtexture_2[X];
		uvtexture_2[X] = uvtexture_3[X];
		uvtexture_3[X] = tmp;

		tmp = uvtexture_2[Y];
		uvtexture_2[Y] = uvtexture_3[Y];
		uvtexture_3[Y] = tmp;

		tmp = uvtexture_2[Z];
		uvtexture_2[Z] = uvtexture_3[Z];
		uvtexture_3[Z] = tmp;
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
	
	
	float z_inter = 0;
	GzColor gouraud_z_inter;
	GzTextureIndex texture_z_inter, newuv;
	GzTextureIndex P1, P2, P3;

	//warping

	P1[0] = uvtexture_1[0] / ((z[0] / (INT_MAX - z[0])) + 1);
	P1[1] = uvtexture_1[1] / ((z[0] / (INT_MAX - z[0])) + 1);
	P2[0] = uvtexture_2[0] / ((z[1] / (INT_MAX - z[1])) + 1);
	P2[1] = uvtexture_2[1] / ((z[1] / (INT_MAX - z[1])) + 1);
	P3[0] = uvtexture_3[0] / ((z[2] / (INT_MAX - z[2])) + 1);
	P3[1] = uvtexture_3[1] / ((z[2] / (INT_MAX - z[2])) + 1);

			for (int i = floor(x_min); i < ceil(x_max); i++){
				for (int j = floor(y_min); j < ceil(y_max); j++){
					d1 = (coefA1*i) + (coefB1*j) + coefC1;
					d2 = (coefA2*i) + (coefB2*j) + coefC2;
					d3 = (coefA3*i) + (coefB3*j) + coefC3;
					z_inter = -(A*i + B*j + D) / C;

					//for texture mapping
					
					GzTextureIndex texA, texB, texC, texD;

					texA[0] = ((y11 * (P3[0] - P1[0])) - (y22 * (P2[0] - P1[0])));
					texB[0] = ((x22 * (P2[0] - P1[0])) - (x11 * (P3[0] - P1[0])));
					texC[0] = ((x11 * y22) - (x22 * y11));
					texD[0] = ((y[0] * texB[0]) + (x[0] * texA[0]) + (P1[0] * texC[0])) *(-1);

					texA[1] = ((y11 * (P3[1] - P1[1])) - (y22 * (P2[1] - P1[1])));
					texB[1] = ((x22 * (P2[1] - P1[1])) - (x11 * (P3[1] - P1[1])));;
					texC[1] = ((x11 * y22) - (x22 * y11));
					texD[1] = ((y[0] * texB[1]) + (x[0] * texA[1]) + (P1[1] * texC[1])) *(-1);

					texture_z_inter[0] = -(texA[0] * i + texB[0] * j + texD[0]) / texC[0];
					texture_z_inter[1] = -(texA[1] * i + texB[1] * j + texD[1]) / texC[1];

					//unwarping

					newuv[0] = texture_z_inter[0] * ((z_inter / (INT_MAX - z_inter)) + 1);
					newuv[1] = texture_z_inter[1] * ((z_inter / (INT_MAX - z_inter)) + 1);
					
					GzColor texturecolor = {0,0,0};
					if (render->tex_fun != (GzPointer)0){
						render->tex_fun(newuv[0], newuv[1], texturecolor);
					}
					if (!(d1 < 0 || d2 < 0 || d3 < 0)){
						GzIntensity a, b, c, d;
						GzDepth z;
						GzGetDisplay(render->display, i, j, &a, &b, &c, &d, &z);
						if (z_inter < z || z == 0)
						{

							if (render->interp_mode == GZ_COLOR){
								if (render->tex_fun != (GzPointer)0){
									render->Ka[RED] = texturecolor[RED];
									render->Ka[GREEN] = texturecolor[GREEN];
									render->Ka[BLUE] = texturecolor[BLUE];
									render->Kd[RED] = texturecolor[RED];
									render->Kd[GREEN] = texturecolor[GREEN];
									render->Kd[BLUE] = texturecolor[BLUE];
									render->Ks[RED] = texturecolor[RED];
									render->Ks[GREEN] = texturecolor[GREEN];
									render->Ks[BLUE] = texturecolor[BLUE];
								}
								getcolor(render, normal1, color_v1);
								getcolor(render, normal2, color_v2);
								getcolor(render, normal3, color_v3);

								//for planes

								GzColor gouraudA, gouraudB, gouraudC, gouraudD;

								gouraudA[0] = ((y11 * (color_v3[0] - color_v1[0])) - (y22 * (color_v2[0] - color_v1[0])));
								gouraudB[0] = ((x22 * (color_v2[0] - color_v1[0])) - (x11 * (color_v3[0] - color_v1[0])));
								gouraudC[0] = ((x11 * y22) - (x22 * y11));
								gouraudD[0] = ((y[0] * gouraudB[0]) + (x[0] * gouraudA[0]) + (color_v1[0] * gouraudC[0])) *(-1);

								gouraudA[1] = ((y11 * (color_v3[1] - color_v1[1])) - (y22 * (color_v2[1] - color_v1[1])));
								gouraudB[1] = ((x22 * (color_v2[1] - color_v1[1])) - (x11 * (color_v3[1] - color_v1[1])));
								gouraudC[1] = ((x11 * y22) - (x22 * y11));
								gouraudD[1] = ((y[0] * gouraudB[1]) + (x[0] * gouraudA[1]) + (color_v1[1] * gouraudC[1])) *(-1);

								gouraudA[2] = ((y11 * (color_v3[2] - color_v1[2])) - (y22 * (color_v2[2] - color_v1[2])));
								gouraudB[2] = ((x22 * (color_v2[2] - color_v1[2])) - (x11 * (color_v3[2] - color_v1[2])));
								gouraudC[2] = ((x11 * y22) - (x22 * y11));
								gouraudD[2] = ((y[0] * gouraudB[2]) + (x[0] * gouraudA[2]) + (color_v1[2] * gouraudC[2])) *(-1);


								gouraud_z_inter[0] = -(gouraudA[0] * i + gouraudB[0] * j + gouraudD[0]) / gouraudC[0];
								gouraud_z_inter[1] = -(gouraudA[1] * i + gouraudB[1] * j + gouraudD[1]) / gouraudC[1];
								gouraud_z_inter[2] = -(gouraudA[2] * i + gouraudB[2] * j + gouraudD[2]) / gouraudC[2];

								render->flatcolor[0] = gouraud_z_inter[0];
								render->flatcolor[1] = gouraud_z_inter[1];
								render->flatcolor[2] = gouraud_z_inter[2];
							
								//	GzPutDisplay(render->display, i, j, ctoi(render->flatcolor[0]), ctoi(render->flatcolor[1]), ctoi(render->flatcolor[2]), a, z_inter);
							}
							else if (render->interp_mode == GZ_NORMALS){
								GzColor phongA, phongB, phongC, phongD;

								phongA[0] = ((y11 * (normal3[0] - normal1[0])) - (y22 * (normal2[0] - normal1[0])));
								phongB[0] = ((x22 * (normal2[0] - normal1[0])) - (x11 * (normal3[0] - normal1[0])));
								phongC[0] = ((x11 * y22) - (x22 * y11));
								phongD[0] = ((y[0] * phongB[0]) + (x[0] * phongA[0]) + (normal1[0] * phongC[0])) *(-1);

								phongA[1] = ((y11 * (normal3[1] - normal1[1])) - (y22 * (normal2[1] - normal1[1])));
								phongB[1] = ((x22 * (normal2[1] - normal1[1])) - (x11 * (normal3[1] - normal1[1])));;
								phongC[1] = ((x11 * y22) - (x22 * y11));
								phongD[1] = ((y[0] * phongB[1]) + (x[0] * phongA[1]) + (normal1[1] * phongC[1])) *(-1);

								phongA[2] = ((y11 * (normal3[2] - normal1[2])) - (y22 * (normal2[2] - normal1[2])));
								phongB[2] = ((x22 * (normal2[2] - normal1[2])) - (x11 * (normal3[2] - normal1[2])));;
								phongC[2] = ((x11 * y22) - (x22 * y11));
								phongD[2] = ((y[0] * phongB[2]) + (x[0] * phongA[2]) + (normal1[2] * phongC[2])) *(-1);

								GzColor phong_z_inter;

								phong_z_inter[0] = -(phongA[0] * i + phongB[0] * j + phongD[0]) / phongC[0];
								phong_z_inter[1] = -(phongA[1] * i + phongB[1] * j + phongD[1]) / phongC[1];
								phong_z_inter[2] = -(phongA[2] * i + phongB[2] * j + phongD[2]) / phongC[2];

								float mod_phong_z_inter = sqrt((phong_z_inter[0] * phong_z_inter[0]) + (phong_z_inter[1] * phong_z_inter[1]) + (phong_z_inter[2] * phong_z_inter[2]));
								phong_z_inter[0] = phong_z_inter[0] / mod_phong_z_inter;
								phong_z_inter[1] = phong_z_inter[1] / mod_phong_z_inter;
								phong_z_inter[2] = phong_z_inter[2] / mod_phong_z_inter;

								if (render->tex_fun != (GzPointer)0){
									render->Ka[RED] = texturecolor[RED];
									render->Ka[GREEN] = texturecolor[GREEN];
									render->Ka[BLUE] = texturecolor[BLUE];
									render->Kd[RED] = texturecolor[RED];
									render->Kd[GREEN] = texturecolor[GREEN];
									render->Kd[BLUE] = texturecolor[BLUE];
								}
								/*getcolor(render, phong_z_inter, texturecolor);
								
								render->flatcolor[0] = texturecolor[0];
								render->flatcolor[1] = texturecolor[1];
								render->flatcolor[2] = texturecolor[2];*/
								
								GzColor color_normal;
								getcolor(render, phong_z_inter,color_normal);
								render->flatcolor[0] = color_normal[0];
								render->flatcolor[1] = color_normal[1];
								render->flatcolor[2] = color_normal[2];
								//GzPutDisplay(render->display, i, j, ctoi(render->flatcolor[0]), ctoi(render->flatcolor[1]), ctoi(render->flatcolor[2]), a, z_inter);
							}

							else if(render->interp_mode == GZ_FLAT){
							
								GzColor color_flat = { 0, 0, 0 };							
								
								getcolor(render,face_normal,color_flat);
								render->flatcolor[0] = color_flat[0];
								render->flatcolor[1] = color_flat[1];
								render->flatcolor[2] = color_flat[2];
								
								//GzPutDisplay(render->display, i, j, ctoi(render->flatcolor[RED]), ctoi(render->flatcolor[GREEN]), ctoi(render->flatcolor[BLUE]), a, z_inter);

							}
							
							if (render->flatcolor[0] > 1){
								render->flatcolor[0] = 1;
							}

							if (render->flatcolor[0] < 0){
								render->flatcolor[0] = 0;
							}

							if (render->flatcolor[1] > 1){
								render->flatcolor[1] = 1;
							}

							if (render->flatcolor[1] < 0){
								render->flatcolor[1] = 0;
							}

							if (render->flatcolor[2] > 1){
								render->flatcolor[2] = 1;
							}

							if (render->flatcolor[2] < 0){
								render->flatcolor[2] = 0;
							}

						GzPutDisplay(render->display, i, j, ctoi(render->flatcolor[RED]), ctoi(render->flatcolor[GREEN]), ctoi(render->flatcolor[BLUE]), a, z_inter);

						}
					}
				}
			} //for ends
		//} //if position ends
	//} // for ends
	return GZ_SUCCESS;
}

int getcolor(GzRender *render, GzCoord normal, GzColor color){

	GzCoord E,R;
	E[0] = 0;
	E[1] = 0;
	E[2] = -1;
	float NdotE,NdotL,RdotE;
	GzColor diffuse_color_intensity, specular_color_intensity;
	diffuse_color_intensity[RED] = 0;
	diffuse_color_intensity[GREEN] = 0;
	diffuse_color_intensity[BLUE] = 0;
	specular_color_intensity[RED] = 0;
	specular_color_intensity[GREEN] = 0;
	specular_color_intensity[BLUE] = 0;
	NdotE = ((normal[0] * E[0]) + (normal[1] * E[1]) + (normal[2] * E[2]));

	GzCoord newN = { 0, 0, 0 };

	for (int i = 0; i < (render->numlights); i++){
		NdotL = ((normal[0] * (render->lights[i].direction[0])) + (normal[1] * (render->lights[i].direction[1])) + (normal[2] * (render->lights[i].direction[2])));
		if (NdotL > 0 && NdotE > 0){

			R[0] = (2 * (NdotL)* normal[0]) - (render->lights[i].direction[0]);
			R[1] = (2 * (NdotL)* normal[1]) - (render->lights[i].direction[1]);
			R[2] = (2 * (NdotL)* normal[2]) - (render->lights[i].direction[2]);

		}

		else if (NdotL < 0 && NdotE < 0){

			newN[0] = (-1 * normal[0]);
			newN[1] = (-1 * normal[1]);
			newN[2] = (-1 * normal[2]);

			NdotL = ((newN[0] * (render->lights[i].direction[0])) + (newN[1] * (render->lights[i].direction[1])) + (newN[2] * (render->lights[i].direction[2])));
			R[0] = (2 * (NdotL)* newN[0]) - (render->lights[i].direction[0]);
			R[1] = (2 * (NdotL)* newN[1]) - (render->lights[i].direction[1]);
			R[2] = (2 * (NdotL)* newN[2]) - (render->lights[i].direction[2]);
		}
		else
		{
			continue;
		}

		float modR = sqrt((R[0] * R[0]) + (R[1] * R[1]) + (R[2] * R[2]));

		R[0] = R[0] / modR;
		R[1] = R[1] / modR;
		R[2] = R[2] / modR;

		RdotE = ((R[0] * E[0]) + (R[1] * E[1]) + (R[2] * E[2]));

		if (RdotE < 0){
			RdotE = 0;
		}
		else if (RdotE > 1){
			RdotE = 1;
		}

		diffuse_color_intensity[RED] = diffuse_color_intensity[RED] + ((NdotL) * (render->lights[i].color[RED]));
		diffuse_color_intensity[GREEN] = diffuse_color_intensity[GREEN] + ((NdotL)* (render->lights[i].color[GREEN]));
		diffuse_color_intensity[BLUE] = diffuse_color_intensity[BLUE] + ((NdotL)* (render->lights[i].color[BLUE]));
		
		specular_color_intensity[RED] = specular_color_intensity[RED] + ((pow(RdotE,render->spec)) * (render->lights[i].color[RED]));
		specular_color_intensity[GREEN] = specular_color_intensity[GREEN] + ((pow(RdotE,render->spec)) * (render->lights[i].color[GREEN]));
		specular_color_intensity[BLUE] = specular_color_intensity[BLUE] + ((pow(RdotE,render->spec)) * (render->lights[i].color[BLUE]));

	} //for ends

		//color = specular + diffuse+ ambient;

		color[RED] = (render->Ks[RED] * specular_color_intensity[RED]) + (render->Kd[RED] * diffuse_color_intensity[RED]) + (render->Ka[RED] * render->ambientlight.color[RED]);
		color[GREEN] = (render->Ks[GREEN] * specular_color_intensity[GREEN]) + (render->Kd[GREEN] * diffuse_color_intensity[GREEN]) + (render->Ka[GREEN] * render->ambientlight.color[GREEN]);
		color[BLUE] = (render->Ks[BLUE] * specular_color_intensity[BLUE]) + (render->Kd[BLUE] * diffuse_color_intensity[BLUE]) + (render->Ka[BLUE] * render->ambientlight.color[BLUE]);
	
	
	return GZ_SUCCESS;
}
/* NOT part of API - just for general assistance */

short	ctoi(float color)		/* convert float color to GzIntensity short */
{
  return(short)((int)(color * ((1 << 12) - 1)));
}

void sort_on_y(GzCoord x, GzCoord y, GzCoord z, GzCoord normal1, GzCoord normal2, GzCoord normal3, GzTextureIndex uvtexture_1, GzTextureIndex uvtexture_2, GzTextureIndex uvtexture_3){
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
				if (j == 1){

					temp111 = normal2[X];
					normal2[X] = normal3[X];
					normal3[X] = temp111;

					temp111 = normal2[Y];
					normal2[Y] = normal3[Y];
					normal3[Y] = temp111;

					temp111 = normal2[Z];
					normal2[Z] = normal3[Z];
					normal3[Z] = temp111;

					temp111 = uvtexture_2[X];
					uvtexture_2[X] = uvtexture_3[X];
					uvtexture_3[X] = temp111;

					temp111 = uvtexture_2[Y];
					uvtexture_2[Y] = uvtexture_3[Y];
					uvtexture_3[Y] = temp111;

					temp111 = uvtexture_2[Z];
					uvtexture_2[Z] = uvtexture_3[Z];
					uvtexture_3[Z] = temp111;
				}
				if (j == 0){

					temp111 = normal1[X];
					normal1[X] = normal2[X];
					normal2[X] = temp111;

					temp111 = normal1[Y];
					normal1[Y] = normal2[Y];
					normal2[Y] = temp111;

					temp111 = normal1[Z];
					normal1[Z] = normal2[Z];
					normal2[Z] = temp111;

					temp111 = uvtexture_1[X];
					uvtexture_1[X] = uvtexture_2[X];
					uvtexture_2[X] = temp111;

					temp111 = uvtexture_1[Y];
					uvtexture_1[Y] = uvtexture_2[Y];
					uvtexture_2[Y] = temp111;

					temp111 = uvtexture_1[Z];
					uvtexture_1[Z] = uvtexture_2[Z];
					uvtexture_2[Z] = temp111;

				}
			}
		}
	}
}
