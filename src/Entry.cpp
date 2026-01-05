#include "canopener.h"
#include <cstring>

using namespace canopener;

Entry::Entry(uint16_t index_, uint8_t subindex_)
        :view(data) {
	index=index_;
	subindex=subindex_;
	setType(INT32);
    dirty=false;
}

int Entry::getTypeSize() {
	switch (type) {
        case INT8:
        case UINT8:
        case BOOL:
        	return 1;

        case INT16:
        case UINT16:
        	return 2;

        case INT32:
        case UINT32:
        case FLOAT32:
        	return 4;

        default:
        	return 0;
	}
}

Entry& Entry::setType(Type type_) {
	type=type_;
	data.resize(getTypeSize());
	return *this;
}
