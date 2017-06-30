#include "Material.h"

Material::Material()
{
	_header.albedoSize = 0;
	_header.roughnessSize = 0;
	_header.metallicSize = 0;
	_header.normalSize = 0;
	_header.displacementSize = 0;
	_header.radienceSize = 0;
	_header.irradienceSize = 0;
}

Material::~Material(){}

void Material::SetData(MObject& object, PackageType packageType)
{
	_identifierSize = 0;
	_size = 0;
	_packageType = packageType;
	switch (_packageType)
	{
	case WHOLE:
		GetWhole(object);
		break;
	case CREATE:
		GetWhole(object);
		break;
	case TEXNAME:
		GetTextureName(object);
		break;
	default:
		break;
	}
}

void Material::SendPackage(void* buffer, unsigned int head)
{
	switch (_packageType)
	{
	case WHOLE:
		SendWhole(buffer, head);
		break;
	case CREATE:
		SendWhole(buffer, head);
		break;
	case TEXNAME:
		SendTextureName(buffer, head);
		break;
	default:
		break;
	}
}

void Material::GetWhole(MObject& object)
{
	MFnDependencyNode depNode(object);
	MPlug plug;
	MObject plugObject;
	MStatus stat;

	plug = depNode.findPlug("color", &stat);
	MColor color;
	if (stat == MS::kSuccess)
	{
		plug.getValue(plugObject);
		MFnNumericData colorValue(plugObject);
		colorValue.getData(color.r, color.g, color.b);
	}

	// Look for texture path if there is one
	MItDependencyGraph itGraph(plug, MFn::kFileTexture, MItDependencyGraph::kUpstream);
	MString texturePath;
	if (!itGraph.isDone())
	{	
		MFnDependencyNode(itGraph.thisNode()).findPlug("fileTextureName", &stat).getValue(texturePath);
	}

	plug = depNode.findPlug("transparency", &stat);
	MColor transparency;
	if (stat == MS::kSuccess)
	{
		plug.getValue(plugObject);
		MFnNumericData transparencyValue(plugObject);
		transparencyValue.getData(transparency.r, transparency.g, transparency.b);
	}

	plug = depNode.findPlug("ambientColor", &stat);
	MColor ambient;
	if (stat == MS::kSuccess)
	{
		plug.getValue(plugObject);
		MFnNumericData ambientColorValue(plugObject);
		ambientColorValue.getData(ambient.r, ambient.g, ambient.b);
	}

	// Fill in data
	_header.albedoSize = texturePath.length();
	_size += sizeof(MaterialHeader);
	_size += _header.albedoSize;
	_size += _header.roughnessSize;
	_size += _header.metallicSize;
	_size += _header.normalSize;
	_size += _header.displacementSize;
	_size += _header.radienceSize;
	_size += _header.irradienceSize;

	_albedo = texturePath.asChar();
}

void Material::GetTextureName(MObject& object)
{
	MFnDependencyNode depNode(object);
	MString texturePath;
	depNode.findPlug("fileTextureName", &stat).getValue(texturePath);

	_albedo = texturePath.asChar();
	_size += texturePath.length();
}

void Material::SendWhole(void * buffer, unsigned int head)
{
	// Header
	memcpy((char*)buffer + head, &_header, sizeof(MaterialHeader));
	head += sizeof(MaterialHeader);
	// Texture paths
	memcpy((char*)buffer + head, _albedo.c_str(), _header.albedoSize);
	head += _header.albedoSize;
	memcpy((char*)buffer + head, _roughness.c_str(), _header.roughnessSize);
	head += _header.roughnessSize;
	memcpy((char*)buffer + head, _metallic.c_str(), _header.metallicSize);
	head += _header.metallicSize;
	memcpy((char*)buffer + head, _normal.c_str(), _header.normalSize);
	head += _header.normalSize;
	memcpy((char*)buffer + head, _displacement.c_str(), _header.displacementSize);
	head += _header.displacementSize;
	memcpy((char*)buffer + head, _radience.c_str(), _header.radienceSize);
	head += _header.radienceSize;
	memcpy((char*)buffer + head, _irradience.c_str(), _header.irradienceSize);
	head += _header.irradienceSize;
}

void Material::SendTextureName(void* buffer, unsigned int head)
{
	// Texture path
	memcpy((char*)buffer + head, _albedo.c_str(), _size);
	head += _size;
}