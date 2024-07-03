//////////////////////////////////////////////////////////////////////////////////////////
//	METABALL.h
//	Class declaration for metaball
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

class METABALL
{
public:
	VECTOR3D position;
	float squaredRadius;

	void Init(VECTOR3D newPosition, float newSquaredRadius)
	{
	position=newPosition;
	squaredRadius=newSquaredRadius;
	}
};