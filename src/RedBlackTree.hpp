#include <map>

template <Typename K, typename V>
class RedBlackTree
{
private:
    std::map<K,V> sut;
public:
    RedBlackTree();
    ~RedBlackTree();
    bool isEmpty()
    {
        return sut.empty();
    }
    void insert(K key, V value)
    {
        sut.insert(key,value);
    }
    void remove(K key)
    {
        sut.erase(key);
    }
