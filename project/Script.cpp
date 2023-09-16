#include <iostream>
#include <algorithm>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) { 
            cout << "Executing command '" << command << "' ..." << endl;
            
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            if (command == "invert"){
                invert();
                continue;
            }
            if (command == "to_gray_scale") {
                to_gray_scale();
                continue;
            } 
            if (command == "replace"){
                replace();
                continue;
            }
            if (command == "fill") {
                fill();
                continue;
            }
            if(command == "h_mirror"){
                h_mirror();
                continue;
            }
            if(command == "v_mirror"){
                v_mirror();
                continue;
            }
            if (command == "add"){
                add();
                continue;
            }
            if(command == "crop"){
                crop();
                continue;
            }
            if(command == "rotate_left"){
                rotate_left();
                continue;
            }
            if(command == "rotate_right"){
                rotate_right();
                continue;
            }
            if(command == "median_filter"){
                median_filter();
                continue;
            }
            if(command == "xpm2_open"){
                xpm2_open();
                continue;
            }
            if(command == "xpm2_save"){
                xpm2_save();
                continue;
            }
        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
    void Script::invert(){
        //invert the rgb value of the PNG file.
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height(); j++){
                image->at(i,j).red() = 255 - image->at(i,j).red(); 
                image->at(i,j).green() = 255 - image->at(i,j).green(); 
                image->at(i,j).blue() = 255 - image->at(i,j).blue(); 
            }
        }
    }
    void Script::to_gray_scale() { 
        //Transforms each individual pixel (r, g, b) to (v, v, v) where v = (r + g + b)/3.
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height(); j++){
                rgb_value v = ((image->at(i, j)).red() + (image->at(i, j)).green() + (image->at(i, j)).blue()) / 3;
                image->at(i, j) = Color(v,v,v);
            }
        }
    }
    void Script::replace(){
        //replace pixels with the rgb individuals r1,g1,b1 to r2,g2,b2
        Color old_color, new_color;
        input >> old_color >> new_color;
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height(); j++){
                if(image->at(i,j) == (old_color)) 
                    image->at(i,j)= new_color;
            }
        }
    }
    void Script::fill(){ 
        int x, y, w, h;
        Color c;
        input >> x >> y >> w >> h >> c;
        for (int i = x; i < x + w; i++){
            for (int j = y; j < y + h; j++){
                image->at(i, j) = c;
            }
        }
    }
    void Script::h_mirror(){ 
        for (int i = 0; i < image->width() / 2; i++){
            for (int j = 0; j < image->height(); j++){
                Color temp = image->at(i,j);
                image->at(i, j) = image->at(image->width() - 1 - i, j);
                image->at(image->width()-1 - i, j) = temp;
            }
        }
    }
    void Script::v_mirror(){ 
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height() / 2; j++){
                Color temp = image->at(i,j);
                image->at(i, j) = image->at(i, image->height() - 1 - j);
                image->at(i, image->height()-1 - j) = temp;
            }
        }
    }
    void Script::add(){
        string filename;
        Color neutral;
        int x,y;
        input >> filename >> neutral >> x >> y;
        Image* given_image = loadFromPNG(filename);
        for(int i = x; i < x + given_image->width(); i++){
            for(int j = y; j < y + given_image->height(); j++){
                if (!(given_image->at(i - x, j - y) == (neutral))) 
                    image->at(i,j) = given_image->at(i-x,j-y);
            }
        }
        delete given_image; //need to delete given_image 
    }
    void Script::crop(){
        int x, y, w, h;
        input >> x >> y >> w >> h;
        Image* new_image = new Image(w, h);
        for (int i = x; i < x + w; i++){
            for (int j = y; j < y + h; j++) {
                new_image->at(i - x, j - y) = image->at(i, j);
            }
        }
        // Set pointer to new_image and delete old image
        delete image;
        image = new_image;  
    }
    void Script::rotate_left() {
        Image* new_image = new Image(image->height(), image->width());
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height(); j++) {
                new_image->at(j, image->width()-1 - i) = image->at(i, j);
            }
        }
        // Set pointer to new_image and delete old image
        delete image;
        image = new_image;  
    }
    void Script::rotate_right() {
        Image* new_image = new Image(image->height(), image->width());
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height(); j++) {
                new_image->at(image->height()-1 - j, i) = image->at(i, j);
            }
        }
        // Set pointer to new_image and delete old image
        delete image;
        image = new_image;  
    }
    void Script::median_filter() {
        int ws;
        input >> ws; //window size will be ws*ws
        Image* new_image = new Image(image->width(), image->height());
        for (int x = 0; x < image->width(); x++){
            for (int y = 0; y < image->height(); y++){
                std::vector<Color> color_values; //vector which will hold all colors of the pixel's neighborhood
                for(int i = max(0, x - ws / 2); i <= min(image->width() - 1, x + ws / 2); i++){
                    for(int j = max(0, y - ws / 2); j <= min(image->height() - 1, y + ws /2); j++){
                        color_values.push_back(image->at(i,j));
                    }
                }
                //sort the vector by each color channel to calculate every median color channel
                sort(color_values.begin(),color_values.end(), [] (Color c1, Color c2) -> bool {return c1.red() < c2.red();});
                rgb_value mr, mg, mb;

                if (color_values.size() % 2 == 0){ // even number of colors, must get the average of the middle two
                    mr = (color_values[color_values.size() / 2].red() + color_values[color_values.size() / 2 - 1].red()) / 2;
                    sort(color_values.begin(),color_values.end(), [] (Color c1, Color c2) -> bool {return c1.green() < c2.green();});
                    mg = (color_values[color_values.size() / 2].green() + color_values[color_values.size() / 2 - 1].green()) / 2;
                    sort(color_values.begin(),color_values.end(), [] (Color c1, Color c2) -> bool {return c1.blue() < c2.blue();});
                    mb = (color_values[color_values.size() / 2].blue() + color_values[color_values.size() / 2 - 1].blue()) / 2;
                }
                else {
                    mr = color_values[color_values.size() / 2].red();
                    sort(color_values.begin(),color_values.end(), [] (Color c1, Color c2) -> bool {return c1.green() < c2.green();});
                    mg = color_values[color_values.size() / 2].green();
                    sort(color_values.begin(),color_values.end(), [] (Color c1, Color c2) -> bool {return c1.blue() < c2.blue();});
                    mb = color_values[color_values.size() / 2].blue();
                }
                new_image->at(x, y) = Color(mr, mg, mb);
            }
        }
        delete image;
        image = new_image;
    }
    void Script::xpm2_open() {
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromXPM2(filename);
    }
    void Script::xpm2_save() {
        string filename;
        input >> filename;
        saveToXPM2(filename, image);
    }
}