// Author: Brian Rieder
// Title: PA05-ImageNormalization.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

Image * Image_load(const char * filename)
{
	FILE * fp = NULL;
	ImageHeader header;
	Image * image = NULL;
	Image * temp_image = NULL;
	int num_read = 0;
	int error_flag = FALSE;

	// Attempt to open file for reading
	if(!error_flag) {
		fp = fopen(filename, "rb");
		if(fp == NULL) error_flag = TRUE;
		if(error_flag) fprintf(stderr, "Failed to open %s\n", filename);
	}

	// Attempt to read header
	if(!error_flag) {
		num_read = fread(&header, 1, sizeof(ImageHeader), fp);
		error_flag = num_read != sizeof(ImageHeader) 
			|| header.magic_number != ECE264_IMAGE_MAGIC_NUMBER
	    	|| header.width == 0
	    	|| header.height == 0
	    	|| header.comment_len == 0;
	    if(error_flag) fprintf(stderr, "Error reading header\n");
	}

	// Allocate space for the temporary image
	if(!error_flag) {
		temp_image->width = header.width;
		temp_image->height = header.height;
		temp_image->comment = malloc(header.comment_len);
		temp_image->data = malloc(header.width * header.height);
		if (temp_image->comment == NULL || temp_image->data == NULL) error_flag = TRUE;
		if(error_flag) fprintf(stderr, "Failed to malloc memory for comment/pixels\n");
	}

	// Read comment
	if(!error_flag) {
		num_read = fread(temp_image->comment, 1, header.comment_len, fp);
		if(num_read != header.comment_len) error_flag = TRUE;
		if(error_flag) fprintf(stderr, "Failed to read image comment\n");
	}
	
	// Check null byte
	if(!error_flag) {
		if(temp_image->comment[header.comment_len - 1] != '\0') error_flag = TRUE;
		if(error_flag) fprintf(stderr, "No terminating null byte on comment\n");
	}

	// Read image data
	if(!error_flag) {
		num_read = fread(temp_image->data, 1, header.width * header.height, fp);
		if(num_read != header.width * header.height) fprintf(stderr, "Failed to read image data\n");
	}

	// Make sure whole file was read
	if(!error_flag) {
		fgetc(fp);
		error_flag = (!feof(fp));
		if(error_flag) fprintf(stderr, "Whole file was not parsed (EOF misplaced)\n");
	}

	// Send it to the real thing and go home
	if(!error_flag) {
		image = temp_image;
		temp_image = NULL;
	}
	if(fp) fclose(fp);
	Image_free(temp_image);
	return image;
}

int Image_save(const char * filename, Image * image)
{
    FILE * fp = fopen(filename, "wb");
    if(fp == NULL) {
		fprintf(stderr, "Failed to open %s\n", filename);
		return FALSE;
    }

    //Initialize necessary parameters
    int success = TRUE;
    ImageHeader header;
    header.magic_number = ECE264_IMAGE_MAGIC_NUMBER;
    header.width = image->width;
    header.height = image->height;
    header.comment_len = strlen(image->comment) + 1;
    int num_written = fwrite(&header, sizeof(ImageHeader), 1, fp);

    // Start writing...
    if(num_written != 1) {
		fprintf(stderr, "Failed to write header to %s\n", filename);
		success = FALSE;
    }

    num_written = fwrite(image->comment, sizeof(char), header.comment_len, fp);
    if(num_written != header.comment_len) {
		fprintf(stderr, "Failed to write comment to %s\n", filename);
		success = FALSE;	
    }

    int num_pixels = image->width * image->height;
    num_written = fwrite(image->data, sizeof(uint8_t), num_pixels, fp);
    if(num_written != num_pixels) {
		fprintf(stderr, "Failed to write data to %s\n", filename);
		success = FALSE;	
    }

    // Clean up and go home
    fclose(fp);
    return success;
}

void Image_free(Image * image)
{
  if(image!=NULL) {
      free(image->data);
      free(image->comment);
  }
  free(image);
}

void linearNormalization(int width, int height, uint8_t * intensity)
{
    int ind;
    int n_pixels = width * height;

    int max_intensity = 0;
    int min_intensity = 255;
  
    for(ind = 0; ind < n_pixels; ++ind) {
		if(intensity[ind] > max_intensity) 
		    max_intensity = intensity[ind];
		if(intensity[ind] < min_intensity) 
		    min_intensity = intensity[ind];
    }

    double range = max_intensity - min_intensity;
    for(ind = 0; ind < n_pixels; ++ind)
		intensity[ind] = (intensity[ind] - min_intensity) * 255.0 / range;
}
