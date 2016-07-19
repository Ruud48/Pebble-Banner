
The bannerfont.bin file contains all UTF-8 characters. 
If you want to support other languages then you will have to creata a different bannerfont.bin as follows:

1. Update BannerFont.xls with Excel

2. Save BannerFont.xls

3. Save BannerFonf.xls as BannerFont.csv

4. Open a command line in Windows.

5. csvtoh bannerfont.csv		This will create bannerfont.h (used in other projects)

6. htobin bannerfont.h			This will create bannerfont.bin

7. Load bannerfont.bin in your CloudPebble project, use identifier BANNER_FONT

					* * * * * * 
