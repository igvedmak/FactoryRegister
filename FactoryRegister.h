#include <memory>
#include <unordered_map>
#include <functional>
#include <optional>

/**
 * @brief                                   A factory register for generating instances of a templated class using a key.
 * 
 * @tparam                                  Instance The class to be instantiated.
 * @tparam Key                              The key type used to generate the instance.
 * @tparam Pointer                          The pointer type to the instance.
 */
template<typename Instance, typename Key, typename Pointer = std::unique_ptr<Instance>>
class FactoryRegister {
private:

    /**
     * @brief                               Define a type alias for the function type that the factory will store.
     *
     */
    using functionType = std::function<Pointer()>;
    
    /**
     * @brief                               The implementation of the FactoryRegister that holds the map of keys 
     *                                      and instance generators.
     */
    struct Impl {

        /**
         * @brief                           Add an instance generator function for a given key.
         * 
         * @param key                       The key for the instance generator function.
         * @param func                      The instance generator function.
         */
        inline void add(Key const& key, 
                        functionType func) {
            _functions.try_emplace(key, std::move(func));
        }

        /**
         * @brief                           Generate an instance for a given key.
         * 
         * @param key                       The key to generate the instance for.
         * @return std::optional<Pointer>   An optional pointer to the generated instance. If the key is not found,
         *                                  an empty optional is returned.
         */
        std::optional<Pointer> generate(Key const& key) const {
            if (auto iter = _functions.find(key); iter != _functions.end()) {
                return iter->second();
            }
            return std::nullopt;
        }

    private:

        /**
         * @brief                           Store the function map in an unordered_map for constant-time lookup by key.
         *
         */
        std::unordered_map<Key, functionType> _functions;
    };

    /**
     * @brief                               Get the singleton instance of the implementation.
     * 
     * @return Impl&                        The singleton instance of the implementation.
     */
    static Impl& getImpl() {
        static Impl instance;
        return instance;
    }

public:
    /**
     * @brief                               C-Tor.
     * 
     */
    FactoryRegister() = default;
    /**
     * @brief                               Add an instance generator function for a given key.
     * 
     * @param key                           The key for the instance generator function.
     * @param func                          The instance generator function.
     */
    inline void add(Key const& key, 
                    functionType func) {
        getImpl().add(key, std::move(func));
    }

    /**
     * @brief                               Generate an instance for a given key.
     * 
     * @param key                           The key to generate the instance for.
     * @return std::optional<Pointer>       An optional pointer to the generated instance. If the key is not found,
     *                                      an empty optional is returned.
     */
    inline std::optional<Pointer> generate(Key const& key) const {
        return getImpl().generate(key);
    }
};
