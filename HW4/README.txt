Assignment 4 for CSCI580


The Visual Studio version that has been used for this project is MS Visual Studio 2013.
 
For HW4, the grader just needs to,

1)	Build the solution
2)	Run the project
3)	The output file is shown in “output.ppm”.

For Gouraud shading: Keep #if 1

#if 1
        interpStyle = GZ_COLOR;         /* Gouraud shading */
#else 
        interpStyle = GZ_NORMALS;       /* Phong shading */
#endif


For Phong shading: Keep #if 0

#if 0
        interpStyle = GZ_COLOR;         /* Gouraud shading */
#else 
        interpStyle = GZ_NORMALS;       /* Phong shading */
#endif


For Flat shading: I have added a new #define for flat shading in gz.h

#define GZ_FLAT				0	/*flatcoloring*/

To view flat shading output set the interpStyle = GZ_FLAT; in Application4.cpp

Currently the phong shading mode is selected.
