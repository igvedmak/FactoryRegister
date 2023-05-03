#include <memory>
#include <unordered_map>
#include <functional>

template<typename Instance, typename Key, typename Pointer = std::unique_ptr<Instance>>
class FactoryRegister {
private:
    using functionType = std::function<Pointer()>;

    struct Impl {

        /**
         * @brief                       This function adds key - value to \c _funcManager.
         * 
         * @tparam INSTANCE             Key of isntance.
         * @param key                   Key for the relevant instance.
         * @param instance              Instance we want to instert. 
         */
        template<class INSTANCE>
        inline void add(Key const& key, 
                        INSTANCE&& instance) { this->_funcManager.try_emplace(key, std::forward<INSTANCE>(instance)); }

        /**
         * @brief                       This function returns relevant collector by key.
         * 
         * @param key                   Key for relevant collector.
         * @return Pointer              Pointer to relevant collector. 
         */
        Pointer generate(Key const& key) const {
            const auto ptr = this->_funcManager.find(key);
            if (ptr != this->_funcManager.end()) {
                return ptr->second();
            }
            return nullptr;
        }

    private:

        /**
         * @brief                       A map that holds keys that will be received from configuration 
         *                              and instances of collectors relevant to configuration.
         */
        std::unordered_map<std::string, functionType> _funcManager;
    };

    /**
     * @brief                           Singleton of Impl struct.
     * 
     * @return                          \c - Impl& - Singleton of Impl struct.
     */
    static Impl& getInstance() {
        static Impl _instance{};
        return _instance;
    }

public:

    /**
     * @brief                           C-Tor.
     */
    FactoryRegister() = default;

    /**
     * @brief                           This function adds isntance to map of instance \c (_funcManager).    
     * 
     * @tparam INSTANCE                 Key of isntance.
     * 
     * @param key                       Key for the relevant instance.
     * @param instance                  Instance we want to instert. 
     */
    template<class INSTANCE>
    inline void add(Key const& key, 
                    INSTANCE&& instance) { 
        getInstance().add(key, std::forward<INSTANCE>(instance)); 
    }
    
    /**
     * @brief                           This function creates relevant instance by relevant key this function gets.
     * 
     * @param key                       Key we want to find and create.
     * @return Pointer                  Pointer to relevant instance.
     */
    inline Pointer generate(Key const& key) const { 
        return getInstance().generate(key); 
    }
};
