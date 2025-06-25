/**
 * @author Garrett Rhoads
 * @date 10/11/24
 * @brief Ascii Art generator, png to ascii
 */

const int CHANNELS = 3;

#include <iostream>
#include <vector>
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
using namespace cv;
namespace fs = filesystem;

void get_files(const string& path, vector<string>& dir) {
    for (const auto & entry : fs::directory_iterator(path)) {
        dir.push_back(entry.path().filename().string());
    }
    sort(dir.begin(), dir.end());
}

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
        
        frame.set_filename(frame_filenames[i]);
        frame.set_output_filename(output_frame_filenames[i]);

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
    string home = getenv("HOME");
    string dir_path;
    cout << "PATH to directory containing frames eg: `~/Downloads/frames/`\n";
    cin >> dir_path;
    dir_path = home + dir_path;

    vector<string> frame_filenames;
    vector<string> output_frame_filenames;
    get_files(dir_path, frame_filenames);

    size_t num_frames = frame_filenames.size();
    for (size_t i = 0; i < num_frames; i++) {
        output_frame_filenames.push_back("examples/output_frames/" + frame_filenames[i]);
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

    img.set_dog_threshold(0);
    img.to_curses(win);
}


void curses_video() {
    initscr();
    cbreak();
    noecho();
    vector<string> dir;
    get_files("/Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter/examples/input_frames", dir);
    for (size_t frame = 0; frame < dir.size(); frame++) {
        string filename = "examples/input_frames/" + dir[frame];
        write_curses(filename.c_str(), stdscr);
    }
    endwin();
}

void mirror() {
    VideoCapture cap;
    cap.open(0);

    cap.set(CAP_PROP_FRAME_WIDTH, 384);
    cap.set(CAP_PROP_FRAME_HEIGHT, 240);

    Mat frame;
    initscr();
    cbreak();
    noecho();

    while (true) {
        bool ret = cap.read(frame);
        flip(frame, frame, 1);
        if (!ret || frame.empty()) {
            cerr << "Failed to capture frame" << endl;
            break;
        }
        Image img;
        img.load_live(frame);
    
        img.set_dog_threshold(0);
        img.to_curses_multithread(stdscr);
    }
    endwin();
    cap.release();
    destroyAllWindows();
}

void parse_input(int argc, vector<string> argv) {
    if (argc != 2) { 
        cout << "expected one command line argument, use -h or --help for a list of options" << endl; 
        return;
    }

    if ((argv[1] == "-h") || (argv[1] == "--help")) {
        cout << "List of command line arguments: \n-h\t--help\t\t\tShows the list of command line arguments\n-i\t--image\t\t\tConverts an image at a given path to ascii art\n-s\t--set\t\t\tConverts a set of images in a given directory to ascii art\n-tv\t--terminal-video\tRenders a set of images in a directory in alphabetical order on the terminal\n-l\t--live\t\t\tOpens a live ascii render of what your webcam sees\n-tui\t\t\t\tOpens a text user interface for ease of use" << endl;
        return;
    }
    
    if ((argv[1] == "-i") || (argv[1] == "--image")) {
        write_image();
        return;
    }

    if ((argv[1] == "-s") || (argv[1] == "--set")) {
        write_video();
        return;
    }

    if ((argv[1] == "-tv") || (argv[1] == "--terminal-video")) {
        curses_video();
        return;
    }

    if ((argv[1] == "-l") || (argv[1] == "--live")) {
        mirror();
        return;
    }

    if (argv[1] == "-tui") {
        cout << "not implemented yet" << endl;
        return;
    }
}

/**
 * @brief I/O and controls operation of the program
 * 
 * @return int 
 */
int main(int argc, char ** argv) {
    vector<string> arguments;
    arguments.reserve(argc);  
    for (size_t i = 0; i < argc; i++) {
        arguments.push_back(argv[i]);
    }
    parse_input(argc, arguments);
    return 0;
}
