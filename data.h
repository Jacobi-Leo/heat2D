#include<stdio.h>
#define CX 0.000002
#define CY 0.000002
/****************************************************************************
 *  subroutine update
 ****************************************************************************/
extern void update(int, int, double *, double *);
extern void inidat(int, int, double *);
extern void prtdat(int, int, double *, char *);
void update(nx, ny, u1, u2)
    int nx, ny;
    /*float u1[nx][ny], u2[nx][ny];*/
    double *u1, *u2;
{
    int ix, iy;

    for (ix = 1; ix <= nx-2; ix++)
    {
        for (iy = 1; iy <= ny-2; iy++)
        {
            *(u2+ix*ny+iy) = *(u1+ix*ny+iy) +
                CX * (*(u1+(ix+1)*ny+iy) + *(u1+(ix-1)*ny+iy) - 2.0 * *(u1+ix*ny+iy)) +
                CY * (*(u1+ix*ny+iy+1) + *(u1+ix*ny+iy-1) - 2.0 * *(u1+ix*ny+iy));
        }
    }
}

/*****************************************************************************
 *  subroutine inidat
 *****************************************************************************/
void inidat(nx, ny, u1)
    int nx, ny;
    /*float u1[nx][ny];*/
    double *u1;
{
    int ix, iy;

    for (ix = 0; ix <= nx-1; ix++)
    {
        for (iy = 0; iy <= ny-1; iy++)
        {
            /* u1[ix][iy] = (float)(ix * (nx - ix - 1) * iy * (ny - iy - 1)); */
            *(u1+ix*ny+iy) = (float)(ix * (nx - ix - 1) * iy * (ny - iy - 1));
        }
    }
    for (ix = 0; ix <= nx-1; ix++) {
        *(u1+ix*ny) =  100.0;
    }
}

/**************************************************************************
 * subroutine prtdat
 **************************************************************************/
void prtdat(nx, ny, u1, fnam)
    int nx, ny;
    /*float u1[nx][ny];*/
    double *u1;
    char *fnam;
{
    int ix, iy;
    FILE *fp;

    fp = fopen(fnam, "w");
    for (iy = ny-1; iy >= 0; iy--)
    {
        for (ix = 0; ix <= nx-1; ix++)
        {
            fprintf(fp, "%8.3f", *(u1+ix*ny+iy));
            if (ix != nx-1)
            {
                fprintf(fp, " ");
            }
            else
            {
                fprintf(fp, "\n");
            }
        }
    }
    fclose(fp);
    printf("Wrote file: %s\n",fnam);
}
