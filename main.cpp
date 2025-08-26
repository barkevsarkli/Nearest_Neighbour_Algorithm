#include <iostream>
#include <fstream>
#include <vector>

#include "image.cpp"
#include "nearest_n.cpp"

#define OUTPUT_SIZE 10
using namespace std;

void write_BMP(const vector<Image> &images);
void put_int(ofstream &output_file, unsigned int v);
void put_short(ofstream &output_file, unsigned int v);

int main(void) 
{
    ifstream file("data_batch_1.bin", ios::binary);

    if (!file) 
    {
        cerr << "main() | Cannot open file" << endl; 
        return 1; 
    }

    vector<Image> images;
    images.reserve(10000);

    Image img;
    while (img.read(file)) 
        images.push_back(img);
    

    cout << "main() | Total images: " << images.size() << endl;

    if (!file.eof()) 
        cerr << "EOF" << endl;

    write_BMP(images);

    size_t n_train = min<size_t>(9500, images.size());
    size_t n_test  = min<size_t>(images.size() - 9500, images.size() - n_train);

    vector<Image> train_images(images.begin(), images.begin() + n_train);
    vector<Image> test_images(images.begin() + n_train, images.begin() + n_train + n_test);

    Nearest_Neighbour knn;
    knn.train(train_images);

    size_t correct = 0;
    for (size_t i = 0; i < n_test; ++i) 
    {
        auto prediction = knn.predict_one(test_images[i]);

        if (prediction == test_images[i].label())
            correct++;

        if ((i+1) % 10 == 0) 
            cout << "Tested: " << (i+1) << "/" << n_test << "\r" << flush;
        
    }
    cout << endl<<"Accuracy (1 - NN, L1): " << (100.0 * correct / n_test) << "%" << endl;

    return 0;
}

void write_BMP(const vector<Image> &images)
{
    for (unsigned int i = 0; i < OUTPUT_SIZE; i++)
    {
        string filename = "data_batch_1/image"+ to_string(i) + ".bmp";

        ofstream output_file(filename, ios::out | ios::binary);

        if (!output_file.is_open())
        {
            cout<< "write_BMP() | Output file couldnt open"<< endl;
            return;
        }

        unsigned int height = 32;
        unsigned int width = 32;
        unsigned int size = 14 + 12 + 3072;

        output_file.put('B');
        output_file.put('M');

        put_int(output_file, size);
        put_int(output_file, 0);
        put_int(output_file, 0x1A);
        put_int(output_file, 12);

        put_short(output_file, width);
        put_short(output_file, height);
        put_short(output_file, 1);
        put_short(output_file, 24);

        for (unsigned int j = 0; j < 1024; j++)
        {
            output_file.put(images[i].get_b(j));
            output_file.put(images[i].get_g(j));
            output_file.put(images[i].get_r(j));   
        }
        
        output_file.close();
    }
}

void put_int(ofstream &output_file, unsigned int v)
{
    output_file.put((v >> 0) & 0xff);
    output_file.put((v >> 8) & 0xff);
    output_file.put((v >> 16) & 0xff);
    output_file.put((v >> 24) & 0xff);
}

void put_short(ofstream &output_file, unsigned int v)
{
    output_file.put((v >> 0) & 0xff);
    output_file.put((v >> 8) & 0xff);
}

