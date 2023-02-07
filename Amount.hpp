#pragma once

class Amount {
    public:
        Amount(int);
        Amount();
        int get();
        void set(int);
        void increase(int);
        void decrease(int);

    private:
        int count;
};