/**
 * @file image.cc
 * @author Garrett Rhoads
 * @brief Image methods
 * @date 2025-01-20
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <cstdio>
#include <ctime>
#include "image.h"
#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;
using namespace cv;

const int CHANNELS = 3;
const int NUM_THREADS = 4;

/**
 * @brief Construct a new Image object
 */
Image::Image() {
    _dog_threshold = 0;
}

/**
 * @brief Construct a new Image object
 * 
 * @param _filename - name of image file
 */
Image::Image(string filename) {
    _filename = filename;
}

void Image::set_filename(string new_filename) {
    _filename = new_filename;
}

void Image::set_output_filename(string new_output_filename) {
    _output_filename = new_output_filename;
}

void Image::set_dog_threshold(int new_dog_threshold) {
    _dog_threshold = new_dog_threshold;
}

/**
 * @brief Destroy the Image object
 */
Image::~Image() {}

/**
 * @brief Converts _image to ascii and exports it to output.txt
 * 
 * @param scalar - how much to down scale the image
 */
void Image::to_ascii_index(const int& scalar) {
    _scalar = scalar;

    _scaled_width = _width / _scalar;
    _scaled_height = _height / _scalar;

    vector<int> _ascii_indeces_row;

    int avg_lumin;
    int ascii_idx;
    double theta;

    scaled_greyscale_image();

    dog(); 
   
    for (int i = 0; i < _scaled_height; i++) {
        for (int j = 0; j < _scaled_width; j++) {
            if ((!((i == 0) || (j == 0) || (i == (_scaled_height - 1)) || 
                   (j == (_scaled_width - 1)))) && (sobel(j, i, theta)) &&
                   (_greyscale_image[i][j] < 192)) {
                
                if ((theta < 0.1) || (theta > 0.9)) {
                    _ascii_indeces_row.push_back(10);
                    
                } else if (theta < 0.4) {
                    _ascii_indeces_row.push_back(11);
                    
                } else if (theta < 0.6) {
                    _ascii_indeces_row.push_back(12);
                    
                } else {
                    _ascii_indeces_row.push_back(13);
                }
            } else {
                avg_lumin = _greyscale_image[i][j] * 100;
                ascii_idx = (avg_lumin / (25500 / (_num_quantized_lumin - 1)));
                _ascii_indeces_row.push_back(ascii_idx);
                
            }
        }
        _ascii_indeces.push_back(_ascii_indeces_row);
        _ascii_indeces_row.clear();
        
    }
    
}

/**
 * @brief Gets _width attribute
 * 
 * @return int 
 */
int Image::get_width() const {
    return _width;
}

/**
 * @brief Gets height attribute
 * 
 * @return int 
 */
int Image::get_height() const {
    return _height;
}

/**
 * @brief Loads the needed info into the _image attribute
 * 
 * @return true 
 * @return false 
 */
bool Image::load() {
    int n;
    unsigned char* data = stbi_load(_filename.c_str(),&_width,&_height,&n, 3);
    if (data != nullptr) {
        _image = vector<unsigned char>(data, data + _width * _height * 3);
    }
    stbi_image_free(data);
    
    return (data != nullptr);
}

void Image::load_live(const Mat & frame) {
    // Get frame dimensions
    _height = frame.rows;
    _width = frame.cols;
    size_t totalBytes = frame.total() * frame.elemSize();
    
    _image.resize(totalBytes);
    
    memcpy(_image.data(), frame.data, totalBytes);
}

bool Image::load_palette() {
    vector<unsigned char> _palette_1D;
    vector<vector<unsigned char>> palette_row;
    vector<unsigned char> pixel;
    size_t RGBA = 3;
    string palette_file = "palette.png";

    int n;
    unsigned char* data = stbi_load(palette_file.c_str(),&_palette_width,&_palette_height,&n, 3);
    if (data != nullptr) {
        _palette_1D = vector<unsigned char>(data, data + _palette_width * _palette_height * 3);
    }
    stbi_image_free(data);
    if (data == nullptr) {
        return false;
    }
    
    for (int i = 0; i < _palette_height; i++) {
        for (int j = 0; j < _palette_width; j++) {
            size_t index = RGBA * (i * _palette_width + j);
            pixel.push_back(static_cast<int>(_palette_1D[index + 0]));
            pixel.push_back(static_cast<int>(_palette_1D[index + 1]));
            pixel.push_back(static_cast<int>(_palette_1D[index + 2]));

            
            palette_row.push_back(pixel);
            pixel.clear();
        }
        _palette.push_back(palette_row);
        palette_row.clear();
    }

    return true;
}

