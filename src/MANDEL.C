/*
 * Draws the Mandelbrot set on the console. See the following pages for a detailed
 * description on what it is and how it is calculated in general:
 *
 * https://web.archive.org/web/20230328052353/https://plus.maths.org/content/what-mandelbrot-set
 * https://web.archive.org/web/20230525074817/https://plus.maths.org/content/unveiling-mandelbrot-set
 * https://web.archive.org/web/20220929135724/https://plus.maths.org/content/computing-mandelbrot-set
 *
 * Take a look at it, interesting reads!
 *
 * History:
 * 		06/19/2023 egrath: initial program
 *		07/26/2023 egrath: some adaptions for borland turbo c++
 *      08/24/2023 egrath: made the output graphical using BGI in CGA mode
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <graphics.h>


#define XFROM   		-1.6
#define XTO				 0.5
#define YFROM			 1.1
#define YTO				-1.1
#define MAXITERATIONS	 100
#define CONVLIMIT		 100

/* I don't want to use the predefined complex function provided
   by some environments like VC */
typedef struct cplx

{
	double r;
	double i;
}
cplx;

const char* cplx_format(const cplx *c);
double cplx_modulus(const cplx *c);
cplx func1(const cplx *c1, const cplx *c2);

void graphics_enter();
void graphics_plot_pixel(int x, int y, int color);
void graphics_restore();

/*
 * calculate the complex result of the quadratic polynomial
 * x^2+c, where x=c1 and c=c2
 */
cplx func1(const cplx *c1, const cplx *c2)
{
	cplx result;

	/*
	result = (c1)^2 => (r+i)(r+i)=r^2+r*i+i*r+i^2
	i^2=-1
	*/
	result.r=c1->r*c1->r;
	result.i=c1->r*c1->i+c1->i*c1->r;
	result.r+=c1->i*c1->i*(-1);

	/* result=result+c2 */
	result.r+=c2->r;
	result.i+=c2->i;

	return result;
};

double cplx_modulus(const cplx *c)
{
	/* the modulus ("length") of a complex number is calculated the
	   same way as with vectors
	   (a^2+b^2)^(1/2) */
	return sqrt(c->r*c->r+c->i*c->i);
}

/*                                      c
 * helper function to format a complex number into
 * a string for printf
 */
const char* cplx_format(const cplx *c)
{
	char *str = NULL;
	str = (char *) malloc(sizeof(char) * 128);
	sprintf(str,"(%f,%fi)",c->r,c->i);
	return str;
}

void graphics_enter()
{
	int driver, mode, error;

	/* we need to register the graphics driver which was generated
	   by bgiobj. The resulting obj file is linked to out application */
	error=registerbgidriver(CGA_driver);
	if(error<0)
	{
		fprintf(stderr,"failed to register CGA driver, was:\n%s",
			grapherrormsg(error));
		exit(0);
	}

	driver=CGA;
	mode=CGAC0;
	initgraph(&driver,&mode,".\\");

	error=graphresult();
	if(error!=grOk)
	{
		fprintf(stderr,"failed to initialize graphics, was:\n%s",
			grapherrormsg(error));
		exit(0);
	}
}

void graphics_plot_pixel(int x, int y, int color)
{
	putpixel(x,y,color);
}

void graphics_restore()
{
	closegraph();
}

int main(void)
{
	cplx c1, c2, r;
	double x, y, modulus, xstep, ystep;
	int div, iter, pixel_x, pixel_y;
	clock_t start, stop;
	char* statustext=NULL;

	/* Initialize the graphics subsystem (CGI 320x200) */
	graphics_enter();

	start=clock();
	c1.r=c1.i=0;

	/* calculate necessary stepping for x and y directions */
	xstep=(XTO-XFROM)/(getmaxx()+1);
	ystep=(YTO-YFROM)/(getmaxy()+1);

	/*
	 * for each pixel in our image, we calculate the orbit of the complex polynomial
	 * x^2+c (e.g calculate the series, starting with x_0=0)
	 * and check if it diverges or not.
	 *
	 * if it's diverging it's outside of the main cardioid and/or bulbs attached
	 * to it. We paint a green pixel in this case. If it's converging, we
	 * simply leave the corresponding pixel black.
	 *
	 */
	for(y=YFROM,pixel_y=0;y>=YTO;y+=ystep,pixel_y++)
	{
		for(x=XFROM,pixel_x=0;x<XTO;x+=xstep,pixel_x++)
		{
			/* complex c is chosen according to the position in the x/y plane  */

			c2.r=x;			/* real part of c is the x position on the coordinate system		*/
			c2.i=y;			/* imaginary part of c is the y position on the coordinate system	*/
			r=c1;			/* if we change x/y, we get a image rotated by 90 degrees.			*/
			div=0;			/* we assume it's not diverging */

			/* If ether the iteration count exceeds a given maximum or
			   the modulus of any computed item of the series exceeds
			   a limit, we assume that the series diverges to infinity */
			for(iter=0;iter<MAXITERATIONS;iter++)
			{
				r=func1(&r,&c2);
				modulus=cplx_modulus(&r);
				if(modulus>=CONVLIMIT)
				{
					/* we detected divergation, so just break the loop */
					div=1;
					break;
				}
			}

			/* draw a colored pixel at every position it's diverging and
			   leave it uncolored when it's converging */
			if(div)
				graphics_plot_pixel(pixel_x,pixel_y,5);
		}
	}
	stop=clock();

	/* print the information how long the calculation took */
	statustext=(char*) malloc(sizeof(char)*100);
	sprintf(statustext,"took: %.2f seconds",(stop-start)/CLK_TCK);

	setcolor(BLACK);
	outtextxy(8,8,statustext);
	outtextxy(8,18,"press any key to exit");
	free(statustext);

	getch();
	graphics_restore();

	return 0;
}