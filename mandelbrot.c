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
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* I don't want to use the predefined complex function provided 
   by some environments like VC */
typedef struct cplx
{
	double r;
	double i;
}
cplx;

const char* format_complex(const cplx *c);
double modulus_complex(const cplx *c);
cplx func1(const cplx *c1, const cplx *c2);

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

double modulus_complex(const cplx *c)
{
	/* the modulus ("length") of a complex number is calculated the 
	   same way as with vectors
	   (a^2+b^2)^(1/2) */
	return sqrt(c->r*c->r+c->i*c->i);
}

/*
 * helper function to format a complex number into
 * a string for printf
 */
const char* format_complex(const cplx *c)
{
	char *str = NULL;
	str = (char *) malloc(sizeof(char) * 128);
	sprintf(str,"(%f,%fi)",c->r,c->i);
	return str;
}

int main(int argc, char **argv)
{
	cplx c1, c2, r;
	
	c1.r=c1.i=0;

	/*
	 * for each pixel in our image, we calculate the orbit of the complex polynomial
	 * x^2+c and check if it diverges or not
	 *
	 * if it diverges, we colorize the pixel appropriatly by choosing a color corresponding
	 * to the iterations it took to reach the threshold. If it does not diverge, we simply
	 * paint it black (whitespace)
	 */
	double x, y;
	for(y=-2;y<2;y+=0.10)		/* 40 lines tall */
	{
		for(x=-2;x<2;x+=0.04)	/* 100 columns wide */
		{
			/* complex c is chosen according to the position in the coordinate system. We
			   simply use -2 .. 2 */
			 
			c2.r=x;			/* real part of c is the x position on the coordinate system		*/
			c2.i=y;			/* imaginary part of c is the y position on the coordinate system	*/
			r=c1;			/* if we change x/y, we get a image rotated by 90 degrees.			*/
						
			/* Maximum iteration depth is 100 */
			int div=0, iter=0;
			for(iter=0;iter<100;iter++)
			{
				r=func1(&r,&c2);
				if(modulus_complex(&r)>100)
				{
					/* its diverging, set at which iteration we detected the 
					   divergation and exit the iteration loop */
					div=iter;
					break;
				}
			}
			
			/* Depending on the level we broke out of the iteration we use different
			   symbols to draw the pixel */
			if(div==0)
				fprintf(stdout," ");
			else if(div>0 && div<=25)
				fprintf(stdout,".");
			else if(div>25 && div<=50)
				fprintf(stdout,":");
			else if(div>50 && div<=75)
				fprintf(stdout,"*");
			else
				fprintf(stdout,"@");
		}
		fprintf(stdout,"\n");
	}

	/* print the orbit of 0 for x^2+c where c=(0,2i)
	   ==> orbit tends to infinity */
	   
	/*
	c1.r=c1.i=0;
	c2.r=0; c2.i=2;
	r=c1;
		
	for(int i=0;i<6;i++)
	{
		complex r=func1(&r,&c2);
		fprintf(stdout,"%s\n",format_complex(&r));
	}
	return 0;
    */
}