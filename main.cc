/**
 * @author Garrett Rhoads
 * @date 10/11/24
 * @brief Ascii Art generator, png to ascii
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

const int CHANNELS = 3;

#ifndef IMAGE_H
#define IMAGE_H

class Image {
public:
// Public methods
    Image();
    Image(string filename);
    ~Image();

    void to_ascii_index(const int& scalar);
    void to_ascii_png();
    bool load();
    bool load_palette();
    int get_width() const;
    int get_height() const;
    void set_filename(string new_filename);
    void set_output_filename(string new_output_filename);
    void set_dog_threshold(int new_dog_threshold);
private:
// Private methods
    void scaled_greyscale_image();
    int convolve(const int& x_pos, const int& y_pos, int& avg_r, int& avg_g, int& avg_b) const;
    int convolve(const vector<vector<int> >& matrix, const int& x_pos, 
                 const int& y_pos, const vector<int>& kernel);
    
    double convolve(const int& x_pos, const int& y_pos, 
                    const vector<vector<int> >& kernel);

    bool sobel(const int& x_pos, const int& y_pos, double& theta);
    void gaussian_blur(vector<vector<int> >& blurred_image, 
                       const vector<int>& kernel);
    void dog(); // woof
// Attributes
    vector<unsigned char> _image;
    unsigned char *_output;
    vector<vector<vector<unsigned char> > > _palette;
    vector<vector<int> > _greyscale_image;
    vector<vector<int> > _dog;
    vector<vector<int> > _ascii_indeces;
    //vector<vector<vector<int> > > _rgb_data;
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
    //const string _ascii_luminance = " .:cCoPO0@";
};

#endif

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

    // string output_filename = "output.txt";
    // ofstream out(output_filename);

    vector<int> _ascii_indeces_row;

    int avg_lumin;
    int ascii_idx;
    double theta;

    scaled_greyscale_image();

    dog(); // woof
    
    for (int i = 0; i < _scaled_height; i++) {
        for (int j = 0; j < _scaled_width; j++) {
            if ((!((i == 0) || (j == 0) || (i == (_scaled_height - 1)) || 
                   (j == (_scaled_width - 1)))) && (sobel(j, i, theta)) &&
                   (_greyscale_image[i][j] < 192)) {
                
                char back_slash = 92;
                
                if ((theta < 0.1) || (theta > 0.9)) {
                    _ascii_indeces_row.emplace_back(10);
                    // out << "--";
                } else if (theta < 0.4) {
                    _ascii_indeces_row.emplace_back(11);
                    // out << back_slash << back_slash;
                } else if (theta < 0.6) {
                    _ascii_indeces_row.emplace_back(12);
                    // out << "||";
                } else {
                    _ascii_indeces_row.emplace_back(13);
                    // out << "//";
                }
            } else {
                avg_lumin = _greyscale_image[i][j] * 100;
                ascii_idx = (avg_lumin / (25500 / 
                (_num_quantized_lumin - 1)));
                _ascii_indeces_row.emplace_back(ascii_idx);
                // out << _ascii_luminance[ascii_idx] << _ascii_luminance[ascii_idx];
            }
        }
        _ascii_indeces.emplace_back(_ascii_indeces_row);
        _ascii_indeces_row.clear();
        // out << endl;
    }
    // out.close();
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
    unsigned char* data = stbi_load(_filename.c_str(),&_width,&_height,&n, 4);
    if (data != nullptr) {
        _image = vector<unsigned char>(data, data + _width * _height * 4);
    }
    stbi_image_free(data);
    return (data != nullptr);
}

bool Image::load_palette() {
    vector<unsigned char> _palette_1D;
    vector<vector<unsigned char> > palette_row;
    vector<unsigned char> pixel;
    size_t RGBA = 4;
    string palette_file = "palette.png";

    int n;
    unsigned char* data = stbi_load(palette_file.c_str(),&_palette_width,&_palette_height,&n, 4);
    if (data != nullptr) {
        _palette_1D = vector<unsigned char>(data, data + _palette_width * _palette_height * 4);
    }
    stbi_image_free(data);
    if (data == nullptr) {
        return false;
    }
    int r, g, b;
    for (int i = 0; i < _palette_height; i++) {
        for (int j = 0; j < _palette_width; j++) {
            size_t index = RGBA * (i * _palette_width + j);
            pixel.emplace_back(static_cast<int>(_palette_1D[index + 0]));
            pixel.emplace_back(static_cast<int>(_palette_1D[index + 1]));
            pixel.emplace_back(static_cast<int>(_palette_1D[index + 2]));

            // cout << r << " " << g << " " << b << "  ";
            palette_row.emplace_back(pixel);
            pixel.clear();
        }
        _palette.emplace_back(palette_row);
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
int Image::convolve(const int& x_pos, const int& y_pos, int& avg_r, int& avg_g, int& avg_b) const {
    const size_t RGBA = 4;
    int r, g, b;
    int avg_lumin = 0;

    for (int i = (y_pos * _scalar); i < (_scalar * (y_pos + 1)); i++) {
        for (int j = (x_pos * _scalar); j < (_scalar * (x_pos + 1)); j++) {
            size_t index = RGBA * (i * _width + j);
            r = static_cast<int>(_image[index + 0]);
            g = static_cast<int>(_image[index + 1]);
            b = static_cast<int>(_image[index + 2]);
            // avg_r += r;
            // avg_g += g;
            // avg_b += b;
            avg_lumin += (r + g + b) / 3;
        }
    }
    //cout << endl;
    // avg_r = avg_r / (_scalar * _scalar);
    // avg_g = avg_g / (_scalar * _scalar);
    // avg_b = avg_b / (_scalar * _scalar);
    avg_lumin = avg_lumin / (_scalar * _scalar);
    return avg_lumin;
}

/**
 * @brief Scales the image and greyscales it
 */
