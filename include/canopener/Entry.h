#pragma once
#include <cstdint>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <string>
#include <memory>
#include "DataView.h"
#include "castx.h"
#include "EntryContainer.h"
#include "Dispatcher.h"

//#include <Arduino.h>

namespace canopener {
	class Entry: public std::enable_shared_from_this<Entry> {
	public:
	    enum Type {
	        INT8, UINT8,
	        INT16, UINT16,
	        INT32, UINT32,
	        FLOAT32, BOOL,
	        STRING
	    };

		std::shared_ptr<Entry> setType(Type type);
		std::string getString();
		void setString(std::string s);
		int getInt();
		void setInt(int v);
		unsigned int getUint();
		void setUint(unsigned int v);
		float getFloat();
		void setFloat(float v);
	    size_t size() { return data.size(); }
	    void setData(int index, uint8_t value);
	    uint8_t getData(int index) { return data[index]; }
	    uint16_t getIndex() { return index; }
	    uint8_t getSubIndex() { return subindex; }
        std::shared_ptr<Entry> subscribe(int pdoChannel);
		Dispatcher<> changeDispatcher;
		static std::shared_ptr<Entry> create(uint16_t index_, uint8_t subindex_);
		void refresh();

	private:
		Entry(uint16_t index_, uint8_t subindex_);
	    void setContainer(EntryContainer *container_) { this->container=container_; }
		Type type;
		uint16_t index;
		uint8_t subindex;
		std::vector<uint8_t> data;
		DataView view;
		int getTypeSize();
		EntryContainer *container=nullptr;

		friend class EntryContainer;
		friend class RemoteDevice;
	};
}

/*

		template<typename T>
		Entry& set (T v) {
			if constexpr (!std::is_same_v<T, const char*>) {
				if (v==get<T>())
					return *this;
			}

			generation=container->getGeneration()+1;
			dirty=true;

            switch (type) {
                case Type::INT8:    view.setInt8(0,castx<int8_t,T>(v)); return *this;
                case Type::INT16:   view.setInt16(0,castx<int16_t,T>(v),true); return *this;
                case Type::INT32:   view.setInt32(0,castx<int32_t,T>(v),true); return *this;
                case Type::UINT8:   view.setUint8(0,castx<uint8_t,T>(v)); return *this;
                case Type::UINT16:  view.setUint16(0,castx<uint16_t,T>(v),true); return *this;
                case Type::UINT32:  view.setUint32(0,castx<uint32_t,T>(v),true); return *this;
                case Type::FLOAT32: view.setFloat32(0,castx<float,T>(v),true); return *this;
                case Type::BOOL:    view.setUint8(0,castx<uint8_t,T>(v)); return *this;
                case Type::STRING:  view.setString(castx<std::string,T>(v)); return *this;
                default: 
 					throw std::logic_error("Type missing on set...");

                break;
            }

			return *this;
		}

		template<typename T>
	    T get() {
            switch (type) {
                case Type::INT8:    return castx<T,int8_t>(view.getInt8(0));
                case Type::INT16:   return castx<T,int16_t>(view.getInt16(0,true));
                case Type::INT32:   return castx<T,int32_t>(view.getInt32(0,true));
                case Type::UINT8:   return castx<T,uint8_t>(view.getUint8(0));
                case Type::UINT16:  return castx<T,uint16_t>(view.getUint16(0,true));
                case Type::UINT32:  return castx<T,uint32_t>(view.getUint32(0,true));
                case Type::FLOAT32: return castx<T,float>(view.getFloat32(0,true));
                case Type::BOOL:    return castx<T,uint8_t>(view.getUint8(0));
                case Type::STRING:  return castx<T,std::string>(view.getString());
                default: 
 					throw std::logic_error("Type missing on get...");

                break;
            }
	    }


*/