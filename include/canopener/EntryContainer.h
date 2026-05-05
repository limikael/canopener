#pragma once
#include <cstdint>
#include <vector>
#include <memory>

namespace canopener {
    class Entry;
    class Pdo;

    class EntryContainer {
    public:
        EntryContainer();
        std::shared_ptr<Entry> insert(uint16_t index, uint8_t subindex);
        std::shared_ptr<Entry> insert(uint16_t index);
        std::shared_ptr<Entry> at(uint16_t index, uint8_t subindex);
        std::shared_ptr<Entry> at(uint16_t index);
        std::shared_ptr<Entry> find(uint16_t index, uint8_t subindex);
        Pdo& pdo(int pdoNum);
        virtual void handleChange(std::shared_ptr<Entry> e);
        virtual void handleRefresh(std::shared_ptr<Entry> e);
        bool popChangeNotificationSuppression();
        void suppressChangeNotification();

    protected:
        std::vector<std::shared_ptr<Entry>> entries;
        std::vector<std::shared_ptr<Pdo>> pdos;

    private:
        bool changesSuppressed;
    };
}
