#pragma once

#include "common.h"

class session : public std::enable_shared_from_this<session>
{
public:
    session() = default;

    virtual ~session() = default;

    virtual void start() {}

protected:
    template<typename T>
    std::shared_ptr<T> getDerivedSharedPtr() {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }
};