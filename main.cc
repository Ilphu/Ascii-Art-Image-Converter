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
#include <filesystem>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "image.h"

using namespace std;
namespace fs = filesystem;


void write_frame(const int& scalar, const int& first_frame, const int& num_frames, 
                 const vector<string>& frame_filenames, 
                 const vector<string>& output_frame_filenames) {

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
        cout << "Frame " << (i + 1 - first_frame) << " of " << num_frames << ": " 
             << ((i + 1 - first_frame) * 100) / num_frames << "%\n";
    }
}

void write_image() {
    string img_filename;
    string output_filename;
    int scalar;
    int dog_threshold = 0;
    cout << "PATH to input image eg: `examples/helloworld.jpg`\n";
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
    cout << "PATH output image ending in `.png` eg: `examples/helloworld_ascii.png`\n";
    cin >> output_filename;
    
    img.set_dog_threshold(dog_threshold);
    img.set_output_filename(output_filename);
    img.to_ascii_index(scalar);
    img.to_ascii_png();
}

void write_video() {
    string frame_filename_base;
    string output_frame_base;
    int num_frames;

    cout << "Input filename base:" << endl;
    cin >> frame_filename_base;
    cout << "Output filename base:" << endl;
    cin >> output_frame_base;
    cout << "Number of frames:" << endl;
    cin >> num_frames;

    vector<string> frame_filenames;
    vector<string> output_frame_filenames;
    for (int i = 0; i < num_frames; i++) {
        frame_filenames.push_back("examples/input_frames/" + frame_filename_base + to_string(i + 1) + ".png");
        output_frame_filenames.push_back("examples/output_frames/" + output_frame_base + to_string(i + 1) + ".png");
        cout << frame_filenames[i] << endl;
    }
    
    int scalar = 8;
    int num_threads = 4;
    int frames_per_thread = num_frames / num_threads;

    vector<thread*> thread_grp;

    for (int i = 0; i < num_threads; i++) {
        thread_grp.push_back(new thread(write_frame, scalar, i * frames_per_thread, 
                    frames_per_thread, frame_filenames, output_frame_filenames));
    }

    for (int i = 0; i < num_threads; i++) {
        thread_grp[i]->join();
        delete thread_grp[i];
    }
}

void write_curses(string img_filename, WINDOW * win) {
    
    Image img;
    img.set_filename(img_filename);
    bool success = img.load();
    if (!success) {
        cout << "Error loading image\n";
        return;
    }

    int width, height;
    getmaxyx(win, height, width);

    img.set_dog_threshold(0);
    img.to_curses(win);
}

void get_files(const string& path, vector<string>& dir) {
    for (const auto & entry : fs::directory_iterator(path)) {
        dir.push_back(entry.path().filename().string());
    }
    sort(dir.begin(), dir.end());
}

void curses_video() {
    initscr();
    cbreak();
    noecho();
    vector<string> dir;
    get_files("/Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter/examples/input_frames", dir);
    for (size_t frame = 0; frame < dir.size(); frame++) {
        //cout << dir[frame] << endl;
        string filename = "examples/input_frames/" + dir[frame];
        write_curses(filename.c_str(), stdscr);
        // clear();
        // refresh();
    }
    endwin();
}

/**
 * @brief I/O and controls operation of the program
 * 
 * @return int 
 */
int main(int argc, char ** argv) {
    if ((argc == 2) && (strcmp(argv[1], "--video") == 0)) {
        write_video();
    } else {
        curses_video();
    }
    return 0;
}
