/******************************************************************
 *** Convert excel format .csv font file to a C .h include file
 ***
 ***  This C source is compiled with the free tiny C compiler from www.tinycc.org
 ***
 ***  Ver 1.0 4 July 2016
 ***
 ******************************************************************/
#include <stdio.h>
#include <stdbool.h>


#define CHARWIDTH 10
#define CHARHEIGHT 16

#define CR 13
#define LF 10

int main(int argc, char **argv) 
{
	int i, j, h, k, l, p, index, count1, linestart, len;
	int fontA[CHARHEIGHT * 256][CHARWIDTH + 1];				
	char srcFile[50], dstFile[50], data[256], str[256];
	
	FILE *srcFP, *dstFP;
	
	if (argc < 2)
	{
		printf("\n  This program will convert an Excel comma separated value file into");
		printf("\n  a .h font include file for the Pebble watch ");
		printf("\nUsage is: \n  CSVtoH fontxx.csv (or another input font file name) \n");
		exit(1);
	}

	strcpy(srcFile, argv[1]);

	if (srcFile == NULL)
	{
		printf("  Please start CSVtoH with the name of an Excel comma separated value file:\n");
		printf("  Example: CSVtoH fontxx.csv \n");
		exit(2);
	}
	strcpy(dstFile, srcFile);
	i = strlen(srcFile);
	
	if ((i > 4) && (srcFile[i - 4] == '.'))
	{
		dstFile[i - 3] = 'h';
		dstFile[i - 2] = 0x0;
	}
	else
	{
		strcat(dstFile, ".h");
	}
	
	srcFP = fopen(srcFile,"r");
 	if ( NULL == srcFP )
	{
		printf("  Could not open input file %s for reading.", srcFile);
		exit(3);
	}
	
	dstFP = fopen(dstFile,"wb");
	if ( NULL == dstFP )
	{
		printf("  Could not open output file %s for writing.", dstFile);
		exit(4);
	}

	count1 = 0;
	fgets(data, 255, srcFP);		//skip first line
	linestart = 1;
	for (i = 0; i < CHARHEIGHT * 255; ++i)
	{
		if (fgets(data, 255, srcFP) == false)
		{
			index = 99999;
			break;
		}
		len = strlen(data);
		++count1;
		if (len < 2)
		{
			printf("line %d, %s too short ", count1 + 2, data);
			exit(5);
		}
		if (data[0] != '0' + linestart)
		{
			printf(" Wrong line start %d, %s ", count1 + 2, data);
			exit(6);
		}	
		linestart = linestart << 1	;
		if (linestart > 8)
			linestart = 1;
			
		p = 2;
		for (j=0; j < CHARWIDTH + 1; j++)
		{
			if (data[p] == 'O')
			{
				fontA[i][j] = 1;
				++p;						
			}	
			else
				fontA[i][j] = 0;
								
			if (++p > len)						// skip 
				break;
		}
	}		
		
	printf("# of lines read %d \n", count1 + 1);

	for (i=0; i < 32; ++i)
	{
		for (j=0; j < CHARWIDTH + 1; ++j)
			printf(" %d", fontA[i][j]);				// debug info;
		printf("\n");	
	}
	
	for (index = 0; index < count1; index += CHARHEIGHT)
	{
		fputs("  {", dstFP);
		fputc(CR, dstFP);
		fputc(LF, dstFP);
		l = 0;
		for (j = 0; j < CHARWIDTH + 1; ++j)
		{
			k = 0;
			for (i=0; i < 16; ++i)
			{
				for (h=0; h < CHARWIDTH + 1; ++h)
				{
					if (fontA[index + i][h] == 1)	
						k = h;
				}		
				if (k > l)
					l = k;
			}

			strcpy(str, "    0b"); 
			for (i=0; i < CHARHEIGHT; ++i)
			{
				if ((fontA[index + i][j] == 1) || ((i == 0) && (j < l)))
				{
					strcat(str, "1");
				}	
				else	
				{
					if ((index == 0) && ((j == 1) || (j == 2)) && (i == 0))
					{
						strcat(str, "1");	// Make space 2 bit wide
					}
					else
					{		
						strcat(str, "0");
					}
				}	
			}
			strcat(str, ",");
			if (j > 0)
				fputs(str, dstFP);
			if (j == 1)
			{
				strcpy(str, "  //  ");
				str[5] = index/16 + 32; 
				str[6] = 0x0;
				fputs(str, dstFP);
			}	
			if (j > 0)
			{
				fputc(CR, dstFP);
				fputc(LF, dstFP);
			}	
		}	
		fputs("  },", dstFP);
		fputc(CR, dstFP);
		fputc(LF, dstFP);
	}
	fputs("};", dstFP);
	fputc(CR, dstFP);
	fputc(LF, dstFP);
	fclose(dstFP);
	fclose(srcFP);
	printf("Ready...%d Items\n", count1);	
	exit(0);
}




