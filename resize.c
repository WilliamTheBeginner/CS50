// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    int *n;

    int x = atoi(argv[1]);

    n = (int*) malloc(sizeof(int));

    n = &x;

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    BITMAPFILEHEADER n_bf;

    n_bf = bf;

    BITMAPINFOHEADER n_bi;

    n_bi = bi;

    n_bi.biWidth = bi.biWidth * *n;
    n_bi.biHeight = bi.biHeight * *n;

    // determine padding for scanlines

    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine padding with the addition of scaled scanlines

    int n_padding = (4 - (n_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + padding) * abs(bi.biHeight);

    n_bi.biSizeImage = (n_bi.biWidth * sizeof(RGBTRIPLE) + n_padding) * abs(n_bi.biHeight);

    n_bf.bfSize = n_bi.biSizeImage + 54;

    // write outfile's BITMAPFILEHEADER

    fwrite(&n_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&n_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int k = 1; k < *n; k++)
        {
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile

                for (int m = 0; m < *n; m++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

                }

                // add padding

                for (int l = 0; l < n_padding; l++)
                {
                    fputc(0x00, outptr);
                }

                int seek_n = bi.biWidth * sizeof(RGBTRIPLE);

                fseek(inptr, seek_n + padding, SEEK_CUR);

            }

            // add padding

            //for (int l = 0; l < n_padding; l++)
            //{
             //   fputc(0x00, outptr);
            //}

            // send file cursor back to beginning of the row
            //fseek(inptr, -bi.biWidth * 3 + padding, SEEK_CUR);
            //fseek(inptr, bi.biWidth * sizeof(RGBTRIPLE) + padding, SEEK_CUR);

        }

        // skip over padding
        fseek(inptr, padding, SEEK_CUR);

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
