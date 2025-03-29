/**
 * @file BatchMandelCalculator.cc
 * @author EMMA KROMPASCIKOVA <xkromp00@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over small batches
 * @date November 2024
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <stdexcept>
#include "BatchMandelCalculator.h"


const int BATCH_SIZE = 64;

BatchMandelCalculator::BatchMandelCalculator (unsigned matrixBaseSize, unsigned limit) :
	BaseMandelCalculator(matrixBaseSize, limit, "BatchMandelCalculator")
{
	data = (int *)aligned_alloc(64, height * width * sizeof(int));
	real = (float *)aligned_alloc(64, width * sizeof(float));  
	imag = (float *)aligned_alloc(64, width *  sizeof(float)); 
}

BatchMandelCalculator::~BatchMandelCalculator() {
	free(data);
	free(real);
	free(imag);
	data = NULL;
	real = NULL;
	imag = NULL;
}


int * BatchMandelCalculator::calculateMandelbrot () {
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
		for (int batch = 0; batch < width/BATCH_SIZE; batch++)	{
			for (int b = 0; b < BATCH_SIZE; b++){
				pimag[b] = y;
				preal[b] = x_start + (b+batch*BATCH_SIZE) * dx;
			} 
			int colored = 0;
			for (int iteration = 0; iteration < limit; iteration++){
				#pragma omp simd aligned(pdata, preal, pimag: 64) reduction(+:colored)
				for (int j = 0; j < BATCH_SIZE; j++){	
					float x = x_start + (batch*BATCH_SIZE + j) * dx; // current real value
					float r2 = preal[j] * preal[j] ;
					float i2 = pimag[j] * pimag[j] ;
					pimag[j] = 2.0f * preal[j] * pimag[j] + y;
					preal[j] = r2 - i2 + x;
					if (r2 + i2 > 4.0f && pdata[i * width + (batch*BATCH_SIZE +  j)] == limit){ //unstable
						pdata[i * width + (batch*BATCH_SIZE + j)]  = iteration;
						colored++;
					} 
				}
				if (colored==BATCH_SIZE){ //all columns in one line are full
					break; //early-stopping
				}
			}	
		}
	}

	#pragma omp simd aligned(data: 64)
	for (int i = 0; i < height / 2; i++) {
		memcpy(&data[(height - 1 - i) * width], &data[i * width], width * sizeof(int));
	}
	return data;
}