/**
 * @file image.h
 * @author Garrett Rhoads
 * @brief Image class definition
 * @date 2025-01-20
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <ncurses.h>

using namespace std;
using namespace cv;

class Image {
public:
// Public methods
    Image();
    Image(string filename);
    ~Image();

    void to_ascii_index(const int& scalar);
    void to_ascii_png();
    void to_curses(WINDOW * win);
    void to_curses_multithread(WINDOW * win);
    bool load();
    void load_live(const Mat & frame);
    bool load_palette();
    int get_width() const;
    int get_height() const;
    void set_filename(string new_filename);
    void set_output_filename(string new_output_filename);
    void set_dog_threshold(int new_dog_threshold);
private:
// Private methods
    void scaled_greyscale_image();
    int convolve(const int& x_pos, const int& y_pos) const;
    int convolve(const vector<vector<int> >& matrix, const int& x_pos, 
                 const int& y_pos, const vector<int>& kernel);
    
    double convolve(const int& x_pos, const int& y_pos, 
                    const vector<vector<int> >& kernel);

    bool sobel(const int& x_pos, const int& y_pos, double& theta);
    void gaussian_blur(vector<vector<int> >& blurred_image, 
                       const vector<int>& kernel);
    void dog(); // woof
    void to_curses_helper(vector<string>&, int start, int end);
    char get_edge_character(double theta);
    
// Attributes
    vector<unsigned char> _image;
    unsigned char *_output;
    vector<vector<vector<unsigned char>>> _palette;
    string _ascii_palette = " .;iroebAM-\\|/";
    vector<vector<int>> _greyscale_image;
    vector<vector<int>> _dog;
    vector<vector<int>> _ascii_indeces;
    int _width;
    int _height;
    int _palette_width;
    int _palette_height;
    int _scaled_width;
    int _scaled_height;
    int _scalar;
    int _dog_threshold;
    string _filename;
    string _output_filename;
    const int _num_quantized_lumin = 10;
};

#endif

