/**
 * @file LineMandelCalculator.cc
 * @author EMMA KROMPASCIKOVA <xkromp00@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over lines
 * @date November 2024
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include "LineMandelCalculator.h"

LineMandelCalculator::LineMandelCalculator (unsigned matrixBaseSize, unsigned limit) :
	BaseMandelCalculator(matrixBaseSize, limit, "LineMandelCalculator")
{
	data = (int *)aligned_alloc(64, height * width * sizeof(int));
	real = (float *)aligned_alloc(64, width * sizeof(float));  
	imag = (float *)aligned_alloc(64, width *  sizeof(float)); 
}

LineMandelCalculator::~LineMandelCalculator() {
	free(data);
	free(real);
	free(imag);
	data = NULL;
	real = NULL;
	imag = NULL;
}

int * LineMandelCalculator::calculateMandelbrot () {
	int *pdata = data;
	float *preal = real;
	float *pimag = imag;

	//filling before
	for (int i = 0; i < height/2; i++){
		#pragma omp simd aligned(pdata: 64)
		for (int j = 0; j < width; j++){
			pdata[i * width + j] = limit;
		}
	}
	
	for (int i = 0; i < height/2; i++){
		float y = y_start + i * dy; // current imaginary value

		#pragma omp simd aligned(preal, pimag: 64)
		for ( int row_width = 0; row_width < width; row_width++){
			pimag[row_width] = y;
			preal[row_width] = x_start + row_width * dx;
		} 

		int colored = 0;
		for (int iteration = 0; iteration < limit; iteration++){
			
			#pragma omp simd aligned(pdata, preal, pimag: 64) reduction(+:colored)
			for (int j = 0; j < width; j++){		
				float x = x_start + j * dx; // current real value
				float r2 = preal[j] * preal[j];
				float i2 = pimag[j] * pimag[j];
				pimag[j] = 2.0f * preal[j] * pimag[j] + y;
				preal[j] = r2 - i2 + x;
				if (r2 + i2 > 4.0f && pdata[i * width + j] == limit){ //unstable
					pdata[i * width + j]  = iteration;
					colored++;
				} 
            }
			if (colored==width){ //all columns in one line are full
				break; //early-stopping
			}
		}
	}

	#pragma omp simd aligned(data: 64)
	for (int i = 0; i < height / 2; i++) {
		memcpy(&data[(height - 1 - i) * width], &data[i * width], width * sizeof(int));
	}
	return data;
}
