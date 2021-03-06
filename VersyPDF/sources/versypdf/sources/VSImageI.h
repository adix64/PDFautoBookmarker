/*********************************************************************

This file is part of the VersyPDF project.
Copyright (C) 2005 - 2016 Sybrex Systems Ltd. All rights reserved.
Authors: Vadzim Shakun , et al.

VersyPDF is free software: you can redistribute it and/or modify it 
under the terms of the GNU Affero General Public License as published 
by the Free Software Foundation, either version 3 of the License, or 
(at your option) any later version, with the addition of the following 
permission added to Section 15 as permitted in Section 7(a):
FOR ANY PART OF THE COVERED WORK IN WHICH THE COPYRIGHT IS OWNED BY 
SYBREX SYSTEMS. SYBREX SYSTEMS DISCLAIMS THE WARRANTY OF NON 
INFRINGEMENT OF THIRD PARTY RIGHTS.
                    
VersyPDF is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
GNU Affero General Public License for more details.
                                                                   
In accordance with Section 7(b) of the GNU Affero General Public License, 
a covered work must retain the producer line in every PDF that is created 
or manipulated using VersyPDF.

You can be released from the requirements of the license by purchasing 
a commercial license. Buying such a license is mandatory as soon as you 
develop commercial activities involving VersyPDF without disclosing 
the source code of your own applications, offering paid services to customers 
as an ASP, serving PDFs on the fly in a commerce web application, 
or shipping VersyPDF with a closed source product.

For more information, please contact Sybrex Systems at http://www.sybrex.com

----------------------------------------------------------------------
VSImageI.h
*********************************************************************/

#ifndef PDF_IMAGEI_H
#define PDF_IMAGEI_H

#include "VSPlatform.h"

#if !defined(MACOSX_PLATFORM)
#include <stdlib.h>
#endif

#include <stdio.h>
#include <string.h>
#include "VSCosAI.h"
#include "VSLibI.h"
#include "jpeglib.h"

#ifndef NOT_USE_PNG
#include "png.h"
#endif

#include "../VSImageA.h"
#include "../VSTypes.h"

#ifdef WINDOWS_PLATFORM
#include <Windows.h>
#endif



#define ScanLine( ppImage, line ) ( ( ppUns8 * ) ( ppImage )->ImageBuffer + ( (line) * ppImage->LineSize ) ) 

#define GetScanlineSize( Width, Depth, Device ) ( ( Width * Depth * Device + 7 ) >> 3 )

#define TImage(x) ((PMImage)(x))


#define CalculateUsedPaletteEntries( bit_count) (  bit_count >= 1 && bit_count <= 8 ? 1 << bit_count : 0 )



typedef struct TMImage{
    PDFLibHandle Lib;
    ppUns32 Width;
    ppUns32 Height;
    ppUns32 LineSize;
    TPDFColorDevice Device;
    ppComponentDepth Depth;
    void * ImageBuffer;
} * PMImage, TMImage;

#ifdef __cplusplus
extern "C" {
#endif

void PDFImageCompressJBIG2 ( PMImage Image, PDFStreamHandle  AStream);
void PDFImageCompressCCITT (  PMImage Image, PDFStreamHandle AStream);

#ifndef NOT_USE_PNG
PMImage PDFImageLoadPNGFromStream ( PDFStreamHandle AStream );
#endif

#ifndef NOT_USE_TIFF
PMImage PDFImageLoadTIFFFromStream ( PDFStreamHandle AStream , ppUns32 ImageIndex );
#endif

void PDFImageCompressJPEG ( PMImage Image, PDFStreamHandle Stream, ppUns8 Quality );

#ifdef WINDOWS_PLATFORM
    ppInt32 PDFDocAppendImageFromBitmapHandle ( PDFDocHandle Doc, HBITMAP Bitmap );
#endif


PMImage PDFImageLoadJPEGFromStream ( PDFStreamHandle AStream );

void PDFImageChange( PMImage Image, ppUns32 Width, ppUns32 Height, ppComponentDepth Depth, TPDFColorDevice Device );


PMImage PDFBMPLoadFromStream ( PDFStreamHandle AStream );

   
#ifdef __cplusplus
}
#endif

#endif /* #ifndef PDF_IMAGEI_H */
