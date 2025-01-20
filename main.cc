/**
 * @author Garrett Rhoads
 * @date 10/11/24
 * @brief Ascii Art generator, png to ascii
 */

const int CHANNELS = 3;

#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <cstdio>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "image.h"

using namespace std;


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
    cout << "PATH to input image eg: `Examples/helloworld.jpg`\n";
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
    cout << "Downscaling factor (multiple of 8): \n";
    cin >> scalar;
    cout << "PATH output image eg: `Examples/helloworld_ascii.jpg\n";
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
