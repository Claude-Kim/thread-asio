#include <memory>
#include <pimpl_h.hpp>

class pimpl::impl // 클래스 파일의 본체
{
    int foo_;
    void baz() {}
public:
    int bar() { baz(); return 1;}
};

pimpl::pimpl(): impl_(std::make_unique<impl>()) {}

pimpl::~pimpl() = default;

int pimpl::bar()
{
    return impl_->bar();
}

