#ifndef WRAPPER_H
#define	WRAPPER_H

#include "AssetManager.h"
#include "UnwrapperUtils.h"
#include "Log.h"

namespace gnsm
{
namespace ts
{

struct Wrapper_t
{

    Wrapper_t ( BaseType::BaseTypePtr_t const& ptr ) : m_ptr ( ptr ) { }

    /**
     * \brief Fill the map passed with the corresponding items
     * Note that the passed map is \ERASED, so that any previous resources are freed.
     * \param map --> Reference to the map
     * \return --
     */
    template <typename T>
    void Unwrap ( Map_t<T>& map ) const
    {
        auto casted_ = std::dynamic_pointer_cast <Type<T, DeducedConfigType_t<T> > >( m_ptr );

        MSG_ASSERT(casted_ != nullptr, "Bad type parameters when unwrapping owner for ",
                   m_ptr->ReadFullName().ReadStr());

        map.clear();
        for ( auto& item_ :  casted_->ReadRawItems() )
        {
            map.insert({ item_.first, item_.second });
        }
    }
    
    /**
     * \brief Fill a vector with the corresponding items
     * Note that the passed vector is \ERASED, so that any previous resources are freed.
     * \param vector --> Reference to the vector
     * \return --
     */
    template <typename T>
    void Unwrap ( Vec_t<T>& vector ) const
    {
        auto casted_ = std::dynamic_pointer_cast <Type<T, DeducedConfigType_t<T> > >( m_ptr );

        MSG_ASSERT(casted_ != nullptr, "Bad type parameters when unwrapping owner for ",
                   m_ptr->ReadFullName().ReadStr());

        vector.clear();
        for ( auto& item_ :  casted_->ReadRawItems() )
        {
            vector.push_back(item_.second );
        }
    }
    

    /**
     * \brief Set the passed ptr pointing to the corresponding resource.
     * Note that the passed pointer is \RESET, so that any previous resources are freed.
     * \param ptr --> Reference to the pointer
     * \return --
     */
    template <typename T>
    void Unwrap ( Ptr_t<T>& ptr ) const
    {
        auto casted_ = std::dynamic_pointer_cast < Type<T, DeducedConfigType_t<T> > >( m_ptr );

        MSG_ASSERT(casted_ != nullptr, "Bad type parameters when unwrapping owner for ",
                   m_ptr->ReadFullName().ReadStr());

        auto size_ = casted_->ReadRawItems().size();
        
        MSG_ASSERT(size_ >= 1u, "There are no items to be passed around from ",
                   m_ptr->ReadFullName().ReadStr());
        if ( size_ > 1u )
        {
            UWARN("Passed a pointer but ", size_, " items are build in ", 
                  m_ptr->ReadFullName().ReadStr());
        }
        ptr.reset();
        ptr = casted_->ReadRawItems().begin()->second;
        
    }
    
    template <typename T>
    void Unwrap ( Pair_t<T>& pair ) const
    {
        auto casted_ = std::dynamic_pointer_cast < Type<T, DeducedConfigType_t<T> > >( m_ptr );

        MSG_ASSERT(casted_ != nullptr, "Bad type parameters when unwrapping owner for ",
                   m_ptr->ReadFullName().ReadStr());

        auto size_ = casted_->ReadRawItems();
        MSG_ASSERT(size_ >= 1u, "There are no items to be passed around from ",
                   m_ptr->ReadFullName().ReadStr());
        if ( size_ > 1u )
        {
            UWARN("Passed a pair but ", size_, " items are build in ", 
                  m_ptr->ReadFullName().ReadStr());
        }
        
        pair = *(size_);
        
    }

private:
    const BaseType::BaseTypePtr_t& m_ptr;

};

} // namespace ts
} // namespace gnsm

#endif	/* WRAPPER_H */

