#include "canopener/Entry.h"
#include "canopener/Pdo.h"
#include <cstring>
#include <memory>

using namespace canopener;

Entry::Entry(uint16_t index_, uint8_t subindex_)
        :view(data) {
    //printf("entry ctor!!!\n");

	index=index_;
	subindex=subindex_;
    type=INT32;
    data.resize(getTypeSize());
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

std::shared_ptr<Entry> Entry::setType(Type type_) {
	type=type_;
	data.resize(getTypeSize());
	return shared_from_this();
}

std::shared_ptr<Entry> Entry::subscribe(int pdoChannel) {
    container->pdo(pdoChannel).add(shared_from_this());
    return shared_from_this();
}

std::shared_ptr<Entry> Entry::create(uint16_t index_, uint8_t subindex_) {
    //std::shared_ptr<Entry> e=std::make_shared<Entry>(index_,subindex_);
    std::shared_ptr<Entry> e=std::shared_ptr<Entry>(new Entry(index_,subindex_));

    return e;
}

void Entry::setData(int index, uint8_t value) { 
    data[index]=value;
}

std::string Entry::getString() {
    switch (this->type) {
        case Entry::INT8: return std::to_string(view.getInt8(0)); break;
        case Entry::UINT8: return std::to_string(view.getUint8(0)); break;
        case Entry::INT16: return std::to_string(view.getInt16(0,true)); break;
        case Entry::UINT16: return std::to_string(view.getUint16(0,true)); break;
        case Entry::INT32: return std::to_string(view.getInt32(0,true)); break;
        case Entry::UINT32: return std::to_string(view.getUint32(0,true)); break;
        case Entry::FLOAT32: return std::to_string(view.getFloat32(0,true)); break;
        case Entry::BOOL: return std::to_string(view.getUint8(0)); break;
        case Entry::STRING: return view.getString(); break;
        default: throw std::logic_error("bad type"); break;
    }
}

void Entry::setString(std::string v) {
    switch (this->type) {
        case Entry::INT8: view.setInt8(0,std::stoi(v)); break;
        case Entry::UINT8: view.setUint8(0,std::stoi(v)); break;
        case Entry::INT16: view.setInt16(0,std::stoi(v),true); break;
        case Entry::UINT16: view.setUint16(0,std::stoi(v),true); break;
        case Entry::INT32: view.setInt32(0,std::stoi(v),true); break;
        case Entry::UINT32: view.setUint32(0,std::stoi(v),true); break;
        case Entry::FLOAT32: view.setFloat32(0,std::stoi(v),true); break;
        case Entry::BOOL: view.setUint8(0,std::stoi(v)); break;
        case Entry::STRING: view.setString(v); break;
        default: throw std::logic_error("bad type"); break;
    }
}

int Entry::getInt() {
    switch (this->type) {
        case Entry::INT8: return view.getInt8(0); break;
        case Entry::UINT8: return view.getUint8(0); break;
        case Entry::INT16: return view.getInt16(0,true); break;
        case Entry::UINT16: return view.getUint16(0,true); break;
        case Entry::INT32: return view.getInt32(0,true); break;
        case Entry::UINT32: return view.getUint32(0,true); break;
        case Entry::FLOAT32: return view.getFloat32(0,true); break;
        case Entry::BOOL: return view.getUint8(0); break;
        case Entry::STRING: return std::stoi(view.getString()); break;
        default: throw std::logic_error("bad type"); break;
    }
}

void Entry::setInt(int v) {
    switch (this->type) {
        case Entry::INT8: view.setInt8(0,v); break;
        case Entry::UINT8: view.setUint8(0,v); break;
        case Entry::INT16: view.setInt16(0,v,true); break;
        case Entry::UINT16: view.setUint16(0,v,true); break;
        case Entry::INT32: view.setInt32(0,v,true); break;
        case Entry::UINT32: view.setUint32(0,v,true); break;
        case Entry::FLOAT32: view.setFloat32(0,v,true); break;
        case Entry::BOOL: view.setUint8(0,v); break;
        case Entry::STRING: view.setString(std::to_string(v)); break;
        default: throw std::logic_error("bad type"); break;
    }
}

unsigned int Entry::getUint() {
    switch (this->type) {
        case Entry::INT8: return view.getInt8(0); break;
        case Entry::UINT8: return view.getUint8(0); break;
        case Entry::INT16: return view.getInt16(0,true); break;
        case Entry::UINT16: return view.getUint16(0,true); break;
        case Entry::INT32: return view.getInt32(0,true); break;
        case Entry::UINT32: return view.getUint32(0,true); break;
        case Entry::FLOAT32: return view.getFloat32(0,true); break;
        case Entry::BOOL: return view.getUint8(0); break;
        case Entry::STRING: return std::stoi(view.getString()); break;
        default: throw std::logic_error("bad type"); break;
    }
}

void Entry::setUint(unsigned int v) {
    switch (this->type) {
        case Entry::INT8: view.setInt8(0,v); break;
        case Entry::UINT8: view.setUint8(0,v); break;
        case Entry::INT16: view.setInt16(0,v,true); break;
        case Entry::UINT16: view.setUint16(0,v,true); break;
        case Entry::INT32: view.setInt32(0,v,true); break;
        case Entry::UINT32: view.setUint32(0,v,true); break;
        case Entry::FLOAT32: view.setFloat32(0,v,true); break;
        case Entry::BOOL: view.setUint8(0,v); break;
        case Entry::STRING: view.setString(std::to_string(v)); break;
        default: throw std::logic_error("bad type"); break;
    }
}

float Entry::getFloat() {
    switch (this->type) {
        case Entry::INT8: return view.getInt8(0); break;
        case Entry::UINT8: return view.getUint8(0); break;
        case Entry::INT16: return view.getInt16(0,true); break;
        case Entry::UINT16: return view.getUint16(0,true); break;
        case Entry::INT32: return view.getInt32(0,true); break;
        case Entry::UINT32: return view.getUint32(0,true); break;
        case Entry::FLOAT32: return view.getFloat32(0,true); break;
        case Entry::BOOL: return view.getUint8(0); break;
        case Entry::STRING: return std::stoi(view.getString()); break;
        default: throw std::logic_error("bad type"); break;
    }
}

void Entry::setFloat(float v) {
    switch (this->type) {
        case Entry::INT8: view.setInt8(0,v); break;
        case Entry::UINT8: view.setUint8(0,v); break;
        case Entry::INT16: view.setInt16(0,v,true); break;
        case Entry::UINT16: view.setUint16(0,v,true); break;
        case Entry::INT32: view.setInt32(0,v,true); break;
        case Entry::UINT32: view.setUint32(0,v,true); break;
        case Entry::FLOAT32: view.setFloat32(0,v,true); break;
        case Entry::BOOL: view.setUint8(0,v); break;
        case Entry::STRING: view.setString(std::to_string(v)); break;
        default: throw std::logic_error("bad type"); break;
    }
}
