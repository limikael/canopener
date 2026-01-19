#pragma once
#include "Entry.h"

namespace canopener {
    class Entry;

    class EntryContainer {
    public:
        Entry& insert(uint16_t index, uint8_t subindex);
        Entry& insert(uint16_t index);
        Entry& at(uint16_t index, uint8_t subindex);
        Entry& at(uint16_t index);
        Entry *find(uint16_t index, uint8_t subindex);
        int getGeneration();
        int getCommitGeneration();

    protected:
        std::vector<Entry*> entries;
    };
}
