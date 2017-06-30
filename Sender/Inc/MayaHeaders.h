#pragma once
#define NT_PLUGIN
#define REQUIRE_IOSTREAM
#define EXPORT __declspec(dllexport)

#include <maya/MFnMesh.h>
#include <maya/MFnTransform.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MPoint.h>
#include <maya/MMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MVector.h>
#include <maya/MFnCamera.h>
#include <maya/M3dView.h>
#include <maya/MPlugArray.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MFnPhongShader.h>
#include <maya/MImage.h>
#include <maya/MFnPointLight.h>
#include <maya/MSelectionList.h>
#include <maya/MFn.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MUuid.h>
#include <maya/MTypeId.h>
#include <maya/MFnAttribute.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MUintArray.h>
#include <maya/MFnMatrixArrayData.h>
#include <maya/MTypeId.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MCommandResult.h>
#include <maya/MSyntax.h>
#include <maya/MExternalDropCallback.h>

// Iters
#include <maya/MItDependencyGraph.h>
#include <maya/MItGeometry.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItMeshFaceVertex.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>

// Animation
#include <maya/MFnIkJoint.h>
#include <maya/MFnSkinCluster.h>

#include <maya/MFnWeightGeometryFilter.h>
#include <maya/MAnimControl.h>

#include <maya/MPxNode.h> 
#include <maya/MSceneMessage.h>
#include <maya/MConditionMessage.h>

// Wrappers
#include <maya/MGlobal.h>
#include <maya/MCallbackIdArray.h>

// Messages
#include <maya/MMessage.h>
#include <maya/MTimerMessage.h>
#include <maya/MDGMessage.h>
#include <maya/MEventMessage.h>
#include <maya/MPolyMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MDagMessage.h>
#include <maya/MUiMessage.h>
#include <maya/MModelMessage.h>
#include <maya/MObjectSetMessage.h>
#include <maya/MCameraSetMessage.h>
#include <maya/MUiMessage.h>

// Commands
#include <maya/MPxCommand.h>

// Libraries to link from Maya
#pragma comment(lib,"Foundation.lib")
#pragma comment(lib,"OpenMaya.lib")
#pragma comment(lib,"OpenMayaAnim.lib")
#pragma comment(lib,"OpenMayaUI.lib")

#define DEBUG false

inline void Check_Error(MStatus status, MString objectName = MString())
{
	if (status != MStatus::kSuccess)
	{
		MGlobal::displayInfo("\"" + objectName + "\": " + status.errorString());
	}
}

inline void Print(MString text)
{
	MGlobal::displayInfo(text);
}

inline void Print(MString text, float value)
{
	MGlobal::displayInfo(MString(text) + " " + value);
}

inline void Print(float matrix[4][4])
{
	MGlobal::displayInfo(MString() + 
		matrix[0][0] + " " + matrix[0][1] + " " + matrix[0][2] + " " + matrix[0][3] + "\n" + 
		matrix[1][0] + " " + matrix[1][1] + " " + matrix[1][2] + " " + matrix[1][3] + "\n" + 
		matrix[2][0] + " " + matrix[2][1] + " " + matrix[2][2] + " " + matrix[2][3] + "\n" + 
		matrix[3][0] + " " + matrix[3][1] + " " + matrix[3][2] + " " + matrix[3][3] + "\n");
}