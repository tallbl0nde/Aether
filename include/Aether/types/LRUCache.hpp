#ifndef AETHER_LRUCACHE_HPP
#define AETHER_LRUCACHE_HPP

#include <functional>
#include <list>
#include <unordered_map>

namespace Aether {
    /**
     * @brief A basic LRU (least recently used) cache implementation.
     * Also supports a custom callback for when an item is removed.
     */
    template <typename Key, typename Value>
    class LRUCache {
        public:
            // Typedef some really long things
            typedef typename std::pair<Key, Value> KeyValuePair;
            typedef typename std::list<KeyValuePair>::iterator ListIterator;

        private:
            /** @brief List containing cached items */
            std::list<KeyValuePair> items;

            /** @brief Map to map from a key to an iterator in the list */
            std::unordered_map<Key, ListIterator> keyMap;

            /** @brief Callback to invoke when a key value pair is removed */
            std::function<void(const Key &, const Value &)> callback;

            /** @brief Maximum cache size */
            unsigned int maxSize;

        public:
            /**
             * @brief Default constructor initializes a cache of size 100 and no callback.
             */
            LRUCache() {
                this->callback = nullptr;
                this->maxSize = 100;
            }

            /**
             * @brief Create a LRU Cache with the given size.
             *
             * @param size Maximum number of elements to cache.
             * @param func Callback to invoke when an element is removed (optional)
             */
            LRUCache(const unsigned int size, const std::function<void(const Key &, const Value &)> & func = nullptr) {
                this->callback = func;
                this->maxSize = size;
            }

            /**
             * @brief Add data to the cache using the given key.
             *
             * @param key Key to use for looking up the data.
             * @param data Data associated with key.
             */
            void addData(const Key & key, const Value & data) {
                // Add data to front of cache
                typename std::unordered_map<Key, ListIterator>::iterator it = this->keyMap.find(key);
                this->items.push_front(KeyValuePair(key, data));
                if (it != this->keyMap.end()) {
                    // If it was in the map, remove old value
                    this->items.erase(it->second);
                    this->keyMap.erase(it);
                }
                this->keyMap[key] = this->items.begin();

                // If we're oversize, remove oldest element
                if (this->keyMap.size() > this->maxSize) {
                    // Get last element
                    typename std::list<KeyValuePair>::iterator it2 = this->items.end();
                    it2--;

                    // Invoke callback if one was set
                    if (this->callback != nullptr) {
                        this->callback(it2->first, it2->second);
                    }

                    // Remove from both containers
                    this->keyMap.erase(it2->first);
                    this->items.pop_back();
                }
            }

            /**
             * @brief Returns the cached value associated with the given key.
             * @note The returned value is undefined if the key is not cached,
             * please use \ref hasKey() first to check that it is in the cache!
             *
             * @param key Key to search for matching value with
             * @return Value/data associated with key
             */
            Value getData(const Key & key) {
                typename std::unordered_map<Key, ListIterator>::iterator it = this->keyMap.find(key);
                if (it != this->keyMap.end()) {
                    // Shift data to front of list
                    this->items.splice(this->items.begin(), this->items, it->second);
                }

                // We hope this is a valid element if key doesn't exist
                return this->items.begin()->second;
            }

            /**
             * @brief Returns if the given key has a cached value.
             *
             * @param key Key to search for
             * @return true if key has cached value, false otherwise.
             */
            bool hasKey(const Key & key) {
                return (this->keyMap.find(key) != this->keyMap.end());
            }

            /**
             * @brief Returns the number of items in the cache.
             *
             * @return Number of items in cache.
             */
            unsigned int size() {
                return this->keyMap.size();
            }

            /**
             * @brief Calls callback function on each stored item when deleted.
             */
            ~LRUCache() {
                if (this->callback != nullptr) {
                    for (const KeyValuePair & pair : this->items) {
                        this->callback(pair.first, pair.second);
                    }
                }
            }
    };
};

#endif

