#pragma once

/**
 * @brief A template class for implementing the Singleton design pattern in C++.
 *
 * The Singleton pattern ensures that a class has only one instance and provides
 * a global point of access to that instance.
 *
 * @tparam T The type of the derived Singleton class.
 */
template <typename T>
class Singleton
{
public:
    /**
     * @brief Function to get the singleton instance.
     *
     * @return A reference to the singleton instance.
     */
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }

    /**
     * @brief Delete the copy constructor to prevent copying of the instance.
     */
    Singleton(const Singleton&) = delete;

    /**
     * @brief Delete the move constructor to prevent moving of the instance.
     */
    Singleton(Singleton&&) = delete;

    /**
     * @brief Delete the copy assignment operator to prevent assignment of the instance.
     */
    Singleton& operator=(const Singleton&) = delete;

    /**
     * @brief Delete the move assignment operator to prevent moving assignment of the instance.
     */
    Singleton& operator=(Singleton&&) = delete;

protected:
    /**
     * @brief Protected constructor to prevent instantiation from outside.
     */
    Singleton() {}

    /**
     * @brief Destructor is left non-virtual to prevent derived classes from overriding it.
     */
    virtual ~Singleton() {}
};
