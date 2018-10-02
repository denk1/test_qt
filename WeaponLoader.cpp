#include "WeaponLoader.h"

namespace RAT
{

WeaponLoader::WeaponLoader()
{
}


void WeaponLoader::parseWeapons(TiXmlElement* xmlNode, std::vector<WeaponDescription>& outParsed)
{
	for(TiXmlNode* child = xmlNode->FirstChild(); child; child = child->NextSibling() )
	{
		if (child->Type() != TiXmlNode::NodeType::TINYXML_ELEMENT)
			continue;

		parseWeapon(static_cast<TiXmlElement*>(child), outParsed);
	}
}

void WeaponLoader::parseWeapon(TiXmlElement* xmlNode, std::vector<WeaponDescription>& outParsed)
{
	WeaponDescription desc;

	desc.mMeshFile = xmlNode->Attribute("mesh");
	desc.mType = xmlNode->Attribute("type");

	xmlNode->QueryFloatAttribute("minRotate", &desc.mMaxAngle);
	xmlNode->QueryFloatAttribute("maxRotate", &desc.mMaxAngle);
	xmlNode->QueryFloatAttribute("rotateSpeed", &desc.mRotateSpeed);
	xmlNode->QueryFloatAttribute("baseDamage", &desc.mBaseDamage);
	xmlNode->QueryFloatAttribute("reloadTime", &desc.mReloadTime);

	TiXmlNode* node = xmlNode->FirstChild("Position");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		TiXmlElement* positionNode = static_cast<TiXmlElement*>(node);
		positionNode->QueryFloatAttribute("x", &desc.mPosition.x);
		positionNode->QueryFloatAttribute("y", &desc.mPosition.y);
		positionNode->QueryFloatAttribute("z", &desc.mPosition.z);
	}

	outParsed.push_back(desc);

}


}