/**
 * @brief Finds the average luminance of an area of the iamge based on the scalar
 * 
 * @param x_pos - x position of the pixel 
 * @param y_pos - y position of the pixel
 * @return int - average luminance
 */
int Image::convolve(const int& x_pos, const int& y_pos) const {
    const size_t RGBA = 3;
    int r, g, b;
    int avg_lumin = 0;

    for (int i = (y_pos * _scalar); i < (_scalar * (y_pos + 1)); i++) {
        for (int j = (x_pos * _scalar); j < (_scalar * (x_pos + 1)); j++) {
            size_t index = RGBA * (i * _width + j);
            r = static_cast<int>(_image[index + 0]);
            g = static_cast<int>(_image[index + 1]);
            b = static_cast<int>(_image[index + 2]);

            avg_lumin += (r + g + b) / 3;
        }
    }

    avg_lumin = avg_lumin / (_scalar * _scalar);
    return avg_lumin;
}

/**
 * @brief Scales the image and greyscales it
 */
void Image::scaled_greyscale_image() {
    vector<int> greyscale_row;
    
    for (int i = 0; i < _scaled_height; i++) {
        for (int j = 0; j < _scaled_width; j++) {
            int greyscale_pix = convolve(j, i);
            greyscale_row.push_back(greyscale_pix);
        }
        _greyscale_image.push_back(greyscale_row);
        greyscale_row.clear();
    }
}


/**
 * @brief Convolves the kernel with the _greyscale_image at pixel x_pos y_pos
 * 
 * @param x_pos - x position of the pixel being convolved
 * @param y_pos - y position of the pixel being convolved
 * @param kernel - kernel to do the convolution
 * @return double - sum total after convolution
 */
double Image::convolve(const int& x_pos, const int& y_pos, 
                       const vector<vector<int>>& kernel) {
    int kernel_size = kernel.size();
    double total = 0.0;
    for (int i = 0; i < kernel_size; i++) {
        for (int j = 0; j < kernel_size; j++) {
            total += _greyscale_image[y_pos + i - (kernel_size / 2)][x_pos + j - 
            (kernel_size / 2)] * kernel[i][j];
        }
    }
    return (total);
}

/**
 * @brief Convolves matrix with kernel at x_pos y_pos of matrix
 * 
 * @param matrix - 2d array
 * @param x_pos - x position to convolve
 * @param y_pos - y position to convolve
 * @param kernel - kernel to convolve with
 * @return int - adjusted total
 */
int Image::convolve(const vector<vector<int>>& matrix, const int& x_pos, 
                    const int& y_pos, const vector<int>& kernel) {

    int total = 0;
    int kernel_size = kernel.size();
    int kernel_sum = pow(2, 2 * (kernel_size + 1));
    for (int i = 0; i < kernel_size; i++) {
        for (int j = 0; j < kernel_size; j++) {
            total += matrix[y_pos + i - (kernel_size / 2)][x_pos + j - 
            (kernel_size / 2)] * (kernel[i] * kernel[j]);
        }
    }
    total = total / kernel_sum;
    return (total);
}

/**
 * @brief Detects edges at x_pos y_pos and gives the angle of the edge
 * 
 * @param x_pos - x position of pixel to check
 * @param y_pos - y position of pixel to check
 * @param theta - angle of edge [0, 1]
 * @return true - if there is an edge
 * @return false - if there is not an edge
 */
bool Image::sobel(const int& x_pos, const int& y_pos, double& theta) {
    vector<vector<int>> x_kernel = {{-1, 0, 1},
                                    {-2, 0, 2},
                                    {-1, 0, 1}};

    vector<vector<int>> y_kernel = {{-1, -2, -1},
                                    { 0,  0,  0},
                                    { 1,  2,  1}};

    double Gx = convolve(x_pos, y_pos, x_kernel) + 0.0001;
    double Gy = convolve(x_pos, y_pos, y_kernel);

    if (sqrt((Gx * Gx) + (Gy * Gy)) > 400) {
        theta = (atan(Gy / Gx) / M_PI) + 0.5;
        return true;
    }
    return false;
}

