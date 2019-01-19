#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <pthread.h>

#define xstart 0
#define xend 800
#define ystart 0
#define yend 500

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

const char zero[][32] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

void assignColor(char *fbp, long int location, int a, int r, int g, int b){
	*(fbp + location) = b;
    *(fbp + location + 1) = g;
    *(fbp + location + 2) = r;
    *(fbp + location + 3) = a;
}

void assignColorLocation(char *fbp, long int location1, long int location2){
    *((unsigned int *) (fbp + location1)) = *((unsigned int *) (fbp + location2));
}

void clearWindow(char *fbp, long int screensize){
	// for (int  y = ystart; y < yend; y++ )
    //     for (int x = xstart; x < xend; x++ ) {
    //         long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
    //                    (y+vinfo.yoffset) * finfo.line_length;

    //         assignColor(fbp, location, 0, 0, 0, 0);
    //     }

    // Super Optimize
    memset(fbp, 0, ((xend+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                    (yend+vinfo.yoffset) * finfo.line_length));
}

void moveWindowUp(char *fbp, long int screensize){
    /* F*cking slow code */
	// for (int  y = ystart; y <= yend-1; y++ ){
    //     for (int x = xstart; x <= xend; x++ ) {
    //         long int location1 = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
    //                    (y+vinfo.yoffset) * finfo.line_length;
    //         long int location2 = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
    //                    (y+1+vinfo.yoffset) * finfo.line_length;

    //         assignColorLocation(fbp, location1, location2);
    //     }
	// }
    // for (int x = xstart; x <= xend; x++ ) {
    //     long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
    //                (yend +vinfo.yoffset) * finfo.line_length;

    //     assignColor(fbp, location, 0, 0, 0, 0);
    // }

    /* Greater Optimization */
    for (int  y = ystart; y <= yend-1; y++ ){
        long int location1 = (vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                    (y+vinfo.yoffset) * finfo.line_length;
        long int location2 = (vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                    (y+1+vinfo.yoffset) * finfo.line_length;
        memcpy(fbp+location1, fbp+location2, (1+vinfo.yoffset) * finfo.line_length);
	}
    memset(fbp + (yend +vinfo.yoffset) * finfo.line_length, 0, (1+vinfo.yoffset) * finfo.line_length);
}

int main()
{
    int fbfd = 0;
    long int screensize = 0;
    char *fbp = 0;
    // int x = 0, y = 0;
    long int location = 0;

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (!fbfd) {
        printf("Error: cannot open framebuffer device.\n");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
        printf("Error reading fixed information.\n");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
        printf("Error reading variable information.\n");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel );

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory	
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
                       fbfd, 0);
    if ((int)fbp == -1) {
        printf("Error: failed to map framebuffer device to memory.\n");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    // x = 100; y = 100;       // Where we are going to put the pixel

    // const char *s[10];
    // s[0] = "Kelompok Hehe";
    // s[1] = "13516010 - Tony - Pontianak";
    // s[2] = "13516099 - HOHOHO - HHHHH";
    printf("mi : %ld\n", 1L*(xstart+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                   1L*(ystart +vinfo.yoffset) * finfo.line_length);
    printf("ma : %ld\n", 1L*(xend+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                   1L*(yend +vinfo.yoffset) * finfo.line_length);
    // pastikan nilai ma < limit ( screensize )
    printf("limit : %ld\n", screensize);
	clearWindow(fbp, screensize);

	for(int i=0;;i=(i+1)%40){
		moveWindowUp(fbp, screensize);
		if(i < 37){
			for(int j=0;j<32;++j){
	            long int location = (j+xstart+10+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
	                       (yend+vinfo.yoffset) * finfo.line_length;

	            assignColor(fbp, location, (zero[i][j])*0, (zero[i][j])*255, (zero[i][j])*255, (zero[i][j])*255);
			}
		}
	}

    // for(int i=0;i<n;++i){
    // 	int len = strlen(s[i]);
    // 	for(int j)
    // }

    // // Figure out where in memory to put the pixel
    // for ( y = 100; y < 700; y++ )
    //     for ( x = 100; x < 1260; x++ ) {

    //         location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
    //                    (y+vinfo.yoffset) * finfo.line_length;

    //         if ( vinfo.bits_per_pixel == 32 ) {
    //             *(fbp + location) = 100;        // Some blue
    //             *(fbp + location + 1) = 15+(x-100)/2;     // A little green
    //             *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
    //             *(fbp + location + 3) = 0;      // No transparency
    //         } else  { //assume 16bpp
    //             int b = 10;
    //             int g = (x-100)/6;     // A little green
    //             int r = 31-(y-100)/16;    // A lot of red
    //             unsigned short int t = r<<11 | g << 5 | b;
    //             *((unsigned short int*)(fbp + location)) = t;
    //         }

    //     }
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}