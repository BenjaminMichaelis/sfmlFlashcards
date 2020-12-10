#include <map>

template <typename K, typename V>
class RedBlackTree
{
private:
    std::map<K,V> sut;
public:
    RedBlackTree()
    {

    }
    ~RedBlackTree()
    {

    }
    bool isEmpty()
    {
        return sut.empty();
    }
    void insert(K key, V value)
    {
        sut.insert(std::pair<K,V>(key,value));
    }
    void remove(K key)
    {
        sut.erase(key);
    }
};