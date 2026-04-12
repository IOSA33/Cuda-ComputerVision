#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libs/stb_image_write.h"
#include <iostream>
#include <chrono>
#include <vector>

void GaussianEdgeFilterWidth(int width, int height, int channels, unsigned char* data) {
    std::vector<unsigned char> source(data, data + (width * height * channels));

    for (int y = 0; y < height; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int index = (y * width + x) * channels;
            int prev  = (y * width + (x - 1)) * channels;
            int next  = (y * width + (x + 1)) * channels;

            for (int c = 0; c < 3; ++c) {
                int diff = std::abs(source[next + c] - source[prev + c]);                
                data[index + c] = static_cast<unsigned char>(diff);
            }
        }
    }
}

void GaussianEdgeFilterRow(int width, int height, int channels, unsigned char* data) {
    std::vector<unsigned char> source(data, data + (width * height * channels));

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * channels;
            int up  = ((y-1) * width + x) * channels;
            int down  = ((y+1) * width + x) * channels;

            for (int c = 0; c < 3; ++c) {
                int diff = std::abs(source[up + c] - source[down + c]);                
                data[index + c] = static_cast<unsigned char>(diff);
            }
        }
    }
}

int main() {
    const auto start { std::chrono::high_resolution_clock::now() };
    int width, height, channels;
    unsigned char* data { stbi_load("../photos/sample1.jpg", &width, &height, &channels, 0) };
    if (!data) {
        std::cout << "Failed to load INPUT image\n";
        return 1;
    }

    GaussianEdgeFilterWidth(width, height, channels, data);
    GaussianEdgeFilterRow(width, height, channels, data);

    if (!stbi_write_jpg("../photos/sample1_copy.jpg", width, height, channels, data, 100)) {
        std::cout << "Failed to write OUTPUT image\n";
    }

    stbi_image_free(data);

    const auto end { std::chrono::high_resolution_clock::now() };
    std::cout << "Time used: " << std::chrono::duration<double>(end - start) << std::endl;
}
