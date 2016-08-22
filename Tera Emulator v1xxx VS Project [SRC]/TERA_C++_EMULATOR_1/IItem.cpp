#include "IItem.h"



IItem::IItem()
{
}


IItem::~IItem()
{
}

int IItem::operator()(int)
{
		return _id;
}

IItem * IItem::Clone()
{
	IItem * out = new IItem();

	memcpy_s((void*)out, sizeof(IItem), (const void*)this, sizeof(IItem));

	return out;
}
