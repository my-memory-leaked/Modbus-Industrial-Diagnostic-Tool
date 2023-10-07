#include <Singleton.hpp>

/**
* @brief Protected constructor to prevent instantiation from outside.
*/
template <typename T>
Singleton<T>::Singleton() {}

/**
 * @brief Destructor is left non-virtual to prevent derived classes from overriding it.
 */
template <typename T>
Singleton<T>::~Singleton() {}

template <typename T>
T& Singleton<T>::GetInstance()
{
    static T instance;
    return instance;
}
