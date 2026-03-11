#ifndef __UNII_DELEGATE__
#define __UNII_DELEGATE__

#include <memory>
#include <functional>
#include <vector>

namespace Unii
{
    template<typename... Args>
    class IDelegate
    {
        public:
            virtual ~IDelegate() = default;
            virtual void Invoke(Args... args) const = 0;
            virtual bool Equals(const IDelegate* other) const = 0;
            virtual std::unique_ptr<IDelegate> Clone() const = 0;
    };

    template<typename... Args>
    class FunctionDelegate : public IDelegate<Args...>
    {
        private:
            void (*m_function)(Args...);
            
        public:
            FunctionDelegate(void (*function)(Args...))
            {
                m_function = function;
            }
            
            void Invoke(Args... args) const override
            {
                if (m_function)
                    m_function(args...);
            }
            
            bool Equals(const IDelegate<Args...>* other) const override
            {
                auto derived = dynamic_cast<const FunctionDelegate<Args...>*>(other);

                return derived && m_function == derived->m_function;
            }
            
            std::unique_ptr<IDelegate<Args...>> Clone() const override
            {
                return std::make_unique<FunctionDelegate<Args...>>(m_function);
            }
    };

    template<typename T, typename... Args>
    class MethodDelegate : public IDelegate<Args...>
    {
        private:
            T* m_object;
            void (T::*m_method)(Args...);
            
        public:
            MethodDelegate(T* object, void (T::*method)(Args...))
            {
                m_object = object;
                m_method = method;
            }
            
            void Invoke(Args... args) const override
            {
                if (m_object && m_method)
                    (m_object->*m_method)(args...);
            }
            
            bool Equals(const IDelegate<Args...>* other) const override
            {
                auto derived = dynamic_cast<const MethodDelegate<T, Args...>*>(other);

                return derived
                    && m_object == derived->m_object
                    && m_method == derived->m_method;
            }
            
            std::unique_ptr<IDelegate<Args...>> Clone() const override
            {
                return std::make_unique<MethodDelegate<T, Args...>>(m_object, m_method);
            }
    };

    template<typename... Args>
    class FunctorDelegate : public IDelegate<Args...>
    {
        private:
            std::function<void(Args...)> m_functor;
            
        public:
            FunctorDelegate(const std::function<void(Args...)>& functor)
            {
                m_functor = functor;
            }
            
            void Invoke(Args... args) const override
            {
                if (m_functor)
                    m_functor(args...);
            }
            
            bool Equals(const IDelegate<Args...>* other) const override
            {
                auto derived = dynamic_cast<const FunctorDelegate<Args...>*>(other);

                return derived && &m_functor == &(derived->m_functor);
            }
            
            std::unique_ptr<IDelegate<Args...>> Clone() const override
            {
                return std::make_unique<FunctorDelegate<Args...>>(m_functor);
            }
    };

    template<typename... Args>
    class Delegate
    {
        private:
            std::unique_ptr<IDelegate<Args...>> m_delegate;
            
        public:
            Delegate(void (*function)(Args...))
            {
                m_delegate = std::make_unique<FunctionDelegate<Args...>>(function);
            }
            
            template<typename T>
            Delegate(T* object, void (T::*method)(Args...))
            {
                m_delegate = std::make_unique<MethodDelegate<T, Args...>>(object, method);
            }
            
            Delegate(const std::function<void(Args...)>& functor)
            {
                m_delegate = std::make_unique<FunctorDelegate<Args...>>(functor);
            }
            
            template<class F>
            Delegate(F&& functor)
            {
                m_delegate = std::make_unique<FunctorDelegate<Args...>>(functor);
            }
            
            Delegate(const Delegate& other)
            {
                m_delegate = other.m_delegate ? other.m_delegate->Clone() : nullptr;
            }
            
            Delegate& operator=(const Delegate& other)
            {
                if (this != &other)
                    m_delegate = other.m_delegate ? other.m_delegate->Clone() : nullptr;

                return *this;
            }
            
            Delegate(Delegate&&) = default;
            Delegate& operator=(Delegate&&) = default;
            
            void Invoke(Args... args) const
            {
                if (m_delegate)
                    m_delegate->Invoke(args...);
            }
            
            void operator()(Args... args) const
            {
                Invoke(args...);
            }
            
            bool operator==(const Delegate& other) const
            {
                if (!m_delegate || !other.m_delegate)
                    return !m_delegate && !other.m_delegate;

                return m_delegate->Equals(other.m_delegate.get());
            }
            
            bool operator!=(const Delegate& other) const
            {
                return !(*this == other);
            }
            
            explicit operator bool() const
            {
                return static_cast<bool>(m_delegate);
            }
    };

    template<typename... Args>
    Delegate<Args...> MakeDelegate(void (*function)(Args...))
    {
        return Delegate<Args...>(function);
    }
    
    template<typename T, typename... Args>
    Delegate<Args...> MakeDelegate(T* object, void (T::*method)(Args...))
    {
        return Delegate<Args...>(object, method);
    }
    
    template<typename... Args>
    Delegate<Args...> MakeDelegate(const std::function<void(Args...)>& functor)
    {
        return Delegate<Args...>(functor);
    }
}

#endif