void Image::scaled_greyscale_image() {
    vector<int> greyscale_row;
    int avg_r, avg_g, avg_b;

    for (int i = 0; i < _scaled_height; i++) {
        for (int j = 0; j < _scaled_width; j++) {
            greyscale_row.emplace_back(convolve(j, i, avg_r, avg_g, avg_b));
        }
        _greyscale_image.emplace_back(greyscale_row);
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
                       const vector<vector<int> >& kernel) {
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
int Image::convolve(const vector<vector<int> >& matrix, const int& x_pos, 
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
    vector<vector<int> > x_kernel;
    vector<int> x_kernel_row;
    
    x_kernel_row.emplace_back(-1);
    x_kernel_row.emplace_back(0);
    x_kernel_row.emplace_back(1);
    x_kernel.emplace_back(x_kernel_row);
    x_kernel_row.clear();    
    x_kernel_row.emplace_back(-2);
    x_kernel_row.emplace_back(0);
    x_kernel_row.emplace_back(2);
    x_kernel.emplace_back(x_kernel_row);
    x_kernel_row.clear();
    x_kernel_row.emplace_back(-1);
    x_kernel_row.emplace_back(0);
    x_kernel_row.emplace_back(1);
    x_kernel.emplace_back(x_kernel_row);
    x_kernel_row.clear();

    vector<vector<int> > y_kernel;
    vector<int> y_kernel_row;

    y_kernel_row.emplace_back(-1);
    y_kernel_row.emplace_back(-2);
    y_kernel_row.emplace_back(-1);
    y_kernel.emplace_back(y_kernel_row);
    y_kernel_row.clear();
    y_kernel_row.emplace_back(0);
    y_kernel_row.emplace_back(0);
    y_kernel_row.emplace_back(0);
    y_kernel.emplace_back(y_kernel_row);
    y_kernel_row.clear();
    y_kernel_row.emplace_back(1);
    y_kernel_row.emplace_back(2);
    y_kernel_row.emplace_back(1);
    y_kernel.emplace_back(y_kernel_row);
    y_kernel_row.clear();

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
void Image::gaussian_blur(vector<vector<int> >& blurred_image, 
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
                blurred_image_row.emplace_back(blur);
            } else {
                blurred_image_row.emplace_back(0);
            }
        }
        blurred_image.emplace_back(blurred_image_row);
        blurred_image_row.clear();
    }
}

/**
 * @brief Summons the Devourer of-- sorry, preforms a simple difference of 
 *        gaussians with a threshold of 9 and kernel sizes of 3 and 7
 */
void Image::dog() {
    vector<int> kernel_1;
    kernel_1.emplace_back(1);
    kernel_1.emplace_back(4);
    kernel_1.emplace_back(6);
    kernel_1.emplace_back(4);
    kernel_1.emplace_back(1);

    vector<int> kernel_2;
    kernel_2.emplace_back(1);
    kernel_2.emplace_back(8);
    kernel_2.emplace_back(28);
    kernel_2.emplace_back(56);
    kernel_2.emplace_back(70);
    kernel_2.emplace_back(56);
    kernel_2.emplace_back(28);
    kernel_2.emplace_back(8);
    kernel_2.emplace_back(1);

    vector<vector<int> > blur_1;
    vector<vector<int> > blur_2;

    gaussian_blur(blur_1, kernel_1);
    gaussian_blur(blur_2, kernel_2);

    vector<int> dog_row;
    for (int i = 0; i < _scaled_height; i++) {
        for (int j = 0; j < _scaled_width; j++) {
            if (abs(blur_1[i][j] - blur_2[i][j]) > _dog_threshold) {
                dog_row.emplace_back(255);
            } else {
                dog_row.emplace_back(0);
            }
        }
        _dog.emplace_back(dog_row);
        dog_row.clear();
    }
}

