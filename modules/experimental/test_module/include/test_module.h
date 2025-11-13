#ifndef TEST_MODULE_H
#define TEST_MODULE_H

#include "godnux/module.hpp"

namespace godnux {

class TestModule : public Module {
public:
    TestModule();
    virtual ~TestModule();
    
    // Module metadata
    virtual std::string get_name() const override;
    virtual std::string get_version() const override;
    virtual std::string get_description() const override;
    
    // Lifecycle
    virtual bool load() override;
    virtual void unload() override;
    
    // Additional functionality for testing
    void say_hello();
    int add_numbers(int a, int b);
    
private:
    bool is_loaded = false;
};

} // namespace godnux

#endif // TEST_MODULE_H