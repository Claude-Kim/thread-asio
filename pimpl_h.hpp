#include <memory>

class pimpl
{
    public:
        pimpl();
        ~pimpl();
        int bar();
    private:
        class impl;
        std::unique_ptr<impl> impl_;    
};

