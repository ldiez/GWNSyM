#include "System.h"
#include "Configurer.h"

using namespace gnsm;
#define GLOB_CONF "./config-files/EXAMPLE_CONF.cfg"

struct ExampleCellConf { };

struct ExampleCell {
    void SetConfiguration(ExampleCellConf const& conf) {
    }
};

struct ExampleMacroConf {
    InnerItemsCounterList_t ReadInnerConf(void) const {
        return {
            {"CELL", 3}};
    }
};

struct ExampleMacro {
    void SetConfiguration(ExampleMacroConf const& conf) {
    }
};

struct ExampleUserConf {};

struct ExampleUser {
    void SetConfiguration(ExampleUserConf const& conf) {
    }
};

struct ExampleAction {
    void operator()(Ptr_t<ExampleCell> m, Ptr_t<ExampleUser> u) {
        std::cout << "One-to-one" << std::endl;
    }
};

struct ExampleAction2 {
    void operator()(Vec_t<ExampleCell> m, Ptr_t<ExampleUser> u) {
        std::cout << "Many-to-one" << std::endl;
    }
};

struct ExampleAction3 {
    void operator()(Vec_t<ExampleCell> m, Vec_t<ExampleUser> u) {
        std::cout << "Many-to-many" << std::endl;
    }
};

int main() {
    System network;
    // Definition of NetTypes
    network.Type<ExampleUser, ExampleUserConf>("USER")
           .Type<ExampleCell, ExampleCellConf>("CELL")
           .Type<ExampleMacro, ExampleMacroConf>("MACRO")
    // Setting global configuration
           .SetTreeBase("MACRO",{"MACRO_INST", "NUMBER"})
           .SetTreeBase("USER",{"USER_INST", "NUMBER"})
           .SetConfig<Configurer>(GLOB_CONF)
    // Deploy network
           .Deploy()
           .PrintTreeInstances();
    // Apply actions
    network.Action<ExampleAction>({"MACRO::*::CELL", "USER"})
           .Action<ExampleAction2>({"MACRO::*::CELL", "USER"})
           .Action<ExampleAction3>({"MACRO::*::CELL", "USER"})
    // Start simulation
           .Run();
    return 0;
}