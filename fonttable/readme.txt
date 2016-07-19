
How to create bannerfont.bin which contains the font table for Banner.

1. Update BannerFont.xls with Excel

2. Save BannerFont.xls

3. Save BannerFonf.xls as BannerFont.csv

4. Open a command line in Windows.

5. csvtoh bannerfont.csv		This will create bannerfont.h (used in other projects)

6. htobin bannerfont.h			This will create bannerfont.bin

7. Load bannerfont.bin in your CloudPebble project, use identifier BANNER_FONT

					* * * * * * 
