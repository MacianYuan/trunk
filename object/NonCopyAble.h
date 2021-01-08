#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

class NonCopyAble
{
protected:
    NonCopyAble() {}
    ~NonCopyAble() {}
private: 
    NonCopyAble(const NonCopyAble&);
    const NonCopyAble& operator=(const NonCopyAble&);
};

#endif //NONCOPYABLE_H
