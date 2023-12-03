#include <map>

namespace stl_utils {

template <class MapType>
class MapKeyIterator {
public:
    class iterator {
    public:
        iterator(typename MapType::iterator it) : it(it) {}
        iterator operator++() { return ++it; }
        bool operator!=(const iterator & other) { return it != other.it; }
        typename MapType::key_type operator*() const { return it->first; }
    private:
        typename MapType::iterator it;
    };
private:
    MapType& map;
public:
    MapKeyIterator(MapType& m) : map(m) {}
    iterator begin() { return iterator(map.begin()); }
    iterator end() { return iterator(map.end()); }
};

template <class MapType>
MapKeyIterator<MapType> MapKeys(MapType& m) {
    return MapKeyIterator<MapType>(m);
}

}  // stl_utils