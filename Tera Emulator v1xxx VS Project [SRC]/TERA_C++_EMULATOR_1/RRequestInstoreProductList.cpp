#include "RRequestInstoreProductList.h"


RRequestInstoreProductList::RRequestInstoreProductList() : SendPacket(C_REQUEST_INGAMESTORE_PRODUCT_LIST)
{
}

void RRequestInstoreProductList::Process(OpCode opCode, Stream * data, Client * caller)
{
	//todo
}
