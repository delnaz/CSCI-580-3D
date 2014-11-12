Assignment 5 for CSCI580


The Visual Studio version that has been used for this project is MS Visual Studio 2013.
 
For HW5, the grader just needs to,

1)	Build the solution
2)	Run the project
3)	The output file is shown in “output.ppm”.

For tex_fun: Keep #if 0

#if 0   /* set up null texture function or valid pointer */
        valueListShader[5] = (GzPointer)0;
#else
        valueListShader[5] = (GzPointer)(tex_fun);	/* or use ptex_fun */


For ptex_fun: Keep #if 0 and change text_fun to ptex_fun

#if 0   /* set up null texture function or valid pointer */
        valueListShader[5] = (GzPointer)0;
#else
        valueListShader[5] = (GzPointer)(ptex_fun);	/* or use ptex_fun */

 in Application5.cpp

Currently the phong shading mode with tex_fun is selected.
