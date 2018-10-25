#ifndef __FOO_HPP__
#define __FOO_HPP__

class Foo {
    public:
        Foo(int);
        int getValue() const;
        void setValue(int);
    private:
        int value;
};

#endif