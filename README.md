# Yet another ascii art generator

## Description:

Hello! I decided to make an ascii art generator in C++. I use the stbi image header files for my image reading and writing purposes. This project was inspired heavily by Acerola's video about ascii art shaders and I used may of the techniques he suggests in his video like a diference of gausians and sobel filter for edges. With that being said all of the code is mine which will become painfully obvious if you take a look at it. It is not clean or efficient but if its stupid and it works, is it really stupid?

## How does the program work?

After compilation of the main.cc file, when you run the resulting exicutable, it will prompt you to input the filename you want to convert to ascii including the file extention, then a down scaling factor (make this a multiple of 8 if you wanna play safe), and finally enter the name of the output file again including the extention. I have included some demo images that I have found to work well and their ascii counter parts using this generator.

This program does have the functionality of converting many images in a sequence to create a video however that functionality is quite messy and I want to rewrite the whole thing before that is fully working. I will be able to write more efficient stuff code and properly implement multithreading.