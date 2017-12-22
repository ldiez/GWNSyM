#ifndef SYSTEMIMPL_H
#define SYSTEMIMPL_H
#include "ActionHelper.h"
#include "SystemUtils.h"


namespace gnsm
{
namespace detail
{

template <typename C, typename... ARGS>
void DoApplyAction1(ActionsVector_t& actions, ts::AssetManager const& am,
                    LocalItems_t const& li, std::string const& name, ARGS&&... args)
{
    actions.push_back(CreateAction<C, ARGS...>(am, li, name, std::forward<ARGS>(args)...));
}

template < typename C, typename... ARGS>
void DoApplyAction2(ActionsVector_t& actions, ts::AssetManager const& am,
                    LocalItems_t const& li, std::string const& name0, std::string const& name1,
                    ARGS&&... args)
{
    actions.push_back(CreateAction<C, ARGS...>(am, li, name0, name1, std::forward<ARGS>(args)...));
}

template < typename C, typename... ARGS>
void DoApplyAction3(ActionsVector_t& actions, ts::AssetManager const& am,
                    LocalItems_t const& li, std::string const& name0, std::string const& name1,
                    std::string const& name2, ARGS&&... args)
{
    actions.push_back(CreateAction<C, ARGS...>
                      (am, li, name0, name1, name2, std::forward<ARGS>(args)...));
}

} // namespace detail
} // namespace gnsm


#endif /* SYSTEMIMPL_H */

