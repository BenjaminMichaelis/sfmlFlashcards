#include <unordered_map>

template <typename K, typename V>
class Hash
{
public:
    std::unordered_map<K, V> sut;

    Hash()
    {
    }
    ~Hash()
    {
    }

    bool isEmpty()
    {
        return sut.empty();
    }

    void insert(K key, V value)
    {
        sut[key] = value;
    }

    void remove(K key)
    {
        sut.erase(key);
    }
};