#include "XPM2.hpp"
#include <sstream>
#include <fstream>
#include <map>
#include <iomanip>
#include <algorithm>

namespace prog {
    Image* loadFromXPM2(const std::string& file) {
        std::ifstream in(file);
        std::string line;
        getline(in, line); //ignore first line
        int width, height, num_colours;
        getline(in, line);
        std::istringstream iss(line);
        iss >> width >> height >> num_colours; //read dimensions

        Image* image = new Image(width, height);
        std::map<char, Color> colors;   //each character is associated to a unique color

        char character;
        for (int i = 0; i < num_colours; i++){
            getline(in, line);
            character = line[0];
            rgb_value red, green, blue;
            red = (rgb_value) std::stoi(line.substr(5,2), 0, 16);
            green = (rgb_value) std::stoi(line.substr(7,2), 0, 16);
            blue = (rgb_value) std::stoi(line.substr(9), 0, 16);
            colors[character] = Color(red, green, blue);
        }

        for(int i = 0; i < height; i++){
            getline(in, line);
            for(int j = 0; j < width; j++){
                character = line[j];
                image->at(j, i) = colors[character];
            }
        }
        return image;
    }


    void saveToXPM2(const std::string& file, const Image* image) {
        std::ofstream out(file);
        out << "! XPM2" << '\n';
        std::vector<Color> colors; //vector to hold all unique colors of the image

        for(int i = 0; i < image->width(); i++){
            for(int j = 0; j < image->height(); j++){
                if(std::find(colors.begin(),colors.end(),image->at(i,j))==colors.end())
                    colors.push_back(image->at(i,j));
            }
        }
        out << image->width() << ' ' << image->height() << ' ' << colors.size() << " 1" << '\n';
        for (size_t i=0; i<colors.size();i++){
            //each color is associated with a character ('a' + its index in the vector)
            out << char('a' + i) << " c #" << std::hex << std::setfill('0') 
            << std::setw(2) << (int)colors[i].red() 
            << std::setw(2) << (int)colors[i].green() 
            << std::setw(2) << (int)colors[i].blue() << '\n';
       }
    
        for(int i = 0; i < image->height(); i++){
            for(int j = 0; j < image->width(); j++){
                //writes the corresponding character for each color of pixel in the image
                out << char('a'+ (std::find(colors.begin(),colors.end(),image->at(j,i)) - colors.begin()));        
                }
            out << '\n'; //change line
            }
        }
    }

