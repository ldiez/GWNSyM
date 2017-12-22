#include "BaseType.h"
#include "Log.h"
#include "AssetManager.h"

LOG_REGISTER_MODULE ( "ts::BaseType" );

namespace gnsm
{
namespace ts
{

BaseType::BaseType ( std::string name, BackRegister_t br )
: m_name ( name ), m_knotName ({ name } ), m_br ( std::move ( br ) )
{
    BEG;
}

KnotName const&
BaseType::ReadFullName ( void ) const
{
    BEG;
    return m_knotName;
}

std::string const&
BaseType::ReadName ( void ) const
{
    BEG END;
    
    return m_name;
}

void
BaseType::AddInner ( BaseTypePtr_t ptr, std::uint32_t num )
{
    BEG;

    ptr->PushFrontKnot(m_knotName);

    m_creation.emplace(ptr->ReadFullName().ReadStr(), num);
    m_innerRefTypes.emplace(ptr->ReadFullName(), std::move(ptr));
    END;
}

void
BaseType::Create ( std::uint32_t num, std::string str )
{
    BEG;
    DoCreate(num, str);
    END;
}

BaseType::BaseTypePtr_t
BaseType::Clone ( void )
{
    BEG;
    return DoClone();
}

bool
BaseType::HasInners ( void ) const
{
    BEG;
    return (m_innerRefTypes.size() > 0 );
}

void
BaseType::PrintInners ( std::string space, NAME const& name ) const
{
    std::cout << space << "|-"
            << ( name == NAME::SHORT ? m_knotName.ReadLast() : m_knotName.ReadStr() )
            << std::endl;

    for ( auto& item_ : m_innerRefTypes )
    {
        auto aux_ = space + "  ";
        item_.second->PrintInners(aux_, name);
    }
}

void
BaseType::PushFrontKnot ( KnotName const& tree )
{
    BEG;
    for ( auto& item_ : m_innerTypes )
    {
        for ( auto& instance_ : item_.second )
        {
            instance_.second->PushFrontKnot(tree);
        }
    }
    m_knotName.PushFront(tree);
}

} // namespace ts
} // namespace gnsm
