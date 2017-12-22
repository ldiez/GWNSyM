#ifndef CONFIGURERCONSTRAINTS_H
#define	CONFIGURERCONSTRAINTS_H

#include "InterfaceConstraints.h"

namespace gnsm
{

#define HAS_GET_PARAM_INT(CLASS_TYPE)\
static_assert(\
has_function_weak_GetParamInt<CLASS_TYPE, int, NamePath_t >::value\
&& has_function_weak_GetParamInt<CLASS_TYPE, int, NamePath_t, int >::value\
&& has_function_weak_GetListInt<CLASS_TYPE, std::vector<int>, NamePath_t >::value\
&& has_function_weak_GetListInt<CLASS_TYPE, std::vector<int>, NamePath_t, std::vector<int> >::value,\
PREAMBLE "===>>> Configurer class does not have the proper interface for int parameters.\n"\
"Required interface is (or convertible to): \n"\
"[ int GetParamInt ( NamePath_t ) ] && [ int GetParamInt ( NamePath_t, int ) ] && \n"\
"[ std::vector<int> GetParamListInt ( NamePath_t ) ] && \n"\
"[ std::vector<int> GetParamListInt ( NamePath_t, std::vector<int> ) ]"\
);

#define HAS_GET_PARAM_FLOAT(CLASS_TYPE)\
static_assert(\
has_function_weak_GetParamFloat<CLASS_TYPE, double, NamePath_t >::value\
&& has_function_weak_GetParamFloat<CLASS_TYPE, double, NamePath_t, double >::value\
&& has_function_weak_GetListFloat<CLASS_TYPE, std::vector<double>, NamePath_t >::value\
&& has_function_weak_GetListFloat<CLASS_TYPE, std::vector<double>, NamePath_t, std::vector<double> >::value,\
PREAMBLE "===>>> Configurer class does not have the proper interface for float parameters.\n"\
"Required interface is (or convertible to): \n"\
"[ double GetParamFloat ( NamePath_t ) ] && [ int GetParamFloat ( NamePath_t, double ) ] && \n"\
"[ std::vector<double> GetParamListFloat ( NamePath_t ) ] && \n"\
"[ std::vector<double> GetParamListFloat ( NamePath_t, std::vector<double> ) ]"\
);

#define HAS_GET_PARAM_STRING(CLASS_TYPE)\
static_assert(\
has_function_weak_GetParamStr<CLASS_TYPE, std::string, NamePath_t >::value\
&& has_function_weak_GetParamStr<CLASS_TYPE, std::string, NamePath_t, std::string >::value\
&& has_function_weak_GetListStr<CLASS_TYPE, std::vector<std::string>, NamePath_t >::value\
&& has_function_weak_GetListStr<CLASS_TYPE, std::vector<std::string>, NamePath_t, std::vector<std::string> >::value,\
PREAMBLE "===>>> Configurer class does not have the proper interface for float parameters.\n"\
"Required interface is (or convertible to): \n"\
"[ std::string GetParamStr ( NamePath_t ) ] && [ int GetParamStr ( NamePath_t, std::string ) ] && \n"\
"[ std::vector<std::string> GetParamListStr ( NamePath_t ) ] && \n"\
"[ std::vector<std::string> GetParamListStr ( NamePath_t, std::vector<std::string> ) ]"\
);

#define HAS_CORRECT_CONF_PARAMS(CLASS_TYPE, ...)\
static_assert(\
std::is_constructible<CLASS_TYPE, __VA_ARGS__>::value,\
PREAMBLE "===>>> Incorrect arguments to build a configuration"\
);

#define HAS_CORRECT_ACTION_PARAMS(CLASS_TYPE, ...)\
static_assert(\
std::is_constructible<CLASS_TYPE, __VA_ARGS__>::value,\
PREAMBLE "===>>> Incorrect arguments to build an action"\
);

#define HAS_CONFIGURER_INTERFACE(CLASS_TYPE)\
HAS_GET_PARAM_INT(CLASS_TYPE);\
HAS_GET_PARAM_FLOAT(CLASS_TYPE);\
HAS_GET_PARAM_STRING(CLASS_TYPE);


} // namespace gnsm

#endif	/* CONFIGURERCONSTRAINTS_H */

