#pragma once

namespace canopener {
    class Entry;
    class Pdo;

    class EntryContainer {
    public:
        EntryContainer();

        Entry& insert(uint16_t index, uint8_t subindex);
        Entry& insert(uint16_t index);
        Entry& at(uint16_t index, uint8_t subindex);
        Entry& at(uint16_t index);
        Entry *find(uint16_t index, uint8_t subindex);
        int getGeneration();
        int getCommitGeneration();

        Pdo& pdo(int pdoNum);

    protected:
        std::vector<Entry*> entries;
        std::vector<Pdo*> pdos;
    };
}
