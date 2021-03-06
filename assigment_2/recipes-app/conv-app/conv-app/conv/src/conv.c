
#define PNGSUITE_PRIMARY
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_DEFINE
#include "stb.h"

#include <string.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <sys/ioctl.h>		/* ioctl */
#include <stdio.h>
#include <stdlib.h>
#include "../driver/chardev.h"

void write_image (char *srcfile, int file_desc);
void print_autor();
void print_help();
int open_device();
void ioctl_set_mode(int file_desc, int value);
void read_image(int file_desc, char *outfile);
int w = 512;
int h = 512;
int depth = 3;
uint8 *org_image;

int main(int argc, char **argv)
{

	int c;
	char *src_image = NULL;
	char *out_image = NULL;
	int kernel = -1;
	int file;

//This while is used to read the arguments added to through the command line to the application
	while ((c = getopt (argc, argv, "i:o:m:ha")) != -1){
/*
The following switch will identify the arguments read, and it will call the proper method
and set the proper variables in order to execute the required methods
*/

		switch (c) {
			case 'a':
			//'a' means the autor information will be printed
				print_autor();
				break;
			case 'i':
			//'b' It will have the path for the binary file to be executed
				src_image = optarg;
				break;			
			case 'h':
			//'h' is used to print the help menu
				print_help();
				break;
			case 'o':
			//'w' has the address which will be monitored during the test execution
				out_image = optarg;
				break;
			case 'm':
			//'s' will execute the binary file step by step
				kernel = atoi(optarg);
				break;

			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				        return 1;
			default:
			//In case the argument read is not supported
				printf("Option '-%i' not recognized.\n",c);
			}
	}

	if(src_image == NULL && out_image == NULL){
		fprintf(stderr, "-Error- Argument -i <source_image> or Argument -o <output_image> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else if(src_image != NULL){
		if(!access(src_image, F_OK)){
			fprintf(stderr, "-Info- Convolution program will use the image %s as the source image\n", src_image);
		}else{
			fprintf(stderr, "-Error- Argument -i <source_image> must exist\n-Error- Use -h to see the help menu\n");
			exit(0);
		}
	}
	
	if(out_image != NULL){
		fprintf(stderr, "-Info- Convolution program will use the image %s as the output image\n", out_image);
	}

	if(kernel <= 0 || kernel >= 6){
		fprintf(stderr, "-Error- Argument -m <kernel desired> is required, and must be set to a valid value\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stderr, "-Info- Convolution program will apply the kernel number %i to the src image\n", kernel);
	}

	file = open_device();
	if(src_image != NULL)	write_image(src_image, file);
	ioctl_set_mode(file, kernel);
	ioctl_start_mode(file);
	if(out_image != NULL)	read_image(file, out_image);

//	run_kernel(src_image, out_image, kernel);

	return 0;

}

/* open_device(): This method is used to open the device 
                  In case of error the method open will return a value different to 0
*/

int open_device(){
	int ret_val;
	static int file_desc;
	file_desc = open(DEVICE_FILE_NAME,2);
	if (file_desc < 0) {
		printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
		exit(-1);
	}
	return file_desc;

}

/* Method used to send through IOCTL the kernel choosen from command line */

void ioctl_set_mode(int file_desc, int value)
{
	int ret_val;
	ret_val = ioctl(file_desc,IOCTL_SET_MODE, value);
	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
}

void ioctl_start_mode(int file_desc){
	int ret_val;
	ret_val = ioctl(file_desc,IOCTL_START_CONV, 0);
	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
}

void read_image (int file_desc, char *outfile){
	uint8 *result;
	int image_size = w*h*depth;
	result = (uint8 *) malloc(sizeof(uint8)*image_size);
	memset( result, 0, image_size);
	read(file_desc, result, image_size);
//	for(int i = 0 ; i < image_size ; i++)
//	{
//		printf("app-jecampos %x\n",result[i]);
//	}
	printf("------- Image Information -----------\n");
	printf("-I- Src Image width     %i\n", w);	
	printf("-I- Src Image height    %i\n", h);	
	printf("-I- Src Image depth     %i\n", depth);

	stbi_write_bmp(outfile,w,h,depth,result);
	close(file_desc);

}

void write_image (char *srcfile, int file_desc){

//	uint8 *org_image;
	int *x; int *y; int req_comp;
//	char tu[5]="A1223";
	org_image = stbi_load(srcfile, &w, &h, &depth, req_comp);
	int image_size = w*h*depth;
	write(file_desc, org_image, image_size);
//	write(file_desc, tu, sizeof(char));
}

//This method will print the autors information
void print_autor(){
	printf("#############################################\n");
	printf("#           Convolution tool                #\n");
	printf("#############################################\n");
	printf("# Autores: Marco Espinoza Murillo           #\n");
	printf("#          Jose Campos Murillo              #\n");
	printf("# Maestria Sistemas Embebidos               #\n");
	printf("# Instituo Tecnologico de Costa Rica        #\n");
	printf("#############################################\n");
	exit(0);
}


//This method will print the help menu

void print_help(){
	printf("####################################################################################\n");
	printf("#                               Convolution tool                                   #\n");
	printf("####################################################################################\n");
	printf("# Valid arguments:                                                                 #\n");
	printf("#   -a: It shows the program autor information                                     #\n");
	printf("#   -h: It shows this help menu                                                    #\n");
	printf("#   -i: It receive the source image to apply the kernel                            #\n");
	printf("#   -m: It is the kernel to apply to the image, one of the follows can be applied: #\n");
	printf("#            1: kernel_left_sobel                                                  #\n");
	printf("#            2: kernel_identity                                                    #\n");
	printf("#            3: kernel_outline                                                     #\n");
	printf("#            4: kernel_sharpen                                                     #\n");
	printf("#            5: kernel_topsobel                                                    #\n");
	printf("#   -o: It receive the name for the output image with the kernel applied           #\n");
	printf("#                                                                                  #\n");
	printf("# Command line examples                                                            #\n");
	printf("#  ./conv -i foto1.jpg   -o result.bmp -m 5                                        #\n");
	printf("#  ./conv -i foto2.jpg   -o result.bmp -m 2                                        #\n");
	printf("#  ./conv -i foto3.bmp   -o result.bmp -m 6                                        #\n");
	printf("#  ./conv -i lena512.bmp -o result.bmp -m 3                                        #\n");
	printf("####################################################################################\n");
	exit(0);
	
}

