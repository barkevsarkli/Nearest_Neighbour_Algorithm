#include <cmath>
#include <climits>
#include <vector>

#include "image.cpp"

typedef unsigned char BYTE;

class Nearest_Neighbour
{
    private:
        vector<Image> Xtr_;

        void ensure_trained_() const 
        {
            if (Xtr_.empty()) throw runtime_error("nearest_n.cpp | Train the model first");
        }

    public:
        void train(const vector<Image>& X) 
        {
            if (X.empty()) throw runtime_error("nearest_n.cpp | train(): empty dataset");
            Xtr_ = X;
        }

        BYTE predict_one(const Image& x) const 
        {
            ensure_trained_();

            int best_dist = INT_MAX;
            BYTE best_label = 255;

            for (const auto& t : Xtr_) 
            {
                int d = 0;
                const BYTE* r1 = x.red_data();
                const BYTE* g1 = x.green_data();
                const BYTE* b1 = x.blue_data();

                const BYTE* r2 = t.red_data();
                const BYTE* g2 = t.green_data();
                const BYTE* b2 = t.blue_data();

                //  MANHATTAN DISTANCE
                for (int i = 0; i < 1024; i++) 
                {
                    d += abs(int(r1[i]) - int(r2[i]));
                    d += abs(int(g1[i]) - int(g2[i]));
                    d += abs(int(b1[i]) - int(b2[i]));
                }

                if (d < best_dist) 
                {
                    best_dist = d;
                    best_label = t.label();
                }
            }

            return best_label;
        }

        vector<BYTE> predict(const vector<Image>& X) const 
        {
            ensure_trained_();
            vector<BYTE> out;

            out.reserve(X.size());
            for (const auto& x : X) out.push_back(predict_one(x));

            return out;
        }
};