/**
 * @brief Blurs the image and puts it in blurred_image
 * 
 * @param blurred_image - storage for blurred image
 * @param kernel - how much u wanna blur?
 */
void Image::gaussian_blur(vector<vector<int>>& blurred_image, 
                          const vector<int>& kernel) {
    vector<int> blurred_image_row;
    int border = (kernel.size() / 2) - 1;
    
    int blur;

    for (int i = 0; i < _scaled_height; i++) {
        for (int j = 0; j < _scaled_width; j++) {
            if (!(((i - border) <= 0) || ((j - border) <= 0) || 
                  ((i + border) >= (_scaled_height - 1)) || 
                  ((j + border) >= (_scaled_width - 1)))) {
                
                blur = convolve(_greyscale_image, j, i, kernel);
                blurred_image_row.push_back(blur);
            } else {
                blurred_image_row.push_back(0);
            }
        }
        blurred_image.push_back(blurred_image_row);
        blurred_image_row.clear();
    }
}

/**
 * @brief Summons the Devourer of-- sorry, preforms a simple difference of 
 *        gaussians with a threshold of 9 and kernel sizes of 3 and 7
 */
void Image::dog() {
    vector<int> kernel_1 = {1, 4, 6, 4, 1};  

    vector<int> kernel_2 = {1, 8, 28, 56, 70, 56, 28, 8, 1};

    vector<vector<int>> blur_1;
    vector<vector<int>> blur_2;

    gaussian_blur(blur_1, kernel_1);
    gaussian_blur(blur_2, kernel_2);

    vector<int> dog_row;
    for (int i = 0; i < _scaled_height; i++) {
        for (int j = 0; j < _scaled_width; j++) {
            if (abs(blur_1[i][j] - blur_2[i][j]) > _dog_threshold) {
                dog_row.push_back(255);
            } else {
                dog_row.push_back(0);
            }
        }
        _dog.push_back(dog_row);
        dog_row.clear();
    }
}

char Image::get_edge_character(double theta) {
    if ((theta < 0.1) || (theta > 0.9)) {
        return _ascii_palette[10];
    } else if (theta < 0.4) {
        return _ascii_palette[11];
    } else if (theta < 0.6) {
        return _ascii_palette[12];
    } else {
        return _ascii_palette[13];
    }
}

void Image::to_curses_helper(vector<string>& screen_lines, int start, int end) {
    double theta = 0;
    int avg_lumin = 0;
    int ascii_idx = 0;
    
    for (int row = start; row < end; row++) {
        screen_lines[row].reserve(_scaled_width * 2);
        
        for (int j = 0; j < _scaled_width; j++) {
            bool is_border = (row == 0) || (j == 0) || (row == _scaled_height - 1) || (j == _scaled_width - 1);
            bool is_dark = _greyscale_image[row][j] < 192;
            
            char char_to_add;
            if (!is_border && sobel(j, row, theta) && is_dark) {
                char_to_add = get_edge_character(theta);
            } else {
                avg_lumin = _greyscale_image[row][j] * 100;
                ascii_idx = (avg_lumin / (25500 / (_num_quantized_lumin - 1)));
                char_to_add = _ascii_palette[ascii_idx];
            }
            
            screen_lines[row].append(2, char_to_add);
        }
    }
}

void Image::to_curses_multithread(WINDOW * win) {
    int win_height, win_width;
    getmaxyx(win, win_height, win_width);
    win_width /= 2;
    double win_aspect = static_cast<double>(win_width) / win_height;
    double img_aspect = static_cast<double>(_width) / _height;

    _scalar = ((img_aspect > win_aspect) ? (_width / win_width) : (_height / win_height)) + 1;
    _scaled_width = _width / _scalar;
    _scaled_height = _height / _scalar;

    int x_offset = (win_width - _scaled_width);
    int y_offset = (win_height - _scaled_height) / 2;

    scaled_greyscale_image();
    dog();
    
    vector<string> screen_lines(_scaled_height);
    vector<thread> thread_grp;
    
    int lines_per_thread = _scaled_height / NUM_THREADS;
    int remaining_lines = _scaled_height % NUM_THREADS;
    
    int current_start = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        int thread_lines = lines_per_thread + (i < remaining_lines ? 1 : 0);
        int thread_end = current_start + thread_lines;
        
        thread_grp.emplace_back(&Image::to_curses_helper, this, 
                               ref(screen_lines), 
                               current_start, 
                               thread_end);
        
        current_start = thread_end;
    }
    
    for (auto& t : thread_grp) {
        t.join();
    }
    
    for (int i = 0; i < _scaled_height; i++) {
        mvwaddstr(win, i + y_offset, x_offset, screen_lines[i].c_str());
    }
    
    wrefresh(win);
}

