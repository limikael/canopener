#pragma once

namespace canopener {
    class IntArray {
    public:
    	IntArray() {}
        int size() { return items.size(); }
        void push(int i) { items.push_back(i); }
        int at(int index) { return items[index]; }

    private:
        std::vector<int> items;
    };
}
