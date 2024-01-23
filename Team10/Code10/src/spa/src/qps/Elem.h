#pragma once

class Elem {
   public:
    Elem() = default;
    virtual ~Elem() = default;
    virtual bool isSynonym() = 0;
    virtual bool isAttrRef() = 0;
};
