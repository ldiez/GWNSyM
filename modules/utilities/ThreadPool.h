#ifndef THREADPOOL_H
#define	THREADPOOL_H

#include <thread>
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <atomic>
#include <deque>

template < typename T >
class ThreadPool
{
public:
    using Function_t = std::function<void (T& )>;

    ThreadPool ( std::vector<std::shared_ptr<T> > const& v, Function_t f )
    : m_func ( f ), m_bTerminate ( false )
    {
        for ( auto& item_ : v )
        {
            m_items.push_back(item_);
        }

        auto nth_ = std::thread::hardware_concurrency();
        for ( auto i = 0u; i < nth_; ++i )
        {
            m_workers.push_back(std::thread(&ThreadPool::Loop, this));
        }
    }

    ~ThreadPool ( )
    {
        for ( auto& worker : m_workers )
        {
            worker.join();
        }
    }

private:

    void
    Loop ( )
    {
        while ( !m_bTerminate )
        {
            auto item_ = GetNext();
            if ( item_ != nullptr )
            {
                m_func(*item_);
            }
        }
    }

    std::shared_ptr<T>
    GetNext ( void )
    {
        std::unique_lock<std::mutex> lockList(m_lockItemsList);
        if ( !m_items.empty() )
        {
            auto item = m_items.front();
            m_items.pop_front();
            return item;
        }
        else
        {
            m_bTerminate = true;
            return
            {
            };
        }
    }

    Function_t m_func;
    std::vector<std::thread> m_workers;
    std::deque<std::shared_ptr<T>> m_items;

    std::mutex m_lockItemsList;
    std::atomic<bool> m_bTerminate;

};

template < typename T >
class ParThreadPool
{
public:
    using Function_t = std::function<void (T )>;
    using Vector_t = std::vector<T>;
    using Iter_t = typename Vector_t::iterator;

    ParThreadPool ( Vector_t &v, Function_t f )
    : m_func ( f )
    {
        auto nth_ = std::thread::hardware_concurrency();

        auto length_ = v.size();
        std::uint32_t step_ = length_ / nth_;

        auto begin_ = v.begin();
        auto curr_ = v.begin();
        auto end_ = v.end();

        for ( auto i = 1u; i <= nth_; ++i )
        {
            if ( i == nth_ )
            {
                m_workers.push_back(std::thread(&ParThreadPool::Loop, this, begin_, end_));
            }
            else
            {
                std::advance(curr_, step_);
                m_workers.push_back(std::thread(&ParThreadPool::Loop, this, begin_, curr_));
            }
            std::advance(begin_, step_);
        }

    }

    ~ParThreadPool ( )
    {
        for ( auto& worker : m_workers )
        {
            worker.join();
        }
    }

private:

    void
    Loop ( Iter_t init, Iter_t end )
    {
        for ( auto it_ = init; it_ != end; ++it_ )
        {
            m_func(*it_);
        }
    }

    Function_t m_func;
    std::vector<std::thread> m_workers;
};

#endif	/* THREADPOOL_H */

