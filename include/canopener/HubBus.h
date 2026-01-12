#pragma once
#include "Bus.h"
#include "cof.h"
#include <queue>
#include <string>
#include <cstdio>

namespace canopener {
	class BusHub {
	private:
		class HubBus: public Bus {
		public:
			bool available() { return buf.size(); };
			void write(cof_t *frame) {
				for (auto it: hub->busses) {
					if (it!=this) {
						it->buf.push(*frame);
						//printf("writing..\n");
					}
				}
			};

			bool read(cof_t *frame) { 
				cof_t c=buf.front();
				cof_cpy(frame,&c);
				buf.pop();
				return true;
			}

	        uint32_t millis() { return hub->mockMillis; }

			std::queue<cof_t> buf;
			BusHub* hub=nullptr;
		};

	public:
		BusHub() {
			mockMillis=0;
		}

		uint32_t mockMillis;
		Bus* createBus() {
			HubBus* b=new HubBus();
			busses.push_back(b);
			b->hub=this;
			return b;
		}

		std::vector<HubBus*> busses;
	};
}
