#ifndef IMAGE_H
#define IMAGE_H

#include <cstddef>
#include <iostream>

#define SIZE 1024

using namespace std;
typedef unsigned char BYTE;

class Image 
{
    private:
        BYTE label_;
        BYTE r_[SIZE];
        BYTE g_[SIZE];
        BYTE b_[SIZE];

    public:
        Image() : label_(255) {
            for (size_t i = 0; i < SIZE; i++)
            {
                r_[i] = 0;
                g_[i] = 0;
                b_[i] = 0;
            }
        }

        bool read(istream &in) 
        {
            int ch = in.get();             
            if (ch == EOF) 
                return false;   

            label_ = (BYTE)(ch);

            if (!in.read((char *)r_, SIZE)) 
                return false;

            if (!in.read((char *)g_, SIZE)) 
                return false;

            if (!in.read((char *)(b_), SIZE)) 
                return false;

            return true;
        }

        inline BYTE get_r(size_t i) const { return r_[i]; }
        inline BYTE get_g(size_t i) const { return g_[i]; }
        inline BYTE get_b(size_t i) const { return b_[i]; }

        inline const BYTE* red_data()   const { return r_; }
        inline const BYTE* green_data() const { return g_; }
        inline const BYTE* blue_data()  const { return b_; }

        inline BYTE label() const { return label_; }
};

#endif