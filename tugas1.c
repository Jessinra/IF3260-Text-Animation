#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>


#define xstart 0
#define xend 1100
#define ystart 0
#define yend 700
#define framejump 4

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
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

int fbfd = 0;
int screensize = 0;
char *fbp = 0;
int xmultiplier;
int ymultiplier;
int xadder;
int yadder;
int height;
int width;
char matrix[1000][1000];

int countlines(char *filename)
{
  // count the number of lines in the file called filename                                    
  FILE *fp = fopen(filename,"r");
  int ch=0;
  int lines=0;

  if (fp == NULL);
  return 0;

  lines++;
  while ((ch = fgetc(fp)) != EOF)
    {
      if (ch == '\n')
    lines++;
    }
  fclose(fp);
  return lines;
}

void readMatrix(const char * file_name) {
    FILE *fp;
    char c[1000000];
    int line = 0;
    int index = 0;
    fp = fopen(file_name, "r"); //read mode
    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    fscanf(fp, "%d", &height);
    printf("%d\n", height);
    fscanf(fp, "%d", &width);
    printf("%d\n", width);
    //printf(countlines(file_name));
    for (int i=0; i<height; ++i) {
        fscanf(fp, "%s", matrix[i]);
    }
    fclose(fp);
}


void assignColor(int location, int c){
	// *(fbp + location) = b;
    // *(fbp + location + 1) = g;
    // *(fbp + location + 2) = r;
    *((unsigned int *) (fbp + location)) = c;
}

void assignColorLocation(int location1, int location2){
    *((unsigned int *) (fbp + location1)) = *((unsigned int *) (fbp + location2));
}

void clearWindow(){
	// for (int  y = ystart; y < yend; y++ )
    //     for (int x = xstart; x < xend; x++ ) {
    //         long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
    //                    (y+vinfo.yoffset) * finfo.line_length;

    //         assignColor(fbp, location, 0, 0, 0, 0);
    //     }

    // Super Optimize
    memset(fbp, 0, (xend*xmultiplier + xadder +
                    yend*ymultiplier + yadder));
}

void moveWindowUp(){
    for (int  y = ystart; y < yend-framejump; y += framejump){
        int location1 = xadder + y*ymultiplier + yadder;
        int location2 = location1 + framejump * ymultiplier;
        memcpy(fbp+location1, fbp+location2, framejump*(1+vinfo.yoffset) * finfo.line_length);
	}
    memset(fbp + (yend-framejump)*ymultiplier + yadder, 0, framejump*(ymultiplier + yadder));
}

int main()
{
    // int x = 0, y = 0;
    int location = 0;

    // read file
    readMatrix("result_D.txt");

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

    xmultiplier = (vinfo.bits_per_pixel/8);
    xadder = (vinfo.xoffset) * xmultiplier;
    ymultiplier = finfo.line_length;
    yadder = vinfo.yoffset * finfo.line_length;
    // x = 100; y = 100;       // Where we are going to put the pixel

    // const char *s[10];
    // s[0] = "Kelompok Hehe";
    // s[1] = "13516010 - Tony - Pontianak";
    // s[2] = "13516099 - HOHOHO - HHHHH";
    printf("mi : %d\n", (xstart+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                   (ystart +vinfo.yoffset) * finfo.line_length);
    printf("ma : %d\n", (xend+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                   (yend +vinfo.yoffset) * finfo.line_length);
    // pastikan nilai ma < limit ( screensize )
    printf("limit : %d\n", screensize);
    // double now = clock();
	clearWindow();
    // printf("c : %lf\n", (clock() - now) / CLOCKS_PER_SEC);

	for(int i=0;;i=(i+framejump)%44){
        // now = clock();
		moveWindowUp();
        // printf("m : %lf\n", (clock() - now) / CLOCKS_PER_SEC);
		if(i < 40){
            for(int j=0;j<framejump;++j){
                for(int k=0;k<32;++k){
                    int location = (k+xstart+10)*xmultiplier + xadder+
                            (yend-framejump + j)*ymultiplier + yadder;

                    assignColor(location, (matrix[i+j][k]-'0')*((255<<16)+(255<<8)+255));
                }
            }
		}
        // printf("a : %lf\n", (clock() - now) / CLOCKS_PER_SEC);
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