void Image::to_ascii_png() {
    int output_size = (_scaled_width * _scalar) * (_scaled_width * _scalar) * CHANNELS;
    _output = new unsigned char[output_size];
    unsigned char *pix = _output;
    
    for (int i = 0; i < _scaled_height; i++) {
        for (int palette_row = 0; palette_row < 8; palette_row++) {
            for (int j = 0; j < _scaled_width; j++) {

                int ascii_texture_start_col = _ascii_indeces[i][j] * 8;

                for (int palette_col = ascii_texture_start_col; palette_col < (ascii_texture_start_col + 8); palette_col++) {
                    for (int c = 0; c < 3; c++) {
                        *(pix + c) = _palette[palette_row][palette_col][c];
                    }
                    pix += CHANNELS;
                }
            }
        }
    }
    stbi_write_png(_output_filename.c_str(), _scaled_width * 8, _scaled_height * 8, CHANNELS, _output, _scaled_width * 8 * CHANNELS);
    delete[] _output;
}

void write_frame(const int& scalar, const int& first_frame, const int& num_frames, const vector<string>& frame_filenames, const vector<string>& output_frame_filenames) {
    for (int i = first_frame; i < (first_frame + num_frames); i++) {
        Image frame;
        bool palette_success = frame.load_palette();
        if (!palette_success) {
            cout << "Error loading palette\n";
            return;
        }
        
        // cout << frame_filename << endl;
        frame.set_filename(frame_filenames[i]);
        frame.set_output_filename(output_frame_filenames[i]);
        //cout << frame_filenames[i] << endl;

        bool success = frame.load();
        if (!success) {
            cout << "Error loading image\n";
            return;
        }
        frame.to_ascii_index(scalar);
        frame.to_ascii_png();
        cout << "Frame " << (i + 1 - first_frame) << " of " << num_frames << ": " << ((i + 1 - first_frame) * 100) / num_frames << "%\n";
    }
}

void write_image() {
    string img_filename;
    string output_filename;
    int scalar;
    int dog_threshold = 0;
    cout << "Input image filename including extention:\n";
    cin >> img_filename;
    
    Image img;
    bool palette_success = img.load_palette();
    if (!palette_success) {
        cout << "Error Loading palette\n";
        return;
    }
    img.set_filename(img_filename);
    bool success = img.load();
    if (!success) {
        cout << "Error loading image\n";
        return;
    }

    cout << "Input image dimensions:\n" << img.get_width() <<
    " x " << img.get_height() << endl;
    cout << "Downscaling factor (≥ 8): \n";
    cin >> scalar;
    cout << "Output \"filename\".png: \n";
    cin >> output_filename;
    
    img.set_dog_threshold(dog_threshold);
    img.set_output_filename(output_filename);
    img.to_ascii_index(scalar);
    img.to_ascii_png();
}

void write_video() {
    string frame_filename_base = "dandadan_0008";
    string output_frame_base = "dandadan_ascii_";
    int num_frames = 2798;

    vector<string> frame_filenames;
    vector<string> output_frame_filenames;
    for (int i = 0; i < num_frames; i++) {
        if (i < 10) {
            frame_filenames.emplace_back(frame_filename_base + "640" + to_string(i) + ".png");
            output_frame_filenames.emplace_back(output_frame_base + "000" + to_string(i) + ".png");
        } else if (i < 100) {
            frame_filenames.emplace_back(frame_filename_base + "64" + to_string(i) + ".png");
            output_frame_filenames.emplace_back(output_frame_base + "00" + to_string(i) + ".png");
        } else if (i < 600) {
            frame_filenames.emplace_back(frame_filename_base + "6" + to_string(i + 400) + ".png");
            output_frame_filenames.emplace_back(output_frame_base + "0" + to_string(i) + ".png");
        } else if (i < 1000) {
            frame_filenames.emplace_back(frame_filename_base + to_string(i + 6400) + ".png");
            output_frame_filenames.emplace_back(output_frame_base + "0" + to_string(i) + ".png");
        // } else if (i < 2000) {
        //     frame_filenames.emplace_back(frame_filename_base + "0" + to_string(i + 8000) + ".png");
        //     output_frame_filenames.emplace_back(output_frame_base + to_string(i) + ".png");
        } else {
            frame_filenames.emplace_back(frame_filename_base + to_string(i + 6400) + ".png");
            output_frame_filenames.emplace_back(output_frame_base + to_string(i) + ".png");
        }
    }
    
    int scalar = 8;
    int num_threads = 4;
    int frames_per_thread = num_frames / num_threads;

    thread th0(write_frame, scalar, (0 * frames_per_thread), frames_per_thread, frame_filenames, output_frame_filenames);
    thread th1(write_frame, scalar, (1 * frames_per_thread), frames_per_thread, frame_filenames, output_frame_filenames);
    thread th2(write_frame, scalar, (2 * frames_per_thread), frames_per_thread, frame_filenames, output_frame_filenames);
    thread th3(write_frame, scalar, (3 * frames_per_thread), frames_per_thread + (num_frames % num_threads), frame_filenames, output_frame_filenames);

    th0.join();
    th1.join();
    th2.join();
    th3.join();
}

/**
 * @brief I/O and controls operation of the program
 * 
 * @return int 
 */
int main() {
    write_image();
    return 0;
}
