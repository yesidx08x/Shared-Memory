#ifndef _MD3D9Renderer
#define _MD3D9Renderer
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc., and/or its licensors.  All
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related
// material (collectively the "Data") in these files contain unpublished
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its
// licensors,  which is protected by U.S. and Canadian federal copyright law
// and by international treaties.
//
// The Data may not be disclosed or distributed to third parties or be
// copied or duplicated, in whole or in part, without the prior written
// consent of Autodesk.
//
// The copyright notices in the Software and this entire statement,
// including the above license grant, this restriction and the following
// disclaimer, must be included in all copies of the Software, in whole
// or in part, and all derivative works of the Software, unless such copies
// or derivative works are solely in the form of machine-executable object
// code generated by a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE,
// OR ARISING FROM A COURSE OF DEALING, USAGE, OR TRADE PRACTICE. IN NO
// EVENT WILL AUTODESK AND/OR ITS LICENSORS BE LIABLE FOR ANY LOST
// REVENUES, DATA, OR PROFITS, OR SPECIAL, DIRECT, INDIRECT, OR
// CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK AND/OR ITS LICENSORS HAS
// BEEN ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES.
// ==========================================================================
//+
//
// CLASS:    MD3D9Renderer
//
// ****************************************************************************

#if defined __cplusplus


#include <maya/MString.h>
#include <maya/MHardwareRenderer.h>

// Forward declarations
//
struct IDirect3DDevice9;

class MImage;
class MColor;
class MGeometryRequirements;

//! \ingroup OpenMayaRender
//! \brief \obsolete
/*!
  \deprecated
  Use the DirectX 11 device from MHWRender::MRenderer::GPUDeviceHandle() instead.

  MD3D9Renderer is a wrapper class to provide access to Maya's DirectX
  resources.
*/
class OPENMAYARENDER_EXPORT MD3D9Renderer /* public MPxHardwareRender */ {

public:

	// Swatch rendering methods
	MStatus			makeSwatchContextCurrent(unsigned int& width,  unsigned int& height);

	MStatus			setBackgroundColor(const MColor& color);

	MStatus			readSwatchContextPixels(MImage & image);

	IDirect3DDevice9*	getD3D9Device();

	static MD3D9Renderer*	theRenderer();

protected:
// No protected members

private:

	static MD3D9Renderer* fsHardwareRenderer;
};

#endif /* __cplusplus */
#endif /* _MD3D9Renderer */


