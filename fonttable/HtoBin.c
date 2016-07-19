/******************************************************************
 *** HtoBin.c Convert Font C source (.h file) to Bin resource file for Pebble watch
 ***
 ***
 ***  This C source is compiled with the free tiny C compiler from www.tinycc.org
 ***
 ***  Ver 1.0 4 July 2016
 ***
 ***********************************************************************/
#include <stdio.h>
#include <stdbool.h>

#define CR 13
#define LF 10

int main(int argc, char **argv) 
{
	int lineCnt, len, i, j, width, height;
	unsigned long binaryVal, bitValue;
	char srcFile[50], dstFile[50], data[256];
	char* p;
		
	FILE *srcFP, *dstFP;
	
	if (argc < 2)
	{
		printf("\n  This program will convert a font source file into a binary file");
		printf("\nUsage is: \n  HtoBin xxxxx.h \n");
		exit(0);
	}

	strcpy(srcFile, argv[1]);

	if (srcFile == NULL)
	{
		printf("  Please start HtoBin with the name of a .h font file:\n");
		printf("  Example: HtoBin mod32x2R.h \n");
		exit(1);
	}
			
	strcpy(dstFile, srcFile);
	i = strlen(srcFile);
	
	if ((i > 2) && (srcFile[i - 2] == '.'))
	{
		dstFile[i - 1] = 'b';
		dstFile[i - 0] = 'i';
		dstFile[i + 1] = 'n';
		dstFile[i + 2] = 0x0;
	}
	else
	{
		strcat(dstFile, ".bin");
	}
	
	srcFP = fopen(srcFile,"r");
 	if ( NULL == srcFP )
	{
		printf("  Could not open input file %s for reading.", srcFile);
		exit(1);
	}
	
	dstFP = fopen(dstFile,"wb");
	if ( NULL == dstFP )
	{
		printf("  Could not open output file %s for writing.", dstFile);
		exit(1);
	}
	
	lineCnt = 0;
	do
	{
		fgets(data, 200, srcFP);		//read till first font line
		++lineCnt;
	} while (data[2] != '{');

	do
	{
		do
		{
			fgets(data, 200, srcFP);		
			++lineCnt;
			if (data[2] == '}')
				break;
			len = strcspn(data, ",") - 6;

			bitValue = 0x80;
			if (len > 8)					
				bitValue = 0x8000;
				
			if (len > 16)
				bitValue = 0x80000000;
				
			binaryVal = 0;
				
			for (j = 0; j < len; ++j)
				binaryVal += (data[6 + j] - 0x30) * (bitValue >> j);
//printf(" %ld  %s %d\n", binaryVal, data, len);				
			if (len > 16)
			{
				fputc((binaryVal >> 24) & 0xff, dstFP);
				fputc((binaryVal >> 16) & 0xff, dstFP);
				fputc((binaryVal >>  8) & 0xff, dstFP);
				fputc(binaryVal & 0xff, dstFP);
			}
			else
			{
				if (len > 8)
				{
					fputc((binaryVal >>  8) & 0xff, dstFP);
					fputc(binaryVal & 0xff, dstFP);
				}
				else
				{	
					fputc(binaryVal, dstFP);
				}	
			}
		} while (true);
		
		fgets(data, 200, srcFP);		//read 
		++lineCnt;
//printf(" Line: %d ", lineCnt);
		if ((data[0] == '}') && (data[1] == ';'))
			break;
		if (data[2] != '{')
		{
			printf("NO open { at line %d \n", lineCnt);
			exit(1);
		}	
	} while	(true);
	
	fputc(0x0, dstFP);

	fclose(dstFP);
	fclose(srcFP);
	printf(" %s Ready  \n", dstFile);	
}