void Image::to_curses(WINDOW * win) {
    int win_height, win_width;
    getmaxyx(win, win_height, win_width);
    win_width /= 2;
    double win_aspect = static_cast<double>(win_width) / win_height;
    double img_aspect = static_cast<double>(_width) / _height;

    _scalar = ((img_aspect > win_aspect) ? (_width / win_width) : (_height / win_height)) + 1;
    _scaled_width = _width / _scalar;
    _scaled_height = _height / _scalar;

    int x_offset = (win_width - _scaled_width);
    int y_offset = (win_height - _scaled_height) / 2;

    scaled_greyscale_image();
    dog();

    vector<int> _ascii_indeces_row;

    int avg_lumin;
    int ascii_idx;
    double theta;
    for (int i = 0; i < _scaled_height; i++) {
        for (int j = 0; j < _scaled_width; j++) {
            bool is_border = (i == 0) || (j == 0) || (i == _scaled_height - 1)|| 
                             (j == _scaled_width - 1);
            bool is_dark = _greyscale_image[i][j] < 192;
            if (!is_border && sobel(j, i, theta) && is_dark) {
                
                if ((theta < 0.1) || (theta > 0.9)) {
                    mvwaddch(win, i + y_offset, (j * 2) + x_offset, _ascii_palette[10]);
                    mvwaddch(win, i + y_offset, (j * 2) + x_offset + 1, _ascii_palette[10]);
                } else if (theta < 0.4) {
                    mvwaddch(win, i + y_offset, (j * 2) + x_offset, _ascii_palette[11]);
                    mvwaddch(win, i + y_offset, (j * 2) + x_offset + 1, _ascii_palette[11]);
                } else if (theta < 0.6) {
                    mvwaddch(win, i + y_offset, (j * 2) + x_offset, _ascii_palette[12]);
                    mvwaddch(win, i + y_offset, (j * 2) + x_offset + 1, _ascii_palette[12]);
                } else {
                    mvwaddch(win, i + y_offset, (j * 2) + x_offset, _ascii_palette[13]);
                    mvwaddch(win, i + y_offset, (j * 2) + x_offset + 1, _ascii_palette[13]);
                }
            } else {
                avg_lumin = _greyscale_image[i][j] * 100;
                ascii_idx = (avg_lumin / (25500 / (_num_quantized_lumin - 1)));
                mvwaddch(win, i + y_offset, (j * 2) + x_offset, _ascii_palette[ascii_idx]);
                mvwaddch(win, i + y_offset, (j * 2) + x_offset + 1, _ascii_palette[ascii_idx]);
            }
        }
    }
    
    wrefresh(win);
}

/**
 * @brief Instanciates the output array with the correct rgb values to be written
 */
void Image::to_ascii_png() {
    int output_size = (_scaled_width * _scalar) * (_scaled_height * _scalar) * CHANNELS;
    _output = new unsigned char[output_size];
    unsigned char *pix = _output;

    for (int i = 0; i < _scaled_height; i++) {
        for (int output_row = 0; output_row < _scalar; output_row++) {  
            // Map output row back to source palette row (0-7)
            int palette_row = (output_row * 8) / _scalar;
            
            for (int j = 0; j < _scaled_width; j++) {
                int ascii_texture_start_col = _ascii_indeces[i][j] * 8;  // Always 8, since palette chars are 8 wide
                
                for (int output_col = 0; output_col < _scalar; output_col++) {
                    // Map output column back to source palette column (0-7 within the character)
                    int palette_col = ascii_texture_start_col + (output_col * 8) / _scalar;
                    
                    for (int c = 0; c < 3; c++) {
                        *(pix + c) = _palette[palette_row][palette_col][c];
                    }
                    pix += CHANNELS;
                }
            }
        }
    }
    stbi_write_png(_output_filename.c_str(), _scaled_width * _scalar, _scaled_height * _scalar, 
                CHANNELS, _output, _scaled_width * _scalar * CHANNELS);
    delete[] _output;
}
