#ifndef TYPE_H
#define	TYPE_H

#include "BaseType.h"
#include "InterfaceConstraints.h"
#include "Log.h"

namespace gnsm
{
namespace ts
{
struct Wrapper_t;

template < typename TYPE, typename CONF = EMPTY>
class Type : public BaseType
{
    using Type_t = TYPE;
    using Conf_t = CONF;
    using RawItems_t = std::unordered_map<Id_t, std::shared_ptr<Type_t> >;
public:

    Type ( std::string name, std::shared_ptr<Conf_t> conf, BackRegister_t br )
    : BaseType ( std::forward<std::string>( name ), std::forward<BackRegister_t>( br ) )
    , m_conf ( std::move ( conf ) ) { }

    void PrintInstancesCounter ( std::string space, NAME const& name ) const
    {
        std::cout << space << "|-" << m_knotName.ReadStr() << " [" << m_rawItems.size()
                << "]" << std::endl;
        for ( auto& item_ : m_innerTypes )
        {
            auto aux_ = space + "  ";
            item_.second.begin()->second->PrintInstancesCounter(aux_, name);
        }
    }

    void PrintAllContent ( std::string space, NAME const& name ) const
    {

        for ( auto& item_ : m_rawItems )
        {
            std::cout << space << "|-"
                    << ( name == NAME::SHORT ? m_knotName.ReadLast() : m_knotName.ReadStr() )
                    << " - " << item_.first
                    << " [" << item_.second << " - " << item_.second.use_count() << "]" << std::endl;
            for ( auto& item2_ : m_innerTypes )
            {
                auto check_ = item2_.second.find(item_.first);
                MSG_ASSERT(check_ != item2_.second.end(), "ERROR WITH IDs!!");
                auto aux_ = space + "  ";
                check_->second->PrintAllContent(aux_, name);
            }
        }
    }

    RawItems_t const& ReadRawItems ( void ) const
    {
        return m_rawItems;
    }
    
    RawItems_t GetMapItems ( void )
    {
        return m_rawItems;
    }

    std::vector<std::shared_ptr<Type_t>> GetRawItems ( void )
    {
        std::vector<std::shared_ptr < Type_t>> vec_;
        for ( auto& item_ : m_rawItems )
        {
            vec_.push_back(item_.second);
        }
        return vec_;
    }

private:

    BaseTypePtr_t DoClone ( void ) final
    {
        auto new_ = new Type<Type_t, Conf_t>( m_name, m_conf, m_br );

        new_->m_knotName = m_knotName;
        new_->m_creation = m_creation;

        for ( auto& item_ : m_innerRefTypes )
        {
            new_->m_innerRefTypes.emplace(item_.first, item_.second->Clone());
        }
        return BaseTypePtr_t(new_);
    }

    void DoCreate ( std::uint32_t num, std::string str ) final
    {
        CreationLoop(num, str);
        BindConfig();
        Aggregate<Type_t>( );
    }

    void CreationLoop ( std::uint32_t num, std::string str )
    {
        for ( Id_t id_ = 1; id_ <= num; ++id_ )
        {
            for ( auto& inner_ : m_innerRefTypes )
            {
                auto new_ = inner_.second->Clone();
                m_br(str + "::" + std::to_string(id_) + "::" + new_->ReadName(), new_);

                new_->Create(m_creation.at(inner_.first.ReadStr()), str + "::" +
                             std::to_string(id_) + "::" + new_->ReadName());
                AddActualInner(inner_.first, id_, new_);
            }
            GenericItemCreator<Type_t, ByIdConstructible_t < Type_t >> ( )( m_rawItems, id_ );
        }
    }

    void AddActualInner ( KnotName const& kn, Id_t id, BaseTypePtr_t ptr )
    {
        auto iter_ = m_innerTypes.find(kn);
        if ( iter_ == m_innerTypes.end() )
        {
            m_innerTypes.emplace(kn, Instances_t({
                {id, ptr }
            }));
        }
        else
        {
            iter_->second.emplace(id, ptr);
        }
    }

    /**
     * \brief Bind the configuration to the locally created items 
     * \return --
     */
    void BindConfig ( void )
    {
        for ( auto& item_ : m_rawItems )
        {
            GenericBindConfig<Type_t, Conf_t>( )( item_.second, *m_conf );
        }
    }

    // \TODO improve how it looks like

    template < typename T>
    typename std::enable_if<has_function_weak_Aggregate<T, void,
    std::string const&, Wrapper_t& >::value, void>::type
    Aggregate ( )
    {
        for ( auto& inner_ : m_innerTypes )
        {
            for ( auto& instance_ : inner_.second )
            {
                auto o_ = Wrapper_t(instance_.second);
                m_rawItems.at(instance_.first)->Aggregate(instance_.second->ReadName(), o_);
            }
        }
    }

    template < typename T>
    typename std::enable_if<!has_function_weak_Aggregate<T, void,
    std::string const&, Wrapper_t& >::value, void>::type
    Aggregate ( ) { }

    RawItems_t m_rawItems;
    const std::shared_ptr<Conf_t> m_conf;
};

} // namespace ts 
} // namespace gnsm

#endif	/* TYPE_H */